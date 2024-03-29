#include <boost/test/unit_test.hpp>

#include "Werk/Config/Config.hpp"
#include "Werk/Config/IniConfigSource.hpp"
#include "Werk/Config/MapConfigSource.hpp"
#include "Werk/Logging/Log.hpp"

BOOST_AUTO_TEST_SUITE(ConfigTest)

BOOST_AUTO_TEST_CASE(TestBasicTypes)
{
    Werk::Clock clock;
    Werk::SyncLog log("stdout", &clock);
    Werk::Config c("config", &log);

    Werk::MapConfigSource mapConfigSource;
    mapConfigSource.values()["Pi"] = "3.25";
    mapConfigSource.values()["Two"] = "2";
    mapConfigSource.values()["Nested.Value"] = "asdf";
    c.addConfigSource(&mapConfigSource);
    c.reloadConfig();
    c.execute();

    BOOST_REQUIRE_EQUAL(c.getString("Pi"), "3.25");
    BOOST_REQUIRE_EQUAL(c.getDouble("Pi"), 3.25);

    BOOST_REQUIRE_EQUAL(c.getString("Two"), "2");
    BOOST_REQUIRE_EQUAL(c.getInt64("Two", 0, "Help test"), 2);
    BOOST_REQUIRE_EQUAL(c.getUint64("Two"), 2);
}

BOOST_AUTO_TEST_CASE(TestDefaultValue)
{
    Werk::NullLog log("null");
    Werk::Config c("config", &log);

    BOOST_REQUIRE_EQUAL(c.getString("Pi", "3.25"), "3.25");
}

BOOST_AUTO_TEST_CASE(TestLoadFile)
{
    Werk::NullLog log("null");
    Werk::Config c("config", &log);

    Werk::IniConfigSource iniConfigSource("src/WerkTest/Config/TestConfig.ini");
    c.addConfigSource(&iniConfigSource);
    c.reloadConfig();
    c.execute();

    BOOST_REQUIRE_EQUAL(c.getString("zxcv", "asdf"), "qwer");
    BOOST_REQUIRE_EQUAL(c.getInt64("abc"), 123);
}

BOOST_AUTO_TEST_SUITE_END()