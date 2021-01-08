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

	int visualize_indent;
	std::string visualize_indent_content;
};

struct GlobalKey {
	std::string project_key;

	std::string key_name;
	std::string key_desc;
	std::string key_patent;

	std::string visualize_key;

	std::string key_indent;
};

enum kDataType { STR, FLT, INT, BOL };

}	// namespace Utils
}	// namespace GeoChain

extern GeoChain::Utils::GlobalVar g_GlobalVars;
extern GeoChain::Utils::GlobalKey g_GlobalKeys;

#endif