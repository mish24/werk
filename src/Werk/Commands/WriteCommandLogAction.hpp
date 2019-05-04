#pragma once

#include "Werk/Utility/Action.hpp"

#include "CommandManager.hpp"

namespace Werk {

	class WriteCommandLogAction : public Action {

	public:
		WriteCommandLogAction(const std::string& name, Log* log, const CommandManager& commandManager, const std::string& path) : //*this for cm
		Action(name), _log(log), _commandManager(commandManager), _path(path) {}

		void execute() override {
			FILE* file = std::fopen(_path.c_str(), "w");
			if(nullptr == file) {
				_log->log(LogLevel::INFO, "Failed to write command to %s", _path.c_str());
				return;
			}
			_commandManager.writeJson(file);
			std::fclose(file);
		}
	private:
		Log* _log;
		const CommandManager& _commandManager;
		const std::string _path;
	};
}