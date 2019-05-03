#pragma once

#include "Werk/Math/OrderStatistics.hpp"
#include "Werk/Math/SummaryStatistics.hpp"
#include "Werk/Utility/NamedObject.hpp"

namespace Werk {
	class Profile : public NamedObject {
	private:
		uint64_t _startTime = 0;
		//samples used for approximating 
		//fractiles efficiently
		uint64_t _sampleSize;
		OrderStatistics<uint64_t> _orderStatistics;

		//stats taken from samples of the orderStats every _sampleSize samples
		SummaryStatistics<uint64_t> _f50Statistics;
		SummaryStatistics<uint64_t> _f75Statistics;
		SummaryStatistics<uint64_t> _f90Statistics;
		SummaryStatistics<uint64_t> _f95Statistics;
		SummaryStatistics<uint64_t> _f99Statistics;
		SummaryStatistics<uint64_t> _f25Statistics;
		SummaryStatistics<uint64_t> _minStatistics;
		SummaryStatistics<uint64_t> _maxStatistics;


	public:
		Profile(const std::string& name  ,uint64_t sampleSize=100) : NamedObject(name), _sampleSize(sampleSize) {}

		uint64_t sampleSize() const { return _sampleSize; }

		const OrderStatistics<uint64_t>& orderStatistics() const {
			return _orderStatistics;
		}
		const SummaryStatistics<uint64_t>& f50Statistics() const {
			return _f50Statistics;
		}
		const SummaryStatistics<uint64_t>& f75Statistics() const {
			return _f75Statistics;
		}
		const SummaryStatistics<uint64_t>& f90Statistics() const {
			return _f90Statistics;
		}
		const SummaryStatistics<uint64_t>& f95Statistics() const {
			return _f95Statistics;
		}
		const SummaryStatistics<uint64_t>& f99Statistics() const {
			return _f99Statistics;
		}
		const SummaryStatistics<uint64_t>& f25Statistics() const {
			return _f25Statistics;
		}
		const SummaryStatistics<uint64_t>& minStatistics() const {
			return _minStatistics;
		}
		const SummaryStatistics<uint64_t>& maxStatistics() const {
			return _maxStatistics;
		}

		void start(uint64_t time) {
			_startTime = time;
		}
		void stop(uint64_t time) {
			//started or not?
			if(_startTime == 0) return;
			uint64_t delta = time - _startTime;
			_orderStatistics.sample(delta);
			_startTime = 0;

			//bin done?
			if(_orderStatistics.count() >= _sampleSize) {
				sampleFractiles();
			}
		}

		void sampleFractiles() {
			if (_orderStatistics.count() == 0) {
				return;
			}

			_minStatistics.sample(_orderStatistics.min());
			_maxStatistics.sample(_orderStatistics.max());
			_f25Statistics.sample(_orderStatistics.fractile(0.25));
			_f50Statistics.sample(_orderStatistics.fractile(0.50));
			_f75Statistics.sample(_orderStatistics.fractile(0.75));
			_f90Statistics.sample(_orderStatistics.fractile(0.90));
			_f95Statistics.sample(_orderStatistics.fractile(0.95));
			_f99Statistics.sample(_orderStatistics.fractile(0.99));
			_orderStatistics.reset();
		}

		void restart(uint64_t time) {
			stop(time);
			start(time);
		}

		void reset() {
			_startTime = 0;
			_orderStatistics.reset();
			_f50Statistics.reset();
			_f75Statistics.reset();
			_f90Statistics.reset();
			_f95Statistics.reset();
			_f99Statistics.reset();
		}

		void writeJson(FILE *file) const {
		fprintf(file, "{\"name\": \"%s\", \"min\": ", name().c_str());
		_minStatistics.writeJson(file);
		fprintf(file, ", \"f25\": ");
		_f25Statistics.writeJson(file);
		fprintf(file, ", \"f50\": ");
		_f50Statistics.writeJson(file);
		fprintf(file, ", \"f75\": ");
		_f75Statistics.writeJson(file);
		fprintf(file, ", \"f90\": ");
		_f90Statistics.writeJson(file);
		fprintf(file, ", \"f95\": ");
		_f95Statistics.writeJson(file);
		fprintf(file, ", \"f99\": ");
		_f99Statistics.writeJson(file);
		fprintf(file, ", \"max\": ");
		_maxStatistics.writeJson(file);
		fprintf(file, "}\n");
		}

		
	}; //end class Profile
} //end namespace Werk