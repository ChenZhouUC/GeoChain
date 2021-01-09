#ifndef _CONFIG_
#define _CONFIG_

#include <jsoncpp/json/json.h>
#include "Global.h"
#include "LogGuardian.h"

namespace GeoChain {
namespace Utils {

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

std::string GenDuplStr(const std::string unit_str, int num) {
	std::string rst = "";
	for (int i_ = 0; i_ < num; i_++) {
		rst += unit_str;
	}
	return rst;
}

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
	LoadStringLikeKV(root_config, g_GlobalKeys.visualize_key, g_GlobalKeys.key_precision,
									 g_GlobalVars.visualize_precision);
}

void DefineGlobalKeys() {
	g_GlobalKeys.project_key = "PROJECT";
	g_GlobalKeys.key_name = "NAME";
	g_GlobalKeys.key_desc = "DESC";
	g_GlobalKeys.key_patent = "PATENT";

	g_GlobalKeys.visualize_key = "VISUALIZE";
	g_GlobalKeys.key_indent = "INDENT";
	g_GlobalKeys.key_precision = "PRECISION";
}

}	// namespace Utils
}	// namespace GeoChain

#endif