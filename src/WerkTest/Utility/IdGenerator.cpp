#include <boost/test/unit_test.hpp>

#include "Werk/Utility/IdGenerator.hpp"

BOOST_AUTO_TEST_SUITE(IdGeneratorTest)

BOOST_AUTO_TEST_CASE(TestBasic)
{
	Werk::IdGenerator<> g;
    BOOST_REQUIRE_EQUAL(g.nextId(), 0);
    BOOST_REQUIRE_EQUAL(g.nextId(), 0);
    BOOST_REQUIRE_EQUAL(g.getNext(), 0);
    BOOST_REQUIRE_EQUAL(g.nextId(), 1);
    BOOST_REQUIRE_EQUAL(g.getNext(), 1);
    BOOST_REQUIRE_EQUAL(g.nextId(), 2);
    BOOST_REQUIRE_EQUAL(g.getNext(), 2);
    BOOST_REQUIRE_EQUAL(g.nextId(), 3);
    BOOST_REQUIRE_EQUAL(g.getNext(), 3);
    BOOST_REQUIRE_EQUAL(g.getNext(), 4);
    BOOST_REQUIRE_EQUAL(g.getNext(), 5);
}

BOOST_AUTO_TEST_CASE(TestBasicFloat) {
    Werk::IdGenerator<float> g;
    BOOST_REQUIRE_EQUAL(g.nextId(), 0.0);
    BOOST_REQUIRE_EQUAL(g.nextId(), 0.0);
    BOOST_REQUIRE_EQUAL(g.getNext(), 0.0);
    BOOST_REQUIRE_EQUAL(g.nextId(), 1.0);
    BOOST_REQUIRE_EQUAL(g.getNext(), 1.0);
    BOOST_REQUIRE_EQUAL(g.nextId(), 2.0);
    BOOST_REQUIRE_EQUAL(g.getNext(), 2.0);
    BOOST_REQUIRE_EQUAL(g.nextId(), 3.0);
    BOOST_REQUIRE_EQUAL(g.getNext(), 3.0);
    BOOST_REQUIRE_EQUAL(g.getNext(), 4.0);
    BOOST_REQUIRE_EQUAL(g.getNext(), 5.0);
}

BOOST_AUTO_TEST_SUITE_END()