## Data Vessels
### Binary Trees
#### Balanced Binary Search Tree (AVL Tree)
+ G. M. Adelson-Velsky & E. M. Landis
+ 1962, An algorithm for the organization of information

```
AVLTree
├── Public
│     ├── Inspect()       // inspect the AVL Tree nodes by traversal  
│     ├── Min()           // output the minimum node
│     ├── Max()           // output the maximum node
│     ├── Predecessor()   // output the predecessor node
│     ├── Successor()     // output the successor node
│     ├── Insert()        // insert one node
│     └── Delete()        // delete one node
├── Private
│     ├── Swap()          // swap two nodes from the structure
│     └── Relayer()       // downgrade or upgrade subtree node layers
```

<div align=center>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/assets/AVL.png" alt="AVL" width="600" height="590" align="center"/>
</div>