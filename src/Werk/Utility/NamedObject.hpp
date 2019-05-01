#pragma once

#include <string> 

namespace Werk {

	class NamedObject {

	private:
		const std::string _name;

	public:
		NamedObject(const std::string& name) : _name(name) {}
		const std::string& name() { return _name; }
	};
}