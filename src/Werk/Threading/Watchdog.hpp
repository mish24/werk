#pragma once

#include <cassert>

#include "Werk/OS/Time.hpp"
#include "Werk/Utility/Action.hpp"

namespace Werk {

	class Watchdog : public Action {

	private:
		const Clock* _clock;
		uint64_t _interval;
		uint64_t _allowedMisses;
		Action* _action;
		uint64_t _lastTime = 0;
		uint64_t _misses = 0;
		volatile bool _flag = 0;

	public:
		Watchdog(const std::string& name, const Clock* clock, Action* action,
			uint64_t interval, uint64_t allowedMisses=0) :
		Action(name), _clock(clock), _interval(interval), _allowedMisses(allowedMisses),
		_action(action) {
			assert(nullptr != action);
		}

		uint64_t interval() const { return _interval; }
		uint64_t allowedMisses() const { return _allowedMisses; }
		Action* action() { return _action; }
		bool flag() const { return _flag; }
		void setFlag(bool flag=true) { _flag = flag; }

		void execute() override {
			uint64_t time = _clock->time();
			if(_flag) {
				_flag = false;
				_lastTime = time;
				_misses = 0;
				return;
			}
			//otherwise if flag is not set, is interval passed?
			if(time > _lastTime + _interval) {
				_misses += 1;
				//only execute the action once
				if(_misses == _allowedMisses + 1) {
					_action->execute();
				}
			}
		}
	};
}