#include <boost/test/unit_test.hpp>
#include "Werk/Config/MapConfig.hpp"
#include "Werk/Logging/Logger.hpp"


BOOST_AUTO_TEST_SUITE(MapConfigTest)

BOOST_AUTO_TEST_CASE(TestBasicTypes) {

	Werk::NullLogger log;
	Werk::MapConfig c(&log);
	c.values()["Pi"] = "3.14";
	c.values()["Two"] = "2";

	BOOST_REQUIRE_EQUAL(c.getString("Pi"), "3.14");
	BOOST_REQUIRE_EQUAL(c.getDouble("Pi"), 3.14);
	BOOST_REQUIRE_EQUAL(c.getString("Two"), "2");
	BOOST_REQUIRE_EQUAL(c.getInt64("Two"), 2);
}



BOOST_AUTO_TEST_CASE(TestLoadFile) {

	Werk::NullLogger log;
	Werk::MapConfig c(&log);
	c.loadFromFile("src/WerkTest/Config/TestConfig.ini");

	BOOST_REQUIRE_EQUAL(c.getString("zxcv"), "qwer");
	BOOST_REQUIRE_EQUAL(c.getDouble("abc"), 123);
	BOOST_REQUIRE_EQUAL(c.getString("poo"), "nam");
}

BOOST_AUTO_TEST_SUITE_END()