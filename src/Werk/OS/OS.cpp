#include "OS.hpp"

#include <unistd.h>

namespace Werk {

	const char* getOS() {
		#ifdef __MACH__
			return "Mac";
		#else
			return "Linux";
		#endif
	}
}