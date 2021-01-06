#ifndef _CONFIG_
#define _CONFIG_

#include <jsoncpp/json/json.h>
#include "Global.h"
#include "LogGuardian.h"

namespace whale {
namespace vision {

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
	globalKeys.device_key = "DEVICE";
	globalKeys.calib_key = "CALIB";
	globalKeys.algo_key = "ALGO";
	globalKeys.demo_key = "DEMO";

	globalKeys.key_sn_file = "SN_FILE";
	globalKeys.key_camera_sn_file = "CAMERA_SN_FILE";

	globalKeys.key_std_file = "STD_FILE";
	globalKeys.key_std_img = "STD_IMG";
	globalKeys.key_calib_file = "CALIB_FILE";
	globalKeys.key_calib_img = "CALIB_IMG";
	globalKeys.key_size_key = "SIZE_KEY";
	globalKeys.key_pos_key = "POS_KEY";
	globalKeys.key_periodic = "PERIODIC";
	globalKeys.key_periodic_max = "PERIODIC_MAX";
	globalKeys.key_kpt_num = "KPT_NUM";

	globalKeys.key_size_w = "SIZE_W";
	globalKeys.key_size_h = "SIZE_H";
	globalKeys.key_sift_kp_num = "SIFT_KP_NUM";
	globalKeys.key_match_dist_gap = "MATCH_DIST_GAP";
	globalKeys.key_bghist = "BGHIST";
	globalKeys.key_bgthresh2 = "BGTHRESH2";
	globalKeys.key_bgshadow = "BGSHADOW";
	globalKeys.key_median_blur = "MEDIAN_BLUR";
	globalKeys.key_morph_size = "MORPH_SIZE";
	globalKeys.key_dilate_size = "DILATE_SIZE";
	globalKeys.key_mvmnt_thresh = "MVMNT_THRESH";
	globalKeys.key_video_input = "VIDEO_INPUT";
	globalKeys.key_view_rotation = "VIEW_ROTATION";
	globalKeys.key_motion_thresh = "MOTION_THRESH";
	globalKeys.key_hip_orientation = "HIP_ORIENTATION_TOP";
	globalKeys.key_hip_score_max = "HIP_SCORE_MAX";
	globalKeys.key_hip_activate_score = "HIP_ACTIVATE_SCORE";
	globalKeys.key_hip_tol_ratio = "HIP_TOL_RATIO";
	globalKeys.key_vibe_nsample = "VIBE_NSAMPLE";
	globalKeys.key_vibe_minmatch = "VIBE_MINMATCH";
	globalKeys.key_vibe_radius = "VIBE_RADIUS";
	globalKeys.key_vibe_updatebg = "VIBE_UPDATE_BG";
	globalKeys.key_vibe_updatefg = "VIBE_UPDATE_FG";
	globalKeys.key_vibe_ghost_max = "VIBE_GHOST_MAX";
	globalKeys.key_vibe_ghost_active = "VIBE_GHOST_ACTIVE";
	globalKeys.key_vibe_gray = "VIBE_GRAY";
	globalKeys.key_amplified_mvmnt = "AMPLIFIED_MVMNT";

	globalKeys.key_show = "SHOW";
	globalKeys.key_display = "DISPLAY";
	globalKeys.key_display_size_w = "DISPLAY_SIZE_W";
	globalKeys.key_display_size_h = "DISPLAY_SIZE_H";
}

void LoadingConfig(Json::Value &root_config) {
	LoadStringLikeKV(root_config, globalKeys.device_key, globalKeys.key_sn_file,
									 globalVars.sn_file);
	LoadStringLikeKV(root_config, globalKeys.device_key,
									 globalKeys.key_camera_sn_file, globalVars.camera_sn_file);

	LoadStringLikeKV(root_config, globalKeys.calib_key, globalKeys.key_std_file,
									 globalVars.std_file);
	LoadStringLikeKV(root_config, globalKeys.calib_key, globalKeys.key_std_img,
									 globalVars.std_img);
	LoadStringLikeKV(root_config, globalKeys.calib_key, globalKeys.key_calib_file,
									 globalVars.calib_file);
	LoadStringLikeKV(root_config, globalKeys.calib_key, globalKeys.key_calib_img,
									 globalVars.calib_img);
	LoadStringLikeKV(root_config, globalKeys.calib_key, globalKeys.key_size_key,
									 globalVars.size_key);
	LoadStringLikeKV(root_config, globalKeys.calib_key, globalKeys.key_pos_key,
									 globalVars.pos_key);
	LoadStringLikeKV(root_config, globalKeys.calib_key, globalKeys.key_periodic,
									 globalVars.periodic);
	LoadStringLikeKV(root_config, globalKeys.calib_key,
									 globalKeys.key_periodic_max, globalVars.periodic_max);
	LoadStringLikeKV(root_config, globalKeys.calib_key, globalKeys.key_kpt_num,
									 globalVars.kpt_num);

	LoadStringLikeKV(root_config, globalKeys.algo_key, globalKeys.key_video_input,
									 globalVars.video_input);
	LoadStringLikeKV(root_config, globalKeys.algo_key, globalKeys.key_size_w,
									 globalVars.algo_size.width);
	LoadStringLikeKV(root_config, globalKeys.algo_key, globalKeys.key_size_h,
									 globalVars.algo_size.height);
	LoadStringLikeKV(root_config, globalKeys.algo_key, globalKeys.key_sift_kp_num,
									 globalVars.sift_kp_num);
	LoadStringLikeKV(root_config, globalKeys.algo_key,
									 globalKeys.key_match_dist_gap, globalVars.match_dist_gap);
	LoadStringLikeKV(root_config, globalKeys.algo_key, globalKeys.key_bghist,
									 globalVars.bg_hist);
	LoadStringLikeKV(root_config, globalKeys.algo_key, globalKeys.key_bgthresh2,
									 globalVars.bg_thresh2);
	LoadStringLikeKV(root_config, globalKeys.algo_key, globalKeys.key_bgshadow,
									 globalVars.bg_shadow);
	LoadStringLikeKV(root_config, globalKeys.algo_key, globalKeys.key_median_blur,
									 globalVars.median_blur);
	LoadStringLikeKV(root_config, globalKeys.algo_key, globalKeys.key_morph_size,
									 globalVars.morph_size);
	LoadStringLikeKV(root_config, globalKeys.algo_key, globalKeys.key_dilate_size,
									 globalVars.dilate_size);
	LoadStringLikeKV(root_config, globalKeys.algo_key,
									 globalKeys.key_mvmnt_thresh, globalVars.mvmnt_thresh);
	LoadStringLikeKV(root_config, globalKeys.algo_key,
									 globalKeys.key_view_rotation, globalVars.rotation_mult);
	LoadStringLikeKV(root_config, globalKeys.algo_key,
									 globalKeys.key_motion_thresh, globalVars.motion_thresh);
	LoadStringLikeKV(root_config, globalKeys.algo_key,
									 globalKeys.key_hip_orientation, globalVars.hip_orientation);
	LoadStringLikeKV(root_config, globalKeys.algo_key,
									 globalKeys.key_hip_score_max, globalVars.hip_score_max);
	LoadStringLikeKV(root_config, globalKeys.algo_key,
									 globalKeys.key_hip_activate_score,
									 globalVars.hip_activate_score);
	LoadStringLikeKV(root_config, globalKeys.algo_key,
									 globalKeys.key_hip_tol_ratio, globalVars.hip_tol_ratio);
	LoadStringLikeKV(root_config, globalKeys.algo_key,
									 globalKeys.key_vibe_ghost_active,
									 globalVars.vibe_ghost_active);
	LoadStringLikeKV(root_config, globalKeys.algo_key,
									 globalKeys.key_vibe_ghost_max, globalVars.vibe_ghost_max);
	LoadStringLikeKV(root_config, globalKeys.algo_key,
									 globalKeys.key_vibe_minmatch, globalVars.vibe_minmatch);
	LoadStringLikeKV(root_config, globalKeys.algo_key,
									 globalKeys.key_vibe_nsample, globalVars.vibe_nsample);
	LoadStringLikeKV(root_config, globalKeys.algo_key, globalKeys.key_vibe_radius,
									 globalVars.vibe_radius);
	LoadStringLikeKV(root_config, globalKeys.algo_key,
									 globalKeys.key_vibe_updatebg, globalVars.vibe_updatebg);
	LoadStringLikeKV(root_config, globalKeys.algo_key,
									 globalKeys.key_vibe_updatefg, globalVars.vibe_updatefg);
	LoadStringLikeKV(root_config, globalKeys.algo_key, globalKeys.key_vibe_gray,
									 globalVars.vibe_gray);
	LoadStringLikeKV(root_config, globalKeys.algo_key,
									 globalKeys.key_amplified_mvmnt, globalVars.amplified_mvmnt);

	LoadStringLikeKV(root_config, globalKeys.demo_key, globalKeys.key_show,
									 globalVars.show_results);
	LoadStringLikeKV(root_config, globalKeys.demo_key, globalKeys.key_display,
									 globalVars.demo_display);
	LoadStringLikeKV(root_config, globalKeys.demo_key,
									 globalKeys.key_display_size_w,
									 globalVars.display_size.width);
	LoadStringLikeKV(root_config, globalKeys.demo_key,
									 globalKeys.key_display_size_h,
									 globalVars.display_size.height);
}

void ParseLabelFile(Json::Value &root_label, cv::Size &img_size,
										std::vector<std::string> &itemIds) {
	int _width = root_label[globalVars.size_key][1].asInt();
	int _height = root_label[globalVars.size_key][0].asInt();
	img_size = cv::Size(_width, _height);

	Json::Value::Members members =
			root_label[globalVars.pos_key].getMemberNames();
	for (auto &&_iter : members) {
		itemIds.push_back(_iter);
	}
}

}	// namespace vision
}	// namespace whale

#endif