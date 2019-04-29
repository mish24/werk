#include "CommandManager.hpp"
#include "Command.hpp"

#include <boost/algorithm/string.hpp>

namespace Werk {

	bool CommandManager::execute(const std::string& commandLine) {
		std::vector<std::string> arguments;
		boost::split(arguments, commandLine, boost::is_any_of(" \t"));
		return execute(arguments);
	}

	bool CommandManager::execute(const std::vector<std::string>& arguments) {
		if(arguments.size() == 0) {
			_log->logRaw(LogLevel::CRITICAL, "Got zero-length arguments to CommandManager::execute");
			return false;
		}

		const std::string& commandName = arguments[0];
		auto commandIter = _commands.find(commandName);
		if(commandIter == _commands.end()) {
			_log->logRaw(LogLevel::CRITICAL, "Command doesn't exist");
			return false;
		}
		Command* command = commandIter->second;
		return command->execute(arguments);
	}
}