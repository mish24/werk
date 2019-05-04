#include <map>
#include <string>

#include "Werk/Logging/Log.hpp"
#include "Werk/Logging/Loggable.hpp"

namespace Werk {


	class LogManager : public Loggable {

	private:
		std::map<std::string, Log*> _logs;
		NullLog* _nullLog;

	public:
		LogManager() {
			_nullLog = new NullLog("null");
			add(_nullLog);
		}

		NullLog* nullLog() { return _nullLog; }

		Log* getLog(const std::string& name) {
			auto i = _logs.find(name);
			return i == _logs.end() ? nullptr : i->second;
		}

		void add(Log* log) {
			_logs[log->name()] = log;
		}

		void logTo(Log* log) const override;
	};
}