#pragma once

#include <cstdint>
#include <limits>

#include "Action.hpp"

namespace Werk {

	enum class CounterMode {
		ADD,
		MULTIPLY,
		LSHIFT,
	};

	//an integer counter that encapuslates most common behaviours
	template<typename T=uint64_t, T initialValue=0, T incrementValue=1, CounterMode mode=CounterMode::ADD>
	class Counter {
	private:
		T _value = initialValue;

	public:
		T value() const { return _value; }
		void reset() { _value = initialValue; }
		void increment() {
			switch(mode) {
				case CounterMode::ADD:
					_value += incrementValue;
					break;
				case CounterMode::MULTIPLY:
					_value *= incrementValue;
					break;
				case CounterMode::LSHIFT:
					_value <<= incrementValue;
					break;
			}
		}

	};

	/**
	 * An action that increments a `Counter`.
	 */
	template <typename T=uint64_t, T initialValue=0, T incrementValue=1, CounterMode mode=CounterMode::ADD>
	class IncrementCounterAction : public Action {
	public:
		IncrementCounterAction(const std::string &name, Counter<T, initialValue, incrementValue, mode> &counter) :
			Action(name), _counter(counter) { }

		void execute() override { _counter.increment(); }

	private:
		Counter<T, initialValue, incrementValue, mode> &_counter;
	};

}