#ifndef _ELEMENTARY_ELEMENTS_
#define _ELEMENTARY_ELEMENTS_

#include "BasicArithmetics.h"
#include "BasicElements.h"

namespace GeoChain {
namespace Euclidean {

/**
 * @brief Basic HalfLine
 * This is the inherited from the Line class.
 */
class HalfLine : public Line {
 public:
	// intercept or not
	bool intercept_state_;

	// init
	HalfLine(kDimension dim, kMaturityStatus status = INIT) : Line(dim, status){};

	// descriptive
	HalfLine(kDimension dim, kMaturityStatus status, Point point, float theta, float phi = 0)
			: Line(dim, status, point, theta, phi) {
		if (this->status_ != DESC) {
			LOG(ERROR) << "HalfLine shoudl be initiated using descriptives!";
			this->status_ = INIT;
		} else {
			if (this->dim_ == 2) {
				float dir_x = cosf32(this->theta_);
				float dir_y = sinf32(this->theta_);

				if (this->theta_ == 0 || this->theta_ == -M_PI) {
					this->intercept_state_ = (this->center_.x_ * dir_x <= 0);
				} else {
					this->intercept_state_ = (this->center_.y_ * dir_y <= 0);
				}
			} else if (this->dim_ == 3) {
				// TODO DIM==3
				LOG(WARNING) << "DIM == 3 not implemented yet!";
				this->status_ = INIT;
			} else {
				LOG(WARNING) << "class HalfLine property error! Dim = " << this->dim_;
				this->status_ = INIT;
			}
		}
	}
	// Maturate: fulfill all the effective member parameters
	using Line::Maturate;
	// Describe: output all the effective member parameters
	bool Describe() {
		if (this->status_ == INIT) {
			LOG(WARNING) << "this Line instance is under initiation status!";
			return false;
		} else {
			LOG(INFO) << std::setprecision(g_GlobalVars.visualize_precision) << "Dim = " << this->dim_
								<< "\n" + g_GlobalVars.visualize_indent_content << "(a,b,c,d,e,f,g,h) = " << this->a_ << "," << this->b_
								<< "," << this->c_ << "," << this->d_ << "," << this->e_ << "," << this->f_ << "," << this->g_ << ","
								<< this->h_ << "\n" + g_GlobalVars.visualize_indent_content << "(theta, phi) = " << this->theta_ << ","
								<< this->phi_ << "\n" + g_GlobalVars.visualize_indent_content << "center = (" << this->center_.x_ << ","
								<< this->center_.y_ << ")"
								<< "\n" + g_GlobalVars.visualize_indent_content << "intercept = (" << this->intercept_.x_ << ","
								<< this->intercept_.y_ << ") state: " << this->intercept_state_;
			return true;
		}
	}
};

/**
 * @brief Basic Segment
 * This is the inherited from the Line class.
 */
class Segment : public HalfLine {
 public:
	// terminal vertices
	Point terminal_vertex_1_, terminal_vertex_2_;
	// segment length
	float length_;

	typedef float (*ptrDistanceFunc)(Point, Point);
	ptrDistanceFunc distance_func_;

	// init
	Segment(kDimension dim, kMaturityStatus status = INIT)
			: HalfLine(dim, status), terminal_vertex_1_(Point(dim)), terminal_vertex_2_(Point(dim)){};

	// descriptive
	Segment(kDimension dim, kMaturityStatus status, Point terminal_vertex_1, Point terminal_vertex_2,
					ptrDistanceFunc distance_func = EuclideanDistance)
			: HalfLine(dim),
				terminal_vertex_1_(terminal_vertex_1),
				terminal_vertex_2_(terminal_vertex_2),
				distance_func_(distance_func) {
		if (status != DESC || this->dim_ != this->terminal_vertex_1_.dim_ || this->dim_ != this->terminal_vertex_2_.dim_ ||
				this->terminal_vertex_1_.status_ != MATR || this->terminal_vertex_2_.status_ != MATR) {
			LOG(ERROR) << "Segment should be initiated using descriptives. the initial dim of class Segment should be "
										"matching those of terminal vertices whose status must "
										"be MATURE!";
			this->status_ = INIT;
		} else {
			this->status_ = status;
			this->length_ = this->distance_func_(this->terminal_vertex_1_, this->terminal_vertex_2_);
			if (this->length_ < 0.0) {
				LOG(ERROR) << "Segment distance function invalid!";
				this->status_ = INIT;
			} else if (this->length_ == 0.0) {
				// DESC: superposition of two terminal vertices
				this->center_.x_ = this->terminal_vertex_1_.x_;
				this->center_.y_ = this->terminal_vertex_1_.y_;
				this->center_.z_ = this->terminal_vertex_1_.z_;
				this->center_.status_ = MATR;

				this->status_ = DESC;
			} else {
				// length > 0 line exists
				if (this->dim_ == 2) {
					if (this->terminal_vertex_1_.y_ == this->terminal_vertex_2_.y_) {
						this->theta_ = 0.0;
						if (this->terminal_vertex_1_.x_ * this->terminal_vertex_2_.x_ <= 0) {
							this->intercept_state_ = true;
						} else {
							this->intercept_state_ = false;
						}
						this->center_.x_ = this->terminal_vertex_1_.x_;
						this->center_.y_ = this->terminal_vertex_1_.y_;
					} else {
						if (this->terminal_vertex_1_.y_ * this->terminal_vertex_2_.y_ <= 0) {
							this->intercept_state_ = true;
						} else {
							this->intercept_state_ = false;
						}
						this->center_.x_ = this->terminal_vertex_1_.x_;
						this->center_.y_ = this->terminal_vertex_1_.y_;

						if (this->terminal_vertex_1_.x_ == this->terminal_vertex_2_.x_) {
							this->theta_ = M_PI_2;
						} else {
							this->theta_ = atanf32((this->terminal_vertex_2_.y_ - this->terminal_vertex_1_.y_) /
																		 (this->terminal_vertex_2_.x_ - this->terminal_vertex_1_.x_));
						}
					}
					if (this->Maturate()) {
						this->status_ = MATR;
					} else {
						LOG(WARNING) << "Segment initiation failed using descriptives!";
						this->status_ = INIT;
					}

				} else if (this->dim_ == 3) {
					// TODO DIM==3
					LOG(WARNING) << "DIM == 3 not implemented yet!";
					this->status_ = INIT;
				} else {
					LOG(WARNING) << "class Line property error! Dim = " << this->dim_;
					this->status_ = INIT;
				}
			}
		}
	};

	// Maturate: fulfill all the effective member parameters
	using HalfLine::Maturate;

	// Describe: output all the effective member parameters
	bool Describe() {
		if (this->status_ == INIT) {
			LOG(WARNING) << "this Line instance is under initiation status!";
			return false;
		} else {
			LOG(INFO) << std::setprecision(g_GlobalVars.visualize_precision) << "Dim = " << this->dim_
								<< "\n" + g_GlobalVars.visualize_indent_content << "(a,b,c,d,e,f,g,h) = " << this->a_ << "," << this->b_
								<< "," << this->c_ << "," << this->d_ << "," << this->e_ << "," << this->f_ << "," << this->g_ << ","
								<< this->h_ << "\n" + g_GlobalVars.visualize_indent_content << "(theta, phi) = " << this->theta_ << ","
								<< this->phi_ << "\n" + g_GlobalVars.visualize_indent_content << "center = (" << this->center_.x_ << ","
								<< this->center_.y_ << ")"
								<< "\n" + g_GlobalVars.visualize_indent_content << "intercept = (" << this->intercept_.x_ << ","
								<< this->intercept_.y_ << ") state: " << this->intercept_state_
								<< "\n" + g_GlobalVars.visualize_indent_content << "terminal vertices: (" << this->terminal_vertex_1_.x_
								<< "," << this->terminal_vertex_1_.y_ << "), (" << this->terminal_vertex_2_.x_ << ","
								<< this->terminal_vertex_2_.y_ << ") length: " << this->length_;
			return true;
		}
	}
};

}	// namespace Euclidean
}	// namespace GeoChain

#endif