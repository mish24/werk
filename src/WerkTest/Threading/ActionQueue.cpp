#include <boost/test/unit_test.hpp>
#include <queue>

#include "Werk/Threading/ActionQueue.hpp"
#include "Werk/Threading/BackgroundThread.hpp"

BOOST_AUTO_TEST_SUITE(ActionQueueTest)

BOOST_AUTO_TEST_CASE(TestBasic) {

	Werk::CounterAction<volatile uint64_t> counterAction("Counter");
	Werk::BackgroundThread backgroundThread(nullptr, 1l*1000*1000);
	Werk::ActionQueue<> actionQueue("ActionQueueTest");
	//start running
	backgroundThread.addTask(&actionQueue);

	//sleep for little more than half
	timespec delay {0, 11l * 1000 * 1000};
	nanosleep(&delay, nullptr);
	BOOST_REQUIRE_EQUAL(counterAction.count(), 0);
	actionQueue.push(&counterAction);
	actionQueue.push(&counterAction);
	actionQueue.push(&counterAction);
	//sleep again
	nanosleep(&delay, nullptr);
	BOOST_REQUIRE_EQUAL(counterAction.count(), 3);
	actionQueue.push(&counterAction);
	actionQueue.push(&counterAction);

    //And again -- the action only executes the first time
    nanosleep(&delay, nullptr);
    BOOST_REQUIRE_EQUAL(counterAction.count(), 5);

    backgroundThread.stop();
}

BOOST_AUTO_TEST_SUITE_END()