#include "DynamicLibraryManager.hpp"

#include <dlfcn.h>

namespace Werk {

	DynamicLibrary::~DynamicLibrary() {
		dlclose(_handle);
	}

	DynamicLibraryManager::~DynamicLibraryManager() {
		_libraries.clear();
	}

	DynamicLibrary* DynamicLibraryManager::load(const std::string& path) {
		auto i = _libraries.find(path);
		if(i != _libraries.end()) {
			return &i->second;
		}

		//load it
		void* handle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
		printf("got it? %p %s\n", handle, dlerror());
		if(nullptr != handle) {
			auto j = _libraries.emplace(path, handle);
			if(j.second) {
				return &j.first->second;
			}
			printf("Wtf?\n");
		}
		return nullptr;

	}

	void DynamicLibraryManager::unload(const std::string& path) {
		auto i = _libraries.find(path);
		if( i == _libraries.end()) {
			return;
		}
		//unload it
		_libraries.erase(i);
	}
}