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
	int id_ = -1;
	std::string key_ = "";
	int balance_ = 0, depth_ = 0;

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

	int balancing_ = 0, deepness_ = 0;

	kWellOrder (*comparer_)(Node<Element>* node_1, Node<Element>* node_2);

	BalancedBinarySearchTree(int id, std::string key, Node<Element>* root,
													 kWellOrder (*comparer)(Node<Element>* node_1, Node<Element>* node_2))
			: id_(id), key_(key), root_(root), comparer_(comparer){};
	~BalancedBinarySearchTree(){};

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
	}

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
	}
};

}	// namespace Vessels
}	// namespace GeoChain