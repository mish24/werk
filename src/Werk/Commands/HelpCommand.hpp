#pragma once

#include <boost/algorithm/string/join.hpp>

#include "Werk/Logging/Log.hpp"
//#include "CommandManager.hpp"
#include "Command.hpp"

namespace Werk {

	class CommandManager;
	//command that outputs all the commands
	class HelpCommand : public Command {

	private:
		Log* _log;
		CommandManager* _commandManager;

	public:
		HelpCommand(Log* log, CommandManager* commandManager) :
		Command("Logs help for all commands"), _log(log), _commandManager(commandManager) {}

		bool execute(const std::vector<std::string>& arguments) override;
	};
}