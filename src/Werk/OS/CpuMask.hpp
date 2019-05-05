#pragma once

#include <cstdint>
#include <cstddef>
#include <sched.h>

#ifdef __MACH__
struct cpu_set_t;
#endif

namespace Werk {

	/*
	returns the number of processors in this system
	*/
	long getProcessorCount();

	/*
	represents a CPU mask to set processor affinity
	*/
	class CpuMask {

	public:
		CpuMask(size_t processorCount);
		~CpuMask();

		void clear(size_t core);
		void set(size_t core);
		//applies the mask to this thread and returns true on success
		bool apply();

	private:
		size_t _processorCount;
		size_t _size;
		cpu_set_t* _mask;
	};




}