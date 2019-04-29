#pragma once

#include <map>
#include "Config.hpp"


namespace Werk {

	class MapConfig : public Config {

	public:
		MapConfig(Logger* log) : Config(log) { }
		std::map<std::string, std::string>& values() { return _values; }
		//const std::map<std::string, std::string>& values() { return _values; }

		virtual const char* getStringRaw(const std::string& key) const {

			auto i = _values.find(key);
			return i == _values.end() ? nullptr : i->second.c_str();
		}

		//load an ini style file
		bool loadFromFile(const char* filename);

	protected:
		std::map<std::string, std::string> _values;
	};

}