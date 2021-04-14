#ifndef _DBCONNEDGELIST_
#define _DBCONNEDGELIST_

#include <map>
#include <algorithm>
#include "ElementaryElements.h"
#include "ConfigLoader.h"

namespace GeoChain {
namespace Vessels {

using namespace Euclidean;

/**
 * @brief PlanarGraph
 * This is a self-defined minimal useful data-structure to describe a Planar
 * Graph with only straight segments but without curves
 */
class PlanarGraph {
 public:
	std::vector<Point> vertices_;
	std::map<int, std::vector<int>> relationships_;
	int num_edges_ = 0;

	PlanarGraph(std::vector<Point> vertices, std::map<int, std::vector<int>> raw_relationships)
			: vertices_(vertices), relationships_(raw_relationships) {
		std::map<int, std::vector<int>>::iterator iter;
		std::map<int, std::vector<int>>::iterator checker;
		for (iter = this->relationships_.begin(); iter != this->relationships_.end(); iter++) {
			int r_1 = iter->first;
			for (auto &&r_2 : iter->second) {
				checker = this->relationships_.find(r_2);
				if (checker != this->relationships_.end()) {
					std::vector<int>::iterator ptr_checker =
							find(this->relationships_[r_2].begin(), this->relationships_[r_2].end(), r_1);
					if (ptr_checker != this->relationships_[r_2].end()) {
						continue;
					} else {
						this->relationships_[r_2].push_back(r_1);
					}
				} else {
					this->relationships_[r_2] = std::vector<int>{r_1};
				}
			}
		}
		for (iter = this->relationships_.begin(); iter != this->relationships_.end(); iter++) {
			int r_1 = iter->first;
			for (auto &&r_2 : iter->second) {
				this->num_edges_++;
			}
		}
		this->num_edges_ /= 2;
	};
};

class PlanarFace {
 public:
	Segment *outer_;
	std::vector<Segment *> inner_;
};

class HalfEdge {
 public:
	Segment *twin_ = nullptr;
	Point *origin_ = nullptr;
	PlanarFace *face_ = nullptr;
	Segment *successor_ = nullptr;
	Segment *predecessor_ = nullptr;

	HalfEdge(){};
	HalfEdge(Point *origin) : origin_(origin){};
	HalfEdge(Segment *twin, Point *origin, PlanarFace *face, Segment *successor, Segment *predecessor)
			: twin_(twin), origin_(origin), face_(face), successor_(successor), predecessor_(predecessor){};
};

/**
 * @brief DoublyConnectedEdgeList
 * This is a standardized Doubly-Connected Edge List structure
 * with decently necessary information
 */
class DoublyConnectedEdgeList {
 public:
	std::vector<Point> vertices_;
	std::vector<Segment> halfedges_;
	std::map<int, std::map<int, Segment *>> relations_;
	std::vector<PlanarFace> faces_;
	std::map<Point *, std::vector<Segment *>> VertexList_;
	std::map<Segment *, HalfEdge> EdgeList_;

	DoublyConnectedEdgeList(PlanarGraph *planar_graph) : vertices_(planar_graph->vertices_) {
		// LOG(WARNING) << planar_graph->num_edges_;
		this->halfedges_.reserve(planar_graph->num_edges_ * 2 + 1);
		this->halfedges_.clear();
		std::map<int, std::vector<int>>::iterator iter;
		for (iter = planar_graph->relationships_.begin(); iter != planar_graph->relationships_.end(); iter++) {
			int r_1 = iter->first;
			this->VertexList_[&(this->vertices_[r_1])] = std::vector<Segment *>{};
			this->relations_[r_1] = std::map<int, Segment *>{};
			for (auto &&r_2 : iter->second) {
				// LOG(WARNING) << r_1 << " " << r_2;
				Segment this_half_edge_(EUC2D, DESC, this->vertices_[r_1], this->vertices_[r_2]);
				this->halfedges_.push_back(this_half_edge_);
				this->relations_[r_1][r_2] = &(this->halfedges_.back());
				this->VertexList_[&(this->vertices_[r_1])].push_back(&(this->halfedges_.back()));
				this->EdgeList_[&(this->halfedges_.back())] = HalfEdge(&(this->vertices_[r_1]));
			}
		}
		for (iter = planar_graph->relationships_.begin(); iter != planar_graph->relationships_.end(); iter++) {
			int r_1 = iter->first;
			for (auto &&r_2 : iter->second) {
				this->EdgeList_[this->relations_[r_1][r_2]].twin_ = this->relations_[r_2][r_1];
				// this->relations_[r_1][r_2]->Describe();
				// this->EdgeList_[this->relations_[r_1][r_2]].twin_->Describe();
			}
		}
	};
};
}	// namespace Vessels
}	// namespace GeoChain
#endif