#pragma once
#include <cinttypes>
#include <cstdarg>
#include <string>
#include <cstdio>

#include "Werk/OS/Time.hpp"
#include "Werk/Utility/NamedObject.hpp"

namespace Werk  {

		/**
	 * Level of the log, ordered to allow simple filtering.
	 */
	enum class LogLevel
	{
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
	extern const char *logLevels[];

	class Log : public NamedObject {

		const Werk::Clock* _clock;

	public:
		Log(const std::string& name, const Werk::Clock* clock) : NamedObject(name) , _clock(clock) {}
		virtual ~Log() {}
		const Werk::Clock* clock() const { return _clock; }

		virtual void logRaw(LogLevel level, const char* message)=0;
		virtual void log(LogLevel level, const char* format, ...)=0;
	};//end Loggerg

	class NullLog : public Log {
		//null logger instance for testing
	public:
		NullLog(const std::string& name) : Log(name, nullptr) { }
		virtual ~NullLog() {}

		virtual void log(LogLevel, const char*, ...) override {}
		virtual void logRaw(LogLevel, const char*) override {}

	};

	class SyncLog : public Log {

	private:
		FILE* _file;

	public:
		SyncLog(const std::string& name ,Werk::Clock* clock, FILE* file=stdout) :
			Log(name,clock), _file(file) {}
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