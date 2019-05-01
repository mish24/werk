#include "Signals.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <signal.h>

namespace Werk {

	static void handleBusError(int, siginfo_t* info, void*) {

		const char* cause;
		switch(info->si_code) {
			case BUS_ADRALN : cause = "Incorrect memory alignment"; break;
			case BUS_ADRERR : cause = "Non existent physical address"; break;
			case BUS_OBJERR : cause = "Object error (hardware)"; break;
			default : cause = "unknown";
		}

		std::printf("\n\n********************** Bus Error ***********************\n\n");
		std::printf("Faulting address : 0x%1x\n Cause: %s (%d)\n", (intptr_t)info->si_addr, cause, info->si_code);
		std::abort();
	}

	static void handleSegfault(int /*signal*/, siginfo_t* info, void* /*context*/) {

		const char* cause;
		switch(info->si_code) {
			case SEGV_ACCERR : cause = "Access Permission error."; break;
			case SEGV_MAPERR : cause = "Map error"; break;
			default : cause = "Unknown";
		}
		std::printf("\n\n***************** Segmentation fault ******************\n\n");
		std::printf("Faulting address: 0x%1x\nCause: %s (%d)\n", (intptr_t)info->si_addr, cause, info->si_code);
		std::abort();
	}

	bool setupSegfaultHandler() {
		struct sigaction sa;
		std::memset(&sa,0,sizeof(sa));
		sa.sa_sigaction = handleSegfault;
		sa.sa_flags = SA_SIGINFO;
		if(0 > sigaction(SIGSEGV, &sa, nullptr)) {
			return false;
		}
		std::memset(&sa,0,sizeof(sa));
		sa.sa_sigaction = handleBusError;
		sa.sa_flags = SA_SIGINFO;
		if(0 > sigaction(SIGBUS, &sa, nullptr)) {
			return false;
		}
		return true;
	}

}