#include "Units.hpp"

namespace Werk
{

const UnitsT STORAGE_UNITS = {
	{ "P", 1024ul * 1024 * 1024 * 1024 * 1024 },
	{ "T", 1024ul * 1024 * 1024 * 1024 },
	{ "G", 1024ul * 1024 * 1024 },
	{ "M", 1024ul * 1024 },
	{ "K", 1024ul },
};
const UnitsT TIME_UNITS = {
	{ "d", 24ul * 60 * 60 * 1000 * 1000 * 1000 },
	{ "h", 60ul * 60 * 1000 * 1000 * 1000 },
	{ "m", 60ul * 1000 * 1000 * 1000 },
	{ "s", 1000ul * 1000 * 1000 },
	{ "ms", 1000ul * 1000 },
	{ "us", 1000ul },
	{ "ns", 1ul },
};

uint64_t parseUnits(const std::string &value, const UnitsT &units)
{
	//Parse out the number
	size_t n = 0;
	bool hadDecimal = false;
	while (n < value.length()) {
		char ch = value[n];
		bool isDecimal = ch == '.';

		//Digits are ok, the negative sign can be the first thing, and there can be a decimal point
		if (('0' <= ch && ch <= '9') || (n == 0 && ch == '-') || (!hadDecimal && isDecimal)) {
			n += 1;
			if (isDecimal) {
				hadDecimal = true;
			}
		} else {
			break;
		}
	}

	//Were there no units?
	if (n == value.length()) {
		return std::stoull(value);
	}

	//Find the units, default multiplier of 1
	uint64_t multiplier = 1;
	std::string unitStr = value.substr(n);
	for (const auto &unit : units) {
		if (unit.first == unitStr) {
			multiplier = unit.second;
			break;
		}
	}

	//Load it as a double so decimals will work
	double baseValue = std::stod(value.substr(0, n));
	return static_cast<uint64_t>(baseValue * multiplier);
}

}