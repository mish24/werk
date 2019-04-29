#include <boost/test/unit_test.hpp>

#include "Werk/Logging/Log.hpp"
#include "Werk/Logging/LogManager.hpp"

BOOST_AUTO_TEST_SUITE(LogManagerTest)

BOOST_AUTO_TEST_CASE(TestDefaultLog) {

	Werk::Clock c;
	Werk::SyncLog* log = new Werk::SyncLog("tree", &c);
	Werk::LogManager logManager;

	BOOST_REQUIRE_EQUAL(logManager.getLog("tree"), log);
}

BOOST_AUTO_TEST_SUITE_END()