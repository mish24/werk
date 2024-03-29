#pragma once

#include <cstring>
#include <cstdint>

#include "Werk/Logging/Log.hpp"

namespace Werk {

	//used for passing log messages bw threads
	struct LogMessage {
	private:
		static const size_t totalMessageSize = 1024;
		static const size_t headerSize = 32;
	public:
		//header
		uint64_t sequenceNumber;
		uint64_t time;
		LogLevel level;
		size_t length;
		static const size_t maxLineLength = totalMessageSize - headerSize;


		//message
		char message[maxLineLength];
		LogMessage() {}

		LogMessage(const LogMessage& m) = default;

		LogMessage& operator=(const LogMessage& m) = default;
	};
}