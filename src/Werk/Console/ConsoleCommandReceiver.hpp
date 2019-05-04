#pragma once

#include "Werk/Commands/CommandManager.hpp"
#include "Werk/Console/IpcConsoleServer.hpp"
#include "Werk/Utility/Action.hpp"

namespace Werk {

	/*
	action that receives a message from the console and
	forwards it to CommandManager
	*/
	class ConsoleCommandReceiver : public Action {

	private:
		IpcConsoleServer& _server;
		CommandManager& _commandManager;

	public:
		ConsoleCommandReceiver(const std::string& name, IpcConsoleServer& server,
			CommandManager& commandManager) :
		Action(name), _server(server), _commandManager(commandManager) {}

		void execute() override {
			//run all commands in the buffer
			uint32_t sequenceNumber = 0;
			std::string message;
			while(_server.receive(sequenceNumber, message)) {
				_commandManager.execute(message);
			}
		}
	};
}