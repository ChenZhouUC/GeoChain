#ifndef _BASIC_ELEMENTS_
#define _BASIC_ELEMENTS_

#include <limits>
#include <math.h>
#include <iomanip>
#include "LogGuardian.h"
#include "Global.h"

namespace GeoChain {
namespace Euclidean {

/**
 * @brief Euclidean Dimension
 * This is coupled with the class Point. The at most 3 dimension coordinates
 * match x - y - z sequentially. The sequence rule adopted here is RIGHTHANDED
 * FRAME.
 */
enum kDimension { EUC0D = 0, EUC1D = 1, EUC2D = 2, EUC3D = 3 };
enum kMaturityStatus { MATR = -1, INIT = 0, PARM = 1, DESC = 2, GENR = 3 };

/**
 * @brief Basic Point
 * This is the basic Point class.
 */
class Point {
 public:
	// problem dimension
	const kDimension dim_ = EUC0D;

	// definition status
	kMaturityStatus status_ = INIT;

	// Euclidean coordinates
	// Here may consider using DOUBLE for coordinates. However, issue in
	// computational geometry may not using such a large scale. Any probelm
	// can be rescaled into a proper range that could fit FLOAT precision.
	float x_ = 0.0, y_ = 0.0, z_ = 0.0;

	// init
	Point(kDimension dim) : dim_(dim) {
		if (this->dim_ > 0) {
			this->status_ = INIT;
		} else {
			this->status_ = MATR;
		}
	};
	Point(kDimension dim, float x) : dim_(dim), x_(x) {
		if (this->dim_ < 1) {
			LOG(ERROR) << "please make sure the dimension matches the coordinates. having (x) but dim = " << this->dim_;
		} else if (this->dim_ == 1) {
			this->status_ = MATR;
		} else {
			this->status_ = INIT;
		}
	};
	Point(kDimension dim, float x, float y) : dim_(dim), x_(x), y_(y) {
		if (this->dim_ < 2) {
			LOG(ERROR) << "please make sure the dimension matches the coordinates. having both (x,y) but dim = "
								 << this->dim_;
		} else if (this->dim_ == 2) {
			this->status_ = MATR;
		} else {
			this->status_ = INIT;
		}
	};
	Point(kDimension dim, float x, float y, float z) : dim_(dim), x_(x), y_(y), z_(z) {
		if (this->dim_ < 3) {
			LOG(ERROR) << "please make sure the dimension matches the coordinates. having all (x,y,z) but dim = "
								 << this->dim_;
		} else if (this->dim_ == 3) {
			this->status_ = MATR;
		} else {
			this->status_ = INIT;
		}
	};
	~Point(void){};
};

/**
 * @brief Basic Line
 * This is the basic Line class.
 */
class Line {
 public:
	// problem dimension
	const kDimension dim_ = EUC0D;

	// definition status
	kMaturityStatus status_ = INIT;

	// parametric equation
	// (a,b,c) is for 2D line while (d,e,f,g,h) is prepared for 3D
	float a_ = 0.0, b_ = 0.0, c_ = 0.0, d_ = 0.0, e_ = 0.0, f_ = 0.0, g_ = 0.0, h_ = 0.0;

	// descriptive coordinates
	// radian system: [-PI, PI)
	//              theta: X-Y ——> [-PI, PI)
	//              phi: [X-Y]-Z ——> [-PI/2, PI/2]
	Point center_;
	float theta_ = 0.0, phi_ = 0.0;

	// generative coordinates
	// intercept:
	//          2D: X axis(intersect with X) ——> Y axis(parallel to X)
	//          3D: X-Y plane(intersect with X-Y) ——> Y-Z plane (intersect with Y-Z) ——> Z-X plane (parallel to Y)
	Point intercept_;

	// init
	Line(kDimension dim, kMaturityStatus status = INIT)
			: dim_(dim), center_(Point(dim)), intercept_(Point(dim)), status_(status) {
		if (this->dim_ < 1) {
			LOG(ERROR) << "please make sure the dimension of class Line > 0. getting dim = " << this->dim_;
			this->status_ = INIT;
		} else if (this->dim_ == 1) {
			this->status_ = MATR;
		} else {
			this->status_ = INIT;
		}
	};
	// parametric
	Line(kDimension dim, float a, float b, float c, float d = 0, float e = 0, float f = 0, float g = 0, float h = 0,
			 kMaturityStatus status = PARM)
			: dim_(dim),
				center_(Point(dim)),
				intercept_(Point(dim)),
				a_(a),
				b_(b),
				c_(c),
				d_(d),
				e_(e),
				f_(f),
				g_(g),
				h_(h),
				status_(status) {
		if (this->dim_ < 2) {
			LOG(ERROR) << "please make sure the dimension matches the parameters. defined parametrics but dim = "
								 << this->dim_;
			this->status_ = INIT;
		} else {
			if ((this->dim_ == 2 && this->a_ == 0 && this->b_ == 0) ||
					(this->dim_ == 3 &&
					 ((this->a_ == 0 && this->b_ == 0 && this->c_ == 0) || (this->e_ == 0 && this->f_ == 0 && this->g_ == 0)))) {
				LOG(ERROR) << "please make sure the defined parametrics effective with dim = " << this->dim_;
				this->status_ = INIT;
			} else {
				this->status_ = PARM;
			}
		}
	};
	// descriptive & generative
	Line(kDimension dim, kMaturityStatus status, Point point, float theta, float phi = 0)
			: dim_(dim), center_(dim), intercept_(dim), theta_(theta), phi_(phi), status_(status) {
		if (this->dim_ < 2 || (this->status_ != DESC && this->status_ != GENR)) {
			LOG(WARNING)
					<< "please make sure the dimension matches the parameters. defined descriptives | generatives but dim = "
					<< this->dim_;
			this->status_ = INIT;
		} else {
			if (point.status_ != MATR || point.dim_ != this->dim_ ||
					(this->dim_ == 2 && (this->theta_ < -M_PIf32 || this->theta_ >= M_PIf32) ||
					 (this->dim_ == 3 && (this->theta_ < -M_PIf32 || this->theta_ >= M_PIf32) &&
						(this->phi_ < -M_PI_2f32 || this->phi_ > M_PI_2f32)))) {
				LOG(ERROR) << "please make sure the defined descriptives | generatives effective with dim = " << this->dim_;
				this->status_ = INIT;
			} else {
				if (this->status_ == DESC) {
					// descriptive
					this->center_.x_ = point.x_;
					this->center_.y_ = point.y_;
					this->center_.status_ = MATR;
				} else {
					// generative
					this->intercept_.x_ = point.x_;
					this->intercept_.y_ = point.y_;
					this->intercept_.status_ = MATR;
				}
			}
		}
	};

	~Line(void){};

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
								<< this->center_.y_ << "," << this->center_.z_ << ") status: " << this->center_.status_
								<< "\n" + g_GlobalVars.visualize_indent_content << "intercept = (" << this->intercept_.x_ << ","
								<< this->intercept_.y_ << "," << this->intercept_.z_ << ") status: " << this->intercept_.status_;
			return true;
		}
	}
	// Maturate: fulfill all the effective member parameters
	bool Maturate() {
		if (this->status_ == INIT) {
			return false;
		} else if (this->status_ == MATR) {
			return true;
		} else {
			if (this->dim_ == 2) {
				if (this->status_ == PARM) {
					if (this->a_ == 0.0) {
						// parallel to X axis
						this->center_.x_ = 0.0;
						this->center_.y_ = -this->c_ / this->b_;
						this->center_.status_ = MATR;

						this->intercept_.x_ = this->center_.x_;
						this->intercept_.y_ = this->center_.y_;
						this->intercept_.status_ = MATR;

						this->theta_ = 0.0;

					} else {
						// intersect with X axis
						this->intercept_.x_ = -this->c_ / this->a_;
						this->intercept_.y_ = 0.0;
						this->intercept_.status_ = MATR;

						this->center_.x_ = this->intercept_.x_;
						this->center_.y_ = this->intercept_.y_;
						this->center_.status_ = MATR;

						if (this->b_ == 0.0) {
							this->theta_ = M_PI_2f32;
						} else {
							this->theta_ = atanf32(-this->a_ / this->b_);
						}
					}
					this->status_ = MATR;
				} else if (this->status_ == DESC) {
					this->a_ = sinf32(this->theta_);
					this->b_ = -cosf32(this->theta_);
					this->c_ = -(this->a_ * this->center_.x_ + this->b_ * this->center_.y_);

					if (this->theta_ == 0.0 || this->theta_ == -M_PIf32) {
						this->intercept_.x_ = 0.0;
						this->intercept_.y_ = this->center_.y_;
						this->intercept_.status_ = MATR;
					} else if (this->theta_ == M_PI_2f32 || this->theta_ == -M_PI_2f32) {
						this->intercept_.x_ = this->center_.x_;
						this->intercept_.y_ = 0.0;
						this->intercept_.status_ = MATR;
					} else {
						this->intercept_.x_ = this->center_.x_ - this->center_.y_ / tanf32(this->theta_);
						this->intercept_.y_ = 0.0;
						this->intercept_.status_ = MATR;
					}
					this->status_ = MATR;
				} else if (this->status_ = GENR) {
					this->a_ = sinf32(this->theta_);
					this->b_ = -cosf32(this->theta_);
					this->c_ = -(this->a_ * this->intercept_.x_ + this->b_ * this->intercept_.y_);

					this->center_.x_ = this->intercept_.x_;
					this->center_.y_ = this->intercept_.y_;
					this->center_.status_ = MATR;
					this->status_ = MATR;
				} else {
					// IMPOSSIBLE
					LOG(ERROR) << "class Line status error! " << this->status_;
					this->status_ = INIT;
					return false;
				}
			} else if (this->dim_ == 3) {
				// TODO DIM==3
				LOG(WARNING) << "DIM == 3 not implemented yet!";
				this->status_ = INIT;
				return false;
			} else {
				LOG(ERROR) << "class Line property error! Dim = " << this->dim_;
				this->status_ = INIT;
				return false;
			}
		}
		return true;
	}
};

};	// namespace Euclidean
}	// namespace GeoChain

#endif