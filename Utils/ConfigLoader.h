#ifndef _CONFIG_
#define _CONFIG_

#include <jsoncpp/json/json.h>
#include "Global.h"
#include "LogGuardian.h"

namespace GeoChain {
namespace Utils {

bool ReadJsonFile(std::string &jsonFileName, Json::Value &root_group,
									bool collCommOpt = false) {
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

bool WriteJsonFile(std::string &jsonFileName, Json::Value &root_group) {
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

void LoadStringLikeKV(Json::Value &root_group, std::string &level_1,
											std::string &level_2, std::string &returnData,
											DataType returnType = STR) {
	returnData = root_group[level_1][level_2].asString();
}

void LoadStringLikeKV(Json::Value &root_group, std::string &level_1,
											std::string &level_2, float &returnData,
											DataType returnType = FLT) {
	returnData = root_group[level_1][level_2].asFloat();
}

void LoadStringLikeKV(Json::Value &root_group, std::string &level_1,
											std::string &level_2, int &returnData,
											DataType returnType = INT) {
	returnData = root_group[level_1][level_2].asInt();
}

void LoadStringLikeKV(Json::Value &root_group, std::string &level_1,
											std::string &level_2, bool &returnData,
											DataType returnType = BOL) {
	returnData = root_group[level_1][level_2].asBool();
}

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

void DefineGlobalKeys() {
	globalKeys.project_key = "PROJECT";
	globalKeys.key_name = "NAME";
	globalKeys.key_desc = "DESC";
	globalKeys.key_patent = "PATENT";
}

void LoadingConfig(Json::Value &root_config) {
	LoadStringLikeKV(root_config, globalKeys.project_key, globalKeys.key_name,
									 globalVars.project_name);
	LoadStringLikeKV(root_config, globalKeys.project_key, globalKeys.key_desc,
									 globalVars.project_desc);
	LoadStringLikeKV(root_config, globalKeys.project_key, globalKeys.key_patent,
									 globalVars.project_patent);
}

}	// namespace Utils
}	// namespace GeoChain

#endif