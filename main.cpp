#include "Utils/ConfigLoader.h"
#include "Elements/ElementaryElements.h"
#include "Arithmetics/BasicArithmetics.h"
#include "Visualizers/Visualizer2D.h"
#include "Vessels/BinaryTree.h"
#include "Topics/Line Segment Intersection/PlaneSweepAlgorithm.h"

GeoChain::Utils::GlobalVar g_GlobalVars;
GeoChain::Utils::GlobalKey g_GlobalKeys;

const std::string g_JsonConfigPath = "./config.json";
Json::Value g_ConfigRoot;

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

GeoChain::Euclidean::Point GeoChain::Algorithms::PlaneSweeper::SweeperState =
		GeoChain::Euclidean::Point(EUC2D, -g_GlobalVars.convention_infinity, -g_GlobalVars.convention_infinity);

void sweepline_test() {
	using namespace GeoChain;
	using namespace Algorithms;
	using namespace Euclidean;

	int total_num = 7;

	PointSegmentAffiliation root_event(EUC2D, total_num);
	Segment root_status(EUC2D);
	Node<PointSegmentAffiliation> ROOT_EVENT(&root_event);
	Node<Segment> ROOT_STATUS(&root_status);

	std::vector<Segment> segments;
	srand((unsigned)time(NULL));
	float range = 10.0;
	double rand_unit_;
	for (int n_ = 0; n_ < total_num; n_++) {
		rand_unit_ = rand() / double(RAND_MAX);
		float coord_x_1 = 0.0;	//(rand_unit_ - 0.5) * range;
		rand_unit_ = rand() / double(RAND_MAX);
		float coord_y_1 = 0.0;	// (rand_unit_ - 0.5) * range;
		rand_unit_ = rand() / double(RAND_MAX);
		float coord_x_2 = (rand_unit_ - 0.5) * range;
		rand_unit_ = rand() / double(RAND_MAX);
		float coord_y_2 = (rand_unit_ - 0.5) * range;
		LOG(WARNING) << "RANDOM: (" << coord_x_1 << " " << coord_y_1 << ") (" << coord_x_2 << " " << coord_y_2 << ")";
		Point pt_1(EUC2D, coord_x_1, coord_y_1);
		Point pt_2(EUC2D, coord_x_2, coord_y_2);
		segments.push_back(Segment(EUC2D, DESC, pt_1, pt_2));
	}

	PlaneSweeper plane_sweeper(EUC2D, &segments, &ROOT_EVENT, comparer2D, &ROOT_STATUS);
	int counter = 0;
	while (plane_sweeper.Update()) {
		LOG(INFO) << "finish sweeping event: " << ++counter;
	}
	LOG(INFO) << "finish sweeping event: " << ++counter;
}

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
	// visualizer_test();
	// avltree_test();

	sweepline_test();
}