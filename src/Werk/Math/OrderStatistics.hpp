#pragma once

#include <algorithm>
#include <cstdint>
#include <limits>
#include <vector>

namespace Werk {
	template<typename T>
	class OrderStatistics {
	private:
		inline void update() const {
			if(_dirty) {
				std::sort(_samples.begin(), _samples.end());
				_dirty = false;
			}
		}

		mutable std::vector<T> _samples;
		mutable bool _dirty = false;
		/*
		mutable values are private but can be altered by public functions
		*/

	public:
		uint64_t count() const { return _samples.size(); }

		double max() const {
			if(count() == 0) {
				return std::numeric_limits<double>::quiet_NaN();
			}
			else {
				update();
				return _samples[count() - 1];
			}
		}

		double min() const {
			if(count() == 0) {
				return std::numeric_limits<double>::quiet_NaN();
			}
			else {
				update();
				return _samples[0];
			}
		}

		double fractile(double f) const {
			if(count() == 0) {
				return std::numeric_limits<double>::quiet_NaN();
			}
			else {
				update();
				return _samples[static_cast<size_t>(f * static_cast<double>(count()))];
			}
		}

		double q1() const { return fractile(0.25); }
		double q3() const { return fractile(0.75); }
		double median() const { return fractile(0.50); }

		void sample(T t) {
			_samples.push_back(t);
			_dirty = true;
		}

		void reset() {
			_samples.clear();
			_dirty = false;
		}
	}; //end class OrderStatistics
} //end namespace Werk