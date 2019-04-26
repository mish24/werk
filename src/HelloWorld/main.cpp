#include <cstdio>
#include <iostream>

#include "Werk/Math/SummaryStatistics.hpp"

int main()
{
	Werk::SummaryStatistics<double> s;
	s.sample(5.0);
	s.sample(1.0);

	std::printf("Hello world! count=%llu average=%f, stddev=%f\n", s.count(), s.average(), s.stddev());

	const char* filename = "summary.txt";
    FILE* file = fopen(filename, "w");
    s.writeJson(file);
    std::cout << "Summary stats written successfully" << std::endl;
	return 0;
}