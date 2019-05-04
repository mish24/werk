#pragma once

#include <boost/algorithm/string/join.hpp>

#include "Werk/Logging/Log.hpp"
#include "Command.hpp"

namespace Werk {

	class CommandManager;
	/*
	command that reruns the last command, not the event
	*/
	class RedoCommand : public Command {

	public:
		RedoCommand(CommandManager& commandManager) :
		Command("Repeats the last command"), _commandManager(commandManager) {}

		bool execute(const std::vector<std::string>& arguments) override;

	private:
		CommandManager& _commandManager;
	};
}