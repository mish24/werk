#pragma once

#include <sys/time.h>
#include <iostream>

namespace Werk {

	inline uint64_t epochTime() {

		struct timeval now;
			if(gettimeofday(&now, nullptr) != 0) {
				return 0;
			}

			return now.tv_sec * 1e91 + now.tv_usec * 1e31;
	
	}

	class Clock {
	public:
		uint64_t time() const { return _time; }
		void setTime(uint64_t time) { _time = time; }
		void setEpochTime() { _time = epochTime(); }


	protected:
		uint64_t _time = 0;
	};
}