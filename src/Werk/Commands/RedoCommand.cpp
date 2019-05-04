#include "RedoCommand.hpp"
#include "CommandManager.hpp"

namespace Werk {

	bool RedoCommand::execute(const std::vector<std::string>&) {
		//since the redo command is already addded, there must be at least one other thing
		size_t size = _commandManager.commandHistory().size();
		if(size < 2) {
			return false;
		}
		//and cant double redo
		const std::string& commandLine = _commandManager.commandHistory()[size-2].commandLine();
		if(commandLine == "redo") {
			return false;
		}
		return _commandManager.execute(commandLine);
	}
}