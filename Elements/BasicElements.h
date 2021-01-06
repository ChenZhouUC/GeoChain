#include <limits>
#include <iostream>

namespace GeoChain {
namespace Euclidean {

/**
 * @brief Euclidean Dimension
 * This is coupled with the class Point. The at most 3 dimension coordinates
 * match x - y - z sequentially. The sequence rule adopted here is RIGHTHANDED
 * FRAME.
 */
enum Dimension { _1D = 1, _2D = 2, _3D = 3 };

/**
 * @brief Basic Point
 * This is the basic Point class.
 */
class Point {
 public:
	// problem dimension
	const Dimension dim;

	// Euclidean coordinates
	// Here may consider using DOUBLE for coordinates. However, issue in
	// computational geometry may not using such a large scale. Any probelm
	// can be rescaled into a proper range that could fit FLOAT precision.
	float x, y, z;

	Point(Dimension dim) : dim(dim){};
	Point(Dimension dim, float x) : dim(dim), x(x){};
	Point(Dimension dim, float x, float y) : dim(dim), x(x), y(y){};
	Point(Dimension dim, float x, float y, float z)
			: dim(dim), x(x), y(y), z(z){};
	~Point(void){};
};

/**
 * @brief Basic Line
 * This is the basic Point class.
 */
class Point {
 public:
	// problem dimension
	const Dimension dim;

	// Euclidean coordinates
	// Here may consider using DOUBLE for coordinates. However, issue in
	// computational geometry may not using such a large scale. Any probelm
	// can be rescaled into a proper range that could fit FLOAT precision.
	float x, y, z;

	Point(Dimension dim) : dim(dim){};
	~Point(void){};
};

}	// namespace Euclidean
}	// namespace GeoChain