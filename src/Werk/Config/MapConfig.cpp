#include "MapConfig.hpp"

#include <fstream>

namespace Werk {

	bool MapConfig::loadFromFile(const char* filename) {
		std::ifstream infile(filename);
		std::string line;
		while(std::getline(infile, line)) {
			//trim beginning of the line 
			std::size_t firstNonWhitespacePos = line.find_first_not_of(' ');
			std::size_t firstNonWhitespace = firstNonWhitespacePos == std::string::npos ? line.length() : firstNonWhitespacePos;
			line = line.substr(firstNonWhitespace);

			//skip blank lines and comments
			if(line.length() == 0 || line[0] == '#') {
				continue;
			}

			//find the =
			std::size_t equalPos = line.find('=');
			if(equalPos == std::string::npos) {
				continue;
			}

			_values[line.substr(0, equalPos)] = line.substr(equalPos + 1);
		}

		return true;
	}
}