#include <limits>
#include <math.h>
#include <cmath>
#include "LogGuardian.h"

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
	const kDimension dim;

	// definition status
	kMaturityStatus status;

	// Euclidean coordinates
	// Here may consider using DOUBLE for coordinates. However, issue in
	// computational geometry may not using such a large scale. Any probelm
	// can be rescaled into a proper range that could fit FLOAT precision.
	float x, y, z;

	// init
	Point(kDimension dim) : dim(dim) {
		if (dim > 0) {
			status = INIT;
		} else {
			status = MATR;
		}
	};
	Point(kDimension dim, float x) : dim(dim), x(x) {
		if (dim < 1) {
			LOG(ERROR) << "please make sure the dimension matches the coordinates. having (x) but dim = " << dim;
		} else if (dim == 1) {
			status = MATR;
		} else {
			status = INIT;
		}
	};
	Point(kDimension dim, float x, float y) : dim(dim), x(x), y(y) {
		if (dim < 2) {
			LOG(ERROR) << "please make sure the dimension matches the coordinates. having both (x,y) but dim = " << dim;
		} else if (dim == 2) {
			status = MATR;
		} else {
			status = INIT;
		}
	};
	Point(kDimension dim, float x, float y, float z) : dim(dim), x(x), y(y), z(z) {
		if (dim < 3) {
			LOG(ERROR) << "please make sure the dimension matches the coordinates. having all (x,y,z) but dim = " << dim;
		} else if (dim == 3) {
			status = MATR;
		} else {
			status = INIT;
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
	const kDimension dim;

	// definition status
	kMaturityStatus status;

	// parametric equation
	// (a,b,c) is for 2D line while (d,e,f,g,h) is prepared for 3D
	float a, b, c, d, e, f, g, h;

	// descriptive coordinates
	// radian system: [0, 2·PI)
	//              theta: X-Y
	//              phi: [X-Y]-Z
	Point center;
	float theta, phi;

	// generative coordinates
	// intercept:
	//          2D: X axis(intersect with X) ——> Y axis(parallel to X)
	//          3D: X-Y plane(intersect with X-Y) ——> Y-Z plane (intersect with Y-Z) ——> Z-X plane (parallel to Y)
	Point intercept;

	// init
	Line(kDimension dim) : dim(dim), center(Point(dim)), intercept(Point(dim)) {
		if (dim < 1) {
			LOG(ERROR) << "please make sure the dimension of class Line > 0. getting dim = " << dim;
		} else if (dim == 1) {
			status = MATR;
		} else {
			status = INIT;
		}
	};
	// parametric
	Line(kDimension dim, float a, float b, float c, float d = 0, float e = 0, float f = 0, float g = 0, float h = 0,
			 kMaturityStatus status = PARM)
			: dim(dim), center(Point(dim)), intercept(Point(dim)), a(a), b(b), c(c), d(d), e(e), f(f), g(g), h(h) {
		if (dim < 2) {
			LOG(ERROR) << "please make sure the dimension matches the parameters. defined parametrics but dim = " << dim;
		} else {
			if ((dim == 2 && a == 0 && b == 0) ||
					(dim == 3 && ((a == 0 && b == 0 && c == 0) || (e == 0 && f == 0 && g == 0)))) {
				LOG(ERROR) << "please make sure the defined parametrics effective with dim = " << dim;
			} else {
				status = PARM;
			}
		}
	};
	// descriptive & generative
	Line(kDimension dim, kMaturityStatus status, Point point, float theta, float phi = 0)
			: dim(dim), center(dim), intercept(dim), theta(theta), phi(phi), status(status) {
		if (dim < 2 || (status != DESC && status != GENR)) {
			LOG(ERROR)
					<< "please make sure the dimension matches the parameters. defined descriptives | generatives but dim = "
					<< dim;
		} else {
			if (point.status != MATR || point.dim != dim ||
					(dim == 2 && (theta < 0 || theta >= M_PI) ||
					 (dim == 3 && (theta < 0 || theta >= 2 * M_PI) && (phi < 0 || phi > M_PI_2)))) {
				LOG(ERROR) << "please make sure the defined descriptives | generatives effective with dim = " << dim;
			} else {
				if (status == DESC) {
					// descriptive
					center.x = point.x;
					center.y = point.y;
					center.status = MATR;
				} else {
					// generative
					intercept.x = point.x;
					intercept.y = point.y;
					intercept.status = MATR;
				}
			}
		}
	};

	~Line(void){};

	bool Maturate() {
		if (status == INIT) {
			return false;
		} else if (status == MATR) {
			return true;
		} else {
			if (dim == 2) {
				if (status == PARM) {
					if (a == 0.0) {
						// parallel to X axis
						center.x = 0.0;
						center.y = -c / b;
						center.status = MATR;

						intercept.x = center.x;
						intercept.y = center.y;
						intercept.status = MATR;

						theta = 0.0;

					} else {
						// intersect with X axis
						intercept.x = -c / a;
						intercept.y = 0.0;
						intercept.status = MATR;

						center.x = intercept.x;
						center.y = intercept.y;
						center.status = MATR;

						if (b == 0.0) {
							theta = M_PI_2;
						} else {
							theta = atanf32(-a / b);
							if (theta < 0) {
								theta += M_PI;
							}
						}
					}
					status = MATR;
				} else if (status == DESC) {
					a = sinf32(theta);
					b = -cosf32(theta);
					c = -(a * center.x + b * center.y);

					if (theta == 0.0) {
						intercept.x = 0.0;
						intercept.y = center.y;
						intercept.status = MATR;
					} else if (theta == M_PI_2) {
						intercept.x = center.x;
						intercept.y = 0.0;
						intercept.status = MATR;
					} else {
						intercept.x = center.x - center.y / tanf32(theta);
						intercept.y = 0.0;
						intercept.status = MATR;
					}
					status = MATR;
				} else if (status = GENR) {
					a = sinf32(theta);
					b = -cosf32(theta);
					c = -(a * intercept.x + b * intercept.y);

					center.x = intercept.x;
					center.y = intercept.y;
					center.status = MATR;
					status = MATR;
				} else {
					LOG(WARNING) << "class Line status error! " << status;
					return false;
				}
			} else if (dim == 3) {
				// TODO DIM==3
				LOG(WARNING) << "DIM == 3 not implemented yet!";
				return false;
			} else {
				LOG(WARNING) << "class Line property error! Dim = " << dim;
				return false;
			}
		}
		return true;
	}
};

};	// namespace Euclidean
}	// namespace GeoChain
