#pragma once

#include <cassert>

#include "Werk/Utility/Action.hpp"
#include "Werk/Utility/Latch.hpp"
#include "Werk/OS/Time.hpp"

namespace Werk {

	/*
	a timer that executes an action a certain number
	of times (-1 for indefinety)
	*/
	class Timer : public Action {

	public:
		Timer(const std::string& name, const Clock* clock, Action* action, uint64_t interval, int64_t count=-1) :
		Action(name), _clock(clock), _interval(interval), _action(action), _count(count) {
			assert(nullptr != action);
		}

		uint64_t interval() const { return _interval; }
		int64_t count() const { return _count; }
		void setCount(int64_t count) { _count = count; }
		Action* action() const { return _action; }
		void execute() override {
			if(0 == _count) {
				return;
			}
			uint64_t time = _clock->time();
			if(_lastTime == 0) {
				_lastTime = time;
			}
			else if(_lastTime + _interval <= time) {
				_action->execute();
				_lastTime = time;
				if(_count > 0) {
					_count -= 1;
				}
			}
		}

	private:
		const Clock* _clock;
		uint64_t _interval;
		Action* _action;
		uint64_t _lastTime = 0;
		int64_t _count = 0;
	};
}