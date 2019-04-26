#pragma once

#include <cstdio>
#include <map>
#include <string>

#include "Werk/Profiling/Profile.hpp"
#include "Werk/OS/Time.hpp"

namespace Werk {

	class ProfileManager {
	private:
		std::map<std::string, Profile*> _profiles;
		Profile _baseProfile {"Base", 1000 };

	public:
		const std::map<std::string, Profile*>& profiles() const {
			return _profiles;
		}

		void add(Profile* profile) {
			_profiles[profile->name()] = profile;
		}

		void writeJson(FILE* file) {
			for(auto i = _profiles.begin(); i != _profiles.end(); ++i) {
				i->second->writeJson(file);
			}
		}

		ProfileManager() {
			//check the time it takes to call the timing function
			add(&_baseProfile);
			for(uint64_t i=0; i < 100 * 1000; ++i) {
				_baseProfile.start(Werk::epochTime());
				_baseProfile.stop(Werk::epochTime());
			}
		}
	};
}