#pragma once

#include <boost/algorithm/string/join.hpp>

#include "Werk/Commands/Command.hpp"
#include "Config.hpp"

namespace Werk {

	class ReloadConfigCommand : public Command {

	private:
		Config& _config;

	public:
		ReloadConfigCommand(Config& config) :
			Command("Sets the configuration reload flag"),
			_config(config) {}

		virtual bool execute(const std::vector<std::string>&) {
			_config.reloadConfig();
			return true;
		}
	};
}