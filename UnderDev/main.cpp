#include "QuadrangleTools.cpp"
#include "PolygonUtils/PolygonUtils.h"
#include <opencv2/opencv.hpp>
#include <ctime>

std::vector<float> PixelTraverseIOU(const CvPoint *points_0, int n_0,
																		const CvPoint *points_1, int n_1) {
	int min_x = points_0[0].x, max_x = points_0[0].x;
	int min_y = points_0[0].y, max_y = points_0[0].y;
	for (int i = 0; i < n_0; i++) {
		if (points_0[i].x > max_x) {
			max_x = points_0[i].x;
		}
		if (points_0[i].x < min_x) {
			min_x = points_0[i].x;
		}
		if (points_0[i].y > max_y) {
			max_y = points_0[i].y;
		}
		if (points_0[i].y < min_y) {
			min_y = points_0[i].y;
		}
	}
	for (int j = 0; j < n_1; j++) {
		if (points_1[j].x > max_x) {
			max_x = points_1[j].x;
		}
		if (points_1[j].x < min_x) {
			min_x = points_1[j].x;
		}
		if (points_1[j].y > max_y) {
			max_y = points_1[j].y;
		}
		if (points_1[j].y < min_y) {
			min_y = points_1[j].y;
		}
	}

	int total_pixel = 0;
	for (int x = min_x; x <= max_x; x++) {
		for (int y = min_y; y <= max_y; y++) {
			CvPoint p;
			p.x = x;
			p.y = y;
			if (pointInPolygon(p, points_0, n_0) &&
					pointInPolygon(p, points_1, n_1)) {
				total_pixel += 1;
			}
		}
	}
	float area0 = computeArea(points_0, n_0);
	float area1 = computeArea(points_1, n_1);
	float total_pixel_f = (float)total_pixel;
	float iou = total_pixel_f / (area0 + area1 - total_pixel_f);

	std::vector<float> output =
			std::vector<float>{iou, area0, area1, total_pixel_f};

	return output;
}

std::vector<float> FastIOU(Polygon &_poly_0, Polygon &_poly_1,
													 Polygon &_inter) {
	float area0 = computeArea(_poly_0.pt, _poly_0.n);
	float area1 = computeArea(_poly_1.pt, _poly_1.n);
	intersectPolygonSHPC(&_poly_0, &_poly_1, &_inter);
	float area = computeArea(_inter.pt, _inter.n);
	float iou = area / (area0 + area1 - area);
	std::vector<float> output = std::vector<float>{iou, area0, area1, area};

	return output;
}

int main() {
	//------------------------------------------------------------------------------------------
	//                             SweepLine Algo Utils
	//------------------------------------------------------------------------------------------

	std::vector<Point> intersections;
	std::vector<Segment> segments = std::vector<Segment>{};

	Quadrangle alpha = Quadrangle(CoordPair(13, 245), CoordPair(216, 409),
																CoordPair(395, 14), CoordPair(58, 37), "alpha");
	Quadrangle beta = Quadrangle(CoordPair(16, 248), CoordPair(219, 412),
															 CoordPair(398, 16), CoordPair(60, 39), "beta");
	segments.insert(segments.end(), alpha.edge_clkws_.begin(),
									alpha.edge_clkws_.end());
	segments.insert(segments.end(), beta.edge_clkws_.begin(),
									beta.edge_clkws_.end());

	clock_t tic_1 = clock();
	intersect(segments, intersections, false);
	clock_t toc_1 = clock();
	std::cout << "============== SweepLine Intersection =============="
						<< std::endl;
	std::cout << "total consuming: "
						<< (double)(toc_1 - tic_1) * 1000.0 / CLOCKS_PER_SEC << "ms"
						<< std::endl;
	std::cout << "Intersection points[" << intersections.size()
						<< "]: " << std::endl;
	for (std::vector<Point>::iterator it = intersections.begin();
			 it != intersections.end(); ++it)
		std::cout << it->letter << "(" << it->x << "," << it->y << ") "
							<< std::endl;
	std::cout << "Segments[" << segments.size() << "]: " << std::endl;
	for (std::vector<Segment>::iterator it = segments.begin();
			 it != segments.end(); ++it)
		std::cout << "[ " << it->beg.letter << "(" << it->beg.x << "," << it->beg.y
							<< "), " << it->end.letter << "(" << it->end.x << "," << it->end.y
							<< ") ] " << std::endl;

	//------------------------------------------------------------------------------------------
	//                             Polygon Util Formatting Converter
	//------------------------------------------------------------------------------------------

	Polygon _poly_0, _poly_1;
	Polygon _inter;

	for (int i = 0; i < alpha.point_clkws_.size(); i++) {
		CvPoint pt =
				cvPoint(int(alpha.point_clkws_[i].x), int(alpha.point_clkws_[i].y));
		_poly_0.add(pt);
	}
	for (int i = 0; i < beta.point_clkws_.size(); i++) {
		CvPoint pt =
				cvPoint(int(beta.point_clkws_[i].x), int(beta.point_clkws_[i].y));
		_poly_1.add(pt);
	}
	std::cout << "============== FastIOU ==============" << std::endl;
	clock_t tic_2 = clock();
	std::vector<float> iou_output = FastIOU(_poly_0, _poly_1, _inter);
	clock_t toc_2 = clock();
	std::cout << "total consuming: "
						<< (double)(toc_2 - tic_2) * 1000.0 / CLOCKS_PER_SEC
						<< "ms IOU: " << iou_output[0] << std::endl;
	// for (int i = 0; i < _inter.size(); i++) {
	// 	std::cout << _inter[i].x << " , " << _inter[i].y << std::endl;
	// }

	std::cout << "============== PixelTraverseIOU ==============" << std::endl;
	clock_t tic_3 = clock();
	std::vector<float> iou_traverse_output =
			PixelTraverseIOU(_poly_0.pt, _poly_0.n, _poly_1.pt, _poly_1.n);
	clock_t toc_3 = clock();
	std::cout << "total consuming: "
						<< (double)(toc_3 - tic_3) * 1000.0 / CLOCKS_PER_SEC
						<< "ms IOU: " << iou_traverse_output[0] << std::endl;

#define SHOW_RESULT
#ifdef SHOW_RESULT
	cv::Mat background(500, 500, CV_8UC3, cv::Scalar(100, 100, 100));
	for (int i = 0; i < alpha.point_clkws_.size(); i++) {
		cv::Point p0, p1;
		p0.x = int(alpha.point_clkws_[i].x);
		p0.y = int(alpha.point_clkws_[i].y);
		p1.x = int(alpha.point_clkws_[(i + 1) % alpha.point_clkws_.size()].x);
		p1.y = int(alpha.point_clkws_[(i + 1) % alpha.point_clkws_.size()].y);
		cv::line(background, p0, p1, cv::Scalar(0, 255, 0), 2);
	}
	char c1[20];
	sprintf(c1, "%.0f", iou_output[1]);
	std::string s1(c1);
	cv::putText(background, "G_Area:" + s1, cv::Point(10, 445),
							cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(0, 255, 0), 2);

	for (int i = 0; i < beta.point_clkws_.size(); i++) {
		cv::Point p0, p1;
		p0.x = int(beta.point_clkws_[i].x);
		p0.y = int(beta.point_clkws_[i].y);
		p1.x = int(beta.point_clkws_[(i + 1) % beta.point_clkws_.size()].x);
		p1.y = int(beta.point_clkws_[(i + 1) % beta.point_clkws_.size()].y);
		cv::line(background, p0, p1, cv::Scalar(0, 0, 255), 2);
	}
	char c2[20];
	sprintf(c2, "%.0f", iou_output[2]);
	std::string s2(c2);
	cv::putText(background, "R_Area:" + s2, cv::Point(10, 470),
							cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(0, 0, 255), 2);

	for (int i = 0; i < _inter.n; i++) {
		cv::circle(background, _inter[i], 3, cv::Scalar(0, 255, 255), 3);
	}
	char c3[20];
	sprintf(c3, "%.0f", iou_output[3]);
	std::string s3(c3);
	cv::putText(background, "Y_Area:" + s3, cv::Point(10, 495),
							cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(0, 255, 255), 2);

	char c4[20];
	sprintf(c4, "%.2f", iou_output[0]);
	std::string s4(c4);
	cv::putText(background, "IOU:" + s4, cv::Point(350, 490),
							cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 255, 255), 2);

	cv::imwrite("PolygonShow.png", background);
	cv::waitKey(0);
#endif

	return 0;
}