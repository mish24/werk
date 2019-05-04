
#include <boost/test/unit_test.hpp>

#include "Werk/Utility/Units.hpp"

BOOST_AUTO_TEST_SUITE(UnitsTest) //haha

BOOST_AUTO_TEST_CASE(TestBasic)
{
	BOOST_REQUIRE_EQUAL(Werk::parseUnits("124", Werk::STORAGE_UNITS), 124);
	BOOST_REQUIRE_EQUAL(Werk::parseUnits("723", Werk::STORAGE_UNITS), 723);

	BOOST_REQUIRE_EQUAL(Werk::parseUnits("1.5K", Werk::STORAGE_UNITS), 1024 + 512);
	BOOST_REQUIRE_EQUAL(Werk::parseUnits("4M", Werk::STORAGE_UNITS), 4 * 1024 * 1024);

	BOOST_REQUIRE_EQUAL(Werk::parseUnits("10us", Werk::TIME_UNITS), 10000);
	BOOST_REQUIRE_EQUAL(Werk::parseUnits("123.456ms", Werk::TIME_UNITS), 123456000);
}

BOOST_AUTO_TEST_SUITE_END()