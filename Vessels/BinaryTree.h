#include "Timer.h"

namespace GeoChain {
namespace Vessels {

/**
 * @brief Well-Order Relationship
 * let (a, b) = ORD   if a < b;
 *            = EQN   if a == b;
 *            = INV   if a > b
 */
enum kWellOrder { INV = -1, EQN = 0, ORD = 1 };

template <class Element>
class Node {
 public:
	// identifiers
	int id_ = -1;
	std::string key_ = "";

	// balance: score of balance = right_depth - left_depth
	// depth: depth of the subtree rooted by this Node, empty tree using 0
	// layer: layer number of this Node within the tree starting from 1 while only ROOT is 0
	int balance_ = 0, depth_ = 0, layer_ = 0;

	Element* geometric_element_;

	Node* lchild_ = nullptr;
	Node* rchild_ = nullptr;
	Node* parent_ = nullptr;
	Node* child_ =
			nullptr;	// if ROOT, then parent_ is Null but child_ is not Null; else, parent_ is not Null but child_ is Null

	Node(int id, std::string key, Element* geo_ele) : id_(id), key_(key), geometric_element_(geo_ele){};
	~Node(){};
};

/**
 * @brief AVL Tree (Balanced Binary Search Tree)
 * G. M. Adelson-Velsky & E. M. Landis
 * 1962, An algorithm for the organization of information
 * lchildTree <= parent <= rchildTree
 */
template <class Element>
class BalancedBinarySearchTree {
 public:
	int id_ = 0;
	std::string key_ = "";
	Node<Element>* root_;

	std::map<int, std::vector<std::string>> layers_;

	// balancing: sum of balance scores for all the branches
	// deepness: depth of the ROOT
	int balancing_ = 0, deepness_ = 0;

	kWellOrder (*comparer_)(Node<Element>* node_1, Node<Element>* node_2);

	BalancedBinarySearchTree(int id, std::string key, Node<Element>* root,
													 kWellOrder (*comparer)(Node<Element>* node_1, Node<Element>* node_2))
			: id_(id), key_(key), root_(root), comparer_(comparer){};
	~BalancedBinarySearchTree(){};

	// Inspect: visualize and inspect the tree structure
	void Inspect() {
		this->layers_.clear();
		bool depth_rst = Inspect(this->root_, 0, this->deepness_);
		if (!depth_rst) {
			LOG(ERROR) << "tree deepness error!";
		} else {
			LOG(INFO) << "total layers including ROOT: " << this->layers_.size();
		}
	};

	bool Inspect(Node<Element>* start_root, int layer, int depth) {
		if (start_root->parent_ == nullptr) {
			// ROOT Node
			if (start_root->child_ == nullptr) {
				LOG(ERROR) << "the Tree hasn't been initiated!";
				this->layers_[0].push_back(start_root->key_);
				return depth == 0;
			} else {
				this->layers_[0].push_back(start_root->key_ + " L" + std::to_string(start_root->layer_) + " D" +
																	 std::to_string(start_root->depth_) + " B" + std::to_string(start_root->balance_));
				return Inspect(start_root->child_, 1, depth);
			}
		} else {
			// Element Node
			this->layers_[layer].push_back(start_root->key_ + " L" + std::to_string(start_root->layer_) + " D" +
																		 std::to_string(start_root->depth_) + " B" + std::to_string(start_root->balance_));
			if (start_root->lchild_ == nullptr && start_root->rchild_ == nullptr) {
				// Leaf node
				return depth == 1;
			} else {
				// Branch node
				bool l_rst = false, r_rst = false;
				if (start_root->lchild_ != nullptr) {
					l_rst = Inspect(start_root->lchild_, layer + 1, depth - 1);
				}
				if (start_root->rchild_ != nullptr) {
					r_rst = Inspect(start_root->rchild_, layer + 1, depth - 1);
				}
				return l_rst || r_rst;
			}
		}
	}

	// searching methods
	// Min: can return ONLY ONE Node with the min value in the searched subTree
	Node<Element>* Min(Node<Element>* start_root) {
		if (start_root->parent_ == nullptr) {
			// ROOT Node
			if (start_root->child_ == nullptr) {
				LOG(ERROR) << "the Tree hasn't been initiated!";
				return nullptr;
			} else {
				Node<Element>* min_ = Min(start_root->child_);
				return min_;
			}
		} else {
			// Element Node
			if (start_root->lchild_ != nullptr) {
				// not the min node
				Node<Element>* l_min_ = Min(start_root->lchild_);
				return l_min_;
			} else {
				// the min node
				return start_root;
			}
		}
	};

	// Max: can return ONLY ONE Node with the max value in the searched subTree
	Node<Element>* Max(Node<Element>* start_root) {
		if (start_root->parent_ == nullptr) {
			// ROOT Node
			if (start_root->child_ == nullptr) {
				LOG(ERROR) << "the Tree hasn't been initiated!";
				return nullptr;
			} else {
				Node<Element>* max_ = Max(start_root->child_);
				return max_;
			}
		} else {
			// Element Node
			if (start_root->rchild_ != nullptr) {
				// not the max node
				Node<Element>* r_max_ = Max(start_root->rchild_);
				return r_max_;
			} else {
				// the max node
				return start_root;
			}
		}
	};

	// searching exact: TODO

	// Predecessor: find the predecessor node
	Node<Element>* Predecessor(Node<Element>* start_root) {
		if (start_root->parent_ == nullptr) {
			// ROOT Node
			if (start_root->child_ == nullptr) {
				LOG(ERROR) << "the Tree hasn't been initiated!";
			}
			return nullptr;
		} else {
			// Element Node
			if (start_root->lchild_ != nullptr) {
				// subtree not empty <= start_root
				Node<Element>* l_max_ = Max(start_root->lchild_);
				return l_max_;
			} else {
				// should find the first rchild-like parent
				while (true) {
					Node<Element>* this_parent_ = start_root->parent_;
					if (this_parent_->parent_ == nullptr) {
						// this_parent_ == ROOT
						break;
					} else {
						// this_parent_ is Element Node
						if (this_parent_->rchild_->id_ == start_root->id_) {
							// rchild-like parent
							start_root = this_parent_;
							break;
						} else {
							// lchild-like parent
							start_root = this_parent_;
						}
					}
				}
				return start_root;
			}
		}
	};

	// Successor: find the successor node
	Node<Element>* Successor(Node<Element>* start_root) {
		if (start_root->parent_ == nullptr) {
			// ROOT Node
			if (start_root->child_ == nullptr) {
				LOG(ERROR) << "the Tree hasn't been initiated!";
			}
			return nullptr;
		} else {
			// Element Node
			if (start_root->rchild_ != nullptr) {
				// subtree not empty >= start_root
				Node<Element>* r_min_ = Min(start_root->rchild_);
				return r_min_;
			} else {
				// should find the first lchild-like parent
				while (true) {
					Node<Element>* this_parent_ = start_root->parent_;
					if (this_parent_->parent_ == nullptr) {
						// this_parent_ == ROOT
						break;
					} else {
						// this_parent_ is Element Node
						if (this_parent_->lchild_->id_ == start_root->id_) {
							// lchild-like parent
							start_root = this_parent_;
							break;
						} else {
							// rchild-like parent
							start_root = this_parent_;
						}
					}
				}
				return start_root;
			}
		}
	};

	// tree node operation
	// Insert: insert ONE node into Tree and maintain the search Tree property
	// return:  1 if succeed and maitain strict order;
	//          0 if succeed but violate strict order;
	//          -1 if failed insertion;
	kWellOrder Insert(Node<Element>* insert_node) {
		Utils::Timer t_;
		if (this->root_->child_ == nullptr) {
			this->root_->child_ = insert_node;
			insert_node->parent_ = this->root_;
			insert_node->lchild_ = insert_node->rchild_ = insert_node->child_ = nullptr;
			insert_node->balance_ = 0;
			insert_node->layer_ = 1;
			insert_node->depth_ = 1;
			this->balancing_ = 0;
			this->deepness_ = 1;
			return ORD;
		} else {
			// locate the insertion
			Node<Element>* starting_ = this->root_->child_;
			kWellOrder flag_ = ORD;
			int layer_counter_ = 1;
			while (true) {
				kWellOrder cmp_ = comparer_(insert_node, starting_);
				if (cmp_ == ORD) {
					if (starting_->lchild_ == nullptr) {
						starting_->lchild_ = insert_node;
						insert_node->parent_ = starting_;
						insert_node->lchild_ = insert_node->rchild_ = insert_node->child_ = nullptr;
						insert_node->balance_ = 0;
						insert_node->layer_ = layer_counter_ + 1;
						insert_node->depth_ = 1;
						this->deepness_ = std::max(this->deepness_, insert_node->layer_);
						break;
					} else {
						starting_ = starting_->lchild_;
						layer_counter_++;
						continue;
					}
				} else if (cmp_ == EQN) {
					if (starting_->lchild_ == nullptr) {
						starting_->lchild_ = insert_node;
						insert_node->parent_ = starting_;
						insert_node->lchild_ = insert_node->rchild_ = insert_node->child_ = nullptr;
						insert_node->balance_ = 0;
						insert_node->layer_ = layer_counter_ + 1;
						insert_node->depth_ = 1;
						this->deepness_ = std::max(this->deepness_, insert_node->layer_);
						flag_ = EQN;
						break;
					} else {
						starting_ = starting_->lchild_;
						flag_ = EQN;
						layer_counter_++;
						continue;
					}
				} else {
					if (starting_->rchild_ == nullptr) {
						starting_->rchild_ = insert_node;
						insert_node->parent_ = starting_;
						insert_node->lchild_ = insert_node->rchild_ = insert_node->child_ = nullptr;
						insert_node->balance_ = 0;
						insert_node->layer_ = layer_counter_ + 1;
						insert_node->depth_ = 1;
						this->deepness_ = std::max(this->deepness_, insert_node->layer_);
						break;
					} else {
						starting_ = starting_->rchild_;
						layer_counter_++;
						continue;
					}
				}
			}
			// recalculate the depth and balance
			starting_ = insert_node->parent_;
			int update_depth_ = 1;
			while (true) {
				if (starting_->parent_ == nullptr) {
					// ROOT
					starting_->depth_ = update_depth_;
					starting_->balance_ = 0;
					break;
				} else {
					// Element
					if (update_depth_ + 1 <= starting_->depth_) {
						this->balancing_ -= 1;
						starting_->balance_ = std::copysign(std::abs(starting_->balance_) - 1, starting_->balance_);
						break;
					} else {
						// update_depth_ + 1 > starting_->depth_
						starting_->depth_ = update_depth_ + 1;
						update_depth_ = starting_->depth_;
						this->balancing_ += 1;
						if (starting_->lchild_ != nullptr && starting_->rchild_ != nullptr) {
							starting_->balance_ = starting_->rchild_->depth_ - starting_->lchild_->depth_;
						} else if (starting_->lchild_ != nullptr) {
							starting_->balance_ = -starting_->lchild_->depth_;
						} else {
							starting_->balance_ = starting_->rchild_->depth_;
						}

						starting_ = starting_->parent_;
						continue;
					}
				}
			}
			return flag_;
		}
	}
};

}	// namespace Vessels
}	// namespace GeoChain
