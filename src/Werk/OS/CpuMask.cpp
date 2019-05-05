#include "CpuMask.hpp"

#include <sched.h>
#include <unistd.h>

namespace Werk {

	long getProcessorCount() {
		return sysconf(_SC_NPROCESSORS_CONF);
	}

	BOOL CpuMask::apply() {
		//TODO
		return true;
	}
}