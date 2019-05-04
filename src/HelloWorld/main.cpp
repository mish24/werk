#include <cstdio>
#include <iostream>
#include <cinttypes>
#include <unistd.h>

#include "Werk/Math/SummaryStatistics.hpp"
#include "Werk/OS/Time.hpp"
#include "Werk/Application/ApplicationContext.hpp"
#include "Werk/Threading/BackgroundThread.hpp"
#include "Werk/Logging/Loggable.hpp"

//example of how to use this library
class ShutdownAction : public Werk::Action {

private:
	Werk::Log* _log;

public:
	ShutdownAction(const std::string& name, Werk::Log* log) :
	Werk::Action(name), _log(log) {}

	void execute() override {
		_log->logRaw(Werk::LogLevel::SUCCESS, "shutdown action!");
	}
};

int main()
{
	Werk::ApplicationContext context("src/HelloWorld/Test.ini");
	context.stdoutLog()->logRaw(Werk::LogLevel::INFO, "Starting.....");

	Werk::SummaryStatistics<double> s;
	s.sample(5.0);
	s.sample(1.0);
	context.stdoutLog()->log(Werk::LogLevel::SUCCESS, "Hello world! count=%" PRIu64 " average=%f stddev=%f", 
		s.count(), s.average(), s.stddev());
	context.commandManager()->execute("help");

	Werk::StringLoggable s1("This is a StringLoggable test...");
	s1.logTo(context.log());
	context.log()->logRaw(Werk::LogLevel::ALERT, "Going to sleep......");
	sleep(5);
	context.shutdownActions().push_back(new ShutdownAction("Shutdown", context.log()));
	context.commandManager()->execute("quit");
	context.log()->logRaw(Werk::LogLevel::INFO, "Done.");

	//This will be called automatically when the context falls out of scope, but is left in as an example
	context.shutdown();
	return 0;
}