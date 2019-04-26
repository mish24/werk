#include "Werk/Profiling/ProfileManager.hpp"

int main() {
	Werk::ProfileManager profileManager;
	profileManager.writeJson(stdout);
	return 0;
}