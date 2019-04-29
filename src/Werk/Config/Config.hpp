#pragma once

#include <cstdint>
#include <cstdarg>
#include <limits>
#include <string>
#include "Werk/Logging/Logger.hpp"

namespace Werk {

	class Config {

	public:

		Config(Logger* log) : _log(log) { }
		//basic value-as-string accessor, this method must be overriden by every inheriting class, return NULL in case key is absemt
		virtual const char* getStringRaw(const std::string& key) const = 0;

		//basic type for string
		virtual const char* getString(const std::string& key, const char* defaultValue=nullptr, const char* help=nullptr) const {
			const char* stringValue = getStringRaw(key);
			if(stringValue == nullptr) {
				_log->log(LogLevel::INFO, "<Config> [%s] = %s [DEFAULT]%s%s",
					key.c_str(), 
					defaultValue,
					help == nullptr ? "" : " -- ",
					help == nullptr ? "" : help);
				return defaultValue;
			}

			_log->log(LogLevel::INFO, "<Config> [%s] = %s%s%s",
				key.c_str(),
				stringValue,
				help == nullptr ? "" : " -- ",
				help == nullptr ? "" : help);
			return stringValue;
		}

		virtual double getDouble(const std::string &key, double defaultValue=0, const char *help=nullptr) const {
		const char *stringValue = getStringRaw(key);
		if (stringValue == nullptr) {
			_log->log(LogLevel::WARNING, "<Config> [%s] = %f [DEFAULT]%s%s",
				key.c_str(),
				defaultValue,
				help == nullptr ? "" : " -- ",
				help == nullptr ? "" : help);
			return defaultValue;
		}

		_log->log(LogLevel::INFO, "<Config> [%s] = %s%s%s",
			key.c_str(),
			stringValue,
			help == nullptr ? "" : " -- ",
			help == nullptr ? "" : help);
		return stringValue != nullptr ? std::stold(stringValue) : defaultValue;
	}
	virtual int64_t getInt64(const std::string &key, int64_t defaultValue=0, const char *help=nullptr) const {
		const char *stringValue = getStringRaw(key);
		if (stringValue == nullptr) {
			_log->log(LogLevel::INFO, "<Config> [%s] = %" PRIi64 " [DEFAULT]%s%s",
				key.c_str(),
				defaultValue,
				help == nullptr ? "" : " -- ",
				help == nullptr ? "" : help);
			return defaultValue;
		}

		_log->log(LogLevel::WARNING, "<Config> [%s] = %s %s%s",
			key.c_str(),
			stringValue,
			help == nullptr ? "" : " -- ",
			help == nullptr ? "" : help);
		return stringValue != nullptr ? std::stoll(stringValue) : defaultValue;
	}
	virtual uint64_t getUint64(const std::string &key, uint64_t defaultValue=0, const char *help=nullptr) const {
		const char *stringValue = getStringRaw(key);
		if (stringValue == nullptr) {
			_log->log(LogLevel::INFO, "<Config> [%s] = %" PRIu64 " [DEFAULT]%s%s",
				key.c_str(),
				defaultValue,
				help == nullptr ? "" : " -- ",
				help == nullptr ? "" : help);
			return defaultValue;
		}

		_log->log(LogLevel::INFO, "<Config> [%s] = %" PRIu64 "%s%s",
			key.c_str(),
			stringValue,
			help == nullptr ? "" : " -- ",
			help == nullptr ? "" : help);
		return stringValue != nullptr ? std::stoull(stringValue) : defaultValue;
}



	protected:
		Logger* _log;
	};
}