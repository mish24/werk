#pragma once
#include <cinttypes>
#include <cstdarg>
#include <cstdio>

#include "LogMessage.hpp"
#include "Werk/OS/Time.hpp"

namespace Werk {

	class Logger {

		Werk::Clock* _clock;

	public:
		Logger(Werk::Clock* clock) : _clock(clock) {}
		virtual ~Logger() {}
		const Werk::Clock* clock() const { return _clock; }

		virtual void logRaw(LogLevel level, const char* message)=0;
		virtual void log(LogLevel level, const char* format, ...)=0;
	};

	class SyncLogger : public Logger {

	private:
		FILE* _file;

	public:
		SyncLogger(Werk::Clock* clock, FILE* file=stdout) :
			Logger(clock), _file(file) {}
		virtual ~SyncLogger() {}

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
}