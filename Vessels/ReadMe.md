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
│     │ // ====== Descriptive ======
│     ├── Inspect()         // inspect the AVL Tree nodes by traversal
│     ├── Min()             // output the minimum node
│     ├── Max()             // output the maximum node
│     ├── Predecessor()     // output the predecessor node
│     ├── Successor()       // output the successor node
│     │ // ====== Operative ======
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

+ Here the `BalancedBinarySearchTree` class has realized all the functionalities of Binary Search Tree. You can set the balaced option as bool value "false" to create a Binary Search Tree instance.

+ In order to record the balance score and depth information, the realization of AVL Tree here is lightly different from the original one mentioned in the paper, both spacially and temporally. Each node stores both children and parent, as well as balance score and subtree depth. What's more, the following methods are protracted a little with rank of 𝓞⟨㏒𝓷⟩ for simplicity of code:
  + `LeftRotate()`: 𝓞⟨1⟩ → 𝓞⟨㏒𝓷⟩
  + `RightRotate()`: 𝓞⟨1⟩ → 𝓞⟨㏒𝓷⟩
  + `LeftRightRotate()`: 𝓞⟨1⟩ → 𝓞⟨㏒𝓷⟩
  + `RightLeftRotate()`: 𝓞⟨1⟩ → 𝓞⟨㏒𝓷⟩
  + `Insert()`: 𝓞⟨㏒𝓷⟩ → 𝓞⟨㏒𝓷⟩ (because `Insert` operation would require at most one round of rotation only)
  + `Delete()`: 𝓞⟨㏒𝓷⟩ → 𝓞⟨㏒²𝓷⟩
