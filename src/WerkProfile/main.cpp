#include "Werk/Profiling/ProfileManager.hpp"
#include "Werk/Logging/Log.hpp"
#include "Werk/Logging/AsyncLog.hpp"
#include "Werk/OS/Time.hpp"
#include "Werk/Threading/BackgroundThread.hpp"

void profileLog(Werk::ProfileManager& profileManager) {
	Werk::BackgroundThread backgroundThread(nullptr, 1l * 1000 * 1000);
	FILE* file = fopen("/dev/null", "a");
	Werk::AsyncLog* log = new Werk::AsyncLog("Log", &backgroundThread.backgroundClock(), file);
	backgroundThread.addTask(log);

		//Raw logging
	Werk::Profile *asyncRawProfile = new Werk::Profile("AsyncLogRaw", 1000, 1000);
	profileManager.add(asyncRawProfile);
	for (size_t i = 0; i < 25000; ++i) {
		asyncRawProfile->start(Werk::epochTime());
		log->logRaw(Werk::LogLevel::INFO, "Testing raw log");
		asyncRawProfile->stop(Werk::epochTime());
	}

	//Formatted logging
	Werk::Profile *asyncProfile = new Werk::Profile("AsyncLog", 1000, 1000);
	profileManager.add(asyncProfile);
	for (int i = 0; i < 25000; ++i) {
		asyncProfile->start(Werk::epochTime());
		log->log(Werk::LogLevel::INFO, "Testing log with formatting %f %d", 3.14, i);
		asyncProfile->stop(Werk::epochTime());
	}
}

int main() {
	Werk::ProfileManager profileManager;
	profileLog(profileManager);
	profileManager.writeJson(stdout);
	return 0;
}