#include <boost/test/unit_test.hpp>

#include "Werk/Math/SummaryStatistics.hpp"

BOOST_AUTO_TEST_SUITE(SummaryStatistics)

BOOST_AUTO_TEST_CASE(TestEmpty) {
	Werk::SummaryStatistics<double> s;
	BOOST_REQUIRE_EQUAL(s.count(),0);
}

BOOST_AUTO_TEST_CASE(TestBasic) {
	Werk::SummaryStatistics<double> s;
	s.sample(5.0);
	s.sample(1.0);
	BOOST_REQUIRE_EQUAL(s.count(), 2);
    BOOST_REQUIRE_EQUAL(s.sum(), 6.0);
    BOOST_REQUIRE_EQUAL(s.average(), 3.0);
    BOOST_REQUIRE_EQUAL(s.variance(), 4.0);
    BOOST_REQUIRE_EQUAL(s.stddev(), 2.0);
    s.reset();
    BOOST_REQUIRE_EQUAL(s.count(),0);
    const char* filename = "summary.txt";
    FILE* file = fopen(filename, "rb");
    s.writeJson(file);
}

BOOST_AUTO_TEST_SUITE_END()