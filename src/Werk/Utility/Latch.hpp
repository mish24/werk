#pragma once

#include "Action.hpp"

namespace Werk {

	/* a set-reset latch */
	/* templated on type so that boolean can be made volatile if necessary */
	template<typename T=bool>
	class Latch {

	private:
		T _value;

	public:
		Latch(bool value=false) : _value(value) {}

		bool value() const { return _value; }
		void set() { _value = true;}
		void reset() { _value = false; }
	
	};

	//an action that resets the latch
	template<typename T=bool>
	class ResetLatchAction : public Action {

	private:
		Latch<T>& _latch;
	public:
		ResetLatchAction(const std::string& name, const Latch<T>& latch) :
		Action(name), _latch(latch) {}
		void execute() override { _latch.reset(); }
	};

	template<typename T=bool>
	class SetLatchAction : public Action {

	private:
		Latch<T>& _latch;
	public:
		SetLatchAction(const std::string& name, const Latch<T>& latch) :
		Action(name), _latch(latch) {}
		void execute() override { _latch.set(); }
	};
}