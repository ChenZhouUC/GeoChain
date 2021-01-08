#ifndef _GLOBAL_
#define _GLOBAL_

#include <fstream>
#include <iostream>
#include <string>

namespace GeoChain {
namespace Utils {

struct GlobalVar {
	std::string project_name;
	std::string project_desc;
	std::string project_patent;
};

struct GlobalKey {
	std::string project_key;

	std::string key_name;
	std::string key_desc;
	std::string key_patent;
};

enum DataType { STR, FLT, INT, BOL };

}	// namespace Utils
}	// namespace GeoChain

extern GeoChain::Utils::GlobalVar globalVars;
extern GeoChain::Utils::GlobalKey globalKeys;

#endif