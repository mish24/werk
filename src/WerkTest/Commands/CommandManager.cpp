#include <boost/test/unit_test.hpp>

#include "Werk/Commands/CommandManager.hpp"
#include "Werk/Logging/Log.hpp"

BOOST_AUTO_TEST_SUITE(CommandManagerTest) 

BOOST_AUTO_TEST_CASE(TestDefaultCommands) {

	Werk::Clock c;
	//Werk::SyncLog* log = new Werk::SyncLog(&c);
	//Werk::CommandManager commandManager(log);
	Werk::SyncLog log("stdout", &c);
	Werk::CommandManager commandManager(&log);

	BOOST_REQUIRE(commandManager.execute("null"));
	BOOST_REQUIRE(commandManager.execute("echo Echo test"));
	BOOST_REQUIRE(!commandManager.execute("asfd"));

	Werk::CommandAction* action = commandManager.newCommandAction("echo Action!");
	action->execute();

	BOOST_REQUIRE(commandManager.newCommandAction("asfd") == nullptr);
	//BOOST_REQUIRE(commandManager.execute("asdf"));
}

BOOST_AUTO_TEST_SUITE_END()