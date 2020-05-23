#include "PolygonUtils/PolygonUtils.h"
#include <iostream>

float FastIOU(Polygon &_poly_0, Polygon &_poly_1, Polygon &_inter) {
	float area0 = computeArea(_poly_0.pt, _poly_0.n);
	float area1 = computeArea(_poly_1.pt, _poly_1.n);
	intersectPolygonSHPC(&_poly_0, &_poly_1, &_inter);
	float area = computeArea(_inter.pt, _inter.n);
	float iou = area / (area0 + area1 - area);

	return iou;
}

int main() {
	Polygon _poly_0, _poly_1;
	Polygon _inter;

	_poly_0.add(CvPoint(13, 245));
	_poly_0.add(CvPoint(216, 409));
	_poly_0.add(CvPoint(395, 14));
	_poly_0.add(CvPoint(58, 37));

	_poly_1.add(CvPoint(14, 357));
	_poly_1.add(CvPoint(234, 136));
	_poly_1.add(CvPoint(198, 35));
	_poly_1.add(CvPoint(43, 25));

	std::cout << "============== FastIOU ==============" << std::endl;
	clock_t tic_2 = clock();
	float iou_output = FastIOU(_poly_0, _poly_1, _inter);
	clock_t toc_2 = clock();
	std::cout << "total consuming: " << toc_2 - tic_2 << "ms IOU: " << iou_output
						<< std::endl;
}