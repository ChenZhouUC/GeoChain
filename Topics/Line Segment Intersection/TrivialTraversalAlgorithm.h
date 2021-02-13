#include "ElementaryElements.h"
#include "BinaryTree.h"
#include "ElementaryArithmetics.h"

#include <algorithm>

namespace GeoChain {
namespace Algorithms {

using namespace Euclidean;

struct PointSegmentAffiliationTraversal {
	kDimension dim_;
	Point *point_;
	std::vector<Segment *> segments_ = std::vector<Segment *>();
	std::vector<int> seg_indeces_ = std::vector<int>();
	int num_ = 0;

	PointSegmentAffiliationTraversal(kDimension dim, int num_segs, Point *pt) : dim_(dim), point_(pt) {
		this->segments_.reserve(num_segs);
		this->seg_indeces_.reserve(num_segs);
	};

	PointSegmentAffiliationTraversal(kDimension dim, int num_segs) : dim_(dim) {
		this->segments_.reserve(num_segs);
		this->seg_indeces_.reserve(num_segs);
	};
	// if only data but not ptr here is useful, reservation is unnecessary
	PointSegmentAffiliationTraversal(kDimension dim) : dim_(dim){};
};

std::vector<PointSegmentAffiliationTraversal> SegmentIntersectionTraversal(std::vector<Segment> *ptr_segs,
																																					 std::vector<Point> *intersection_pts,
																																					 bool simplify_opt = true) {
	std::map<int, std::vector<int>> jump_dict;
	std::vector<PointSegmentAffiliationTraversal> inter_info;

	for (int i_ = 0; i_ < (*ptr_segs).size() - 1; i_++) {
		std::vector<PointSegmentAffiliationTraversal> inter_info_tmp;
		std::vector<int> jump_list;
		if (simplify_opt) {
			std::map<int, std::vector<int>>::iterator ptr_jump_tmp = jump_dict.find(i_);
			if (ptr_jump_tmp != jump_dict.end()) {
				jump_list = ptr_jump_tmp->second;
			} else {
				jump_list = std::vector<int>();
			}
		}
		for (int j_ = i_ + 1; j_ < (*ptr_segs).size(); j_++) {
			if (simplify_opt) {
				std::vector<int>::iterator ptr_jump_ = find(jump_list.begin(), jump_list.end(), j_);
				if (ptr_jump_ != jump_list.end()) {
					continue;
				}
			}
			Point intersection_ = SegmentIntersection(&((*ptr_segs)[i_]), &((*ptr_segs)[j_]));
			if (intersection_.status_ != MATR) {
				continue;
			}
			int match_index_ = -1;
			for (int inter_ = 0; inter_ < inter_info_tmp.size(); inter_++) {
				if (PointCoordSequence(inter_info_tmp[inter_].point_, &intersection_, g_GlobalVars.convention_epsilon) != EQN) {
					// not match
				} else {
					// match
					match_index_ = inter_;
					break;
				}
			}
			if (match_index_ > 0) {
				// match one tmp
				inter_info_tmp[match_index_].segments_.push_back(&((*ptr_segs)[j_]));
				inter_info_tmp[match_index_].num_ += 1;
				if (simplify_opt) {
					for (int index_ = 1; index_ < inter_info_tmp[match_index_].seg_indeces_.size(); index_++) {
						std::map<int, std::vector<int>>::iterator ptr_jump_index_tmp = jump_dict.find(index_);
						if (ptr_jump_index_tmp != jump_dict.end()) {
							jump_dict[index_] = std::vector<int>(j_);
						} else {
							jump_dict[index_].push_back(j_);
						}
					}
					inter_info_tmp[match_index_].seg_indeces_.push_back(j_);
				}
				// LOG(INFO) << "find one existing intersection: (" << intersection_.x_ << "," << intersection_.y_ << ")";
			} else {
				if (simplify_opt) {
					// new one
					intersection_pts->push_back(intersection_);
					PointSegmentAffiliationTraversal info_tmp(EUC2D, (*ptr_segs).size(), &(intersection_pts->back()));
					info_tmp.segments_.push_back(&((*ptr_segs)[i_]));
					info_tmp.segments_.push_back(&((*ptr_segs)[j_]));
					info_tmp.num_ += 2;
					info_tmp.seg_indeces_.push_back(i_);
					info_tmp.seg_indeces_.push_back(j_);
					inter_info_tmp.push_back(info_tmp);
					// LOG(INFO) << "find one new intersection: (" << intersection_.x_ << "," << intersection_.y_ << ")";
				} else {
					// new one or previously generated
					for (int inter_ = 0; inter_ < inter_info.size(); inter_++) {
						if (PointCoordSequence(inter_info[inter_].point_, &intersection_, g_GlobalVars.convention_epsilon) != EQN) {
							// not match
						} else {
							// match
							match_index_ = inter_;
							break;
						}
					}
					if (match_index_ > 0) {
						// match one previously generated
					} else {
						// new one
						intersection_pts->push_back(intersection_);
						PointSegmentAffiliationTraversal info_tmp(EUC2D, (*ptr_segs).size(), &(intersection_pts->back()));
						info_tmp.segments_.push_back(&((*ptr_segs)[i_]));
						info_tmp.segments_.push_back(&((*ptr_segs)[j_]));
						info_tmp.num_ += 2;
						inter_info_tmp.push_back(info_tmp);
						// LOG(INFO) << "find one new intersection: (" << intersection_.x_ << "," << intersection_.y_ << ")";
					}
				}
			}
		}
		inter_info.insert(inter_info.end(), inter_info_tmp.begin(), inter_info_tmp.end());
	}
	return inter_info;
}
}	// namespace Algorithms
}	// namespace GeoChain