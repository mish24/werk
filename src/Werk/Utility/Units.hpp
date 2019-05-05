#pragma once

#include <string>
#include <vector>

namespace Werk {

	typedef std::vector<std::pair<std::string, uint64_t> > UnitsT;

	extern const UnitsT STORAGE_UNITS;
	extern const UnitsT TIME_UNITS;

	/*
	Calculates the info needed to fromat the values with units
	the scaled value, and the units themselves
	*/
	void formatUnits(uint64_t value, const UnitsT& units, double& scaledValue, const char* unitStr);
	
	/*
	parses a value with a given sort of units
	*/
	uint64_t parseUnits(const std::string& value, const UnitsT& units);
}