

#include <boost/test/unit_test.hpp>

#include "Werk/Utility/Counter.hpp"

BOOST_AUTO_TEST_SUITE(CounterTest)

BOOST_AUTO_TEST_CASE(TestAdd)
{
	Werk::Counter<> c;
	BOOST_REQUIRE_EQUAL(c.value(), 0);

	c.increment();
	BOOST_REQUIRE_EQUAL(c.value(), 1);

	c.increment();
	BOOST_REQUIRE_EQUAL(c.value(), 2);

	c.reset();
	BOOST_REQUIRE_EQUAL(c.value(), 0);
}

BOOST_AUTO_TEST_CASE(TestMultiply)
{
	Werk::Counter<uint64_t, 7, 3, Werk::CounterMode::MULTIPLY> c;
	BOOST_REQUIRE_EQUAL(c.value(), 7);

	c.increment();
	BOOST_REQUIRE_EQUAL(c.value(), 21);

	c.increment();
	BOOST_REQUIRE_EQUAL(c.value(), 63);

	c.reset();
	BOOST_REQUIRE_EQUAL(c.value(), 7);
}

BOOST_AUTO_TEST_CASE(TestShift)
{
	Werk::Counter<uint64_t, 1, 1, Werk::CounterMode::LSHIFT> c;
	BOOST_REQUIRE_EQUAL(c.value(), 1);

	c.increment();
	BOOST_REQUIRE_EQUAL(c.value(), 2);

	c.increment();
	BOOST_REQUIRE_EQUAL(c.value(), 4);

	c.reset();
	BOOST_REQUIRE_EQUAL(c.value(), 1);
}

BOOST_AUTO_TEST_SUITE_END()