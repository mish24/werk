#pragma once

#include <vector>
#include <string>

#include "Werk/Utility/NamedObject.hpp"

namespace Werk {

	class Action : public NamedObject {

	public:
		Action(const std::string& name) : NamedObject(name) {}
		virtual void execute() = 0;
	};

	class NullAction : public Action {

	public:
		NullAction(const std::string& name) : Action(name) {}
		void execute() override {}
	};

	template<typename T=uint64_t>
	class CounterAction : public Action {

	private:
		T _count = 0;

	public:
		CounterAction(const std::string& name) : Action(name) {}

		T count() const { return _count; }
		void reset() const { _count = 0;}
		void execute() override { _count += 1; }
	};

	/**
	 * An action that resets a component (many components have a `void reset()` method).
	 */
	template <typename T>
	class ResetAction : public Action
	{
	public:
		ResetAction(const std::string &name, T &object) :
			Action(name), _object(object) { }

		void execute() override { _object.reset(); }

	private:
		T &_object;
	};


	/**
	 * An action that executes a pair of other actions.
	 */
	class CompoundAction : public Action
	{
	public:
		CompoundAction(const std::string &name) : Action(name) { }

		std::vector<Action *> &actions() { return _actions; }
		const std::vector<Action *> &actions() const { return _actions; }

		void execute() override {
			for (Action *action : _actions) {
				action->execute();
			}
		}

	private:
		std::vector<Action *> _actions;
	};

	/**
	 * An action that executes a pair of other actions.
	 */
	class PairAction : public Action
	{
	public:
		PairAction(const std::string &name, Action *action1, Action *action2) :
			Action(name), _action1(action1), _action2(action2) { }

		void execute() override {
			_action1->execute();
			_action2->execute();
		}

	private:
		Action *_action1;
		Action *_action2;
	};

}