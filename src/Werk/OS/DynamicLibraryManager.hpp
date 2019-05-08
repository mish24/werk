#pragma once

#include <map>
#include <string>

namespace Werk {

	class DynamicLibrary {
	protected:
		void* _handle;
	public:
		DynamicLibrary(void* handle) : _handle(handle){}
		~DynamicLibrary();

		void* getSymbol(const char* name);
	};

	/*
	loads dynamic libraries and caches them, functional infra for
	capabilities like on the fly code generation and loading
	*/
	class DynamicLibraryManager {

	protected:
		std::map<std::string, DynamicLibrary> _libraries;

	public:
		~DynamicLibraryManager();

		DynamicLibrary* load(const std::string& path);
		void unload(const std::string& path);
	};
}