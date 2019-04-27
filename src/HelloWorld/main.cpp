#include <cstdio>
#include <iostream>
#include <cinttypes>

#include "Werk/Math/SummaryStatistics.hpp"
#include "Werk/OS/Time.hpp"
#include "Werk/Logging/Logger.hpp"

int main()
{
	Werk::Clock clock;
	clock.setEpochTime();
	Werk::Logger* log = new Werk::SyncLogger(&clock);
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
    return 0;
}