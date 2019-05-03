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

}