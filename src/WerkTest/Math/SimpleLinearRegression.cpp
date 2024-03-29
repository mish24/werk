#include <boost/test/unit_test.hpp>

#include "Werk/Math/SimpleLinearRegression.hpp"

BOOST_AUTO_TEST_SUITE(SimpleLinearRegressionTest)

BOOST_AUTO_TEST_CASE(TestEmpty)
{
	Werk::SimpleLinearRegression r;
    BOOST_REQUIRE_EQUAL(r.count(), 0);
}

BOOST_AUTO_TEST_CASE(TestBasic)
{
    Werk::SimpleLinearRegression r;

    r.sample(1.0, 4.0);
    r.sample(3.0, 8.0);
    BOOST_REQUIRE_EQUAL(r.count(), 2);
    BOOST_REQUIRE_EQUAL(r.beta(), 2.0);
    BOOST_REQUIRE_EQUAL(r.correlation(), 1.0);

    r.sample(5.0, 12.0);
    BOOST_REQUIRE_EQUAL(r.count(), 3);
    BOOST_REQUIRE_EQUAL(r.correlation(), 1.0);
    BOOST_REQUIRE_CLOSE(r.beta(), 2.0, 0.000000001);
    BOOST_REQUIRE_CLOSE(r.alpha(), 2.0, 0.000000001);

    r.reset();
    BOOST_REQUIRE_EQUAL(r.count(), 0);
}

BOOST_AUTO_TEST_CASE(TestZero)
{
    Werk::SimpleLinearRegression r;

    r.sample(-1.0, 3.0);
    r.sample(0.0, 0.0);
    r.sample(1.0, 3.0);
    BOOST_REQUIRE_EQUAL(r.count(), 3);
    BOOST_REQUIRE_EQUAL(r.correlation(), 0.0);
    BOOST_REQUIRE_EQUAL(r.beta(), 0.0);
    BOOST_REQUIRE_CLOSE(r.alpha(), 2.0, 0.000000001);
}

BOOST_AUTO_TEST_SUITE_END()