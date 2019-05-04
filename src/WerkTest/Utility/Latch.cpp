#include <boost/test/unit_test.hpp>

#include "Werk/Utility/Latch.hpp"

BOOST_AUTO_TEST_SUITE(LatchTest) 

BOOST_AUTO_TEST_CASE(TestBasics) {
	Werk::Latch<> l;
	BOOST_REQUIRE(!l.value());
	l.set();
	BOOST_REQUIRE(l.value());
	l.reset();
	BOOST_REQUIRE(!l.value());
}

BOOST_AUTO_TEST_CASE(TestSetResetLatchActions) {
	Werk::Latch<> l;
	Werk::SetLatchAction<> s("SetLatch", l);
	Werk::ResetLatchAction<> r("ResetLatch", l);

	BOOST_REQUIRE(!l.value());
	s.execute();
	BOOST_REQUIRE(l.value());
	s.execute();
	BOOST_REQUIRE(l.value());
	r.execute();
	BOOST_REQUIRE(!l.value());
}

BOOST_AUTO_TEST_CASE(TestConditionalLatch) {
	Werk::Latch<> l;
	Werk::CounterAction<> counter("Counter");
	Werk::ConditionalAction<> c("Conditional", l, &counter);
	BOOST_REQUIRE_EQUAL(counter.count(), 0);
	//latch is not set yet
	c.execute();
	BOOST_REQUIRE_EQUAL(counter.count(), 0);
	c.execute();
	BOOST_REQUIRE_EQUAL(counter.count(), 0);
	counter.execute();
	BOOST_REQUIRE_EQUAL(counter.count(), 1);
	c.execute();
	BOOST_REQUIRE_EQUAL(counter.count(), 1);
	l.set();
	c.execute();
	BOOST_REQUIRE_EQUAL(counter.count(), 2);
	c.execute();
	BOOST_REQUIRE_EQUAL(counter.count(), 3);
	
}

BOOST_AUTO_TEST_SUITE_END()