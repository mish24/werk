#include <boost/test/unit_test.hpp>

#include "Werk/Math/WeightedSummaryStatistics.hpp"

BOOST_AUTO_TEST_SUITE(WeightedSummaryStatisticsTest)

BOOST_AUTO_TEST_CASE(TestEmpty)
{
	Werk::WeightedSummaryStatistics s;
	BOOST_REQUIRE_EQUAL(s.count(), 0);
	BOOST_REQUIRE_EQUAL(s.weightSum(), 0.0);
	BOOST_REQUIRE_EQUAL(s.sum(), 0.0);
}

BOOST_AUTO_TEST_CASE(TestBasicUnweighted)
{
	Werk::WeightedSummaryStatistics s;
	s.sample(5.0, 1.0);
	s.sample(1.0, 1.0);

	BOOST_REQUIRE_EQUAL(s.count(), 2);
	BOOST_REQUIRE_EQUAL(s.weightSum(), 2.0);
	BOOST_REQUIRE_EQUAL(s.sum(), 6.0);
	BOOST_REQUIRE_EQUAL(s.average(), 3.0);
	BOOST_REQUIRE_EQUAL(s.variance(), 4.0);
	BOOST_REQUIRE_EQUAL(s.stddev(), 2.0);

	s.reset();
	BOOST_REQUIRE_EQUAL(s.count(), 0);
}

BOOST_AUTO_TEST_CASE(TestBasicEqualWeights)
{
	Werk::WeightedSummaryStatistics s;
	s.sample(5.0, 2.0);
	s.sample(1.0, 2.0);

	BOOST_REQUIRE_EQUAL(s.count(), 2);
	BOOST_REQUIRE_EQUAL(s.weightSum(), 4.0);
	BOOST_REQUIRE_EQUAL(s.sum(), 12.0);
	BOOST_REQUIRE_EQUAL(s.average(), 3.0);
	BOOST_REQUIRE_EQUAL(s.variance(), 4.0);
	BOOST_REQUIRE_EQUAL(s.stddev(), 2.0);
}

BOOST_AUTO_TEST_CASE(TestBasicUnequalWeights)
{
	Werk::WeightedSummaryStatistics s;
	s.sample(5.0, 1.0);
	s.sample(1.0, 3.0);

	BOOST_REQUIRE_EQUAL(s.count(), 2);
	BOOST_REQUIRE_EQUAL(s.weightSum(), 4.0);
	BOOST_REQUIRE_EQUAL(s.sum(), 8.0);
	BOOST_REQUIRE_EQUAL(s.average(), 2.0);
	BOOST_REQUIRE_EQUAL(s.variance(), 3.0);
}

BOOST_AUTO_TEST_SUITE_END()