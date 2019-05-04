#include "AsyncLog.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <cstdarg>
#include <cstdio>

#include "Werk/OS/Time.hpp"

namespace Werk {


void AsyncLog::log(LogLevel level, const char* format, ...) {

	//create buffer
	//lmessage -> sequenceNumber, time, level, length in header and real "message" of maxLineLength
	//////// private header initialization /////////////
	LogMessage lmessage;
	lmessage.sequenceNumber = _nextSendSequenceNumber++;
	lmessage.time = clock()->time();
	lmessage.level = level;

	//format the messge
	va_list args;
	va_start(args, format);
	//vsnprintf -> s(buffer to be writtem, size, format message, args)
	int n = std::vsnprintf(lmessage.message, LogMessage::maxLineLength, format, args);
	if(n < 0) {
		lmessage.length = LogMessage::maxLineLength;
		//n -> no. of characters actually written, w/o null termination
	}
	else if (n >= static_cast<int>(LogMessage::maxLineLength)) {
		//warn about truncation
		std::fprintf(stderr, "truncated log message: %s\n", lmessage.message);
		lmessage.length = LogMessage::maxLineLength;
	}
	else {
		lmessage.length = n+1;
	}
	va_end(args);
	_messages.push(lmessage);
}

void AsyncLog::logRaw(LogLevel level, const char* rawMessage) {
	LogMessage lmessage;
	lmessage.time = clock()->time();
	lmessage.level = level;
	lmessage.sequenceNumber = _nextReceiveSequenceNumber++;
	//log the message
	lmessage.length = LogMessage::maxLineLength; //this could be calculated on the fly
	std::strncpy(lmessage.message, rawMessage, lmessage.length);
	lmessage.message[lmessage.length - 1] = 0; //null terimate
	_messages.push(lmessage);
}

void AsyncLog::execute() {

	bool wrote = false;
	LogMessage lmessage;
	while(_messages.pop(lmessage)) {
		if(lmessage.sequenceNumber != _nextSendSequenceNumber) {
			// std::fprintf(_file, "{\"t\":%" PRIu64 ",\"n\":%" PRIu64 ",\"level\":\"%s\",\"message\":\"Incorrect log sequence number; expecting %" PRIu64 " but received %" PRIu64 "\"}\n",
			// 	lmessage.time,
			// 	_nextReceiveSequenceNumber,
			// 	logLevels[static_cast<size_t>(LogLevel::CRITICAL)],
			// 	_nextReceiveSequenceNumber,
			// 	lmessage.sequenceNumber);
		}
		_nextReceiveSequenceNumber = lmessage.sequenceNumber + 1;

		if(lmessage.level == LogLevel::JSON) {
			std::fprintf(_file, "{\t\":%" PRIu64 ",\"\":%" PRIu64 ",\"data\":%s}\n",
				lmessage.time, lmessage.sequenceNumber, lmessage.message);
		}
		else {
			std::fprintf(_file, "{\"t\":%" PRIu64 ",\"n\":%" PRIu64 ",\"level\":\"%s\",\"message\":\"%s\"}\n",
				lmessage.time, lmessage.sequenceNumber, logLevels[static_cast<size_t>(lmessage.level)], lmessage.message);
		}
		wrote = true;
	}

	if(wrote) {
		std::fflush(_file);
	}
}


} //end namespace Werk

