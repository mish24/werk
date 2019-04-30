#include "ApplicationContext.hpp"

#include <cstdio>

namespace Werk {

	ApplicationContext::ApplicationContext(const std::string& logFilePath) {

		FILE* file = logFilePath.length() == 0 ? stdout : std::fopen(logFilePath.c_str(), "r");
		//assuming bg thread has been created
		_log = new AsyncLog("Log", &_backgroundThread.backgroundClock(), file);
		_backgroundThread.addTask(_log);

		_stdoutLog = new AsyncLog("stdoutLog", &_backgroundThread.backgroundClock());
		_backgroundThread.addTask(_stdoutLog);

		_config = new Config("Config", _log);
		_backgroundThread.addTask(_config);
		_commandManager = new CommandManager(_log);
	}
}