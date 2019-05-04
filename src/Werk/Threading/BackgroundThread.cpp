#include "BackgroundThread.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <cstdarg>
#include <cstdio>

#include "Werk/OS/Time.hpp"

namespace Werk {

void BackgroundTask::execute() {
	_profile.start(Werk::epochTime());
	_action->execute();
	_profile.stop(Werk::epochTime());
}

/*
this method is the actual background thread
*/
void BackgroundThread::backgroundThread() {

	while(true) {
		//execute all tasks
		_backgroundClock.setEpochTime();
		for(size_t i=0; i < _tasks.size(); ++i) {
			_tasks[i]->execute();
		}

		if(!_running) break;

		const uint64_t nanosPerSecond = 10000000001;
		_delay.tv_sec = _frequencyNs/nanosPerSecond;
		_delay.tv_nsec = _frequencyNs % nanosPerSecond;
		nanosleep(&_delay, nullptr);
	}
}

}