#include "BasicElements.h"

namespace GeoChain {
namespace Euclidean {

class HalfLine : public Line {
 public:
	// intercept or not
	bool intercept_state;

	// init
	HalfLine(kDimension dim) : Line(dim){};
	/*
	// parametric
	HalfLine(kDimension dim, Point point, float a, float b, float c, float d = 0, float e = 0, float f = 0, float g = 0,
					 float h = 0, kMaturityStatus status = PARM)
			: Line(dim, a, b, c, d, e, f, g, h, status) {
		if (status != PARM) {
			LOG(ERROR) << "HalfLine shoudl be initiated using parametrics!";
			status = INIT;
		} else {
			if (point.dim != dim) {
				LOG(ERROR) << "please make sure the defined center matches with dim = " << dim;
				status = INIT;
			} else {
				if (dim == 2) {
					center.x = point.x;
					center.y = point.y;
					center.status = MATR;

					if (a == 0.0) {
						theta = 0.0;
					} else if (b == 0.0) {
						theta = M_PI_2;
					} else {
						theta = atanf32(-a / b);
					}

				} else {
					// TODO DIM==3
				}
				status = DESC;
			}
		}
	}
	*/
	// descriptive
	HalfLine(kDimension dim, kMaturityStatus status, Point point, float theta, float phi = 0)
			: Line(dim, status, point, theta, phi) {
		if (status != DESC) {
			LOG(ERROR) << "HalfLine shoudl be initiated using descriptives!";
			status = INIT;
		} else {
			if (dim == 2) {
				float dir_x = cosf32(theta);
				float dir_y = sinf32(theta);

				if (theta == 0 || theta == -M_PI) {
					intercept_state = (center.x * dir_x <= 0);
				} else {
					intercept_state = (center.y * dir_y <= 0);
				}
			} else {
				// TODO DIM==3
			}
			status = DESC;
		}
	}
	// Maturate: fulfill all the effective member parameters
	using Line::Maturate;
	// Describe: output all the effective member parameters
	bool Describe() {
		if (status == INIT) {
			LOG(WARNING) << "this Line instance is under initiation status!";
			return false;
		} else {
			LOG(INFO) << "Dim = " << dim << "\n" + g_GlobalVars.visualize_indent_content << "(a,b,c,d,e,f,g,h) = " << a << ","
								<< b << "," << c << "," << d << "," << e << "," << f << "," << g << "," << h
								<< "\n" + g_GlobalVars.visualize_indent_content << "(theta, phi) = " << theta << "," << phi
								<< "\n" + g_GlobalVars.visualize_indent_content << "center = (" << center.x << "," << center.y << ")"
								<< "\n" + g_GlobalVars.visualize_indent_content << "intercept = (" << intercept.x << "," << intercept.y
								<< ") state: " << intercept_state;
			return true;
		}
	}
};

}	// namespace Euclidean
}	// namespace GeoChain