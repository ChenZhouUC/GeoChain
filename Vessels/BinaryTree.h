#ifndef _BINARYTREE_
#define _BINARYTREE_

#include <math.h>
#include "Timer.h"
#include "ConfigLoader.h"

namespace GeoChain {
namespace Vessels {

/**
 * @brief Well-Order Relationship
 * let (a, b) = ORD   if a < b;
 *            = EQN   if a == b;
 *            = INV   if a > b
 */
enum kWellOrder { INV = -1, EQN = 0, ORD = 1 };

/**
 * @brief AVL Tree Node (Balanced Binary Search Tree Node)
 * recording balace score, depth of subtree, layer of the whole tree
 * bidirectional by parent node ptr
 */
template <class Element>
class Node {
 public:
	// identifiers
	int id_ = -1;
	std::string key_ = "";
	int tree_id_ = -1;
	std::string tree_key_ = "";

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

	Node(int id, std::string key, Element* geo_ele) : id_(id), key_(key), geometric_element_(geo_ele) {
		if (g_GlobalVars.convention_element_id >= this->id_) {
			g_GlobalVars.convention_element_id++;
			this->id_ = g_GlobalVars.convention_element_id;
		} else {
			g_GlobalVars.convention_element_id = this->id_;
		}
	};
	Node(Element* geo_ele) : geometric_element_(geo_ele) {
		g_GlobalVars.convention_element_id++;
		this->id_ = g_GlobalVars.convention_element_id;
		this->key_ = Utils::ExtractClassName(std::string(typeid(*(this->geometric_element_)).name())) +
								 std::to_string(this->geometric_element_->dim_) + "d {" + std::to_string(this->id_) + "}";
	};
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
 private:
	int placeholder_ = g_GlobalVars.visualize_placeholder;
	std::map<int, std::map<int, std::string>> layers_;

	// layer: exact layer number of the start_root
	// depth: exact depth to check
	bool Inspect(Node<Element>* start_root, int layer, int depth, int loc) {
		if (start_root->parent_ == nullptr) {
			// ROOT Node
			if (start_root->child_ == nullptr) {
				LOG(ERROR) << "the Tree hasn't been initiated!";
				this->layers_[0][0] = "[" + start_root->key_ + "]";
				if (start_root->depth_ != 0 || start_root->layer_ != 0 || layer != 0) {
					LOG(WARNING) << "query ROOT property error: "
											 << "D" << start_root->depth_ << " L" << start_root->layer_ << " Query layer: " << layer;
				}
				if (this->deepness_ != 0 || this->balancing_ != 0) {
					LOG(WARNING) << "tree property error: "
											 << "TD" << this->deepness_ << " TB" << this->balancing_;
				}
				return depth == 0;
			} else {
				this->layers_[0][0] = "[" + start_root->key_ + "]:(L" + std::to_string(start_root->layer_) + " D" +
															std::to_string(start_root->depth_) + " B" + std::to_string(start_root->balance_) + ")";
				if (start_root->child_->parent_->id_ != start_root->id_) {
					LOG(WARNING) << "Node[" << start_root->child_->key_ << "] parent is not: [" << start_root->key_ << "]";
				}
				if (start_root->depth_ != depth || start_root->layer_ != 0 || layer != 0) {
					LOG(WARNING) << "query ROOT property error: "
											 << "D" << start_root->depth_ << " L" << start_root->layer_ << " Query layer: " << layer;
				}
				if (this->deepness_ != depth) {
					LOG(WARNING) << "tree property error: "
											 << "TD" << this->deepness_;
				}
				return Inspect(start_root->child_, 1, depth, 0);
			}
		} else {
			// Element Node
			this->layers_[layer][loc] = "[" + start_root->key_ + "]:(L" + std::to_string(start_root->layer_) + " D" +
																	std::to_string(start_root->depth_) + " B" + std::to_string(start_root->balance_) +
																	")";
			if (start_root->lchild_ == nullptr && start_root->rchild_ == nullptr) {
				// Leaf node
				if (start_root->depth_ != 1 || start_root->layer_ != layer) {
					LOG(WARNING) << "query Leaf property error: "
											 << "D" << start_root->depth_ << " L" << start_root->layer_ << " Query layer: " << layer;
				}
				return depth == 1;
			} else {
				// Branch node
				if (start_root->layer_ != layer) {
					LOG(WARNING) << "query Branch property error: "
											 << " L" << start_root->layer_ << " Query layer: " << layer;
				}
				bool l_rst = false, r_rst = false;
				if (start_root->lchild_ != nullptr) {
					if (start_root->lchild_->parent_->id_ != start_root->id_) {
						LOG(WARNING) << "Node[" << start_root->lchild_->key_ << "] parent is not: [" << start_root->key_ << "]";
					}
					l_rst = Inspect(start_root->lchild_, layer + 1, depth - 1, loc * 2);
				}
				if (start_root->rchild_ != nullptr) {
					if (start_root->rchild_->parent_->id_ != start_root->id_) {
						LOG(WARNING) << "Node[" << start_root->rchild_->key_ << "] parent is not: [" << start_root->key_ << "]";
					}
					r_rst = Inspect(start_root->rchild_, layer + 1, depth - 1, loc * 2 + 1);
				}
				return l_rst || r_rst;
			}
		}
	}

	// Relayer: renote the layers of a subtree
	bool Relayer(Node<Element>* start_root, int diff) {
		if (start_root->parent_ == nullptr) {
			// ROOT Node
			if (start_root->child_ == nullptr) {
				LOG(ERROR) << "the Tree hasn't been initiated!";
				return false;
			} else {
				return Relayer(start_root->child_, diff);
			}
		} else {
			// Element Node
			start_root->layer_ += diff;
			if (start_root->lchild_ == nullptr && start_root->rchild_ == nullptr) {
				// LEAF
				return true;
			}
			bool l_opr = false, r_opr = false;
			if (start_root->lchild_ != nullptr) {
				l_opr = Relayer(start_root->lchild_, diff);
			}
			if (start_root->rchild_ != nullptr) {
				r_opr = Relayer(start_root->rchild_, diff);
			}
			return l_opr || r_opr;
		}
	};

	// Swap: swap two nodes
	bool Swap(Node<Element>* node_1, Node<Element>* node_2) {
		if (node_1->tree_id_ != this->id_ || node_2->tree_id_ != this->id_) {
			// not this tree
			LOG(ERROR) << "swapping nodes not owned by this tree!";
			return false;
		} else if (node_1->parent_ == nullptr || node_2->parent_ == nullptr) {
			// ROOT
			LOG(ERROR) << "swapping ROOT is not permitted";
			return false;
		}
		// two Element Nodes
		Node<Element> node_1_(*(node_1));
		Node<Element>* node_1_t = &node_1_;

		node_1->balance_ = node_2->balance_;
		node_1->layer_ = node_2->layer_;
		node_1->depth_ = node_2->depth_;

		node_2->balance_ = node_1_t->balance_;
		node_2->layer_ = node_1_t->layer_;
		node_2->depth_ = node_1_t->depth_;

		if (node_1->parent_->id_ != node_2->id_ && node_2->parent_->id_ != node_1->id_) {
			// not two parent-child-like neighbors
			node_1->parent_ = node_2->parent_;
			node_1->child_ = node_2->child_;
			node_1->lchild_ = node_2->lchild_;
			node_1->rchild_ = node_2->rchild_;
			if (node_2->parent_->parent_ == nullptr) {
				// ROOT
				node_2->parent_->child_ = node_1;
			} else {
				// Element
				if (node_2->parent_->lchild_ != nullptr && node_2->parent_->lchild_->id_ == node_2->id_) {
					// lchild
					node_2->parent_->lchild_ = node_1;
				} else {
					// rchild
					node_2->parent_->rchild_ = node_1;
				}
			}
			if (node_2->lchild_ != nullptr) {
				node_2->lchild_->parent_ = node_1;
			}
			if (node_2->rchild_ != nullptr) {
				node_2->rchild_->parent_ = node_1;
			}

			node_2->parent_ = node_1_t->parent_;
			node_2->child_ = node_1_t->child_;
			node_2->lchild_ = node_1_t->lchild_;
			node_2->rchild_ = node_1_t->rchild_;
			if (node_1_t->parent_->parent_ == nullptr) {
				// ROOT
				node_1_t->parent_->child_ = node_2;
			} else {
				// Element
				if (node_1_t->parent_->lchild_ != nullptr && node_1_t->parent_->lchild_->id_ == node_1_t->id_) {
					// lchild
					node_1_t->parent_->lchild_ = node_2;
				} else {
					// rchild
					node_1_t->parent_->rchild_ = node_2;
				}
			}
			if (node_1_t->lchild_ != nullptr) {
				node_1_t->lchild_->parent_ = node_2;
			}
			if (node_1_t->rchild_ != nullptr) {
				node_1_t->rchild_->parent_ = node_2;
			}
			return true;
		} else {
			if (node_1->parent_->id_ == node_2->id_) {
				// node_2 is parent of node_1
				node_1->parent_ = node_2->parent_;
				node_2->parent_ = node_1;

				if (node_1->parent_->parent_ == nullptr) {
					// ROOT
					node_1->parent_->child_ = node_1;
				} else {
					// Element
					if (node_1->parent_->lchild_ != nullptr && node_1->parent_->lchild_->id_ == node_2->id_) {
						// lchild
						node_1->parent_->lchild_ = node_1;
					} else {
						// rchild
						node_1->parent_->rchild_ = node_1;
					}
				}

				if (node_2->lchild_ != nullptr && node_2->lchild_->id_ == node_1->id_) {
					// node_1 is lchild of node_2
					node_1->lchild_ = node_2;
					node_1->rchild_ = node_2->rchild_;
					node_2->lchild_ = node_1_t->lchild_;
					node_2->rchild_ = node_1_t->rchild_;
					if (node_1->rchild_ != nullptr) {
						node_1->rchild_->parent_ = node_1;
					}
				} else {
					// node_1 is rchild of node_2
					node_1->rchild_ = node_2;
					node_1->lchild_ = node_2->lchild_;
					node_2->lchild_ = node_1_t->lchild_;
					node_2->rchild_ = node_1_t->rchild_;
					if (node_1->lchild_ != nullptr) {
						node_1->lchild_->parent_ = node_1;
					}
				}

				if (node_2->lchild_ != nullptr) {
					node_2->lchild_->parent_ = node_2;
				}
				if (node_2->rchild_ != nullptr) {
					node_2->rchild_->parent_ = node_2;
				}

			} else {
				// node_1 is parent of node_2
				node_2->parent_ = node_1->parent_;
				node_1->parent_ = node_2;
				node_1->lchild_ = node_2->lchild_;
				node_1->rchild_ = node_2->rchild_;

				if (node_2->parent_->parent_ == nullptr) {
					// ROOT
					node_2->parent_->child_ = node_2;
				} else {
					// Element
					if (node_2->parent_->lchild_ != nullptr && node_2->parent_->lchild_->id_ == node_1->id_) {
						// lchild
						node_2->parent_->lchild_ = node_2;
					} else {
						// rchild
						node_2->parent_->rchild_ = node_2;
					}
				}

				if (node_1_t->lchild_ != nullptr && node_1_t->lchild_->id_ == node_2->id_) {
					// node_2 is lchild of node_1
					node_2->lchild_ = node_1;
					node_2->rchild_ = node_1_t->rchild_;
					if (node_2->rchild_ != nullptr) {
						node_2->rchild_->parent_ = node_2;
					}
				} else {
					// node_1 is rchild of node_2
					node_2->rchild_ = node_1;
					node_2->lchild_ = node_1_t->lchild_;
					if (node_2->lchild_ != nullptr) {
						node_2->lchild_->parent_ = node_2;
					}
				}

				if (node_1->lchild_ != nullptr) {
					node_1->lchild_->parent_ = node_1;
				}
				if (node_1->rchild_ != nullptr) {
					node_1->rchild_->parent_ = node_1;
				}
			}
			return true;
		}
	};

	// BacktraceDepthBalance: recalculate depth and balance
	int BacktraceDepthBalance(Node<Element>* starting, int update_depth, int balancing = 0) {
		while (true) {
			if (starting->parent_ == nullptr) {
				// ROOT
				starting->depth_ = update_depth;
				starting->balance_ = 0;
				this->deepness_ = update_depth;
				break;
			} else {
				// Element
				int new_balance_, new_depth_;
				if (starting->lchild_ != nullptr and starting->rchild_ != nullptr) {
					new_balance_ = starting->rchild_->depth_ - starting->lchild_->depth_;
					new_depth_ = std::max(starting->rchild_->depth_, starting->lchild_->depth_) + 1;
				} else if (starting->lchild_ != nullptr) {
					// only has lchild
					new_balance_ = -starting->lchild_->depth_;
					new_depth_ = update_depth + 1;
				} else {
					// only has rchild
					new_balance_ = starting->rchild_->depth_;
					new_depth_ = update_depth + 1;
				}
				int update_balancing_ = 0;
				if (new_balance_ == starting->balance_) {
					// both not changed
					break;
				} else if (new_depth_ == starting->depth_) {
					// this depth not changed but balance changed
					update_balancing_ = std::abs(new_balance_) - std::abs(starting->balance_);
					balancing += update_balancing_;
					this->balancing_ += update_balancing_;
					starting->balance_ = new_balance_;
					break;
				} else {
					// depth and balace both changed
					update_balancing_ = std::abs(new_balance_) - std::abs(starting->balance_);
					balancing += update_balancing_;
					this->balancing_ += update_balancing_;
					starting->balance_ = new_balance_;
					starting->depth_ = new_depth_;

					update_depth = starting->depth_;
					starting = starting->parent_;
					continue;
				}
			}
		}
		return balancing;
	};

 public:
	int id_ = -1;
	std::string key_ = "";
	Node<Element>* root_;

	// balancing: sum of balance scores for all the branches
	// deepness: depth of the ROOT
	int balancing_ = 0, deepness_ = 0;

	kWellOrder (*comparer_)(Node<Element>* node_1, Node<Element>* node_2);

	BalancedBinarySearchTree(int id, std::string key, Node<Element>* root,
													 kWellOrder (*comparer)(Node<Element>* node_1, Node<Element>* node_2))
			: id_(id), key_(key), root_(root), comparer_(comparer) {
		if (g_GlobalVars.convention_element_id >= this->id_) {
			g_GlobalVars.convention_element_id++;
			this->id_ = g_GlobalVars.convention_element_id;
		} else {
			g_GlobalVars.convention_element_id = this->id_;
		}
		this->root_->tree_id_ = this->id_;
		this->root_->tree_key_ = this->key_;
	};
	BalancedBinarySearchTree(Node<Element>* root, kWellOrder (*comparer)(Node<Element>* node_1, Node<Element>* node_2))
			: root_(root), comparer_(comparer) {
		g_GlobalVars.convention_element_id++;
		this->id_ = g_GlobalVars.convention_element_id;
		this->key_ =
				"AVLTree" + Utils::ExtractClassName(std::string(typeid(*this).name())) + " {" + std::to_string(this->id_) + "}";
		this->root_->tree_id_ = this->id_;
		this->root_->tree_key_ = this->key_;
	}
	~BalancedBinarySearchTree(){};

	// Inspect: visualize and inspect the tree structure
	void Inspect() {
		this->layers_.clear();
		bool depth_rst = Inspect(this->root_, 0, this->deepness_, 0);
		if (!depth_rst) {
			LOG(ERROR) << "tree deepness error!";
		} else {
			LOG(INFO) << "total layers including ROOT: " << this->layers_.size();
		}
		for (auto&& l_ : this->layers_) {
			for (auto&& n_ : l_.second) {
				this->placeholder_ = std::max(int(n_.second.size()) + 2, this->placeholder_);
			}
		}
		if (this->placeholder_ % 2 != 0) {
			// make sure it's even to ensure center calculation
			this->placeholder_ += 1;
		}
		// LOG(INFO) << "placeholder: " << this->placeholder_;
		std::string output_tree_structure_ = "Tree:[" + this->key_ + "] Structure\n";
		int total_placeholder_ = std::max(1, int(std::pow(2, int(this->layers_.size()) - 2)));
		std::string root_str_ = "ROOT＠" + this->layers_[0][0];
		root_str_ = Utils::CenterizePlaceHolder(root_str_, total_placeholder_ * this->placeholder_, 2) + "\n";
		LOG(INFO) << output_tree_structure_;
		LOG(INFO) << root_str_;
		for (int l_ = 1; l_ < layers_.size(); l_++) {
			std::string this_layer_structure_t = "";
			for (int n_ = 0; n_ < std::pow(2, l_ - 1); n_++) {
				if (this->layers_[l_].count(n_) == 0) {
					this_layer_structure_t += Utils::GenDuplStr(" ", this->placeholder_ * total_placeholder_);
				} else {
					this_layer_structure_t +=
							Utils::CenterizePlaceHolder(this->layers_[l_][n_], this->placeholder_ * total_placeholder_);
				}
			}
			if (l_ > 1) {
				int this_layer_thickness_ = this->placeholder_ * total_placeholder_ / 2;
				for (int t_ = 0; t_ < this_layer_thickness_; t_ += 1) {
					std::string this_thick_t = "";
					for (int n_ = 0; n_ < std::pow(2, l_ - 1); n_++) {
						if (this->layers_[l_].count(n_) == 0) {
							this_thick_t += Utils::GenDuplStr(" ", this->placeholder_ * total_placeholder_);
						} else {
							if (n_ % 2 == 0) {
								this_thick_t += Utils::GenDuplStr(" ", this->placeholder_ * total_placeholder_ - 1 - t_);
								this_thick_t += "╱";
								this_thick_t += Utils::GenDuplStr(" ", t_);
							} else {
								this_thick_t += Utils::GenDuplStr(" ", t_);
								this_thick_t += "╲";
								this_thick_t += Utils::GenDuplStr(" ", this->placeholder_ * total_placeholder_ - 1 - t_);
							}
						}
					}
					LOG(INFO) << this_thick_t + "\n";
				}
			} else {
				std::string this_thick_t = "||";
				LOG(INFO) << Utils::CenterizePlaceHolder(this_thick_t, this->placeholder_ * total_placeholder_) + "\n";
			}
			total_placeholder_ /= 2;
			LOG(INFO) << this_layer_structure_t;
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
	// here we assume the operated node belongs to the tree instance

	// LeftRotate: left rotate the subtree structure
	// return:  1 if succeed and maintain balace;
	//          0 if succeed but weaken balace;
	//          -1 if failed rotation;
	kWellOrder LeftRotate(Node<Element>* start_node) {
		if (start_node->tree_id_ != this->id_) {
			// not this tree
			LOG(ERROR) << "rotate node not owned by this tree but: " << start_node->tree_id_ << ":" << start_node->tree_key_;
			return INV;
		} else if (start_node->parent_ == nullptr) {
			// ROOT
			LOG(ERROR) << "rotate node cannot be ROOT!";
			return INV;
		} else if (start_node->rchild_ == nullptr) {
			// empty rchild
			LOG(ERROR) << "left rotate need rchild not to be empty!";
			return INV;
		}

		if (start_node->parent_->parent_ == nullptr) {
			// ROOT
			start_node->parent_->child_ = start_node->rchild_;
			start_node->rchild_->parent_ = start_node->parent_;
		} else {
			// Element
			start_node->rchild_->parent_ = start_node->parent_;
			if (start_node->parent_->lchild_ != nullptr && start_node->parent_->lchild_->id_ == start_node->id_) {
				// lchild
				start_node->parent_->lchild_ = start_node->rchild_;
			} else {
				// rchild
				start_node->parent_->rchild_ = start_node->rchild_;
			}
		}

		start_node->parent_ = start_node->rchild_;
		start_node->rchild_ = start_node->parent_->lchild_;
		if (start_node->rchild_ != nullptr) {
			start_node->rchild_->parent_ = start_node;
		}
		start_node->parent_->lchild_ = start_node;

		start_node->layer_ += 1;
		start_node->parent_->layer_ -= 1;
		int top_depth_ = 0;
		if (start_node->parent_->rchild_ != nullptr) {
			Relayer(start_node->parent_->rchild_, -1);
			top_depth_ = start_node->parent_->rchild_->depth_;
		}
		if (start_node->lchild_ != nullptr) {
			Relayer(start_node->lchild_, +1);
		}

		int l_depth_ = 0, r_depth_ = 0;
		if (start_node->lchild_ != nullptr) {
			l_depth_ = start_node->lchild_->depth_;
		}
		if (start_node->rchild_ != nullptr) {
			r_depth_ = start_node->rchild_->depth_;
		}
		start_node->depth_ = std::max(l_depth_, r_depth_) + 1;
		int flag_ = std::abs(r_depth_ - l_depth_) - std::abs(start_node->balance_);
		start_node->balance_ = r_depth_ - l_depth_;

		flag_ += std::abs(top_depth_ - start_node->depth_) - std::abs(start_node->parent_->balance_);
		start_node->parent_->depth_ = std::max(top_depth_, start_node->depth_) + 1;
		start_node->parent_->balance_ = top_depth_ - start_node->depth_;

		this->balancing_ += flag_;
		flag_ = BacktraceDepthBalance(start_node->parent_->parent_, start_node->parent_->depth_, flag_);

		if (flag_ > 0) {
			return EQN;
		} else {
			return ORD;
		}
	};

	// RightRotate: Right rotate the subtree structure
	// return:  1 if succeed and maintain balace;
	//          0 if succeed but weaken balace;
	//          -1 if failed rotation;
	kWellOrder RightRotate(Node<Element>* start_node) {
		if (start_node->tree_id_ != this->id_) {
			// not this tree
			LOG(ERROR) << "rotate node not owned by this tree but: " << start_node->tree_id_ << ":" << start_node->tree_key_;
			return INV;
		} else if (start_node->parent_ == nullptr) {
			// ROOT
			LOG(ERROR) << "rotate node cannot be ROOT!";
			return INV;
		} else if (start_node->lchild_ == nullptr) {
			// empty rchild
			LOG(ERROR) << "right rotate need lchild not to be empty!";
			return INV;
		}

		if (start_node->parent_->parent_ == nullptr) {
			// ROOT
			start_node->parent_->child_ = start_node->lchild_;
			start_node->lchild_->parent_ = start_node->parent_;
		} else {
			// Element
			start_node->lchild_->parent_ = start_node->parent_;
			if (start_node->parent_->lchild_ != nullptr && start_node->parent_->lchild_->id_ == start_node->id_) {
				// lchild
				start_node->parent_->lchild_ = start_node->lchild_;
			} else {
				// rchild
				start_node->parent_->rchild_ = start_node->lchild_;
			}
		}

		start_node->parent_ = start_node->lchild_;
		start_node->lchild_ = start_node->parent_->rchild_;
		if (start_node->lchild_ != nullptr) {
			start_node->lchild_->parent_ = start_node;
		}
		start_node->parent_->rchild_ = start_node;

		start_node->layer_ += 1;
		start_node->parent_->layer_ -= 1;
		int top_depth_ = 0;
		if (start_node->parent_->lchild_ != nullptr) {
			Relayer(start_node->parent_->lchild_, -1);
			top_depth_ = start_node->parent_->lchild_->depth_;
		}
		if (start_node->rchild_ != nullptr) {
			Relayer(start_node->rchild_, +1);
		}

		int l_depth_ = 0, r_depth_ = 0;
		if (start_node->lchild_ != nullptr) {
			l_depth_ = start_node->lchild_->depth_;
		}
		if (start_node->rchild_ != nullptr) {
			r_depth_ = start_node->rchild_->depth_;
		}
		start_node->depth_ = std::max(l_depth_, r_depth_) + 1;
		int flag_ = std::abs(r_depth_ - l_depth_) - std::abs(start_node->balance_);
		start_node->balance_ = r_depth_ - l_depth_;

		flag_ += std::abs(start_node->depth_ - top_depth_) - std::abs(start_node->parent_->balance_);
		start_node->parent_->depth_ = std::max(top_depth_, start_node->depth_) + 1;
		start_node->parent_->balance_ = start_node->depth_ - top_depth_;

		this->balancing_ += flag_;
		flag_ = BacktraceDepthBalance(start_node->parent_->parent_, start_node->parent_->depth_, flag_);

		if (flag_ > 0) {
			return EQN;
		} else {
			return ORD;
		}
	};

	// LeftRightRotate: left-right rotate the subtree structure
	// return:  1 if succeed and maintain balace;
	//          0 if succeed but weaken balace;
	//          -1 if failed rotation;
	kWellOrder LeftRightRotate(Node<Element>* start_node) {
		if (start_node->tree_id_ != this->id_) {
			// not this tree
			LOG(ERROR) << "rotate node not owned by this tree but: " << start_node->tree_id_ << ":" << start_node->tree_key_;
			return INV;
		} else if (start_node->parent_ == nullptr) {
			// ROOT
			LOG(ERROR) << "rotate node cannot be ROOT!";
			return INV;
		} else if (start_node->lchild_ == nullptr) {
			// empty lchild
			LOG(ERROR) << "left-right rotate need lchild not to be empty!";
			return INV;
		}
		int bal_ = this->balancing_, update_bal_ = 0;
		kWellOrder l_rst_ = LeftRotate(start_node->lchild_);
		if (l_rst_ == INV) {
			return INV;
		}
		update_bal_ += (this->balancing_ - bal_);
		bal_ = this->balancing_;
		kWellOrder r_rst_ = RightRotate(start_node);
		if (r_rst_ == INV) {
			l_rst_ = RightRotate(start_node->lchild_);
			if (l_rst_ == INV) {
				LOG(WARNING) << "left child left rotation already processed!";
			}
			return INV;
		} else {
			update_bal_ += (this->balancing_ - bal_);
			if (update_bal_ > 0) {
				return EQN;
			} else {
				return ORD;
			}
		}
	};

	// RightLeftRotate: right-left rotate the subtree structure
	// return:  1 if succeed and maintain balace;
	//          0 if succeed but weaken balace;
	//          -1 if failed rotation;
	kWellOrder RightLeftRotate(Node<Element>* start_node) {
		if (start_node->tree_id_ != this->id_) {
			// not this tree
			LOG(ERROR) << "rotate node not owned by this tree but: " << start_node->tree_id_ << ":" << start_node->tree_key_;
			return INV;
		} else if (start_node->parent_ == nullptr) {
			// ROOT
			LOG(ERROR) << "rotate node cannot be ROOT!";
			return INV;
		} else if (start_node->rchild_ == nullptr) {
			// empty rchild
			LOG(ERROR) << "right-left rotate need rchild not to be empty!";
			return INV;
		}
		int bal_ = this->balancing_, update_bal_ = 0;
		kWellOrder r_rst_ = RightRotate(start_node->rchild_);
		if (r_rst_ == INV) {
			return INV;
		}
		update_bal_ += (this->balancing_ - bal_);
		bal_ = this->balancing_;
		kWellOrder l_rst_ = LeftRotate(start_node);
		if (l_rst_ == INV) {
			r_rst_ = LeftRotate(start_node->rchild_);
			if (r_rst_ == INV) {
				LOG(WARNING) << "right child right rotation already processed!";
			}
			return INV;
		} else {
			update_bal_ += (this->balancing_ - bal_);
			if (update_bal_ > 0) {
				return EQN;
			} else {
				return ORD;
			}
		}
	};

	// Insert: insert ONE node into Tree and maintain the search Tree property
	// return:  1 if succeed and maitain strict order;
	//          0 if succeed but violate strict order;
	//          -1 if failed insertion;
	kWellOrder Insert(Node<Element>* insert_node) {
		Utils::Timer t_;
		if (insert_node->tree_id_ != -1 || insert_node->tree_key_ != "") {
			// already owned by tree
			LOG(ERROR) << "inserted node already owned by tree: " << insert_node->tree_id_ << ":" << insert_node->tree_key_;
			return INV;
		}
		insert_node->lchild_ = insert_node->rchild_ = insert_node->child_ = nullptr;
		insert_node->balance_ = 0;
		insert_node->depth_ = 1;
		insert_node->tree_id_ = this->id_;
		insert_node->tree_key_ = this->key_;
		if (this->root_->child_ == nullptr) {
			this->root_->child_ = insert_node;
			insert_node->parent_ = this->root_;
			insert_node->layer_ = 1;
			insert_node->depth_ = 1;
			this->root_->depth_ = 1;
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
						insert_node->layer_ = layer_counter_ + 1;
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
						insert_node->layer_ = layer_counter_ + 1;
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
						insert_node->layer_ = layer_counter_ + 1;
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
	};

	// Delete: delete ONE node from Tree and maintain the search Tree property
	// return:  1 if succeed and maintain balace;
	//          0 if succeed but weaken balace;
	//          -1 if failed deletion;
	kWellOrder Delete(Node<Element>* delete_node) {
		if (delete_node->tree_id_ != this->id_) {
			// not this tree
			LOG(ERROR) << "deleted node not owned by this tree but: " << delete_node->tree_id_ << ":"
								 << delete_node->tree_key_;
			return INV;
		} else if (delete_node->parent_ == nullptr) {
			// ROOT
			LOG(ERROR) << "deleted node cannot be ROOT!";
			return INV;
		}
		int flag_ = 0;
		flag_ = -std::abs(delete_node->balance_);
		if (delete_node->lchild_ == nullptr && delete_node->rchild_ == nullptr) {
			// Leaf Node
			delete_node->balance_ = 0;
			delete_node->depth_ = 0;
			delete_node->layer_ = 0;
			delete_node->tree_key_ = "";
			delete_node->tree_id_ = -1;

			Node<Element>* p_ = delete_node->parent_;
			if (p_->parent_ == nullptr) {
				// ROOT
				p_->child_ = p_->lchild_ = p_->rchild_ = nullptr;
				p_->depth_ = p_->balance_ = 0;
				this->deepness_ = this->balancing_ = 0;
				// clear the data
				delete_node->parent_ = delete_node->child_ = nullptr;
				return ORD;
			} else {
				// Element
				this->balancing_ += flag_;
				if (p_->lchild_ != nullptr && p_->lchild_->id_ == delete_node->id_) {
					// lchild
					p_->lchild_ = nullptr;
					p_->balance_ += 1;
					if (p_->balance_ > 0) {
						// rdepth >= ldepth
						this->balancing_ += 1;
						flag_ += 1;
					} else {
						// rdepth = 0, p_ become Leaf
						p_->depth_ -= 1;	// =1
						this->balancing_ -= 1;
						flag_ -= 1;
					}
				} else {
					// rchild
					p_->rchild_ = nullptr;
					p_->balance_ -= 1;
					if (p_->balance_ < 0) {
						// rdepth <= ldepth
						this->balancing_ += 1;
						flag_ += 1;
					} else {
						// ldepth = 0, p_ become Leaf
						p_->depth_ -= 1;	// =1
						this->balancing_ -= 1;
						flag_ -= 1;
					}
				}
				// recalculate the depth and balance
				Node<Element>* starting_ = p_->parent_;
				int update_depth_ = p_->depth_;
				flag_ = BacktraceDepthBalance(starting_, update_depth_, flag_);
				// clear the data
				delete_node->parent_ = delete_node->child_ = nullptr;
				if (flag_ > 0) {
					return EQN;
				} else {
					return ORD;
				}
			}
		} else if (delete_node->rchild_ == nullptr) {
			// no rchild but has lchild
			delete_node->balance_ = 0;
			delete_node->depth_ = 0;
			delete_node->layer_ = 0;
			delete_node->tree_key_ = "";
			delete_node->tree_id_ = -1;

			Relayer(delete_node->lchild_, -1);
			Node<Element>* p_ = delete_node->parent_;
			if (p_->parent_ == nullptr) {
				// ROOT
				p_->child_ = delete_node->lchild_;
				p_->depth_ = p_->child_->depth_;
				this->deepness_ = p_->depth_;
				this->balancing_ += flag_;
				// child
				p_->child_->parent_ = p_;
				// clear the data
				delete_node->parent_ = delete_node->lchild_ = nullptr;
				return ORD;
			} else {
				// Element
				if (p_->lchild_ != nullptr && p_->lchild_->id_ == delete_node->id_) {
					// p_'s lchild
					p_->lchild_ = delete_node->lchild_;
				} else {
					// p_'s rchild
					p_->rchild_ = delete_node->lchild_;
				}
				delete_node->lchild_->parent_ = p_;
				// recalculate the depth and balance
				Node<Element>* starting_ = p_;
				int update_depth_ = delete_node->lchild_->depth_;
				this->balancing_ += flag_;
				flag_ = BacktraceDepthBalance(starting_, update_depth_, flag_);
				// clear the data
				delete_node->parent_ = delete_node->lchild_ = nullptr;
				if (flag_ > 0) {
					return EQN;
				} else {
					return ORD;
				}
			}
		} else if (delete_node->lchild_ == nullptr) {
			// no lchild but has rchild
			delete_node->balance_ = 0;
			delete_node->depth_ = 0;
			delete_node->layer_ = 0;
			delete_node->tree_key_ = "";
			delete_node->tree_id_ = -1;

			Relayer(delete_node->rchild_, -1);
			Node<Element>* p_ = delete_node->parent_;
			if (p_->parent_ == nullptr) {
				// ROOT
				p_->child_ = delete_node->rchild_;
				p_->depth_ = p_->child_->depth_;
				this->deepness_ = p_->depth_;
				this->balancing_ += flag_;
				// child
				p_->child_->parent_ = p_;
				// clear the data
				delete_node->parent_ = delete_node->rchild_ = nullptr;
				return ORD;
			} else {
				// Element
				if (p_->lchild_ != nullptr && p_->lchild_->id_ == delete_node->id_) {
					// p_'s lchild
					p_->lchild_ = delete_node->rchild_;
				} else {
					// p_'s rchild
					p_->rchild_ = delete_node->rchild_;
				}
				delete_node->rchild_->parent_ = p_;
				// recalculate the depth and balance
				Node<Element>* starting_ = p_;
				int update_depth_ = delete_node->rchild_->depth_;
				this->balancing_ += flag_;
				flag_ = BacktraceDepthBalance(starting_, update_depth_, flag_);
				// clear the data
				delete_node->parent_ = delete_node->rchild_ = nullptr;
				if (flag_ > 0) {
					return EQN;
				} else {
					return ORD;
				}
			}
		} else {
			// a full branch Node
			Node<Element>* pre_ = Predecessor(delete_node);
			Swap(pre_, delete_node);
			return Delete(delete_node);
		}
	};
};

}	// namespace Vessels
}	// namespace GeoChain

#endif