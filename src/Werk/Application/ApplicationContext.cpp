#include "ApplicationContext.hpp"
#include "Werk/OS/Signals.hpp"

#include <cstdio>

namespace Werk {

	ApplicationContext::ApplicationContext(const std::string& logFilePath) {

		FILE* file = logFilePath.length() == 0 ? stdout : std::fopen(logFilePath.c_str(), "r");
		if(nullptr == file) {
			std::fprintf(stderr, "Could not open log file, redirecting to stderr.\n");
			file = stderr;
		}
		//assuming bg thread has been created
		_log = new AsyncLog("Log", &_backgroundThread.backgroundClock(), file);
		_backgroundThread.addTask(_log);
		_log->logRaw(LogLevel::SUCCESS, "<Log> initialized");
		_log->logRaw(LogLevel::INFO, "Initializing other subsystems now.");

		setupSegfaultHandler();

		_stdoutLog = new AsyncLog("stdoutLog", &_backgroundThread.backgroundClock());
		_backgroundThread.addTask(_stdoutLog);

		_config = new Config("Config", _log);
		_backgroundThread.addTask(_config);
		_log->logRaw(LogLevel::SUCCESS, "<Config> initialized.");
		_commandManager = new CommandManager(_log);
		_log->logRaw(LogLevel::SUCCESS, "<CommandManager> initialized.");
	}

	ApplicationContext::~ApplicationContext() {
		if(!_backgroundThread.stopped()) {
			shutdown();
		}
	}

	void ApplicationContext::shutdown() {
		if(_backgroundThread.stopped()) {
			_log->logRaw(LogLevel::WARNING, "Already shut down context.");
			return;
		}
		_log->logRaw(LogLevel::INFO, "Shutting down.");
		_backgroundThread.stop();
	}
}