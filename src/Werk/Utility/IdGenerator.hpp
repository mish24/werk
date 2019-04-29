#pragma once

#include <cstdint>

namespace Werk {

	template<typename T=uint64_t>
	class IdGenerator {
	private:
		T _step;
		T _nextId;

	public:

		IdGenerator(T step=1, T start=0) :  _step(step),  _nextId(start) {}

		T step() const { return _step; }
		T nextId() const { return _nextId; }

		T getNext() {
			T id = _nextId;
			++_nextId;
			return id;
		}

};

}