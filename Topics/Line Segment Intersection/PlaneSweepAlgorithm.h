#include "ElementaryElements.h"
#include "BinaryTree.h"
#include "ElementaryArithmetics.h"

namespace GeoChain {
namespace Algorithms {

using namespace Euclidean;
using namespace Vessels;

enum kSweeperRelation { PASSED = -1, RELATE = 0, NOTYET = 1 };

struct PointSegmentAffiliation {
	kDimension dim_;
	Point* point_;
	std::vector<Node<Segment>*> segments_ = std::vector<Node<Segment>*>();
	std::vector<Node<Segment>*> u_segs_ = std::vector<Node<Segment>*>(), l_segs_ = std::vector<Node<Segment>*>(),
															m_segs_ = std::vector<Node<Segment>*>();
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

kWellOrder PointComparer2D(Node<PointSegmentAffiliation>* node_1, Node<PointSegmentAffiliation>* node_2) {
	return PointCoordSequence(node_1->geometric_element_->point_, node_2->geometric_element_->point_,
														g_GlobalVars.convention_epsilon);
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
				// LOG(WARNING) << "[U]:" << starting_->geometric_element_->u_num_
				// 						 << " [L]:" << starting_->geometric_element_->l_num_;
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

	std::vector<Node<PointSegmentAffiliation>> events_nodes_ = std::vector<Node<PointSegmentAffiliation>>();
	std::vector<Node<Segment>> status_nodes_ = std::vector<Node<Segment>>();

 public:
	std::vector<Segment*> segments_;
	BalancedBinarySearchTree<PointSegmentAffiliation> events_table_;
	BalancedBinarySearchTree<Segment> status_table_;
	Node<PointSegmentAffiliation>* event_state_;
	std::vector<Point> new_events_;
	std::vector<PointSegmentAffiliation> events_list_ = std::vector<PointSegmentAffiliation>();

	Point SweeperState;

	bool UpdateSweeper(Point* event) {
		if (SweeperState.dim_ != event->dim_ || event->status_ == INIT) {
			LOG(ERROR) << "please make sure that the updating events dimension match: " << SweeperState.dim_ << " "
								 << event->dim_;
			return false;
		} else {
			// LOG(WARNING) << "previous status: (" << SweeperState.x_ << "," << SweeperState.y_ << ")";
			SweeperState.x_ = event->x_;
			SweeperState.y_ = event->y_;
			SweeperState.z_ = event->z_;
			// LOG(WARNING) << "new status: (" << SweeperState.x_ << "," << SweeperState.y_ << ")";
			return true;
		}
	}

	kWellOrder SegmentComparer(Node<Segment>* node_1, Node<Segment>* node_2) {
		float intersect_1, intersect_2;
		float theta_1, theta_2;	// (-PI/2, PI/2]
		// LOG(WARNING) << "sweeper status: (" << SweeperState.x_ << "," << SweeperState.y_ << ")";
		// LOG(WARNING) << "node1 status: (" << node_1->geometric_element_->terminal_vertex_1_.x_ << ","
		// 						 << node_1->geometric_element_->terminal_vertex_1_.y_ << ") ("
		// 						 << node_1->geometric_element_->terminal_vertex_2_.x_ << ","
		// 						 << node_1->geometric_element_->terminal_vertex_2_.y_ << ")";
		// LOG(WARNING) << "node2 status: (" << node_2->geometric_element_->terminal_vertex_1_.x_ << ","
		// 						 << node_2->geometric_element_->terminal_vertex_1_.y_ << ") ("
		// 						 << node_2->geometric_element_->terminal_vertex_2_.x_ << ","
		// 						 << node_2->geometric_element_->terminal_vertex_2_.y_ << ")";

		if (node_1->geometric_element_->terminal_vertex_1_.x_ == node_1->geometric_element_->terminal_vertex_2_.x_) {
			// node_1 parellel to sweeper
			intersect_1 = SweeperState.y_;
			theta_1 = M_PI_2f32;
		} else {
			intersect_1 = (SweeperState.x_ - node_1->geometric_element_->terminal_vertex_1_.x_) *
												node_1->geometric_element_->terminal_vertex_2_.y_ -
										(SweeperState.x_ - node_1->geometric_element_->terminal_vertex_2_.x_) *
												node_1->geometric_element_->terminal_vertex_1_.y_;
			intersect_1 /=
					(node_1->geometric_element_->terminal_vertex_2_.x_ - node_1->geometric_element_->terminal_vertex_1_.x_);
			if (node_1->geometric_element_->theta_ <= -M_PI_2f32) {
				theta_1 = node_1->geometric_element_->theta_ + M_PIf32;
			} else if (node_1->geometric_element_->theta_ > M_PI_2f32) {
				theta_1 = node_1->geometric_element_->theta_ - M_PIf32;
			} else {
				theta_1 = node_1->geometric_element_->theta_;
			}
		}
		if (node_2->geometric_element_->terminal_vertex_1_.x_ == node_2->geometric_element_->terminal_vertex_2_.x_) {
			// node_1 parellel to sweeper
			intersect_2 = SweeperState.y_;
			theta_2 = M_PI_2f32;
		} else {
			intersect_2 = (SweeperState.x_ - node_2->geometric_element_->terminal_vertex_1_.x_) *
												node_2->geometric_element_->terminal_vertex_2_.y_ -
										(SweeperState.x_ - node_2->geometric_element_->terminal_vertex_2_.x_) *
												node_2->geometric_element_->terminal_vertex_1_.y_;
			intersect_2 /=
					(node_2->geometric_element_->terminal_vertex_2_.x_ - node_2->geometric_element_->terminal_vertex_1_.x_);
			if (node_2->geometric_element_->theta_ <= -M_PI_2f32) {
				theta_2 = node_2->geometric_element_->theta_ + M_PIf32;
			} else if (node_2->geometric_element_->theta_ > M_PI_2f32) {
				theta_2 = node_2->geometric_element_->theta_ - M_PIf32;
			} else {
				theta_2 = node_2->geometric_element_->theta_;
			}
		}

		if (intersect_1 < intersect_2 - g_GlobalVars.convention_epsilon) {
			return ORD;
		} else if (intersect_1 > intersect_2 + g_GlobalVars.convention_epsilon) {
			return INV;
		} else {
			if (theta_1 < theta_2 - g_GlobalVars.convention_epsilon) {
				return ORD;
			} else if (theta_1 > theta_2 + g_GlobalVars.convention_epsilon) {
				return INV;
			} else {
				return EQN;
			}
		}
	}

	// Constructor function
	PlaneSweeper(kDimension dim, std::vector<Segment>* seg_list, Node<PointSegmentAffiliation>* root_events,
							 kWellOrder (*comparer_events)(Node<PointSegmentAffiliation>*, Node<PointSegmentAffiliation>*),
							 Node<Segment>* root_status)
			: dim_(dim),
				events_table_(BalancedBinarySearchTree<PointSegmentAffiliation>(root_events, comparer_events)),
				SweeperState(Point(EUC2D, -g_GlobalVars.convention_infinity, -g_GlobalVars.convention_infinity)) {
		this->status_table_ = BalancedBinarySearchTree<Segment>(
				root_status, std::bind(&PlaneSweeper::SegmentComparer, this, std::placeholders::_1, std::placeholders::_2));

		// reserve the vector space could avoid data relocating in order to use ptr further
		int max_event_num_ = (seg_list->size() * seg_list->size() - seg_list->size()) / 2;
		events_list_.reserve(max_event_num_ + 2 * seg_list->size());
		events_nodes_.reserve(max_event_num_ + 2 * seg_list->size());
		status_nodes_.reserve(seg_list->size());
		new_events_.reserve(max_event_num_);
		// here though we considered the maximum number of potential events, we cannot assure that each event in the
		// vector could be useful. Some duplicated events may occur in the vector but only the first one is useful in
		// the tree and stored correct data value.

		for (auto&& s : (*seg_list)) {
			this->segments_.push_back(&s);
			if (s.dim_ != this->dim_ || s.status_ == INIT) {
				status_ = INIT;
				LOG(ERROR) << "please make sure the dimensions match as well as segments status NOT ONLY INIT";
				return;
			}
			kWellOrder this_order_ =
					PointCoordSequence(&(s.terminal_vertex_1_), &(s.terminal_vertex_2_), g_GlobalVars.convention_epsilon);
			Point *this_upper_, *this_lower_;
			if (this_order_ > 0) {
				// including points superposition
				this_upper_ = &(s.terminal_vertex_1_);
				this_lower_ = &(s.terminal_vertex_2_);
			} else if (this_order_ < 0) {
				this_upper_ = &(s.terminal_vertex_2_);
				this_lower_ = &(s.terminal_vertex_1_);
			} else {
				// only apply lower one to avoid counting duplicated
				PointSegmentAffiliation this_event_(this->dim_);
				this_event_.point_ = &(s.terminal_vertex_1_);

				Node<Segment> this_status_(&s);
				this->status_nodes_.push_back(this_status_);
				this_event_.l_segs_.push_back(&(this->status_nodes_.back()));
				this_event_.segments_.push_back(&(this->status_nodes_.back()));
				this_event_.l_num_++;
				this_event_.num_++;

				this->events_list_.push_back(this_event_);
				Node<PointSegmentAffiliation> this_event_node_(&(this->events_list_.back()));
				this->events_nodes_.push_back(this_event_node_);
				InsertEvent(&(this->events_table_), &(this->events_nodes_.back()));
				continue;
			}
			PointSegmentAffiliation upper_event_(this->dim_), lower_event_(this->dim_);
			upper_event_.point_ = this_upper_;

			Node<Segment> this_status_(&s);
			this->status_nodes_.push_back(this_status_);
			upper_event_.u_segs_.push_back(&(this->status_nodes_.back()));
			upper_event_.segments_.push_back(&(this->status_nodes_.back()));
			upper_event_.u_num_++;
			upper_event_.num_++;

			lower_event_.point_ = this_lower_;
			lower_event_.l_segs_.push_back(&(this->status_nodes_.back()));
			lower_event_.segments_.push_back(&(this->status_nodes_.back()));
			lower_event_.l_num_++;
			lower_event_.num_++;

			this->events_list_.push_back(upper_event_);
			Node<PointSegmentAffiliation> upper_event_node_(&(this->events_list_.back()));
			this->events_nodes_.push_back(upper_event_node_);
			InsertEvent(&(this->events_table_), &(this->events_nodes_.back()));

			this->events_list_.push_back(lower_event_);
			Node<PointSegmentAffiliation> lower_event_node_(&(this->events_list_.back()));
			this->events_nodes_.push_back(lower_event_node_);
			InsertEvent(&(this->events_table_), &(this->events_nodes_.back()));
		}

		// this->events_table_.Inspect();

		if (this->segments_.size() > 0) {
			this->event_state_ = this->events_table_.Min(this->events_table_.root_);
			Point* min_pt = this->event_state_->geometric_element_->point_;
			this->UpdateSweeper(min_pt);
			status_ = MATR;
		} else {
			status_ = INIT;
		}
	};

	~PlaneSweeper(){};

	// query status tree in order to find all related segments to the current sweeper
	void QueryStatus(Node<Segment>* starting) {
		if (starting->geometric_element_->terminal_vertex_1_.x_ == starting->geometric_element_->terminal_vertex_2_.x_) {
			// starting parellel to sweeper
			if ((SweeperState.y_ - starting->geometric_element_->terminal_vertex_1_.y_) *
							(SweeperState.y_ - starting->geometric_element_->terminal_vertex_2_.y_) <
					0) {
				this->event_state_->geometric_element_->m_segs_.push_back(starting);
				this->event_state_->geometric_element_->segments_.push_back(starting);
				this->event_state_->geometric_element_->m_num_++;
				this->event_state_->geometric_element_->num_++;
				if (starting->rchild_ != nullptr) {
					QueryStatus(starting->rchild_);
				}
				// if (starting->lchild_ != nullptr) {
				// 	QueryStatus(starting->lchild_);
				// }
			} else {
				// sweeper is the lower of starting
				if (starting->rchild_ != nullptr) {
					QueryStatus(starting->rchild_);
				}
			}
		} else {
			// starting cross to sweeper
			if (PointInSegment(&(this->SweeperState), starting->geometric_element_)) {
				// cross the sweeper
				if (PointCoordSequence(&SweeperState, &(starting->geometric_element_->terminal_vertex_1_),
															 g_GlobalVars.convention_epsilon) != EQN &&
						PointCoordSequence(&SweeperState, &(starting->geometric_element_->terminal_vertex_2_),
															 g_GlobalVars.convention_epsilon) != EQN) {
					// sweeper is not the lower of starting
					this->event_state_->geometric_element_->m_segs_.push_back(starting);
					this->event_state_->geometric_element_->segments_.push_back(starting);
					this->event_state_->geometric_element_->m_num_++;
					this->event_state_->geometric_element_->num_++;
				}
				if (starting->rchild_ != nullptr) {
					QueryStatus(starting->rchild_);
				}
				if (starting->lchild_ != nullptr) {
					QueryStatus(starting->lchild_);
				}
			} else {
				float r1_ =
						(SweeperState.x_ - starting->geometric_element_->terminal_vertex_1_.x_) /
						(starting->geometric_element_->terminal_vertex_2_.x_ - starting->geometric_element_->terminal_vertex_1_.x_);
				float r2_ =
						(SweeperState.x_ - starting->geometric_element_->terminal_vertex_2_.x_) /
						(starting->geometric_element_->terminal_vertex_2_.x_ - starting->geometric_element_->terminal_vertex_1_.x_);
				float intersect_y = r1_ * starting->geometric_element_->terminal_vertex_2_.y_ -
														r2_ * starting->geometric_element_->terminal_vertex_1_.y_;

				if (intersect_y > SweeperState.y_) {
					if (starting->lchild_ != nullptr) {
						QueryStatus(starting->lchild_);
					}
				} else {
					if (starting->rchild_ != nullptr) {
						QueryStatus(starting->rchild_);
					}
				}
			}
		}
	}

	void SweepAcross() {
		if (this->event_state_->geometric_element_->num_ == 0) {
			LOG(FATAL) << "one event has no belonging segments, this might be caused by computational precision!";
			return;
		}
		Node<Segment>*l_neighbor = nullptr, *r_neighbor = nullptr;
		if (this->event_state_->geometric_element_->u_num_ + this->event_state_->geometric_element_->m_num_ == 0) {
			// ending event should compute the neighbors to generate a new event
			for (int i_ = 0; i_ < this->event_state_->geometric_element_->l_num_; i_++) {
				if (i_ == this->event_state_->geometric_element_->l_num_ - 1) {
					r_neighbor = this->status_table_.Successor(this->event_state_->geometric_element_->l_segs_[i_]);
					l_neighbor = this->status_table_.Predecessor(this->event_state_->geometric_element_->l_segs_[i_]);
				}
				// LOG(WARNING) << "1 " << this->SweeperState.x_ << "," << this->SweeperState.y_;
				this->status_table_.Delete(this->event_state_->geometric_element_->l_segs_[i_]);
			}
			if (l_neighbor != nullptr && r_neighbor != nullptr) {
				Point intersection_ = SegmentIntersection(l_neighbor->geometric_element_, r_neighbor->geometric_element_);
				if (intersection_.status_ == MATR) {
					// LOG(ERROR) << "(" << intersection_.x_ << "," << intersection_.y_ << ") status: " << intersection_.status_;
					this->new_events_.push_back(intersection_);
					PointSegmentAffiliation new_event_(this->dim_);
					new_event_.point_ = &(this->new_events_.back());
					this->events_list_.push_back(new_event_);
					Node<PointSegmentAffiliation> new_event_node_(&(this->events_list_.back()));
					this->events_nodes_.push_back(new_event_node_);
					kWellOrder flag_ = InsertEvent(&(this->events_table_), &(this->events_nodes_.back()));
					if (flag_ == 0) {
						// LOG(ERROR) << "duplicated events";
						this->new_events_.pop_back();
						this->events_list_.pop_back();
						this->events_nodes_.pop_back();
					}
				}
			}
		} else {
			// LOG(WARNING) << "2 " << this->SweeperState.x_ << "," << this->SweeperState.y_;
			for (auto&& l_ : this->event_state_->geometric_element_->l_segs_) {
				this->status_table_.Delete(l_);
			}
			for (auto&& m_ : this->event_state_->geometric_element_->m_segs_) {
				this->status_table_.Delete(m_);
			}
			std::vector<Node<Segment>*> U_M_ = std::vector<Node<Segment>*>();
			U_M_.insert(U_M_.end(), this->event_state_->geometric_element_->m_segs_.begin(),
									this->event_state_->geometric_element_->m_segs_.end());
			U_M_.insert(U_M_.end(), this->event_state_->geometric_element_->u_segs_.begin(),
									this->event_state_->geometric_element_->u_segs_.end());
			for (int i_ = 0; i_ < U_M_.size(); i_++) {
				this->status_table_.Insert(U_M_[i_]);
				if (i_ == 0) {
					r_neighbor = this->status_table_.Successor(U_M_[i_]);
					l_neighbor = this->status_table_.Predecessor(U_M_[i_]);
				}
			}

			if (l_neighbor != nullptr) {
				Node<Segment>* newone_ = this->status_table_.Successor(l_neighbor);
				if (newone_ != nullptr) {
					Point intersection_ = SegmentIntersection(l_neighbor->geometric_element_, newone_->geometric_element_);
					if (intersection_.status_ == MATR) {
						// LOG(ERROR) << "(" << intersection_.x_ << "," << intersection_.y_ << ") status: " <<
						// intersection_.status_;
						this->new_events_.push_back(intersection_);
						PointSegmentAffiliation new_event_(this->dim_);
						new_event_.point_ = &(this->new_events_.back());
						this->events_list_.push_back(new_event_);
						Node<PointSegmentAffiliation> new_event_node_(&(this->events_list_.back()));
						this->events_nodes_.push_back(new_event_node_);
						kWellOrder flag_ = InsertEvent(&(this->events_table_), &(this->events_nodes_.back()));
						if (flag_ == 0) {
							// LOG(ERROR) << "duplicated events";
							this->new_events_.pop_back();
							this->events_list_.pop_back();
							this->events_nodes_.pop_back();
						}
					}
				}
			}

			if (r_neighbor != nullptr) {
				Node<Segment>* newone_ = this->status_table_.Predecessor(r_neighbor);
				if (newone_ != nullptr) {
					Point intersection_ = SegmentIntersection(r_neighbor->geometric_element_, newone_->geometric_element_);
					if (intersection_.status_ == MATR) {
						// LOG(ERROR) << "(" << intersection_.x_ << "," << intersection_.y_ << ") status: " <<
						// intersection_.status_;
						this->new_events_.push_back(intersection_);
						PointSegmentAffiliation new_event_(this->dim_);
						new_event_.point_ = &(this->new_events_.back());
						this->events_list_.push_back(new_event_);
						Node<PointSegmentAffiliation> new_event_node_(&(this->events_list_.back()));
						this->events_nodes_.push_back(new_event_node_);
						kWellOrder flag_ = InsertEvent(&(this->events_table_), &(this->events_nodes_.back()));
						if (flag_ == 0) {
							// LOG(ERROR) << "duplicated events";
							this->new_events_.pop_back();
							this->events_list_.pop_back();
							this->events_nodes_.pop_back();
						}
					}
				}
			}
		}
	}

	bool Update() {
		if (this->status_table_.root_->child_ == nullptr) {
			LOG(WARNING) << "status tree empty!";
		} else {
			QueryStatus(this->status_table_.root_->child_);
		}
		SweepAcross();
		this->event_state_ = this->events_table_.Successor(this->event_state_);
		if (this->event_state_ != nullptr) {
			Point* min_pt = this->event_state_->geometric_element_->point_;
			this->UpdateSweeper(min_pt);
			return true;
		} else {
			return false;
		}
	}
};	// namespace Algorithms

}	// namespace Algorithms
}	// namespace GeoChain
