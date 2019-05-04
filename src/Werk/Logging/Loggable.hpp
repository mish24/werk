#pragma once

#include "Werk/Utility/Action.hpp"
#include "LogMessage.hpp"

namespace Werk {

	class Log;
	/**
	Loggable interface for objects that can be logged
	**/
	class Loggable {
	public:
		virtual void logTo(Log* log) const = 0;
	};

	/*
	implementation of loggable that outputs a constant string
	*/
	class StringLoggable : public Loggable {

	private:
		std::string _value;
		LogLevel _level;

	public:
		StringLoggable(const std::string value, LogLevel level=LogLevel::INFO) : 
		_value(value), _level(level) {}
		const std::string& value() const { return _value; }
		void logTo(Log* log) const override;
	};

	/*
	Action that logs a loggable to specific log
	*/
	class LogAction : public Action {

	private:
		Loggable* _loggable;
		Log* _log;

	public:
		LogAction(const std::string& name, Loggable* loggable, Log* log) :
		Action(name), _loggable(loggable), _log(log) {}

		void execute() override {
			_loggable->logTo(_log); 
		}
	};
}