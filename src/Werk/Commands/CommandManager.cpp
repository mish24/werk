#include "CommandManager.hpp"
#include "Command.hpp"

#include <boost/algorithm/string.hpp>

namespace Werk {

	bool CommandManager::execute(const std::string& commandLine) {
		std::vector<std::string> arguments;
		boost::split(arguments, commandLine, boost::is_any_of(" \t"));
		_commandHistory.emplace_back(_clock.time(), commandLine);
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

	CommandAction* CommandManager::newCommandAction(const std::string& name ,const std::string& commandLine) {
		std::vector<std::string> arguments;
		boost::split(arguments, commandLine, boost::is_any_of(" \t"));
		const std::string& commandName = arguments[0];
		auto commandIter = _commands.find(commandName);
		if(commandIter == _commands.end()) {
			_log->log(LogLevel::ERROR, "Command not found for action: %s", 
				commandName.c_str());
			return nullptr;
		}

		Command* command = commandIter->second;
		CommandAction* commandAction = new CommandAction(name,command,arguments);
		_log->log(LogLevel::INFO, "CommandAction created for %s command", commandName.c_str());
		return commandAction;
	}

	void CommandManager::logTo(Log *log) const  {
	log->log(LogLevel::INFO, "<CommandManager> Commands (%zu):", _commands.size());
	for (auto i = _commands.begin(); i != _commands.end(); ++i) {
		_log->log(LogLevel::INFO, "  %16s   %s", i->first.c_str(), i->second->help().c_str());
	}
}
}