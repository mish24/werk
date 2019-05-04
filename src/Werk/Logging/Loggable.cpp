#include "Loggable.hpp"

#include "Log.hpp"

namespace Werk {

	void StringLoggable::logTo(Log* log) const {
		log->logRaw(LogLevel::INFO, value().c_str());
	}
}