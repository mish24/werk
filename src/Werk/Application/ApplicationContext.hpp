#pragma once

#include <string>
#include <vector>

#include "Werk/Commands/CommandManager.hpp"
#include "Werk/Config/IniConfigSource.hpp"
#include "Werk/Config/Config.hpp"
#include "Werk/Logging/LogManager.hpp"
#include "Werk/Logging/AsyncLog.hpp"
#include "Werk/Profiling/ProfileManager.hpp"
#include "Werk/Threading/BackgroundThread.hpp"

namespace Werk {

	/*
	standard application context with all basic components
	*/
	class ApplicationContext {

	private:
		BackgroundThread _backgroundThread{&_profileManager};
		AsyncLog* _log;
		AsyncLog* _stdoutLog;
		Config* _config;
		LogManager _logManager;
		CommandManager* _commandManager;
		Clock _realTimeClock;
		ProfileManager _profileManager;
		std::vector<Action*> _shutdownActions;
		std::vector<std::string> _startupCommands;
		std::vector<std::string> _shutdownCommands;

	public:
		ApplicationContext(const std::string& logFilePath);
		~ApplicationContext();
		bool isShutdown();
		void shutdown();

		//background thread and tasks
		BackgroundThread& backgroundThread() { return _backgroundThread; }
		const BackgroundThread& backgroundThread() const { return _backgroundThread; }
		AsyncLog* log() { return _log; }
		const AsyncLog* log() const { return _log; }
		std::vector<Action*>& shutdownActions() { return _shutdownActions;}
		const std::vector<Action*>& shutdownActions() const { return _shutdownActions; }
		AsyncLog* stdoutLog() { return _stdoutLog; }
		const AsyncLog* stdoutLog() const { return _stdoutLog; }
		Config* config() { return _config; }
		const Config* config() const { return _config; }
		const Clock& realTimeClock() const { return _realTimeClock; }
		CommandManager* commandManager() { return _commandManager; }
		const CommandManager* commandManager() const { return _commandManager; }
		ProfileManager& profileManager() { return _profileManager; }
		const ProfileManager& profileManager() const { return _profileManager; }
		LogManager& logManager() { return _logManager; }
		const LogManager& logManager() const { return _logManager; }
		std::vector<std::string>& startupCommands() { return _startupCommands; }
		const std::vector<std::string>& startupCommands() const { return _startupCommands; }
		std::vector<std::string>& shutdownCommands() { return _shutdownCommands; }
		const std::vector<std::string>& shutdownCommands() const { return _shutdownCommands; }
	};
}