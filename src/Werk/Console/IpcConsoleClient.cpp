#include "IpcConsoleClient.hpp"
#include "ConsoleMessage.hpp"

namespace Werk {

	bool IpcConsoleClient::send(const std::string& message) {

		ConsoleMessage consoleMessage;
		consoleMessage.sequenceNumber = _nextSequenceNumber;

		//copt the message
		strncpy(consoleMessage.message, message.c_str(), sizeof(consoleMessage.message));
		consoleMessage.message[sizeof(consoleMessage.message) - 1] = 0;

		//try to send it
		if(!_queue.try_send(&consoleMessage, sizeof(consoleMessage), 0)) {
			return false;
		}
		++_nextSequenceNumber;
		return true;
	}
}