#include "BackgroundThread.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <cstdarg>
#include <cstdio>
#include "Werk/Logging/Log.hpp"
#include "Werk/OS/Time.hpp"

namespace Werk {

void BackgroundTask::execute() {
	_profile.start(Werk::epochTime());
	_action->execute();
	_profile.stop(Werk::epochTime());
}

void BackgroundThread::logTo(Log* log) const {
	log->log(LogLevel::INFO, "<BackgroundThread> Frequency (ns) : %" PRIu64, _frequencyNs);
	log->log(LogLevel::INFO, "<BackgroundThread> Tasks (%zu):", _tasks.size());
	for(BackgroundTask* task : _tasks) {
		log->log(LogLevel::INFO, " %24s %" PRIu64, task->action()->name().c_str(),
			task->profile().count());
	}
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