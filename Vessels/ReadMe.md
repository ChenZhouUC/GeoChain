# Data Vessels

## Binary Trees

### Balanced Binary Search Tree (AVL Tree)

+ G. M. Adelson-Velsky & E. M. Landis
+ 1962, An algorithm for the organization of information

#### Structure

<div align=center>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/assets/AVL.png" alt="AVL" width="700" height="575" align="center"/>
</div>

#### Functionality

```{class}
AVLTree(balanced = true)
├── Public
│     ├── Inspect()         // inspect the AVL Tree nodes by traversal  
│     ├── Min()             // output the minimum node
│     ├── Max()             // output the maximum node
│     ├── Predecessor()     // output the predecessor node
│     ├── Successor()       // output the successor node
│     ├── LeftRotate()      // left rotate the node
│     ├── RightRotate()     // right rotate the node
│     ├── LeftRightRotate() // left-right rotate the node
│     ├── RightLeftRotate() // right-left rotate the node
│     ├── Insert()          // insert one node
│     └── Delete()          // delete one node
├── Private
│     ├── Swap()            // swap two nodes from the structure
│     └── Relayer()         // downgrade or upgrade subtree node layers
```

#### Remarks

Here the BalancedBinarySearchTree class has realized all the functionality of Binary Search Tree. You can set the balaced option as bool value "false" to create a Binary Search Tree instance.
