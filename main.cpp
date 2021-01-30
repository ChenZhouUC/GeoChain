#include "Utils/ConfigLoader.h"
#include "Elements/ElementaryElements.h"
#include "Arithmetics/BasicArithmetics.h"
#include "Visualizers/Visualizer2D.h"
#include "Vessels/BinaryTree.h"

GeoChain::Utils::GlobalVar g_GlobalVars;
GeoChain::Utils::GlobalKey g_GlobalKeys;

const std::string g_JsonConfigPath = "./config.json";
Json::Value g_ConfigRoot;

int init_logger_config(int argc, char **argv) {
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
	return 0;
}

void visualizer_test() {
	LOG(INFO) << "Half Line - no intercept";
	GeoChain::Euclidean::Point center1(GeoChain::Euclidean::EUC2D, 0, 1);
	float theta1 = M_PI_4f32;
	GeoChain::Euclidean::HalfLine halfline1(GeoChain::Euclidean::EUC2D, GeoChain::Euclidean::DESC, center1, theta1);
	halfline1.Describe();
	halfline1.Maturate();
	halfline1.Describe();

	LOG(INFO) << "Line - DESC";
	GeoChain::Euclidean::Line line1(GeoChain::Euclidean::EUC2D, GeoChain::Euclidean::DESC, center1, -theta1);
	line1.Describe();
	line1.Maturate();
	line1.Describe();

	LOG(INFO) << "Line - PARM";
	GeoChain::Euclidean::Line line2(GeoChain::Euclidean::EUC2D, 1, 2, 3);
	line2.Describe();
	line2.Maturate();
	line2.Describe();

	LOG(INFO) << "Segment - DESC";
	GeoChain::Euclidean::Point term1(GeoChain::Euclidean::EUC2D, 2, 6);
	GeoChain::Euclidean::Point term2(GeoChain::Euclidean::EUC2D, -1, 10);
	GeoChain::Euclidean::Segment segment1(GeoChain::Euclidean::EUC2D, GeoChain::Euclidean::DESC, term1, term2);
	segment1.Describe();

	LOG(INFO) << "Half Line - 1D";
	GeoChain::Euclidean::Point term3(GeoChain::Euclidean::EUC1D, 2);
	GeoChain::Euclidean::HalfLine halfline2(GeoChain::Euclidean::EUC1D, GeoChain::Euclidean::DESC, term3, -M_PIf32);
	halfline2.Describe();

	LOG(INFO) << "Segment - DESC";
	GeoChain::Euclidean::Point term4(GeoChain::Euclidean::EUC2D, 2, 10);
	GeoChain::Euclidean::Point term5(GeoChain::Euclidean::EUC2D, -3, -6);
	GeoChain::Euclidean::Point term6(GeoChain::Euclidean::EUC2D, -4, 5);
	GeoChain::Euclidean::Point term7(GeoChain::Euclidean::EUC2D, 9, -1);
	GeoChain::Euclidean::Segment segment2(GeoChain::Euclidean::EUC2D, GeoChain::Euclidean::DESC, term4, term5,
																				GeoChain::Euclidean::EuclideanDistance);
	segment2.Describe();
	GeoChain::Euclidean::Segment segment3(GeoChain::Euclidean::EUC2D, GeoChain::Euclidean::DESC, term5, term6);
	GeoChain::Euclidean::Segment segment4(GeoChain::Euclidean::EUC2D, GeoChain::Euclidean::DESC, term6, term7);
	GeoChain::Euclidean::Segment segment5(GeoChain::Euclidean::EUC2D, GeoChain::Euclidean::DESC, term7, term4);

	std::vector<GeoChain::Euclidean::Point> attendents = {line1.intercept_,
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
	GeoChain::Euclidean::Visualizer2D visual(attendents, g_GlobalVars.visualize_standardize,
																					 g_GlobalVars.visualize_spacer);
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
	// AVL Tree Test
	GeoChain::Euclidean::Point root_pt_(GeoChain::Euclidean::EUC1D);
	GeoChain::Vessels::Node<GeoChain::Euclidean::Point> ROOT(&root_pt_);
	GeoChain::Vessels::BalancedBinarySearchTree<GeoChain::Euclidean::Point> AVLTREE(&ROOT, comparer1D);
	AVLTREE.Inspect();

	std::vector<GeoChain::Vessels::Node<GeoChain::Euclidean::Point>> node_series;
	std::vector<GeoChain::Euclidean::Point> pt_series;
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
		GeoChain::Euclidean::Point pt_(GeoChain::Euclidean::EUC3D, coord_x, coord_y, coord_z);
		pt_series.push_back(pt_);
	}

	for (int n_ = 0; n_ < pt_series.size(); n_++) {
		GeoChain::Vessels::Node<GeoChain::Euclidean::Point> node_(&pt_series[n_]);
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

int main(int argc, char **argv) {
	int init_flag = init_logger_config(argc, argv);
	if (init_flag != 0) {
		return init_flag;
	}
	visualizer_test();
	avltree_test();
}