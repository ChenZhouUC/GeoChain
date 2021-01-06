#include "SweepLine/SweepLineAlgo.cpp"
#include <cstring>

struct CoordPair {
	double x_, y_;
	CoordPair(double _x, double _y) : x_(_x), y_(_y) {}
};

class Quadrangle {
 public:
	std::vector<Point> point_clkws_;
	std::vector<Segment> edge_clkws_;
	std::vector<Segment> sgmnt_clkws_;

 public:
	/* Quadrangle Constructor
	This constructor initiate a Quadrangle object with input of clockwise arranged
	coordinates pairs.
	*/
	Quadrangle(CoordPair _A, CoordPair _B, CoordPair _C, CoordPair _D,
						 std::string _QuadName) {
		char *_a, *_b, *_c, *_d;
		if (_QuadName == "alpha") {
			_a = (char *)"A";
			_b = (char *)"B";
			_c = (char *)"C";
			_d = (char *)"D";
		} else {
			_a = (char *)"H";
			_b = (char *)"I";
			_c = (char *)"J";
			_d = (char *)"K";
		}
		point_clkws_.push_back(Point(*_a, _A.x_, _A.y_));
		point_clkws_.push_back(Point(*_b, _B.x_, _B.y_));
		point_clkws_.push_back(Point(*_c, _C.x_, _C.y_));
		point_clkws_.push_back(Point(*_d, _D.x_, _D.y_));

		edge_clkws_.push_back(
				Segment(Point(*_a, _A.x_, _A.y_), Point(*_b, _B.x_, _B.y_)));
		edge_clkws_.push_back(
				Segment(Point(*_b, _B.x_, _B.y_), Point(*_c, _C.x_, _C.y_)));
		edge_clkws_.push_back(
				Segment(Point(*_c, _C.x_, _C.y_), Point(*_d, _D.x_, _D.y_)));
		edge_clkws_.push_back(
				Segment(Point(*_d, _D.x_, _D.y_), Point(*_a, _A.x_, _A.y_)));
	}
};

bool IsPointInQuadrangle(Point &point, Quadrangle &quad) {
	double vec1_x, vec1_y, vec2_x, vec2_y;
	for (int idx = 0; idx < quad.point_clkws_.size(); idx++) {
		vec1_x = quad.point_clkws_[idx].x - point.x;
		vec1_y = quad.point_clkws_[idx].y - point.y;
		vec2_x =
				quad.point_clkws_[(idx + 1) % quad.point_clkws_.size()].x - point.x;
		vec2_y =
				quad.point_clkws_[(idx + 1) % quad.point_clkws_.size()].y - point.y;

		if (vec1_x * vec2_y - vec1_y * vec2_x > 0) {
			return false;
		} else {
			continue;
		}
	}
	return true;
}

// void ExtractIntersectArea(Quadrangle alpha, Quadrangle beta,
// 													std::vector<Point> intersects,
// 													std::vector<Segment> segments) {}