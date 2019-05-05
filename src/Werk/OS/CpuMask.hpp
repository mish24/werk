#pragma once

#include <cstdint>

namespace Werk {

	/*
	returns the number of processors in this system
	*/
	long getProcessorsCount();

	/*
	represents a CPU mask to set processor affinity
	*/
	class CpuMask {

	public:
		CpuMask(size_t processorCount) : _processorCount(processorCount) {}
		//applies the mask to this thread and returns true on success
		bool apply();

	private:
		size_t _processorCount;
	};



	
}