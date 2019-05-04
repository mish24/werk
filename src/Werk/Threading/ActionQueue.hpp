#pragma once

#include <boost/lockfree/spsc_queue.hpp>
#include <queue>

#include "Werk/Utility/Action.hpp"

namespace Werk {

	/*
	a queue for one time Actions, templated to be used on one or
	differnt threads
	*/
	template<typename Q=boost::lockfree::spsc_queue<Action*, boost::lockfree::capacity<1024> > >
	class ActionQueue : public Action {

	private:
		Q _queue;
	public:
		ActionQueue(const std::string& name) : Action(name) {}
		void push(Action* action) { _queue.push(action); }
		void execute() override {
			Action* action;
			while(_queue.pop(action)) {
				action->execute();
			}
		}
	};
}