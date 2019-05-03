#include <cstdio>
#include <iostream>
#include <cinttypes>

#include "Werk/Math/SummaryStatistics.hpp"
#include "Werk/OS/Time.hpp"
#include "Werk/Application/ApplicationContext.hpp"
#include "Werk/Threading/BackgroundThread.hpp"

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

	context.shutdownActions().push_back(new ShutdownAction("Shutdown", context.log()));
	context.commandManager()->execute("quit");
	context.commandManager()->execute("help");
	context.backgroundThread().stop();
	return 0;
}