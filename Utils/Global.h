#ifndef _GLOBAL_
#define _GLOBAL_

#include <fstream>
#include <iostream>
#include <string>

struct GlobalVar {
	std::string sn_file;
	std::string camera_sn_file;
	char device_sn[20];
	char camera_sn[20];
	int frames_hearbeat;
	int vt_shift_index = 50001;

	std::string std_file;
	std::string std_img;
	std::string calib_file;
	std::string calib_img;
	std::string size_key;
	std::string pos_key;
	std::string json_config;
	int periodic;
	int periodic_max;
	int kpt_num;

	cv::Size algo_size;
	cv::Size label_size;
	std::vector<std::string> itemIds;
	cv::Mat STD_IMG;
	cv::Mat CALIB_IMG;
	int sift_kp_num;
	float match_dist_gap;
	std::vector<cv::Point2f> match_filtered_std;
	std::vector<cv::Point2f> match_filtered_calib;
	int bg_hist;
	float bg_thresh2;
	bool bg_shadow;
	int median_blur;
	int morph_size;
	int dilate_size;
	float mvmnt_thresh;
	std::string video_input;
	int rotation_mult;
	float motion_thresh;
	bool hip_orientation;
	float hip_score_max;
	float hip_activate_score;
	float hip_tol_ratio;
	int vibe_nsample;
	int vibe_minmatch;
	int vibe_radius;
	int vibe_updatebg;
	int vibe_updatefg;
	int vibe_ghost_max;
	int vibe_ghost_active;
	bool vibe_gray;
	float amplified_mvmnt;

	bool show_results;
	bool demo_display;
	cv::Size display_size;
};

extern GlobalVar globalVars;

struct GlobalKey {
	std::string device_key;
	std::string calib_key;
	std::string algo_key;
	std::string demo_key;

	std::string key_sn_file;
	std::string key_camera_sn_file;

	std::string key_std_file;
	std::string key_std_img;
	std::string key_calib_file;
	std::string key_calib_img;
	std::string key_size_key;
	std::string key_pos_key;
	std::string key_periodic;
	std::string key_periodic_max;
	std::string key_kpt_num;

	std::string key_size_w;
	std::string key_size_h;
	std::string key_sift_kp_num;
	std::string key_match_dist_gap;
	std::string key_bghist;
	std::string key_bgthresh2;
	std::string key_bgshadow;
	std::string key_median_blur;
	std::string key_morph_size;
	std::string key_dilate_size;
	std::string key_mvmnt_thresh;
	std::string key_video_input;
	std::string key_view_rotation;
	std::string key_motion_thresh;
	std::string key_hip_score_max;
	std::string key_hip_activate_score;
	std::string key_hip_tol_ratio;
	std::string key_vibe_nsample;
	std::string key_vibe_minmatch;
	std::string key_vibe_radius;
	std::string key_vibe_updatebg;
	std::string key_vibe_updatefg;
	std::string key_vibe_ghost_max;
	std::string key_vibe_ghost_active;
	std::string key_vibe_gray;
	std::string key_amplified_mvmnt;

	std::string key_show;
	std::string key_display;
	std::string key_display_size_w;
	std::string key_display_size_h;
	std::string key_hip_orientation;
};

extern GlobalKey globalKeys;

enum DataType { STR, FLT, INT, BOL };

#endif