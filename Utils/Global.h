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
	int visualize_placeholder;
	int visualize_precision;
	float visualize_standardize;
	int visualize_spacer;
	int visualize_line_width;
	int visualize_point_size;
	float visualize_font_scale;
	int visualize_font_thickness;
	int visualize_window_width;
	int visualize_window_height;

	float convention_epsilon;
	float convention_infinity;
	int convention_error_code;
	int convention_element_id;
};

struct GlobalKey {
	std::string project_key;

	std::string key_name;
	std::string key_desc;
	std::string key_patent;

	std::string visualize_key;

	std::string key_indent;
	std::string key_placeholder;
	std::string key_precision;
	std::string key_standardize;
	std::string key_spacer;
	std::string key_line_width;
	std::string key_point_size;
	std::string key_font_scale;
	std::string key_font_thickness;
	std::string key_window_width;
	std::string key_window_height;

	std::string convention_key;

	std::string key_epsilon;
	std::string key_infinity;
	std::string key_error_code;
	std::string key_element_id;
};

enum kDataType { STR, FLT, INT, BOL };

}	// namespace Utils
}	// namespace GeoChain

extern GeoChain::Utils::GlobalVar g_GlobalVars;
extern GeoChain::Utils::GlobalKey g_GlobalKeys;

#endif