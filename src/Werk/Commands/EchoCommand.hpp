/*
echo command 
*/
#pragma once
#include <boost/algorithm/string/join.hpp>

#include "Command.hpp"
#include "Werk/Logging/Logger.hpp"

namespace Werk {

	class EchoCommand : public Command {

	private:
		Logger* _log;
		LogLevel _level;

	public:
		EchoCommand(Logger* log, LogLevel level=LogLevel::INFO) :
			Command("Echoes the arguments back to the log"), _log(log), _level(level) {}

			virtual bool execute(const std::vector<std::string>& arguments) {
				_log->logRaw(_level, boost::algorithm::join(arguments, " ").c_str());
				return true;
			}

	};
}