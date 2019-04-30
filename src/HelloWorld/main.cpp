#include <cstdio>
#include <iostream>
#include <cinttypes>

#include "Werk/Math/SummaryStatistics.hpp"
#include "Werk/OS/Time.hpp"
#include "Werk/Application/ApplicationContext.hpp"
#include "Werk/Threading/BackgroundThread.hpp"

int main()
{
	Werk::ApplicationContext context("/dev/null");
	context.stdoutLog()->logRaw(Werk::LogLevel::INFO, "Starting.....");

	Werk::SummaryStatistics<double> s;
	s.sample(5.0);
	s.sample(1.0);
	context.stdoutLog()->log(Werk::LogLevel::SUCCESS, "Hello world! count=%" PRIu64 " average=%f stddev=%f", 
		s.count(), s.average(), s.stddev());
	context.backgroundThread().stop();
	return 0;
}