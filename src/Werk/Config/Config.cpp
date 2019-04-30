#include "Config.hpp"

namespace Werk
{

//This runs on the background thread
void Config::execute()
{
	//Don't reload unless asked
	if (!_reloadConfig) {
		return;
	}

	//Select which values dictionary is not active
	ConfigValuesT &newValues = _values.load() == &_values1 ? _values2 : _values1;
	newValues.clear();

	//Reload all configs
	for (ConfigSource *configSource : _configSources) {
		if (!configSource->reloadConfig(newValues)) {
			_log->logRaw(LogLevel::ERROR, "Failed to load config from");
		}
	}

	//Swap the configs atomically
	_values.store(&newValues);
	_changed = true;
}

//Run on whatever thread the configurables live on
void Config::reloadConfigurables() {
	//Don't reload unless changed
	if (!_reloadConfig) {
		return;
	}

	for (Configurable *configurable : _configurables) {
		configurable->reloadConfig(*this);
	}
}

//Basic types
const char *Config::getString(const std::string &key, const char *defaultValue, const char *help) const {
	const char *stringValue = getStringRaw(key);
	if (stringValue == nullptr) {
		_log->log(LogLevel::INFO, "<Config> [%s] = %s [DEFAULT]%s%s",
			key.c_str(),
			defaultValue,
			help == nullptr ? "" : " -- ",
			help == nullptr ? "" : help);
		return defaultValue;
	}

	_log->log(LogLevel::INFO, "<Config> [%s] = %s%s%s",
		key.c_str(),
		stringValue,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	return stringValue;
}

double Config::getDouble(const std::string &key, double defaultValue, const char *help) const {
	const char *stringValue = getStringRaw(key);
	if (stringValue == nullptr) {
		_log->log(LogLevel::INFO, "<Config> [%s] = %f [DEFAULT]%s%s",
			key.c_str(),
			defaultValue,
			help == nullptr ? "" : " -- ",
			help == nullptr ? "" : help);
		return defaultValue;
	}

	_log->log(LogLevel::INFO, "<Config> [%s] = %f%s%s",
		key.c_str(),
		stringValue,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	return stringValue != nullptr ? std::stod(stringValue) : defaultValue;
}

int64_t Config::getInt64(const std::string &key, int64_t defaultValue, const char *help) const {
	const char *stringValue = getStringRaw(key);
	if (stringValue == nullptr) {
		_log->log(LogLevel::INFO, "<Config> [%s] = %" PRIi64 " [DEFAULT]%s%s",
			key.c_str(),
			defaultValue,
			help == nullptr ? "" : " -- ",
			help == nullptr ? "" : help);
		return defaultValue;
	}

	_log->log(LogLevel::INFO, "<Config> [%s] = %" PRIi64 "%s%s",
		key.c_str(),
		stringValue,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	return stringValue != nullptr ? std::stoll(stringValue) : defaultValue;
}

uint64_t Config::getUint64(const std::string &key, uint64_t defaultValue, const char *help) const {
	const char *stringValue = getStringRaw(key);
	if (stringValue == nullptr) {
		_log->log(LogLevel::INFO, "<Config> [%s] = %" PRIu64 " [DEFAULT]%s%s",
			key.c_str(),
			defaultValue,
			help == nullptr ? "" : " -- ",
			help == nullptr ? "" : help);
		return defaultValue;
	}

	_log->log(LogLevel::INFO, "<Config> [%s] = %" PRIu64 "%s%s",
		key.c_str(),
		stringValue,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	return stringValue != nullptr ? std::stoull(stringValue) : defaultValue;
}

}