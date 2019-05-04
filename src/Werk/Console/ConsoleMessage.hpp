#pragma once 

namespace Werk {

	/* represents a single message from a console client to the server */
	struct ConsoleMessage {
		//TODO - client ID or something?
		uint32_t sequenceNumber;
		char message[252];
	};
}