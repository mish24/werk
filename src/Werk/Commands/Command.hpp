#pragma once

#include <string>
#include <vector>

#include "Werk/Utility/Action.hpp"

namespace Werk {

	class Command {

	private:
		std::string _help;

	public:
		Command(const std::string& help) : _help(help) {}
		const std::string& help() const { return _help; }
		virtual bool execute(const std::vector<std::string>& arguments)=0;
	};

	class NullCommand : public Command {

	public:
		NullCommand() : Command("Null command -- does nothing.") {}

		virtual bool execute(const std::vector<std::string>&) override {
			return true;
		}
	};


	/*
	assumes a command and its arguments are already created, this just returns the
	arguments + overrides the execute() function
	*/
	class CommandAction : public Action {

	private:
		Command* _command;
		std::vector<std::string> _arguments;

	public:

		CommandAction(const std::string& name, Command* command, std::vector<std::string>& arguments) :Action(name), _command(command), 
		_arguments(arguments) {}

		void execute() override {
			_command->execute(_arguments);
		}

		std::vector<std::string>& arguments() { return _arguments; }
		const std::vector<std::string>& arguments() const { return _arguments; }
	};

	/*
	a command that performs an action
	*/
	class ActionCommand : public Command {

	private:
		Action* _action;

	public:
		ActionCommand(Action* action, const std::string& help) :
		Command(help), _action(action) {}

		virtual bool execute(const std::vector<std::string>& /*arguments*/) override {
		_action->execute();
		return true;
	}
	};
}