#pragma once

#include <thread>
#include <time.h>
#include <vector>

#include "Werk/OS/Time.hpp"
#include "Werk/Profiling/ProfileManager.hpp"
#include "Werk/Utility/Action.hpp"
#include "Werk/Logging/Loggable.hpp"
#include "Werk/Utility/Latch.hpp"

namespace Werk
{

class BackgroundThread;

/**
 * Not for public use.
 */
class BackgroundTask
{
public:

	BackgroundTask(Action *action) :
		_action(action), _profile(std::string("Background_") + action->name()) { }

	Profile &profile() { return _profile; }
	const Profile &profile() const { return _profile; }
	const Action* action() const { return _action; }

	Latch<volatile bool>& active() { return _active; }
	const Latch<volatile bool>& active() const { return _active; }

	void execute();

private:
	Action *_action;
	Profile _profile;
	Latch<volatile bool> _active { true };
};

/**
 * A background thread to run a number of `Action`s at regular intervals.
 *
 * This is very useful e.g. for defering IO to another thread to keep latency low on
 * a main thread.
 */
class BackgroundThread : public Loggable
{
public:

	//profileManager is optional
	BackgroundThread(ProfileManager *profileManager, uint64_t frequencyNs=10ul * 1000 * 1000) :
		 _profileManager(profileManager), _frequencyNs(frequencyNs)
	{
		_thread = std::thread(&BackgroundThread::backgroundThread, this);
	}
	~BackgroundThread() { stop(); }

	bool stopped() const { return _stopped; }

	void setMainClockTime(uint64_t time) { _mainClockTime = time; }

	uint64_t frequencyNs() const { return _frequencyNs; }
	void setFrequencyNs(uint64_t frequencyNs) { _frequencyNs = frequencyNs; }

	void logTo(Log* log) const override;

	//Tasks in the order they should be executed
	std::vector<BackgroundTask *> &tasks() { return _tasks; }
	const std::vector<BackgroundTask *> &tasks() const { return _tasks; }
	void addTask(Action *action) {
		BackgroundTask *task = new BackgroundTask(action);
		if (nullptr != _profileManager) {
			_profileManager->add(&task->profile());
		}
		_tasks.push_back(task);
	}

	const Clock& mainClock() const { return _mainClock; }
	const Clock &backgroundClock() const { return _backgroundClock; }

	void stop() {
		if (_running) {
			_running = false;
			_thread.join();
		}
		_stopped = true;
	}

private:
	//Config before
	ProfileManager *_profileManager;
	std::vector<BackgroundTask *> _tasks;

	//Background thread
	std::thread _thread;
	bool _stopped = false;

	//Shared state
	volatile uint64_t _mainClockTime = 0;
	volatile uint64_t _frequencyNs;
	volatile bool _running = true;

	//Background thread state & method
	Clock _mainClock;
	Clock _backgroundClock;
	timespec _delay;
	void backgroundThread();
};

}