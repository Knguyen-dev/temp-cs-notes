# Heaps 

### What are heaps?
Also known as a **priority queue**, this is balanced binary tree. The main difference is that the nodes are not sorted in BST order. So there are two types of heaps:
- **Min heaps:** For any given node, it's children will be larger.
- **Max heaps:** For any given node it's children will be smaller:

That's the ordering that heaps need to follow. The heap has three core operations:
- Find min
- Remove min
- Insert 

For the rest of today, we're talking about min heaps. The motivation behind heaps is the idea of optimization. They're critical in the optimization of  search and pathfinding algorithms.

### Operations

#### Find Minimum
- The is the root node of the tree. So just return the root.

#### Insertion
- Go to the bottom level of the tree and insert the node. However so to a level don't want to go to a level with all nodes filled in, so go to a incomplete layer.
- To maintain order, compare the current node with its parent. If the current node is smaller than the parent, then we're out of order. So swap the values. We keep doing this until the current > parent or until we reach the root node.
- This is called the move up technique, as we swap the newly inserted node up the tree until things are balanced.

#### Remove Minimum
- Remove the root node. Then take a node from the bottom of the tree which takes the place of the root.
- Compare the node to its 2 children. If the node is bigger than its children then we're out of orderr, so we'll swap the values. We will choose to swap with the child with the lowest value.   
- Continue doing this until the current node is smaller than both children. 

#### The "Heapify" operation
- Let's say you have sequence 5,7,2,4,1,9,6,. Build a heap out of it.

```
       5
     /   \
    2     7
   / \   / \ 
  4   1 9   6
```
Think about the number of swaps you need to satisfy the min heap ordering rule. A node is out of order when a given node has children that are larger than it. We can reason that the bottom row is fine since it has no children, so we just have to sort the upper layers. We have 16 leaf nodes in the last layer, and so you can reason that the second to last layer has only half of that.


### How heaps are implemented: Using an array
A heap is represented under an array
``` 
          A
       /    \
      B      C
     / \    / \
    D   E  F   G
[ A , B, C ,D ,E ,F ,G] 
```
- Root is array[0]
- parent(i): The parent if the ith node is the node at roundDown(i-1 / 2)
- leftChild(i) = The left child of the ith node is at index 2i + 1
- rightChild(i) = The right child of the ith 2i+2


### Example 1: Insertion
Let's insert 5
```
        4             
      /   \           
     7     9          
    / \   / \         
   8  12 10 20        
  / \     \           
 25  5    11          
```
- At current (5), we can say that current < parent, so current should be higher. Swap 8 and 5.
- 5 is smaller than 7, so swap 5 and 7. 
- 5 is not smaller than 4, so we're in the right place. Here's the resulting tree:
1. [ 4, 7, 9, 8, 12, 10, 20, 25, 5, 11 ]; The initial state of the array
2. [ 4, 7, 9, 8, 12, 10, 20, 25, 5, 11 ]; Add 5, this is 8's right child. So that's 3*2+2=8 index, so do array[8] = 5
3. [ 4, 7, 9, 5, 12, 10, 20, 25, 8, 11 ]; Swap 8 and 5's positions by swapping their index positions
4. [ 4, 5, 9, 7, 12, 10, 20, 25, 8, 11 ]; Swap 7 and 5's indices. 5's parent index = 3-1/2 = 1. Thinsg are now sorted.
```
        4             
      /   \           
     5     9          
    / \   / \         
   7  12 10 20        
  / \     \           
 25  8    11          
```


### Example 2: Removal of minimum
```
     1
   /   \
  2     3
 / \   / \
4   5 6   7
```
- You'll remove the root and replace it with the last element in the heap (7). You can read the heap starting from the top down, and left to right. Basically the last element in the heap is 7, at array[6] = 7.
- Now we'll move down the tree. If the children of 7 (current node), are smaller than it, then we need to keep moving down. We'll stop when the current node's children are larger equal or larger. If the smallest child is still larger than the current node, then we know that it's guaranteed that we're in the right layer.
- 7's children are 2 and 3, so swap 7 and 2. Then 7's children are 4 and 5, we can still swap, swap 7 with 4. Now we're at the leaf.

1. [ 1, 2, 3, 4, 5, 6, 7 ]; initial state
2. [ 7, 2, 3, 4, 5, 6 ]; Root node is removed and then replaced with the last element to prepare the sorting process.
3. [ 2, 7, 3, 4, 5, 6 ]; Swap 2 and 7.
4. [ 2, 4, 3, 7, 5, 6]; Swap 4 and 7 
```
     7             2
   /   \         /   \
  2     3  ->   4     3
 / \   /       / \    / 
4   5 6       7   5  6
```
