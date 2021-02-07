#include "ElementaryElements.h"
#include "BinaryTree.h"

namespace GeoChain {
namespace Algorithms {

using namespace Euclidean;
using namespace Vessels;

enum kSweeperRelation { PASSED = -1, RELATE = 0, NOTYET = 1 };

struct PointSegmentAffiliation {
	kDimension dim_;
	Point* point_;
	std::vector<Segment*> segments_ = std::vector<Segment*>();
	std::vector<Segment*> u_segs_ = std::vector<Segment*>(), l_segs_ = std::vector<Segment*>(),
												m_segs_ = std::vector<Segment*>();
	int num_ = 0;
	int u_num_ = 0, l_num_ = 0, m_num_ = 0;

	PointSegmentAffiliation(kDimension dim, int num_segs) : dim_(dim) {
		segments_.reserve(num_segs);
		u_segs_.reserve(num_segs);
		l_segs_.reserve(num_segs);
		m_segs_.reserve(num_segs);
	};
	// if only data but not ptr here is useful, reservation is unnecessary
	PointSegmentAffiliation(kDimension dim) : dim_(dim){};
};

struct SweeperSegmentRelation {
	kDimension dim_;
	Point sweeper_state_;	// the coordinates which sweeper intersects
	Segment* segment_;
	kSweeperRelation relation_;

	SweeperSegmentRelation(kDimension dim) : dim_(dim), sweeper_state_(Point(dim)){};
};

kWellOrder comparer2D(Node<PointSegmentAffiliation>* node_1, Node<PointSegmentAffiliation>* node_2) {
	return PointCoordSequence(node_1->geometric_element_->point_, node_2->geometric_element_->point_);
};

kWellOrder comparerSweepedSegments(Node<SweeperSegmentRelation>* node_1, Node<SweeperSegmentRelation>* node_2) {
	return PointCoordSequence(&(node_1->geometric_element_->sweeper_state_),
														&(node_2->geometric_element_->sweeper_state_));
};

// InsertEvent: insert ONE node into Tree and maintain the search Tree property
// return:  1 if succeed and maitain strict order;
//          0 if succeed but meet a same events;
//          -1 if failed insertion;
kWellOrder InsertEvent(BalancedBinarySearchTree<PointSegmentAffiliation>* events_tree,
											 Node<PointSegmentAffiliation>* insert_event) {
	if (insert_event->tree_id_ != -1 || insert_event->tree_key_ != "") {
		// already owned by tree
		LOG(ERROR) << "inserted node already owned by tree: " << insert_event->tree_id_ << ":" << insert_event->tree_key_;
		return INV;
	}

	insert_event->lchild_ = insert_event->rchild_ = insert_event->child_ = nullptr;
	insert_event->balance_ = 0;
	insert_event->depth_ = 1;
	insert_event->tree_id_ = events_tree->id_;
	insert_event->tree_key_ = events_tree->key_;
	if (events_tree->root_->child_ == nullptr) {
		events_tree->root_->child_ = insert_event;
		insert_event->parent_ = events_tree->root_;
		insert_event->layer_ = 1;
		insert_event->depth_ = 1;
		events_tree->root_->depth_ = 1;
		events_tree->balancing_ = 0;
		events_tree->deepness_ = 1;
		return ORD;
	} else {
		// locate the insertion
		Node<PointSegmentAffiliation>* starting_ = events_tree->root_->child_;
		kWellOrder flag_ = ORD;
		int layer_counter_ = 1;
		while (true) {
			kWellOrder cmp_ = events_tree->comparer_(insert_event, starting_);
			if (cmp_ == ORD) {
				if (starting_->lchild_ == nullptr) {
					starting_->lchild_ = insert_event;
					insert_event->parent_ = starting_;
					insert_event->layer_ = layer_counter_ + 1;
					events_tree->deepness_ = std::max(events_tree->deepness_, insert_event->layer_);
					break;
				} else {
					starting_ = starting_->lchild_;
					layer_counter_++;
					continue;
				}
			} else if (cmp_ == EQN) {
				starting_->geometric_element_->segments_.insert(starting_->geometric_element_->segments_.end(),
																												insert_event->geometric_element_->segments_.begin(),
																												insert_event->geometric_element_->segments_.end());
				starting_->geometric_element_->u_segs_.insert(starting_->geometric_element_->u_segs_.end(),
																											insert_event->geometric_element_->u_segs_.begin(),
																											insert_event->geometric_element_->u_segs_.end());
				starting_->geometric_element_->m_segs_.insert(starting_->geometric_element_->m_segs_.end(),
																											insert_event->geometric_element_->m_segs_.begin(),
																											insert_event->geometric_element_->m_segs_.end());
				starting_->geometric_element_->l_segs_.insert(starting_->geometric_element_->l_segs_.end(),
																											insert_event->geometric_element_->l_segs_.begin(),
																											insert_event->geometric_element_->l_segs_.end());
				starting_->geometric_element_->num_ += insert_event->geometric_element_->num_;
				starting_->geometric_element_->u_num_ += insert_event->geometric_element_->u_num_;
				starting_->geometric_element_->m_num_ += insert_event->geometric_element_->m_num_;
				starting_->geometric_element_->l_num_ += insert_event->geometric_element_->l_num_;
				LOG(WARNING) << "U num:" << starting_->geometric_element_->u_num_;
				flag_ = EQN;
				break;
			} else {
				if (starting_->rchild_ == nullptr) {
					starting_->rchild_ = insert_event;
					insert_event->parent_ = starting_;
					insert_event->layer_ = layer_counter_ + 1;
					events_tree->deepness_ = std::max(events_tree->deepness_, insert_event->layer_);
					break;
				} else {
					starting_ = starting_->rchild_;
					layer_counter_++;
					continue;
				}
			}
		}
		// recalculate the depth and balance
		starting_ = insert_event->parent_;
		if (flag_ != EQN) {
			int update_depth_ = 1;
			events_tree->BacktraceDepthBalance(starting_, update_depth_);
			if (events_tree->balanced_) {
				events_tree->BacktraceBalancing(starting_);
			}
		}
		return flag_;
	}
};

class PlaneSweeper {
 private:
	const kDimension dim_ = EUC0D;
	kMaturityStatus status_ = INIT;
	std::vector<PointSegmentAffiliation> events_list_ = std::vector<PointSegmentAffiliation>();
	std::vector<SweeperSegmentRelation> status_list_ = std::vector<SweeperSegmentRelation>();
	std::vector<Node<PointSegmentAffiliation>> events_nodes_ = std::vector<Node<PointSegmentAffiliation>>();
	std::vector<Node<SweeperSegmentRelation>> status_nodes_ = std::vector<Node<SweeperSegmentRelation>>();

 public:
	std::vector<Segment> segments_;
	BalancedBinarySearchTree<PointSegmentAffiliation> events_table_;
	BalancedBinarySearchTree<SweeperSegmentRelation> status_table_;

	PlaneSweeper(kDimension dim, std::vector<Segment>* seg_list, Node<PointSegmentAffiliation>* root_events,
							 kWellOrder (*comparer_events)(Node<PointSegmentAffiliation>* node_1,
																						 Node<PointSegmentAffiliation>* node_2),
							 Node<SweeperSegmentRelation>* root_status,
							 kWellOrder (*comparer_status)(Node<SweeperSegmentRelation>* node_1,
																						 Node<SweeperSegmentRelation>* node_2))
			: dim_(dim),
				segments_(*seg_list),
				events_table_(BalancedBinarySearchTree<PointSegmentAffiliation>(root_events, comparer_events)),
				status_table_(BalancedBinarySearchTree<SweeperSegmentRelation>(root_status, comparer_status)) {
		// reserve the vector space could avoid data relocating in order to use ptr further
		events_list_.reserve(seg_list->size() * seg_list->size());
		status_list_.reserve(seg_list->size());
		events_nodes_.reserve(seg_list->size() * seg_list->size());
		status_nodes_.reserve(seg_list->size());

		for (auto&& s : this->segments_) {
			if (s.dim_ != this->dim_ || s.status_ == INIT) {
				status_ = INIT;
				LOG(ERROR) << "please make sure the dimensions match as well as segments status NOT ONLY INIT";
				return;
			}
			kWellOrder this_order_ = PointCoordSequence(&(s.terminal_vertex_1_), &(s.terminal_vertex_2_));
			Point* this_upper_;
			if (this_order_ >= 0) {
				// including points superposition
				this_upper_ = &(s.terminal_vertex_1_);
			} else {
				this_upper_ = &(s.terminal_vertex_2_);
			}
			PointSegmentAffiliation this_event_(this->dim_);
			this_event_.point_ = this_upper_;
			this_event_.u_segs_.push_back(&s);
			this_event_.segments_.push_back(&s);
			this_event_.u_num_++;
			this_event_.num_++;
			this->events_list_.push_back(this_event_);

			Node<PointSegmentAffiliation> this_event_node_(&(this->events_list_.back()));
			this->events_nodes_.push_back(this_event_node_);
			LOG(WARNING) << InsertEvent(&(this->events_table_), &(this->events_nodes_.back()));
		}

		this->events_table_.Inspect();

		if (segments_.size() > 0) {
			status_ = MATR;
		} else {
			status_ = INIT;
		}
	};

	~PlaneSweeper(){};
};

}	// namespace Algorithms
}	// namespace GeoChain