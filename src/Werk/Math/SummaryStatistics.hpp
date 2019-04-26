#pragma once

#include <cstdint>
#include <limits>
#include <cmath>

namespace Werk {

	template<typename T>
	class SummaryStatistics {
		uint64_t _count = 0;
		T _sum = 0;
		double _average = 0;
		double _m2 = 0;

	public:
		uint64_t count() const { return _count; }
		T sum() const { return _sum; }
		double average() const { return _average; }
		double m2() const { return _m2; }

		double variance() const {
			return _count >= 2 ? _m2 / static_cast<double>(_count) : 0.0;
		}

		double stddev() const {
			return _count >= 2 ? std::sqrt(_m2 / static_cast<double>(_count)) : 0.0;
		}

		void sample(T t) {
			_count += 1;
			_sum += t;
			double delta = t - _average;
			_average += delta/static_cast<double>(_count);
			_m2 += delta * (t - _average);
		}

		void reset() {
			_count = 0;
			_sum = 0;
			_average = 0;
			_m2 = 0;
		}
	};
} //end namespace Werk