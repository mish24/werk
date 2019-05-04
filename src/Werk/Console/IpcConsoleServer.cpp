#include "IpcConsoleServer.hpp"

namespace Werk {

	bool IpcConsoleServer::receive(uint32_t& sequenceNumber, std::string& message) {
		//read  the message
		char buffer[sizeof(ConsoleMessage)];
		size_t receivedSize;
		unsigned int priority;
		if(!_queue.try_receive(buffer, sizeof(ConsoleMessage), receivedSize, priority)) {
			return false;
		}

		//deserialize the message
		ConsoleMessage* consoleMessage = (ConsoleMessage*) buffer;
		sequenceNumber = consoleMessage->sequenceNumber;
		message = consoleMessage->message;
		return true;
	}
}