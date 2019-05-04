#pragma once
#include <map>
#include <string>
#include <vector>
#include <cinttypes>
#include <cstdio>

#include "Werk/Commands/Command.hpp"
#include "Werk/Logging/Loggable.hpp"
#include "Werk/Commands/HelpCommand.hpp"
#include "Werk/Commands/EchoCommand.hpp"
#include "Werk/Commands/RedoCommand.hpp"
#include "Werk/Logging/Log.hpp"
#include "Werk/OS/Time.hpp"
#include "Werk/version.hpp"

namespace Werk {

	//represents a time a command was run
	class CommandHistory {

	public:

		CommandHistory(uint64_t time, const std::string& commandLine) :
		_time(time), _commandLine(commandLine) {}

		uint64_t time() const { return _time; }
		const std::string& commandLine() const { return _commandLine; }

		void writeJson(FILE* file) const {
			std::fprintf(file, "{\"t\": %" PRIu64 ", \"commandLine\": \"%s\"}\n", _time, _commandLine.c_str());
		}

	private:
		uint64_t _time;
		std::string _commandLine;
	};

	class CommandManager : public Loggable {

	private:
		const Clock& _clock;
		Log* _log;
		std::vector<CommandHistory> _commandHistory;
		std::map<std::string, Command*> _commands;

	public:
		CommandManager(const Clock& clock ,Log* log, bool defaultCommands=true) : _clock(clock), _log(log) {
			if(defaultCommands) {
				//_commands["help"] = new HelpCommand(log, this);
				_commands["echo"] = new EchoCommand(log);
				_commands["null"] = new NullCommand();
				_commands["redo"] = new RedoCommand(*this);
				_commands["warning"] = new EchoCommand(log, LogLevel::WARNING);
				_commands["error"] = new EchoCommand(log, LogLevel::ERROR);
				_commands["?"] = _commands["help"] = new ActionCommand(new LogAction("LogHelp", this, _log), 
					"Logs command help.");
				_commands["version"] = new ActionCommand(new LogAction("LogVersion", new StringLoggable(std::string("Version: ") + getVersion()), log),
					"Logs the version of Werk underlying the application");
	
				}
			}

		std::map<std::string, Command*>& commands() { return _commands; }
		const std::map<std::string, Command*>& commands() const { return _commands; }
		const std::vector<CommandHistory>& commandHistory() const { return _commandHistory; }

		void add(const std::string& name, Command* command) {
			_commands[name] = command;
		}

		bool execute(const std::string& commandLine);
		bool execute(const std::vector<std::string>& arguments);

		CommandAction* newCommandAction(const std::string& name ,const std::string& commandLine);

		void logTo(Log* log) const override;

		void writeJson(FILE* file) const {
			for(const CommandHistory& c : _commandHistory) {
				c.writeJson(file);
			}
		}
	};
}
