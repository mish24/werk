#pragma once
#include <map>
#include <string>
#include <vector>

#include "Werk/Commands/Command.hpp"
#include "HelpCommand.hpp"
#include "Werk/Commands/EchoCommand.hpp"
#include "Werk/Logging/Log.hpp"
#include "Werk/Commands/HelpCommand.hpp"

namespace Werk {

	class CommandManager {

	private:
		Log* _log;
		std::map<std::string, Command*> _commands;

	public:
		CommandManager(Log* log, bool defaultCommands=true) : _log(log) {
			if(defaultCommands) {
				_commands["help"] = new HelpCommand(log, this);
				_commands["echo"] = new EchoCommand(log);
				_commands["null"] = new NullCommand();
				_commands["warning"] = new EchoCommand(log, LogLevel::WARNING);
				_commands["error"] = new EchoCommand(log, LogLevel::ERROR);
				}
			}

		std::map<std::string, Command*>& commands() { return _commands; }
		const std::map<std::string, Command*>& commands() const { return _commands; }

		void add(const std::string& name, Command* command) {
			_commands[name] = command;
		}

		bool execute(const std::string& commandLine);
		bool execute(const std::vector<std::string>& arguments);

		CommandAction* newCommandAction(const std::string& name ,const std::string& commandLine);
	};
}
