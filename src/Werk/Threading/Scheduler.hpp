#pragma once

#include <map>

#include "Werk/OS/Time.hpp"
#include "Werk/Utility/Action.hpp"

namespace Werk {

	/*
	a task for the scheduler, with info on when to execute and when to repeat
	*/
	class ScheduledTask {

	public:
		ScheduledTask(Action* action, uint64_t interval, int64_t count=-1) :
		_action(action), _interval(interval), _count(count) {}

		uint64_t interval() const { return _interval; }
		int64_t count() const { return _count; }
		bool repeat() const { return _count != 0; }

		void execute() {
			_action->execute();
			if(_count > 0) {
				_count -= 1;
			}
		}

	protected:
		Action* _action;
		uint64_t _interval;
		int64_t _count;
	};


	class Scheduler : public Action {

	public:

		Scheduler(const std::string& name, const Clock* clock) :
		Action(name), _clock(clock) {}

		void schedule(Action* action, uint64_t time, uint64_t interval, int64_t repeat=-1) {
			ScheduledTask* task = new ScheduledTask(action, interval, repeat);
			_tasks.emplace(time, task);
		}

		void execute() override {
			if(_tasks.size() == 0) return;
			uint64_t time = _clock->time();
			for(auto i = _tasks.begin(); i != _tasks.end(); i = _tasks.begin()) {
				uint64_t nextTime = i->first;
				ScheduledTask* task = i->second;
				if(nextTime > time) {
					break;
				}
				task->execute();
				_tasks.erase(i);
				if(task->repeat()) {
					_tasks.emplace(nextTime + task->interval(), task);
				}
				else {
					delete task;
				}
			}
		}

	protected:
		const Clock* _clock;
		std::multimap<uint64_t, ScheduledTask*> _tasks;
	};
}