#pragma once

#include <boost/lockfree/spsc_queue.hpp>
#include <cstdio>
#include <cstdint>

#include "Logger.hpp"
#include "Werk/Threading/BackgroundTask.hpp"

namespace Werk {

	class AsyncLogger : public Logger, public BackgroundTask {

	private:
		FILE* _file;
		uint64_t _nextSendSequenceNumber = 0;
		uint64_t _nextReceiveSequenceNumber = 0;
		boost::lockfree::spsc_queue<LogMessage, boost::lockfree::capacity<2048> > _messages;

	public:

		AsyncLogger(Werk::Clock* clock, FILE* file=stdout) :
		Logger(clock), _file(file) {}

		virtual void log(LogLevel level, const char* format, ...) override;
		virtual void logRaw(LogLevel level, const char* rawMessage) override;

		virtual void execute() override;
		
	};

}