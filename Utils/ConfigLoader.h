#ifndef _CONFIG_
#define _CONFIG_

#include <jsoncpp/json/json.h>
#include "Global.h"
#include "LogGuardian.h"

namespace GeoChain {
namespace Utils {

// IsNum: judge if a string is totally digits or not
bool IsNum(std::string s) {
	std::stringstream sin(s);
	double t;
	char p;
	if (!(sin >> t)) return false;
	if (sin >> p)
		return false;
	else
		return true;
}

// GenDuplStr: generate string-like duplicated character
std::string GenDuplStr(const std::string unit_str, int num) {
	std::string rst = "";
	for (int i_ = 0; i_ < num; i_++) {
		rst += unit_str;
	}
	return rst;
}

// ExtractClassName: extract class name from typeid.name()
std::string ExtractClassName(const std::string str) {
	std::string rst_ = "";
	for (int i_ = 0; i_ < str.size() - 1; i_++) {
		int tmp_ = (int)str[str.size() - 2 - i_];
		if (tmp_ >= 48 && tmp_ <= 57) {
			break;
		} else {
			rst_ = str[str.size() - 2 - i_] + rst_;
		}
	}
	return rst_;
}

// CenterizePlaceHolder: centerize a string into a placeholder
std::string CenterizePlaceHolder(std::string &str, int placeholder, int extra_adjust = 0) {
	std::string c_str = "";
	int len_ = (placeholder + extra_adjust - str.size()) / 2;
	c_str += GenDuplStr(" ", len_);
	c_str += str;
	len_ = placeholder + extra_adjust - c_str.size();
	c_str += GenDuplStr(" ", len_);
	return c_str;
};

bool ReadJsonFile(const std::string &jsonFileName, Json::Value &root_group, bool collCommOpt = false) {
	Json::CharReaderBuilder rbuilder;
	rbuilder["collectComments"] = collCommOpt;
	JSONCPP_STRING errs;

	std::fstream f;
	f.open(jsonFileName, std::ios::in);
	if (!f.is_open()) {
		LOG(ERROR) << "[ERROR] ===> Open json file error! " << jsonFileName;
		f.close();
		return false;
	}

	bool parse_ok = Json::parseFromStream(rbuilder, f, &root_group, &errs);
	if (!parse_ok) {
		LOG(ERROR) << "[ERROR] ===> Parse json file error! " << jsonFileName;
		f.close();
		return false;
	} else {
		LOG(INFO) << "[INFO] ===> Parse json file ok! " << jsonFileName;
		// LOG(INFO) << root_group.toStyledString();
		// try {
		// 	LOG(INFO) << root_group["positions"]["0"];
		// 	LOG(INFO) << root_group["size"];
		// } catch (const Json::LogicError &ex) {
		// 	LOG(ERROR) << "Parse json string error!";
		// }
	}

	f.close();
	return true;
}

bool WriteJsonFile(const std::string &jsonFileName, Json::Value &root_group) {
	std::fstream f;
	f.open(jsonFileName, std::ios::out);
	if (!f.is_open()) {
		LOG(ERROR) << "[ERROR] ===> Open json file error! " << jsonFileName;
		return false;
	}
	f << root_group.toStyledString();
	f.close();
	LOG(INFO) << "[INFO] ===> Write json file ok! " << jsonFileName;
	return true;
}

void LoadStringLikeKV(const Json::Value &root_group, std::string &level_1, std::string &level_2,
											std::string &returnData, kDataType returnType = STR) {
	returnData = root_group[level_1][level_2].asString();
}

void LoadStringLikeKV(const Json::Value &root_group, std::string &level_1, std::string &level_2, float &returnData,
											kDataType returnType = FLT) {
	returnData = root_group[level_1][level_2].asFloat();
}

void LoadStringLikeKV(const Json::Value &root_group, std::string &level_1, std::string &level_2, int &returnData,
											kDataType returnType = INT) {
	returnData = root_group[level_1][level_2].asInt();
}

void LoadStringLikeKV(const Json::Value &root_group, std::string &level_1, std::string &level_2, bool &returnData,
											kDataType returnType = BOL) {
	returnData = root_group[level_1][level_2].asBool();
}

void LoadingConfig(const Json::Value &root_config) {
	LoadStringLikeKV(root_config, g_GlobalKeys.project_key, g_GlobalKeys.key_name, g_GlobalVars.project_name);
	LoadStringLikeKV(root_config, g_GlobalKeys.project_key, g_GlobalKeys.key_desc, g_GlobalVars.project_desc);
	LoadStringLikeKV(root_config, g_GlobalKeys.project_key, g_GlobalKeys.key_patent, g_GlobalVars.project_patent);

	LoadStringLikeKV(root_config, g_GlobalKeys.visualize_key, g_GlobalKeys.key_indent, g_GlobalVars.visualize_indent);
	g_GlobalVars.visualize_indent_content = GenDuplStr(" ", g_GlobalVars.visualize_indent);
	LoadStringLikeKV(root_config, g_GlobalKeys.visualize_key, g_GlobalKeys.key_placeholder,
									 g_GlobalVars.visualize_placeholder);
	LoadStringLikeKV(root_config, g_GlobalKeys.visualize_key, g_GlobalKeys.key_precision,
									 g_GlobalVars.visualize_precision);
	LoadStringLikeKV(root_config, g_GlobalKeys.visualize_key, g_GlobalKeys.key_standardize,
									 g_GlobalVars.visualize_standardize);
	LoadStringLikeKV(root_config, g_GlobalKeys.visualize_key, g_GlobalKeys.key_spacer, g_GlobalVars.visualize_spacer);
	LoadStringLikeKV(root_config, g_GlobalKeys.visualize_key, g_GlobalKeys.key_line_width,
									 g_GlobalVars.visualize_line_width);
	LoadStringLikeKV(root_config, g_GlobalKeys.visualize_key, g_GlobalKeys.key_point_size,
									 g_GlobalVars.visualize_point_size);
	LoadStringLikeKV(root_config, g_GlobalKeys.visualize_key, g_GlobalKeys.key_font_scale,
									 g_GlobalVars.visualize_font_scale);
	LoadStringLikeKV(root_config, g_GlobalKeys.visualize_key, g_GlobalKeys.key_font_thickness,
									 g_GlobalVars.visualize_font_thickness);
	LoadStringLikeKV(root_config, g_GlobalKeys.visualize_key, g_GlobalKeys.key_window_width,
									 g_GlobalVars.visualize_window_width);
	LoadStringLikeKV(root_config, g_GlobalKeys.visualize_key, g_GlobalKeys.key_window_height,
									 g_GlobalVars.visualize_window_height);

	LoadStringLikeKV(root_config, g_GlobalKeys.convention_key, g_GlobalKeys.key_infinity,
									 g_GlobalVars.convention_infinity);
	LoadStringLikeKV(root_config, g_GlobalKeys.convention_key, g_GlobalKeys.key_error_code,
									 g_GlobalVars.convention_error_code);
	LoadStringLikeKV(root_config, g_GlobalKeys.convention_key, g_GlobalKeys.key_element_id,
									 g_GlobalVars.convention_element_id);
}

void DefineGlobalKeys() {
	g_GlobalKeys.project_key = "PROJECT";
	g_GlobalKeys.key_name = "NAME";
	g_GlobalKeys.key_desc = "DESC";
	g_GlobalKeys.key_patent = "PATENT";

	g_GlobalKeys.visualize_key = "VISUALIZE";
	g_GlobalKeys.key_indent = "INDENT";
	g_GlobalKeys.key_placeholder = "PLACEHOLDER";
	g_GlobalKeys.key_precision = "PRECISION";
	g_GlobalKeys.key_standardize = "STANDARDIZE";
	g_GlobalKeys.key_spacer = "SPACER";
	g_GlobalKeys.key_line_width = "LINEWIDTH";
	g_GlobalKeys.key_point_size = "POINTSIZE";
	g_GlobalKeys.key_font_scale = "FONTSCALE";
	g_GlobalKeys.key_font_thickness = "FONTTHICKNESS";
	g_GlobalKeys.key_window_height = "WINDOWHEIGHT";
	g_GlobalKeys.key_window_width = "WINDOWWIDTH";

	g_GlobalKeys.convention_key = "CONVENTION";
	g_GlobalKeys.key_infinity = "INFINITY";
	g_GlobalKeys.key_error_code = "ERRORCODE";
	g_GlobalKeys.key_element_id = "ELEMENTID";
}

}	// namespace Utils
}	// namespace GeoChain

#endif