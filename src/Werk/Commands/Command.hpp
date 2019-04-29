#pragma once

#include <string>
#include <vector>

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

		virtual bool execute(const std::vector<std::string>&) {
			return true;
		}
	};
}