#pragma once

#include <cstring>
#include <cstdint>

namespace Werk {

	enum class LogLevel {
		CRITICAL,
		ERROR,
		WARNING,
		ALERT,
		SUCCESS,
		CONFIG,
		INFO,
		DETAIL,
		JSON,
		TRACE
	};

	extern const char* logLevels[];

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

		LogMessage(const LogMessage& m) {
			std::memcpy(this, &m, headerSize + m.length);
		}

		LogMessage& operator=(const LogMessage& m) {
			std::memcpy(this, &m, headerSize + m.length);
			return *this; //returned by value
		}
	};
}