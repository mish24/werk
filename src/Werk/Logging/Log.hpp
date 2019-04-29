#pragma once
#include <cinttypes>
#include <cstdarg>
#include <cstdio>

#include "LogMessage.hpp"
#include "Werk/OS/Time.hpp"

namespace Werk {

	class Log {

		Werk::Clock* _clock;

	public:
		Log(Werk::Clock* clock) : _clock(clock) {}
		virtual ~Log() {}
		const Werk::Clock* clock() const { return _clock; }

		virtual void logRaw(LogLevel level, const char* message)=0;
		virtual void log(LogLevel level, const char* format, ...)=0;
	};//end Loggerg

	class NullLog : public Log {
		//null logger instance for testing
	public:
		NullLog() : Log(nullptr) { }
		virtual ~NullLog() {}

		virtual void log(LogLevel, const char*, ...) override {}
		virtual void logRaw(LogLevel, const char*) override {}

	};

	class SyncLog : public Log {

	private:
		FILE* _file;

	public:
		SyncLog(Werk::Clock* clock, FILE* file=stdout) :
			Log(clock), _file(file) {}
		virtual ~SyncLog() {}

		virtual void log(LogLevel level, const char* format, ...) override {
			va_list args;
			va_start(args, format);
			std::fprintf(_file, "%" PRId64 " [%7s] ", clock()->time(),
				logLevels[static_cast<size_t>(level)]);
			std::vfprintf(_file, format, args);
			std::fprintf(_file, "\n");
			va_end(args);
		}

		virtual void logRaw(LogLevel level, const char* rawMessage) override {
			std::fprintf(_file, "%" PRId64 " [%7s] %s\n", clock()->time(), logLevels[static_cast<size_t>(level)], rawMessage);
		}
	};
} //end of Werk