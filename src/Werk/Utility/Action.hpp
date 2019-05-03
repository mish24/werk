#pragma once

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

	//action that sets the flag (which can e rest, like a latch)
	//templated on type so that boolean can be made volatile if necessary
	template<typename T=bool>
	class LatchAction : public Action {

	private:
		T _flag = false;

	public:
		LatchAction(const std::string& name) : Action(name) {}

		bool flag() const { return _flag; }
		void set() { _flag = true; }
		void reset() { _flag = false; }

		void execute() override { set(); }		
	};
}