#pragma once

#include <string>
#include <vector>

namespace Werk {

	typedef std::vector<std::pair<std::string, uint64_t> > UnitsT;

	extern const UnitsT STORAGE_UNITS;
	extern const UnitsT TIME_UNITS;

	/*
	parses a value with a given sort of units
	*/
	uint64_t parseUnits(const std::string& value, const UnitsT& units);
}