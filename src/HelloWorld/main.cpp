#include <cstdio>
#include <iostream>
#include <cinttypes>

#include "Werk/Math/SummaryStatistics.hpp"
#include "Werk/OS/Time.hpp"
#include "Werk/Logging/AsyncLogger.hpp"
#include "Werk/Threading/BackgroundThread.hpp"

int main()
{
	Werk::Clock clock;
	clock.setEpochTime();

	Werk::AsyncLogger* log = new Werk::AsyncLogger(&clock);

	Werk::BackgroundThread* backgroundThread = new Werk::BackgroundThread();
	backgroundThread->addTask(new Werk::BackgroundLogWriterTask(log));
	log->logRaw(Werk::LogLevel::INFO, "Starting....");

	Werk::SummaryStatistics<double> s;
	s.sample(5.0);
	s.sample(1.0);

	std::printf("Hello world! count=%llu average=%f, stddev=%f\n", s.count(), s.average(), s.stddev());

	const char* filename = "summary.txt";
    FILE* file = fopen(filename, "w");
    s.writeJson(file);
    std::cout << "Summary stats written successfully" << std::endl;

    log->log(Werk::LogLevel::SUCCESS, "Hello there! count=%" PRIu64 " average=%f stddev=%f", s.count(), s.average(), s.stddev());
    backgroundThread->stop();
    return 0;
}