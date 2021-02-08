#ifndef _ELEMENTARY_ARITHMETICS_
#define _ELEMENTARY_ARITHMETICS_

#include "ElementaryElements.h"

namespace GeoChain {
namespace Euclidean {

Point LineIntersection(Line *l1, Line *l2) {
	if (l1->dim_ != l2->dim_) {
		LOG(ERROR) << "line dimension not match!";
		return Point(EUC0D);
	} else if (l1->status_ != MATR || l2->status_ != MATR) {
		LOG(ERROR) << "lines status should be MATR!";
		return Point(EUC0D);
	}
	if (l1->dim_ == EUC1D) {
		LOG(WARNING) << "lines dim = 1, return any point.";
		return Point(EUC1D);
	} else if (l1->dim_ == EUC2D) {
		if (tanf32(l1->theta_) == tanf32(l2->theta_)) {
			// parallel
			if (l2->a_ * l1->center_.x_ + l2->b_ * l1->center_.y_ + l2->c_ == 0) {
				// function match
				LOG(WARNING) << "lines superposition, return any point.";
				return Point(EUC2D);
			} else {
				LOG(WARNING) << "lines parallel.";
				return Point(EUC0D);
			}
		} else {
			// cross
			float divider_ = l2->a_ * l1->b_ - l1->a_ * l2->b_;
			float x_ = (l2->b_ * l1->c_ - l1->b_ * l2->c_) / divider_;
			float y_ = (l1->a_ * l2->c_ - l2->a_ * l1->c_) / divider_;
			return Point(EUC2D, x_, y_);
		}
	} else {
		// TODO DIM==3
		LOG(WARNING) << "DIM == 3 not implemented yet!";
		return Point(EUC0D);
	}
}

bool PointInSegment(Point *pt, Segment *sgmnt) {
	if (pt->dim_ != sgmnt->dim_) {
		LOG(ERROR) << "point & segment dimension not match!";
		return false;
	} else if (pt->status_ != MATR || sgmnt->status_ != MATR) {
		LOG(ERROR) << "point or segment not initiated properly!";
		return false;
	}
	if (pt->dim_ == 1) {
		return (pt->x_ - sgmnt->terminal_vertex_1_.x_) * (pt->x_ - sgmnt->terminal_vertex_2_.x_) <= 0;
	} else if (pt->dim_ == 2) {
		if (sgmnt->a_ * pt->x_ + sgmnt->b_ * pt->y_ + sgmnt->c_ == 0) {
			// match the function on the line
			return ((pt->x_ - sgmnt->terminal_vertex_1_.x_) * (pt->x_ - sgmnt->terminal_vertex_2_.x_) <= 0) &&
						 ((pt->y_ - sgmnt->terminal_vertex_1_.y_) * (pt->y_ - sgmnt->terminal_vertex_2_.y_) <= 0);
		} else {
			return false;
		}
	} else {
		// TODO DIM==3
		LOG(WARNING) << "DIM == 3 not implemented yet!";
		return false;
	}
}

float CrossProduct2D(Point *vec1, Point *vec2) {
	if (vec1->dim_ != vec1->dim_) {
		LOG(ERROR) << "vector dimension not match!";
		return 0;
	} else if (vec1->dim_ != EUC2D) {
		LOG(ERROR) << "CrossProduct2D only accept dimension 2!";
		return 0;
	}
	return vec1->x_ * vec2->y_ - vec1->y_ * vec2->x_;
}

Point SegmentIntersection(Segment *s1, Segment *s2) {
	if (s1->dim_ != s2->dim_) {
		LOG(ERROR) << "segment dimension not match!";
		return Point(EUC0D);
	} else if (s1->status_ != MATR || s2->status_ != MATR) {
		LOG(ERROR) << "segments not initiated properly!";
		return Point(EUC0D);
	}
	Point intersect_(s1->dim_);
	if (s1->length_ == 0) {
		if (PointInSegment(&(s1->terminal_vertex_1_), s2)) {
			intersect_.x_ = s1->terminal_vertex_1_.x_;
			intersect_.y_ = s1->terminal_vertex_1_.y_;
			intersect_.z_ = s1->terminal_vertex_1_.z_;
			intersect_.status_ = MATR;
		} else {
			intersect_.status_ = INIT;
		}
	} else if (s2->length_ == 0) {
		if (PointInSegment(&(s2->terminal_vertex_1_), s1)) {
			intersect_.x_ = s2->terminal_vertex_1_.x_;
			intersect_.y_ = s2->terminal_vertex_1_.y_;
			intersect_.z_ = s2->terminal_vertex_1_.z_;
			intersect_.status_ = MATR;
		} else {
			intersect_.status_ = INIT;
		}
	} else {
		if (intersect_.dim_ == 1) {
			if (s1->terminal_vertex_1_.x_ == s2->terminal_vertex_1_.x_) {
				if ((s1->terminal_vertex_1_.x_ - s1->terminal_vertex_2_.x_) *
								(s1->terminal_vertex_1_.x_ - s2->terminal_vertex_2_.x_) <
						0) {
					intersect_.x_ = s2->terminal_vertex_1_.x_;
					intersect_.y_ = s2->terminal_vertex_1_.y_;
					intersect_.z_ = s2->terminal_vertex_1_.z_;
					intersect_.status_ = MATR;
				} else {
					intersect_.status_ = INIT;
				}
			} else if (s1->terminal_vertex_1_.x_ == s2->terminal_vertex_2_.x_) {
				if ((s1->terminal_vertex_1_.x_ - s1->terminal_vertex_2_.x_) *
								(s1->terminal_vertex_1_.x_ - s2->terminal_vertex_1_.x_) <
						0) {
					intersect_.x_ = s2->terminal_vertex_1_.x_;
					intersect_.y_ = s2->terminal_vertex_1_.y_;
					intersect_.z_ = s2->terminal_vertex_1_.z_;
					intersect_.status_ = MATR;
				} else {
					intersect_.status_ = INIT;
				}
			} else if (s1->terminal_vertex_2_.x_ == s2->terminal_vertex_1_.x_) {
				if ((s1->terminal_vertex_2_.x_ - s1->terminal_vertex_1_.x_) *
								(s1->terminal_vertex_2_.x_ - s2->terminal_vertex_2_.x_) <
						0) {
					intersect_.x_ = s2->terminal_vertex_2_.x_;
					intersect_.y_ = s2->terminal_vertex_2_.y_;
					intersect_.z_ = s2->terminal_vertex_2_.z_;
					intersect_.status_ = MATR;
				} else {
					intersect_.status_ = INIT;
				}
			} else if (s1->terminal_vertex_2_.x_ == s2->terminal_vertex_2_.x_) {
				if ((s1->terminal_vertex_2_.x_ - s1->terminal_vertex_1_.x_) *
								(s1->terminal_vertex_2_.x_ - s2->terminal_vertex_1_.x_) <
						0) {
					intersect_.x_ = s2->terminal_vertex_2_.x_;
					intersect_.y_ = s2->terminal_vertex_2_.y_;
					intersect_.z_ = s2->terminal_vertex_2_.z_;
					intersect_.status_ = MATR;
				} else {
					intersect_.status_ = INIT;
				}
			} else {
				intersect_.status_ = INIT;
			}
		} else if (intersect_.dim_ == 2) {
			if (tanf32(s1->theta_) == tanf32(s2->theta_)) {
				// parallel only to check one dim
				if (s1->theta_ != M_PI_2f32) {
					if (s1->terminal_vertex_1_.x_ == s2->terminal_vertex_1_.x_) {
						if ((s1->terminal_vertex_1_.x_ - s1->terminal_vertex_2_.x_) *
										(s1->terminal_vertex_1_.x_ - s2->terminal_vertex_2_.x_) <
								0) {
							intersect_.x_ = s2->terminal_vertex_1_.x_;
							intersect_.y_ = s2->terminal_vertex_1_.y_;
							intersect_.z_ = s2->terminal_vertex_1_.z_;
							intersect_.status_ = MATR;
						} else {
							intersect_.status_ = INIT;
						}
					} else if (s1->terminal_vertex_1_.x_ == s2->terminal_vertex_2_.x_) {
						if ((s1->terminal_vertex_1_.x_ - s1->terminal_vertex_2_.x_) *
										(s1->terminal_vertex_1_.x_ - s2->terminal_vertex_1_.x_) <
								0) {
							intersect_.x_ = s2->terminal_vertex_1_.x_;
							intersect_.y_ = s2->terminal_vertex_1_.y_;
							intersect_.z_ = s2->terminal_vertex_1_.z_;
							intersect_.status_ = MATR;
						} else {
							intersect_.status_ = INIT;
						}
					} else if (s1->terminal_vertex_2_.x_ == s2->terminal_vertex_1_.x_) {
						if ((s1->terminal_vertex_2_.x_ - s1->terminal_vertex_1_.x_) *
										(s1->terminal_vertex_2_.x_ - s2->terminal_vertex_2_.x_) <
								0) {
							intersect_.x_ = s2->terminal_vertex_2_.x_;
							intersect_.y_ = s2->terminal_vertex_2_.y_;
							intersect_.z_ = s2->terminal_vertex_2_.z_;
							intersect_.status_ = MATR;
						} else {
							intersect_.status_ = INIT;
						}
					} else if (s1->terminal_vertex_2_.x_ == s2->terminal_vertex_2_.x_) {
						if ((s1->terminal_vertex_2_.x_ - s1->terminal_vertex_1_.x_) *
										(s1->terminal_vertex_2_.x_ - s2->terminal_vertex_1_.x_) <
								0) {
							intersect_.x_ = s2->terminal_vertex_2_.x_;
							intersect_.y_ = s2->terminal_vertex_2_.y_;
							intersect_.z_ = s2->terminal_vertex_2_.z_;
							intersect_.status_ = MATR;
						} else {
							intersect_.status_ = INIT;
						}
					} else {
						intersect_.status_ = INIT;
					}
				} else {
					if (s1->terminal_vertex_1_.y_ == s2->terminal_vertex_1_.y_) {
						if ((s1->terminal_vertex_1_.y_ - s1->terminal_vertex_2_.y_) *
										(s1->terminal_vertex_1_.y_ - s2->terminal_vertex_2_.y_) <
								0) {
							intersect_.x_ = s2->terminal_vertex_1_.x_;
							intersect_.y_ = s2->terminal_vertex_1_.y_;
							intersect_.z_ = s2->terminal_vertex_1_.z_;
							intersect_.status_ = MATR;
						} else {
							intersect_.status_ = INIT;
						}
					} else if (s1->terminal_vertex_1_.y_ == s2->terminal_vertex_2_.y_) {
						if ((s1->terminal_vertex_1_.y_ - s1->terminal_vertex_2_.y_) *
										(s1->terminal_vertex_1_.y_ - s2->terminal_vertex_1_.y_) <
								0) {
							intersect_.x_ = s2->terminal_vertex_1_.x_;
							intersect_.y_ = s2->terminal_vertex_1_.y_;
							intersect_.z_ = s2->terminal_vertex_1_.z_;
							intersect_.status_ = MATR;
						} else {
							intersect_.status_ = INIT;
						}
					} else if (s1->terminal_vertex_2_.y_ == s2->terminal_vertex_1_.y_) {
						if ((s1->terminal_vertex_2_.y_ - s1->terminal_vertex_1_.y_) *
										(s1->terminal_vertex_2_.y_ - s2->terminal_vertex_2_.y_) <
								0) {
							intersect_.x_ = s2->terminal_vertex_2_.x_;
							intersect_.y_ = s2->terminal_vertex_2_.y_;
							intersect_.z_ = s2->terminal_vertex_2_.z_;
							intersect_.status_ = MATR;
						} else {
							intersect_.status_ = INIT;
						}
					} else if (s1->terminal_vertex_2_.y_ == s2->terminal_vertex_2_.y_) {
						if ((s1->terminal_vertex_2_.y_ - s1->terminal_vertex_1_.y_) *
										(s1->terminal_vertex_2_.y_ - s2->terminal_vertex_1_.y_) <
								0) {
							intersect_.x_ = s2->terminal_vertex_2_.x_;
							intersect_.y_ = s2->terminal_vertex_2_.y_;
							intersect_.z_ = s2->terminal_vertex_2_.z_;
							intersect_.status_ = MATR;
						} else {
							intersect_.status_ = INIT;
						}
					} else {
						intersect_.status_ = INIT;
					}
				}
			} else {
				// cross
				Point ab_ = Point(EUC2D, s1->terminal_vertex_2_.x_ - s1->terminal_vertex_1_.x_,
													s1->terminal_vertex_2_.y_ - s1->terminal_vertex_1_.y_);
				Point ac_ = Point(EUC2D, s2->terminal_vertex_1_.x_ - s1->terminal_vertex_1_.x_,
													s2->terminal_vertex_1_.y_ - s1->terminal_vertex_1_.y_);
				Point ad_ = Point(EUC2D, s2->terminal_vertex_2_.x_ - s1->terminal_vertex_1_.x_,
													s2->terminal_vertex_2_.y_ - s1->terminal_vertex_1_.y_);
				Point cd_ = Point(EUC2D, s2->terminal_vertex_2_.x_ - s2->terminal_vertex_1_.x_,
													s2->terminal_vertex_2_.y_ - s2->terminal_vertex_1_.y_);
				Point ca_ = Point(EUC2D, s1->terminal_vertex_1_.x_ - s2->terminal_vertex_1_.x_,
													s1->terminal_vertex_1_.y_ - s2->terminal_vertex_1_.y_);
				Point cb_ = Point(EUC2D, s1->terminal_vertex_2_.x_ - s2->terminal_vertex_1_.x_,
													s1->terminal_vertex_2_.y_ - s2->terminal_vertex_1_.y_);
				if (CrossProduct2D(&ab_, &ac_) * CrossProduct2D(&ad_, &ab_) >= 0 &&
						CrossProduct2D(&cd_, &ca_) * CrossProduct2D(&cb_, &cd_) >= 0) {
					// intersect
					Point inter_ = LineIntersection(s1, s2);
					intersect_.x_ = inter_.x_;
					intersect_.y_ = inter_.y_;
					intersect_.z_ = inter_.z_;
					intersect_.status_ = MATR;
				} else {
					intersect_.status_ = INIT;
				}
			}
		} else {
			// TODO DIM==3
			LOG(WARNING) << "DIM == 3 not implemented yet!";
			intersect_.status_ = INIT;
		}
	}

	return intersect_;
}

}	// namespace Euclidean
}	// namespace GeoChain

#endif