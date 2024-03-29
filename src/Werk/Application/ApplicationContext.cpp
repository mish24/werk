#include "ApplicationContext.hpp"

#include <boost/algorithm/string.hpp>
#include <cstdio>
#include <signal.h>
#include <unistd.h>

#include "Werk/Commands/WriteCommandLogAction.hpp"
#include "Werk/Console/ConsoleCommandReceiver.hpp"
#include "Werk/Console/IpcConsoleServer.hpp"
#include "Werk/OS/Signals.hpp"
#include "Werk/Profiling/WriteProfilesAction.hpp"
#include "Werk/Threading/Watchdog.hpp"
#include "Werk/OS/CpuMask.hpp"
#include "Werk/OS/OS.hpp"

namespace Werk
{

ApplicationContext::ApplicationContext(const std::string &configPath)
{
	//And let's pull ourselves up by our bootstraps...

	/********** Pre-Initialization **********/

	//Setup handlers for certain signals
	setupSegfaultHandler();
	_realTimeClock.setEpochTime();

	/********** Stdout Log **********/

	_stdoutLog = new AsyncLog("StdoutLog", &_backgroundThread.backgroundClock());
	_logManager.add(_stdoutLog);
	_backgroundThread.addTask(_stdoutLog);
	_backgroundThread.addTask(&_backgroundActionQueue);

	/********** Config **********/

	_config = new Config("Config", _stdoutLog);

	//Synchronously execute the reload since the values are needed for the next step
	_config->addConfigSource(new IniConfigSource(configPath));
	_config->reloadConfig();
	_config->execute();

	//Register other config files
	const char *configPathsStr = _config->getString("Application.ConfigPaths");
	if (nullptr != configPathsStr) {
		//Add all the new configs
		std::vector<std::string> configPaths;
		boost::split(configPaths, configPathsStr, boost::is_any_of(","));
		for (auto &path : configPaths) {
			_config->addConfigSource(new IniConfigSource(path));
		}
	}

	//Synchronously reload with everything
	_config->addConfigSource(new IniConfigSource(configPath));
	_config->reloadConfig();
	_config->execute();

	//Finally, with all files loaded, start reloading in the background
	_backgroundThread.addTask(_config);
	_stdoutLog->logRaw(LogLevel::SUCCESS, "<Config> Initialized.");

	/********** Main Log **********/

	const char *logPath = _config->getString("Application.LogPath");
	FILE *file = nullptr == logPath ? stdout : std::fopen(logPath, "a");
	if (nullptr == file) {
		_stdoutLog->logRaw(LogLevel::ERROR, "Could not open log file, redirecting to stderr.\n");
		file = stderr;
	}

	_log = new AsyncLog("Log", &_backgroundThread.backgroundClock(), file);
	_logManager.add(_log);
	_backgroundThread.addTask(_log);
	_config->setLog(_log);
	_log->logRaw(LogLevel::SUCCESS, "<Log> Initialized.");

	/********** Detect software state **************************/
	//PIDs
	pid_t pid = getpid();
	pid_t ppid = getppid();
	//current working directory
	char currentWorkingDir[1024];
	if(nullptr == getcwd(currentWorkingDir, sizeof(currentWorkingDir))) {
		strcpy(currentWorkingDir, "Error getting current working directory.");
	}

	//hostname
	char hostname[1024];
	if(0 != gethostname(hostname, sizeof(hostname))) {
		strcpy(hostname, "Error getting hostname.");
	}

	_log->log(LogLevel::JSON, "{\"type\":\"startup.software\",\"os\":\"%s\",\"pid\":%ld,\"ppid\":%ld,\"cwd\":\"%s\",\"hostname\":\"%s\"}", getOS(), pid, ppid, currentWorkingDir, hostname);

	/********** Configure Existing Components Now That Log Is Setup **********/

	//Background thread
	uint64_t backgroundThreadFrequencyNs = _config->getUint64("Application.BackgroundFrequencyNs", _backgroundThread.frequencyNs());
	_backgroundThread.setFrequencyNs(backgroundThreadFrequencyNs);

	/* Add number of CPU cores in AC */
	_processorCount = getProcessorCount();
	_log->log(LogLevel::JSON, "{\"type\": \"startup.hardware\", \"processorCount\":%zu}", _processorCount);


	//Set the instance ID
	_instanceId = _config->getString("Application.InstanceID", "", "ID of this instance of the application");

	_debug = _config->getBool("Application.Debug", false, "Indicates whether the application should output additional Debug info.");

	//Real time mode or not? In non-real time mode, a secondary clock is instantiated
	_realTime = _config->getBool("Application.RealTime", true,
		"Indicates whether the application is running in real time or from a different clock");
	if (_realTime) {
		_clock = &_realTimeClock;
	} else {
		_clock = new Clock();
	}

	_simulation = _config->getBool("Application.Simulation", false,
		"Indicates whether the application is running a simulation or not");

	/********** Command Manager **********/

	_commandManager = new CommandManager(_backgroundThread.backgroundClock() , _log);
	

	_commandManager->add("app", new ActionCommand(
		new LogAction("LogApplicationContext", this, _log),
		"Logs information about the application."));
	_commandManager->add("bg", new ActionCommand(
		new LogAction("LogBackgroundTasks", &_backgroundThread, _log),
		"Logs information about background tasks."));
	_commandManager->add("logs", new ActionCommand(
		new LogAction("LogLogs", &_logManager, _log),
		"Logs information about existing log files."));
	_commandManager->add("reload", new ActionCommand(
		_config->getReloadConfigAction(),
		"Reloads the configuration."));

	Action *quitAction = new SetLatchAction<volatile bool>("Quit", _quitting);
	_commandManager->add("quit", new ActionCommand(quitAction, "Quits application cleanly."));

	_log->logRaw(LogLevel::SUCCESS, "<CommandManager> Initialized.");

	const char *commandLogPath = _config->getString("Application.CommandLogPath");
	if (nullptr != commandLogPath) {
		_log->log(LogLevel::INFO, "Writing command log to %s on shutdown.", commandLogPath);
		_shutdownActions.push_back(new WriteCommandLogAction("WriteCommandLog", _log, *_commandManager, commandLogPath));
	}

	const char *profilesPath = _config->getString("Application.ProfilesPath");
	if (nullptr != profilesPath) {
		_log->log(LogLevel::INFO, "Writing profiles to %s on shutdown.", profilesPath);
		_shutdownActions.push_back(new WriteProfilesAction("WriteProfiles", _log, _profileManager, profilesPath));
	}

	/********** Console **********/

	//Consoles are only available for real time runs
	if (_realTime) {
		const char *ipcConsoleName = _config->getString("Application.IpcConsoleName");
		if (nullptr != ipcConsoleName) {
			_consoleServer.reset(new IpcConsoleServer(ipcConsoleName));

			//Setup a background task to forward tasks to the command manager
			ConsoleCommandReceiver *consoleCommandReceiver = new ConsoleCommandReceiver("IpcConsole", *_consoleServer, *_commandManager);
			_backgroundThread.addTask(consoleCommandReceiver);
		}
	}

	/********** Scheduler *********************/
	_scheduler = new Scheduler("Scheduler", &_backgroundThread.mainClock());
	_backgroundThread.addTask(_scheduler);
	//configure scheduled commands
	const char* scheduledCommandsStr = _config->getString("Application.ScheduledCommands");
	if(nullptr != scheduledCommandsStr) {
		//split and add each command
		std::vector<std::string> scheduledCommands;
		boost::split(scheduledCommands, scheduledCommandsStr, boost::is_any_of(";"));
		for(auto& command : scheduledCommands) {
			(void) command;
		}
	}

	/********** Finish Initialization **********/

	//Setup remaining signals
	//SIGHUP -> reload config
	setupSignalHandler(SIGHUP, _config->getReloadConfigAction());
	setupSignalHandler(SIGINT, quitAction);
	//Load and run startup commands
	//TODO: consider defering this until later, once the user has setup everything they need?
	const char *startupCommandsStr = _config->getString("Application.StartupCommands");
	if (nullptr != startupCommandsStr) {
		//Split and run each command
		boost::split(_startupCommands, startupCommandsStr, boost::is_any_of(";"));
		for (auto &command : _startupCommands) {
			_commandManager->execute(command);
		}
	}

	_log->logRaw(LogLevel::JSON, "{\"type\": \"startup.initialized\"}");
}

ApplicationContext::~ApplicationContext()
{
	//Shut down, if not already done
	if (!isShutdown()) {
		shutdown();
	}
}

void ApplicationContext::logTo(Log *log) const
{
	log->log(LogLevel::INFO, "<Context>  Real Time: %s", _realTime ? "Yes" : "No");
	log->log(LogLevel::INFO, "<Context> Simulation: %s", _simulation ? "Yes" : "No");
}

bool ApplicationContext::isShutdown()
{
	return _backgroundThread.stopped();
}

void ApplicationContext::shutdown()
{
	//Don't shut down twice
	if (isShutdown()) {
		fprintf(stderr, "ApplicationContext::shutdown - Already shut down.\n");
		return;
	}

	//Load and run shutdown commands
	const char *shutdownCommandsStr = _config->getString("Application.ShutdownCommands");
	if (nullptr != shutdownCommandsStr) {
		//Split on ; and run each command
		boost::split(_shutdownCommands, shutdownCommandsStr, boost::is_any_of(";"));
		for (auto &command : _startupCommands) {
			_commandManager->execute(command);
		}
	}

	//Run shutdown actions
	_log->logRaw(LogLevel::JSON, "{\"type\": \"shutdown.shutdownActions\"}");
	for (Action *action : _shutdownActions) {
		action->execute();
	}
	_log->logRaw(LogLevel::JSON, "{\"type\": \"shutdown.shutdownActionsComplete\"}");

	_log->logRaw(LogLevel::JSON, "{\"type\": \"shutdown.shuttingDown\"}");
	_backgroundThread.stop();

	//Cleanup the console
	_consoleServer.reset();
}

void ApplicationContext::run(Action* mainAction) {
	//setup the background watchdog timer
	uint64_t watchdogInterval = _config->getUint64("Application.watchdogInterval", 0, "Interval of the main thread watchdog (ns)");
	uint64_t watchdogAllowedMisses = _config->getUint64("Application.WatchdogAllowedMisses", 1, "Number of times the main thread can miss the watchdog");
	Watchdog* watchdog = new Watchdog("Watchdog", &_backgroundThread.backgroundClock(),
		new LogAction("WatchdogWarning", new StringLoggable("Missed watchdog!", LogLevel::WARNING), _log),
		watchdogInterval, watchdogAllowedMisses);

		if (0 != watchdogInterval) {
		//TODO: check against background thread frequency
		_backgroundThread.addTask(watchdog);
		}

		_log->logRaw(LogLevel::JSON, "{\"type\": \"mainLoop.enter\"}");
		while (!_quitting.value()) {
			//TODO: run a main loop action
			//run other queued actions
			_realTimeClock.setEpochTime();
			mainAction->execute();
			_foregroundActionQueue.execute();
			_backgroundThread.setMainClockTime(_clock->time());
			//Made it through another loop, set the watchdog
			watchdog->reset();
		}
		_log->logRaw(LogLevel::JSON, "{\"type\": \"mainLoop.exit\"}");

		shutdown();
	}

}