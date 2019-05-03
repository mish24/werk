#include "ApplicationContext.hpp"
#include "Werk/OS/Signals.hpp"
#include "Werk/Config/ReloadConfigCommand.hpp"
#include "Werk/Commands/QuitCommand.hpp"
#include "Werk/Profiling/WriteProfilesAction.hpp"
#include <boost/algorithm/string.hpp>

#include <cstdio>

namespace Werk {


	ApplicationContext::ApplicationContext(const std::string& configPath) {
		//setup the handlers
		// lets pull up the boostraps
		/***************** Pre - initialization *****************/
		setupSegfaultHandler();
		//stdout log
		_stdoutLog = new AsyncLog("StdoutLog", &_backgroundThread.backgroundClock());
		_backgroundThread.addTask(_stdoutLog);
		//config
		_config = new Config("Config", _stdoutLog);
		//synchronously execute the reload since this is needed in the next step
		_config->addConfigSource(new IniConfigSource(configPath));
		_config->reloadConfig();
		_config->execute();

		//register other config files
		const char* configPathStr = _config->getString("Application.ConfigPaths");
		if(nullptr != configPathStr) {
			//add all new configs
			std::vector<std::string> configPaths;
			boost::split(configPaths, configPathStr, boost::is_any_of(","));
			for(auto& path : configPaths) {
				_config->addConfigSource(new IniConfigSource(path));
			}
		}

		//synchronously load with everything
		_config->addConfigSource(new IniConfigSource(configPath));
		_config->reloadConfig();
		//finally, with all files loaded, start reloading in the background
		_backgroundThread.addTask(_config);
		_stdoutLog->logRaw(LogLevel::SUCCESS, "<Config> initialized.");

		/*********************** Main log *****************************/
		const char* logPath = _config->getString("Application.LogPath");
		FILE* file = nullptr == logPath ? stdout : std::fopen(logPath, "r");
		if(nullptr == file) {
			_stdoutLog->logRaw(LogLevel::ERROR, "Could not open log file, redirecting to stderr\n");
			file = stderr;
		}
		_log = new AsyncLog("Log", &_backgroundThread.backgroundClock(), file);
		_backgroundThread.addTask(_log);
		_config->setLog(_log);
		_log->logRaw(LogLevel::SUCCESS, "<Log> initialized.");

		/*************** Command Manager **********************/
		_commandManager = new CommandManager(_log);
		_commandManager->add("reload", new ReloadConfigCommand(*_config));
		_commandManager->add("quit", new QuitCommand(this));
		_log->logRaw(LogLevel::SUCCESS, "<CommandManager> initialized.");

		/************** profiles initialization *********************/
		const char* profilesPath = _config->getString("Application.ProfilesPath");
		if(nullptr != profilesPath) {
			_log->log(LogLevel::INFO, "Writing Profiles to %s on shutdown", profilesPath);
			_shutdownActions.push_back(new WriteProfilesAction("WriteProfiles", _log, _profileManager,
				profilesPath));
		}

		/************************** Finish Initialisation *************/
		_log->logRaw(LogLevel::SUCCESS, "<ApplicationContext> initialized.");
	}

	ApplicationContext::~ApplicationContext() {
		if(!_backgroundThread.stopped()) {
			shutdown();
		}
	}


	void ApplicationContext::shutdown() {
		if(_backgroundThread.stopped()) {
			fprintf(stderr, "ApplicationContext::shutdown - Already shut down.\n");
		}
		_log->logRaw(LogLevel::INFO, "Running shutdown actions...");
		for(Action* action : _shutdownActions) {
			action->execute();
		}
		_log->logRaw(LogLevel::SUCCESS, "Shutdown actions complete");
		_log->logRaw(LogLevel::INFO, "Shutting down..");
		_backgroundThread.stop();
	}
}