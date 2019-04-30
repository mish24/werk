#pragma once

namespace Werk {

	class Action {

	public:
		Action(const std::string& name) : _name(name) {}
		const std::string& name() const { return _name; }
		virtual void execute() = 0;

	private:
		std::string _name;
	};

	class NullAction : public Action {

	public:
		NullAction(const std::string& name) : Action(name) {}
		void execute() override {}
	}

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