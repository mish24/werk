#pragma once

#include <boost/lockfree/spsc_queue.hpp>
#include <cstdio>
#include <cstdint>

#include "Log.hpp"
#include "Werk/Utility/Action.hpp"

namespace Werk {

	class AsyncLog : public Log, public Action {

	private:
		FILE* _file;
		uint64_t _nextSendSequenceNumber = 0;
		uint64_t _nextReceiveSequenceNumber = 0;
		boost::lockfree::spsc_queue<LogMessage, boost::lockfree::capacity<2048> > _messages;

	public:

		AsyncLog(const std::string& name, const Werk::Clock* clock, FILE* file=stdout) :
		Log(name, clock), Action(name + "_Writer"),_file(file) {}

		virtual void log(LogLevel level, const char* format, ...) override;
		virtual void logRaw(LogLevel level, const char* rawMessage) override;

		virtual void execute() override;
		
	};

}