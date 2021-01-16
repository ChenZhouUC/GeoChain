#include "Utils/ConfigLoader.h"
#include "Elements/ElementaryElements.h"
#include "Arithmetics/BasicArithmetics.h"
#include "Visualizers/Visualizer2D.h"
#include "Vessels/BinaryTree.h"

GeoChain::Utils::GlobalVar g_GlobalVars;
GeoChain::Utils::GlobalKey g_GlobalKeys;

const std::string g_JsonConfigPath = "./config.json";
Json::Value g_ConfigRoot;

GeoChain::Vessels::kWellOrder comparer1D(GeoChain::Vessels::Node<GeoChain::Euclidean::Point> *node_1,
																				 GeoChain::Vessels::Node<GeoChain::Euclidean::Point> *node_2) {
	if (node_1->geometric_element_->x_ < node_2->geometric_element_->x_) {
		return GeoChain::Vessels::ORD;
	} else if (node_1->geometric_element_->x_ > node_2->geometric_element_->x_) {
		return GeoChain::Vessels::INV;
	} else {
		return GeoChain::Vessels::EQN;
	}
};

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
	// visual.Visualize("GEOCHAIN");

	// AVL Tree Test
	GeoChain::Euclidean::Point ROOT_PT(GeoChain::Euclidean::EUC1D);
	GeoChain::Vessels::Node<GeoChain::Euclidean::Point> ROOT(&ROOT_PT);
	GeoChain::Euclidean::Point pt_1(GeoChain::Euclidean::EUC1D, 0);
	GeoChain::Vessels::Node<GeoChain::Euclidean::Point> n1(&pt_1);
	GeoChain::Euclidean::Point pt_2(GeoChain::Euclidean::EUC1D, 1);
	GeoChain::Vessels::Node<GeoChain::Euclidean::Point> n2(&pt_2);
	GeoChain::Euclidean::Point pt_3(GeoChain::Euclidean::EUC1D, -2);
	GeoChain::Vessels::Node<GeoChain::Euclidean::Point> n3(&pt_3);
	GeoChain::Euclidean::Point pt_4(GeoChain::Euclidean::EUC1D, 5);
	GeoChain::Vessels::Node<GeoChain::Euclidean::Point> n4(&pt_4);
	GeoChain::Euclidean::Point pt_5(GeoChain::Euclidean::EUC1D, -1);
	GeoChain::Vessels::Node<GeoChain::Euclidean::Point> n5(&pt_5);
	GeoChain::Euclidean::Point pt_6(GeoChain::Euclidean::EUC1D, -3);
	GeoChain::Vessels::Node<GeoChain::Euclidean::Point> n6(&pt_6);

	GeoChain::Vessels::BalancedBinarySearchTree<GeoChain::Euclidean::Point> AVLTREE(&ROOT, comparer1D);
	AVLTREE.Inspect();

	LOG(WARNING) << AVLTREE.Insert(&n6);
	AVLTREE.Inspect();
	LOG(WARNING) << AVLTREE.balancing_;

	LOG(WARNING) << AVLTREE.Insert(&n3);
	AVLTREE.Inspect();
	LOG(WARNING) << AVLTREE.balancing_;

	LOG(WARNING) << AVLTREE.Insert(&n5);
	AVLTREE.Inspect();
	LOG(WARNING) << AVLTREE.balancing_;

	LOG(WARNING) << AVLTREE.Insert(&n1);
	AVLTREE.Inspect();
	LOG(WARNING) << AVLTREE.balancing_;

	LOG(WARNING) << AVLTREE.Insert(&n2);
	AVLTREE.Inspect();
	LOG(WARNING) << AVLTREE.balancing_;

	LOG(WARNING) << AVLTREE.Insert(&n4);
	AVLTREE.Inspect();
	LOG(WARNING) << AVLTREE.balancing_;

	LOG(WARNING) << AVLTREE.Delete(&n2);
	AVLTREE.Inspect();
	LOG(WARNING) << AVLTREE.balancing_;

	LOG(WARNING) << AVLTREE.Delete(&n4);
	AVLTREE.Inspect();
	LOG(WARNING) << AVLTREE.balancing_;

	LOG(WARNING) << AVLTREE.Delete(&n6);
	AVLTREE.Inspect();
	LOG(WARNING) << AVLTREE.balancing_;

	LOG(WARNING) << AVLTREE.Delete(&n1);
	AVLTREE.Inspect();
	LOG(WARNING) << AVLTREE.balancing_;

	LOG(WARNING) << AVLTREE.Delete(&n3);
	AVLTREE.Inspect();
	LOG(WARNING) << AVLTREE.balancing_;

	LOG(WARNING) << AVLTREE.Delete(&n5);
	AVLTREE.Inspect();
	LOG(WARNING) << AVLTREE.balancing_;

	GeoChain::Vessels::BalancedBinarySearchTree<GeoChain::Euclidean::Point> AVLTREE_F(&ROOT, comparer1D, false);
	AVLTREE_F.Inspect();

	LOG(WARNING) << AVLTREE_F.Insert(&n6);
	AVLTREE_F.Inspect();
	LOG(WARNING) << AVLTREE_F.balancing_;

	LOG(WARNING) << AVLTREE_F.Insert(&n3);
	AVLTREE_F.Inspect();
	LOG(WARNING) << AVLTREE_F.balancing_;

	LOG(WARNING) << AVLTREE_F.Insert(&n5);
	AVLTREE_F.Inspect();
	LOG(WARNING) << AVLTREE_F.balancing_;

	LOG(WARNING) << AVLTREE_F.Insert(&n1);
	AVLTREE_F.Inspect();
	LOG(WARNING) << AVLTREE_F.balancing_;

	LOG(WARNING) << AVLTREE_F.Insert(&n2);
	AVLTREE_F.Inspect();
	LOG(WARNING) << AVLTREE_F.balancing_;

	LOG(WARNING) << AVLTREE_F.Insert(&n4);
	AVLTREE_F.Inspect();
	LOG(WARNING) << AVLTREE_F.balancing_;

	LOG(WARNING) << AVLTREE_F.Delete(&n2);
	AVLTREE_F.Inspect();
	LOG(WARNING) << AVLTREE_F.balancing_;

	LOG(WARNING) << AVLTREE_F.Delete(&n4);
	AVLTREE_F.Inspect();
	LOG(WARNING) << AVLTREE_F.balancing_;

	LOG(WARNING) << AVLTREE_F.Delete(&n6);
	AVLTREE_F.Inspect();
	LOG(WARNING) << AVLTREE_F.balancing_;

	LOG(WARNING) << AVLTREE_F.Delete(&n1);
	AVLTREE_F.Inspect();
	LOG(WARNING) << AVLTREE_F.balancing_;

	LOG(WARNING) << AVLTREE_F.Delete(&n3);
	AVLTREE_F.Inspect();
	LOG(WARNING) << AVLTREE_F.balancing_;

	LOG(WARNING) << AVLTREE_F.Delete(&n5);
	AVLTREE_F.Inspect();
	LOG(WARNING) << AVLTREE_F.balancing_;
}