#include "Utils/ConfigLoader.h"
#include "Elements/ElementaryElements.h"
#include "Arithmetics/BasicArithmetics.h"
#include "Visualizers/Visualizer2D.h"
#include "Vessels/BinaryTree.h"
#include "Topics/Line Segment Intersection/PlaneSweepAlgorithm.h"
#include "Topics/Line Segment Intersection/TrivialTraversalAlgorithm.h"
#include "Vessels/DoublyConnectedEdgeList.h"

// ================ Configure Loading ================

GeoChain::Utils::GlobalVar g_GlobalVars;
GeoChain::Utils::GlobalKey g_GlobalKeys;

const std::string g_JsonConfigPath = "./config.json";
Json::Value g_ConfigRoot;

// ================ Visualizer Test ================

void visualizer_test() {
	using namespace GeoChain;
	using namespace Euclidean;

	LOG(INFO) << "Half Line - no intercept";
	Point center1(EUC2D, 0, 1);
	float theta1 = M_PI_4f32;
	HalfLine halfline1(EUC2D, DESC, center1, theta1);
	halfline1.Describe();
	halfline1.Maturate();
	halfline1.Describe();

	LOG(INFO) << "Line - DESC";
	Line line1(EUC2D, DESC, center1, -theta1);
	line1.Describe();
	line1.Maturate();
	line1.Describe();

	LOG(INFO) << "Line - PARM";
	Line line2(EUC2D, 1, 2, 3);
	line2.Describe();
	line2.Maturate();
	line2.Describe();

	LOG(INFO) << "Segment - DESC";
	Point term1(EUC2D, 2, 6);
	Point term2(EUC2D, -1, 10);
	Segment segment1(EUC2D, DESC, term1, term2);
	segment1.Describe();

	LOG(INFO) << "Half Line - 1D";
	Point term3(EUC1D, 2);
	HalfLine halfline2(EUC1D, DESC, term3, -M_PIf32);
	halfline2.Describe();

	LOG(INFO) << "Segment - DESC";
	Point term4(EUC2D, 2, 10);
	Point term5(EUC2D, -3, -6);
	Point term6(EUC2D, -4, 5);
	Point term7(EUC2D, 9, -1);
	Segment segment2(EUC2D, DESC, term4, term5, EuclideanDistance);
	segment2.Describe();
	Segment segment3(EUC2D, DESC, term5, term6);
	Segment segment4(EUC2D, DESC, term6, term7);
	Segment segment5(EUC2D, DESC, term7, term4);

	std::vector<Point> attendents = {line1.intercept_,
																	 line2.intercept_,
																	 halfline1.center_,
																	 segment1.terminal_vertex_1_,
																	 segment1.terminal_vertex_2_,
																	 segment2.terminal_vertex_1_,
																	 segment2.terminal_vertex_2_,
																	 segment3.terminal_vertex_1_,
																	 segment3.terminal_vertex_2_,
																	 segment4.terminal_vertex_1_,
																	 segment4.terminal_vertex_2_,
																	 segment5.terminal_vertex_1_,
																	 segment5.terminal_vertex_2_};
	Visualizer2D visual(attendents, g_GlobalVars.visualize_standardize, g_GlobalVars.visualize_spacer);
	visual.Init();

	visual.Draw(line1);
	visual.Draw(line2);
	visual.Draw(halfline1);
	visual.Draw(segment1);
	visual.Draw(segment2);
	visual.Draw(segment3);
	visual.Draw(segment4);
	visual.Draw(segment5);

	visual.Draw(halfline1.center_);
	visual.Draw(segment1.terminal_vertex_1_);
	visual.Draw(segment1.terminal_vertex_2_);
	visual.Draw(segment2.terminal_vertex_1_);
	visual.Draw(segment2.terminal_vertex_2_);
	visual.Draw(segment3.terminal_vertex_1_);
	visual.Draw(segment3.terminal_vertex_2_);
	visual.Draw(segment4.terminal_vertex_1_);
	visual.Draw(segment4.terminal_vertex_2_);
	visual.Draw(segment5.terminal_vertex_1_);
	visual.Draw(segment5.terminal_vertex_2_);
	visual.Visualize("GEOCHAIN");
}

// ================ AVL-Tree Test ================

GeoChain::kWellOrder comparer1D(GeoChain::Vessels::Node<GeoChain::Euclidean::Point> *node_1,
																GeoChain::Vessels::Node<GeoChain::Euclidean::Point> *node_2) {
	return GeoChain::Euclidean::PointCoordSequence(node_1->geometric_element_, node_2->geometric_element_);
};

void avltree_test() {
	using namespace GeoChain;
	using namespace Euclidean;
	using namespace Vessels;

	// AVL Tree Test
	Point root_pt_(EUC1D);
	Node<Point> ROOT(&root_pt_);
	BalancedBinarySearchTree<Point> AVLTREE(&ROOT, comparer1D);
	AVLTREE.Inspect();

	std::vector<Node<Point>> node_series;
	std::vector<Point> pt_series;
	srand((unsigned)time(NULL));
	int total_num = 7;
	float range = 10.0;
	for (int n_ = 0; n_ < total_num; n_++) {
		double rand_unit_ = rand() / double(RAND_MAX);
		float coord_x = (rand_unit_ - 0.5) * range;
		rand_unit_ = rand() / double(RAND_MAX);
		float coord_y = (rand_unit_ - 0.5) * range;
		rand_unit_ = rand() / double(RAND_MAX);
		float coord_z = (rand_unit_ - 0.5) * range;
		LOG(WARNING) << "RANDOM:" << coord_x << " " << coord_y << " " << coord_z;
		Point pt_(EUC3D, coord_x, coord_y, coord_z);
		pt_series.push_back(pt_);
	}

	for (int n_ = 0; n_ < pt_series.size(); n_++) {
		Node<Point> node_(&pt_series[n_]);
		node_series.push_back(node_);
	}

	for (auto &&n_ : node_series) {
		LOG(WARNING) << AVLTREE.Insert(&n_);
		AVLTREE.Inspect();
		LOG(WARNING) << AVLTREE.balancing_;
	}

	for (auto &&n_ : node_series) {
		LOG(WARNING) << AVLTREE.Delete(&n_);
		AVLTREE.Inspect();
		LOG(WARNING) << AVLTREE.balancing_;
	}
}

// ================ line Segment Intersection Test ================

GeoChain::Euclidean::Point GeoChain::Algorithms::PlaneSweeper::SweeperState =
		GeoChain::Euclidean::Point(EUC2D, -g_GlobalVars.convention_infinity, -g_GlobalVars.convention_infinity);

GeoChain::Algorithms::PlaneSweeper line_intersection_sweepline(std::vector<GeoChain::Euclidean::Segment> *ptr_segs,
																															 int repeat_num) {
	using namespace GeoChain;
	using namespace Algorithms;
	using namespace Euclidean;
	Utils::ExperimentalTimer t_(repeat_num);

	for (int r = 1; r < repeat_num; r++) {
		PointSegmentAffiliation root_event(EUC2D, (*ptr_segs).size());
		Segment root_status(EUC2D);
		Node<PointSegmentAffiliation> ROOT_EVENT(&root_event);
		Node<Segment> ROOT_STATUS(&root_status);
		PlaneSweeper plane_sweeper(EUC2D, ptr_segs, &ROOT_EVENT, PointComparer2D, &ROOT_STATUS);
		int counter = 0;
		while (plane_sweeper.Update()) {
			counter++;
			// LOG(INFO) << "finish sweeping event: " << counter;
		}
		// LOG(INFO) << "finish sweeping event: " << ++counter;
	}

	PointSegmentAffiliation root_event(EUC2D, (*ptr_segs).size());
	Segment root_status(EUC2D);
	Node<PointSegmentAffiliation> ROOT_EVENT(&root_event);
	Node<Segment> ROOT_STATUS(&root_status);
	PlaneSweeper plane_sweeper(EUC2D, ptr_segs, &ROOT_EVENT, PointComparer2D, &ROOT_STATUS);
	int counter = 0;
	while (plane_sweeper.Update()) {
		counter++;
		// LOG(INFO) << "finish sweeping event: " << counter;
	}
	// LOG(INFO) << "finish sweeping event: " << ++counter;

	return plane_sweeper;
}

std::vector<GeoChain::Algorithms::PointSegmentAffiliationTraversal> line_intersection_traversal(
		std::vector<GeoChain::Euclidean::Segment> *ptr_segs, std::vector<GeoChain::Euclidean::Point> *intersection_pts,
		int repeat_num, bool simplyfy_opt) {
	using namespace GeoChain;
	using namespace Euclidean;
	using namespace Algorithms;
	Utils::ExperimentalTimer t_(repeat_num);

	for (int r = 1; r < repeat_num; r++) {
		intersection_pts->clear();
		std::vector<PointSegmentAffiliationTraversal> inter_info =
				SegmentIntersectionTraversal(ptr_segs, intersection_pts, simplyfy_opt);
	}
	intersection_pts->clear();
	std::vector<PointSegmentAffiliationTraversal> inter_info =
			SegmentIntersectionTraversal(ptr_segs, intersection_pts, simplyfy_opt);

	return inter_info;
}

void sweepline_test(float range, float expand, int repeat_experiments, bool visual_opt = false) {
	using namespace GeoChain;
	using namespace Algorithms;
	using namespace Euclidean;

	std::vector<Segment> segments;
	srand((unsigned)time(NULL));
	float unit = 1.0;
	float thresh = 0.3;
	double rand_unit_;

	std::vector<Point> attendents;
	for (int c_ = -range; c_ <= range; c_ += unit) {
		for (int r_ = -range; r_ <= range; r_ += unit) {
			rand_unit_ = rand() / double(RAND_MAX);
			float coord_x_1 = c_ + (rand_unit_ - 0.5) * expand * unit;
			rand_unit_ = rand() / double(RAND_MAX);
			float coord_y_1 = r_ + (rand_unit_ - 0.5) * expand * unit;
			rand_unit_ = rand() / double(RAND_MAX);
			float coord_x_2 = c_ + (rand_unit_ - 0.5) * expand * unit;
			rand_unit_ = rand() / double(RAND_MAX);
			float coord_y_2 = r_ + (rand_unit_ - 0.5) * expand * unit;
			LOG(WARNING) << "RANDOM: (" << coord_x_1 << " " << coord_y_1 << ") (" << coord_x_2 << " " << coord_y_2 << ")";
			Point pt_1(EUC2D, coord_x_1, coord_y_1);
			Point pt_2(EUC2D, coord_x_2, coord_y_2);
			if (EuclideanDistance(pt_1, pt_2) > thresh * expand) {
				segments.push_back(Segment(EUC2D, DESC, pt_1, pt_2));
				attendents.push_back(pt_1);
				attendents.push_back(pt_2);
			}
		}
	}

	Point pt_1(EUC2D, -2, 1);
	Point pt_2(EUC2D, 2, 1);
	segments.push_back(Segment(EUC2D, DESC, pt_1, pt_2));
	attendents.push_back(pt_1);
	attendents.push_back(pt_2);

	Point pt_3(EUC2D, -1, -1);
	Point pt_4(EUC2D, -1, 1);
	segments.push_back(Segment(EUC2D, DESC, pt_3, pt_4));
	attendents.push_back(pt_3);
	attendents.push_back(pt_4);

	Visualizer2D visual(attendents, g_GlobalVars.visualize_standardize, g_GlobalVars.visualize_spacer);
	Visualizer2D visual_traverse(attendents, g_GlobalVars.visualize_standardize, g_GlobalVars.visualize_spacer);
	visual.Init();
	visual_traverse.Init();
	for (auto &&s : segments) {
		visual.Draw(s);
		visual_traverse.Draw(s);
	}

	PlaneSweeper plane_sweeper = line_intersection_sweepline(&segments, repeat_experiments);
	int counter = 0;
	for (auto &&e_ : plane_sweeper.events_list_) {
		// LOG(INFO) << e_.num_ << ": " << e_.u_num_ << " " << e_.m_num_ << " " << e_.l_num_;
		if (e_.num_ >= 2) {
			counter++;
			visual.Draw(*(e_.point_), std::to_string(e_.num_));
		}
	}
	LOG(INFO) << "[#] PlaneSweep intersections find: " << counter;
	LOG(INFO) << std::setprecision(g_GlobalVars.visualize_precision)
						<< "[%] intersections/segments: " << float(counter) / segments.size() * 100 << "%";

	std::vector<Point> intersection_pts;
	intersection_pts.reserve((segments.size() * segments.size() - segments.size()) / 2);
	std::vector<PointSegmentAffiliationTraversal> intersection =
			line_intersection_traversal(&segments, &intersection_pts, repeat_experiments, false);
	for (auto &&inter_ : intersection) {
		visual_traverse.Draw(*(inter_.point_), std::to_string(inter_.num_));
	}

	LOG(INFO) << "[#] TrivialTraversal intersections find: " << intersection.size();
	LOG(INFO) << std::setprecision(g_GlobalVars.visualize_precision)
						<< "[%] intersections/segments: " << float(intersection.size()) / segments.size() * 100 << "%";

	intersection = line_intersection_traversal(&segments, &intersection_pts, repeat_experiments, true);

	LOG(INFO) << "[#] OptimizedTraversal intersections find: " << intersection.size();
	LOG(INFO) << std::setprecision(g_GlobalVars.visualize_precision)
						<< "[%] intersections/segments: " << float(intersection.size()) / segments.size() * 100 << "%";

	if (visual_opt) {
		visual.Visualize("Sweepline", "Topics/Line Segment Intersection/Sweepline.png");
		visual_traverse.Visualize("Traversal", "Topics/Line Segment Intersection/Traversal.png");
		cv::destroyAllWindows();
	}
}

// ================ Doubly-Connected Edge List ================

void dbconnected_edgelist_test() {
	using namespace GeoChain;
	using namespace Euclidean;
	using namespace Vessels;

	std::vector<Point> vertices;
	vertices.push_back(Point(EUC2D, 3, 0));
	vertices.push_back(Point(EUC2D, -1, 1));
	vertices.push_back(Point(EUC2D, 1, 3));
	vertices.push_back(Point(EUC2D, 2, 2));
	vertices.push_back(Point(EUC2D, 2, -2));
	vertices.push_back(Point(EUC2D, 4, -2));
	vertices.push_back(Point(EUC2D, 0, -3));
	vertices.push_back(Point(EUC2D, 0, -5));
	vertices.push_back(Point(EUC2D, 1, -4));

	std::map<int, std::vector<int>> relationships;
	relationships[0] = std::vector<int>{4, 5};
	relationships[4] = std::vector<int>{5};
	relationships[1] = std::vector<int>{2, 3};
	relationships[2] = std::vector<int>{3};
	relationships[6] = std::vector<int>{7, 8};
	relationships[7] = std::vector<int>{8};

	PlanarGraph planar_graph(vertices, relationships);
	planar_graph.CheckConnectedComponents();
	DoublyConnectedEdgeList dcel(&planar_graph);
}

// ================ Main Entrance ================

int main(int argc, char **argv) {
	// initiate logger
	GeoChain::Utils::LogGuardian g_Logger(argc, argv);

	// define global config keys
	GeoChain::Utils::DefineGlobalKeys();
	// load config file into global variables
	if (!GeoChain::Utils::ReadJsonFile(g_JsonConfigPath, g_ConfigRoot)) {
		LOG(FATAL) << "[FATAL] ===> Reading config file failed! Exit with code 1.";
		return 1;
	} else {
		GeoChain::Utils::LoadingConfig(g_ConfigRoot);
		LOG(INFO) << g_ConfigRoot;
	}

	// visualizer test part
	// visualizer_test();

	// avl-tree test part
	// avltree_test();

	// line segment intersection test part
	sweepline_test(4.0, 2.5, 50, true);
	for (float exp_ = 2.4; exp_ < 3.5; exp_ += 0.1) {
		for (int expr_ = 0; expr_ < 1; expr_ += 1) {
			sweepline_test(10.0, exp_, 10, true);	// atof(argv[1]), atoi(argv[2])
		}
	}

	// dbconnected_edgelist_test();
}