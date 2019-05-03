#pragma once

#include "Werk/Logging/Log.hpp"

#include "Command.hpp"

namespace Werk {

	class ApplicationContext;

	class QuitCommand : public Command {

	private:
		ApplicationContext* _applicationContext;

	public:
		QuitCommand(ApplicationContext* applicationContext) :
		Command("Quits the application context and hence program cleanly."),
		_applicationContext(applicationContext) {}

		bool execute(const std::vector<std::string>& arguments) override;
	};
}