#include <opencv2/opencv.hpp>
#include "ElementaryElements.h"

namespace GeoChain {
namespace Euclidean {

class Visualizer2D {
 public:
	kDimension dim_ = EUC2D;
	int spacer_;
	// Euclidean coord + shift = pixel_coord
	float shift_x_, shift_y_;
	std::vector<Point> attendant;

	Visualizer2D(){};
	~Visualizer2D(){};
};

}	// namespace Euclidean
}	// namespace GeoChain