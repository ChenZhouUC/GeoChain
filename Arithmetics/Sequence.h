#ifndef _SEQUENCE_
#define _SEQUENCE_

namespace GeoChain {
/**
 * @brief Well-Order Relationship
 * let (a, b) = ORD   if a < b;
 *            = EQN   if a == b;
 *            = INV   if a > b
 */
enum kWellOrder { INV = -1, EQN = 0, ORD = 1 };

namespace Euclidean {

kWellOrder PointCoordSequence(Point* pt_1, Point* pt_2, float epsilon = 0.0) {
	if (pt_1->status_ == INIT || pt_2->status_ == INIT) {
		LOG(ERROR) << "points haven't been initiated completedly!";
		return EQN;
	} else if (pt_1->dim_ != pt_2->dim_ || pt_1->dim_ == 0) {
		LOG(ERROR) << "point dims do not match: pt1-" << pt_1->dim_ << " pt2-" << pt_2->dim_;
		return EQN;
	}
	if (pt_1->dim_ == 1) {
		if (pt_1->x_ < pt_2->x_ - epsilon) {
			return ORD;
		} else if (pt_1->x_ > pt_2->x_ + epsilon) {
			return INV;
		} else {
			return EQN;
		}
	} else if (pt_1->dim_ == 2) {
		if (pt_1->x_ < pt_2->x_ - epsilon) {
			return ORD;
		} else if (pt_1->x_ > pt_2->x_ + epsilon) {
			return INV;
		} else {
			if (pt_1->y_ < pt_2->y_ - epsilon) {
				return ORD;
			} else if (pt_1->y_ > pt_2->y_ + epsilon) {
				return INV;
			} else {
				return EQN;
			}
		}
	} else if (pt_1->dim_ == 3) {
		if (pt_1->x_ < pt_2->x_ - epsilon) {
			return ORD;
		} else if (pt_1->x_ > pt_2->x_ + epsilon) {
			return INV;
		} else {
			if (pt_1->y_ < pt_2->y_ - epsilon) {
				return ORD;
			} else if (pt_1->y_ > pt_2->y_ + epsilon) {
				return INV;
			} else {
				if (pt_1->z_ < pt_2->z_ - epsilon) {
					return ORD;
				} else if (pt_1->z_ > pt_2->z_ + epsilon) {
					return INV;
				} else {
					return EQN;
				}
			}
		}
	} else {
		LOG(ERROR) << "point Line property error! Dim = " << pt_1->dim_;
		return EQN;
	}
};

}	// namespace Euclidean
}	// namespace GeoChain

#endif