#ifndef _BASIC_ARITHMETICS_
#define _BASIC_ARITHMETICS_

#include "BasicElements.h"

namespace GeoChain {
namespace Euclidean {

float pNormDistance(Point pt_1, Point pt_2, int p) {
	if (p <= 0) {
		LOG(ERROR) << "pNormDistance encountered negative int p!";
		return -3.0;
	}
	if (pt_1.status_ != MATR || pt_2.status_ != MATR) {
		LOG(ERROR) << "pNormDistance encountered immature points!";
		return -2.0;
	} else if (pt_1.dim_ != pt_2.dim_ || pt_1.dim_ <= 0 || pt_1.dim_ > 3) {
		LOG(ERROR) << "pNormDistance encountered points with different dims or dim invalid!";
		return -1.0;
	} else {
		int dim_ = pt_1.dim_;
		if (dim_ == 1) {
			float vec_x = std::abs(pt_1.x_ - pt_2.x_);
			return vec_x;
		} else if (dim_ == 2) {
			float vec_x = std::abs(pt_1.x_ - pt_2.x_);
			float vec_y = std::abs(pt_1.y_ - pt_2.y_);
			return powf32(powf32(vec_x, p) + powf32(vec_y, p), 1.0 / float(p));
		} else if (dim_ == 3) {
			float vec_x = std::abs(pt_1.x_ - pt_2.x_);
			float vec_y = std::abs(pt_1.y_ - pt_2.y_);
			float vec_z = std::abs(pt_1.z_ - pt_2.z_);
			return powf32(powf32(vec_x, p) + powf32(vec_y, p) + powf32(vec_z, p), 1.0 / float(p));
		} else {
			// IMPOSSIBLE
		}
	}
}

float MinkowskiDistance(Point pt_1, Point pt_2, int p) { return pNormDistance(pt_1, pt_2, p); }
// float ChebyshevDistance: MAX NORM
float EuclideanDistance(Point pt_1, Point pt_2) { return pNormDistance(pt_1, pt_2, 2); }
float ManhattanDistance(Point pt_1, Point pt_2) { return pNormDistance(pt_1, pt_2, 1); }

}	// namespace Euclidean
}	// namespace GeoChain

#endif