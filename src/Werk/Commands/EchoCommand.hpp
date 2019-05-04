/*
echo command 
*/
#pragma once
#include <boost/algorithm/string/join.hpp>

#include "Command.hpp"
#include "Werk/Logging/Log.hpp"

namespace Werk {

	class EchoCommand : public Command {

	private:
		Log* _log;
		LogLevel _level;

	public:
		EchoCommand(Log* log, LogLevel level=LogLevel::INFO) :
			Command("Echoes the arguments back to the log"), _log(log), _level(level) {}

			bool execute(const std::vector<std::string>& arguments) override {
				//join all but the first argument
				_log->logRaw(_level, boost::algorithm::join(boost::make_iterator_range(arguments.begin() + 1, arguments.end()), " ").c_str());
				return true;
			}

	};
}