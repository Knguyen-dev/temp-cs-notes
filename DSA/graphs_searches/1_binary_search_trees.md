# Binary Search Trees

---
## Regular BST (Binary Search Tree)
- Order: Anything to the left is smaller than the root. Then anything to the right is larger than the root. It's almost impossible to get both conditions at the same time, so you prioritize one condition, but AVL trees fix this. AVL trees will try to do operations to correct the tree for the other condition.
- A node in a binary tree is either empty, or has a value and two subtrees (could be empty).
- Height: The height of an empty tree (at the bottom of the tree) is zero. The height of a leaf node is 1. Given two children with different heights, the height of the parent is the bigger (max) height between its two subtrees PLUS 1 (to include itself of course). Start at root = 0
  - If $h_{l} - h_{r} < 2$, for all parts of the tree then the tree is balance. Else, it's wrong. So basically for every given node, the heights of the left and right subtrees must not be greater than 1.
- Balance is not enforced for a BST, but it is for an AVL tree. The height of a BST can be O(n) in the worst case, but an AVL tree is always O(log n) because it is balanced.

### BST Operations 

#### Insertion
Inserting v in BST:
- If BST is empty, v becomes root
- if v < parent: insert v in left 
- if v > parent: insert v in Right
- Else v == w, this is a duplicate node, so do nothing.



#### Deletion
The most complex part about deletion is when you're deleting a root node. In this case 
to replace the root, you'd choose the largest number on the left subtree. This works because this mysterious node b, is still going to be greater than any node on the previous root's left subtree, whilst still being less than any node on the previous root's right subtree. The equivalent option is picking the smallest node on the right subtree. The same idea applies, as placing this node as the root would still satisfy the order property of the BST. You may also pick the smallest node on the right subtree as that will still satisfy the ordering in a BST.

However now you need to merge the tree:
1. merge (empty, R) = R; If the left subtree/node is mepty, use the right node.
2. merge(L, empty) = L; If right is empty, return left.
3. merge(L, R): Both aren't empty, we use this algorithm:
    a. m = Find maximum in left subtree. Since you already have the left subtree, keep traversing right to get the max value. 
    b. You should be able to remove that leaf node from the left subtree. Then the actual merging is literally just updating the root and making it point to the current left and right subtrees.

In the case when you're dealing with only one subtree, this is called finding the inorder predecessor or successor. 

#### Deletion Example: Predecessor and successor
In the rules above, I talked about using the left subtree, and getting the maximum node in that tree. This works to maintain the order property of a BST and similarly you can also get the minimum value in the right subtree to maintain order. **So to emphasize, these are two common strategies to maintain order**. To call back to tree traversals, there are names for these types of nodes: 
- **Inorder predecessor:** The maximum in the left subtree.
- **Inorder successor:** The minimum in the right subtree.

Let's say you were trying to delete node 15:
``` 
     15
    /  \
  10    20
 / \    / \
8  12  17  25
```
The inorder traversal would be $(8,10,12,15,17,20,25)$. Since 15 has both of its subtrees, we have two main strategies:
1. **Use the inorder predecessor:** This would be 12, which is before 15 in the inorder traversal. It's also the largest value in 15's left subtree.
2. **Use the inorder successor:** This would be 17, which is after 15 in the inorder traversal. It's also the smallest value in 15's right subtree.

---
## AVL Tree
Important properties of an AVL tree:
- Balance: AVL Trees are balanced, meaning the maximum height difference between any two nodes is 1. Takes the BST tree, and applies the balancing
- Order: The nodes must be ordered. Lesser nodes on the left, whilst greater nodes are on the right.

### Traversal
You know that for a given node, everything to the right is larger, regardless if its higher or lower. The same property applies ot the left. So if you want to traverse this node in sorted order it would look like: traverse left (recursive), then process current node, then traverse right (recursive). This is called an in order traversal. This is opposed to "pre" or "post" order traversals. The prefix in each of them refer to the position of the node.

- preorder: Root, Left, Right; Root is processed first
- inorder: Left, Root, Right; Root is processed in between left and right subtrees.
- postorder: Left, Right, Root: Root is processed last.

In the worse case scenario, the tree is a long link, a linked list. So it's a O(n). 

---
### Handling Operations in AVL Trees

#### Simple Insertion (Right Rotation)
```
      40
     /  \
   20    50
  /  \
10    30
  \
   15
```
Imbalance is detected at 40, with $h_{L} = 3$ and $h_{r} = 1$, which is a difference in height of 2:
- The left subtree is taller, do a right rotation at node since it was detected at 40. In this rotation, imagine rotating clockwise, with 20 becoming the parent, and then 40 becoming a right subtree. Of course there's a little more, but we'll get to it.
- Here I'll show the intermediate step:
```
       20
     / |  \
   10  \   40
     \  30   \ 
      15      50 
```
- 20 becomes the new root, with the old root getting pushed down and to the right; The right subtree of the old root is unchanged. The left subtree of the new root wasn't affected at all.
- The only thing left is handling 30, which was the right child of 20 before the rotation (left subtree's right subtree).
- Know that 30 was greater than the new root (20), but less than the old root (40). So if we can keep 30 to the right of 20 AND to the left of 40, we're good.
- The solution is to make 30 the left child of 40. It becomes the right subtree's left child.
```
       20
     /    \
   10      40
     \    /  \ 
      15 30   50 
```
Checking it, 30 is still to the left of 20 and the right of 40, the order is preserved. And now the tree is balanced as for any given node, the heights of the left and right subtrees of that node have a difference of at most 1. But let's look at this in a more form, so we don't have to worry about numbers anymore:
```
       P              L1
     /  \            /   \
    L1   R1   ->   L2     P
   /  \             \    / \
 L2    R2            R3 R2  R1
  \
   R3
```
The rotation is done by making L1 the new parent, and P becomes the right child of L1 as. P is pushed down and to the right, and L1 is pushed up to the right. The left subtree of P (L2) becomes the right subtree of L1, and R2 becomes the left subtree of P.

---
#### Simple Insertion (Left/Counter-clockwise Rotation)
```
   P              R1
 /  \            /  \
L1   R1   ->    P    R2
    /  \       / \   /
   L2  R2     L1 L2 L3 
       /
      L3
```
- The right subtree is larger, so do a left rotation.

---
#### Complex Insertion (Right-Left Rotation)
```
  20
 /  \
10   40
    /  \
   30   50
  /
25
```
Imbalance is detected at 20, with $h_{L} = 1$ and $h_{R} = 3$, which is a difference in height of 2. You might think we do a left rotation since right subtree is bigger, however you'll realize even if you do that left rotation, it's still imbalanced. The issue is that the right subtree's left subtree is taller, so we need to rotate it as well:
1. Check the right subtree of 20 (which is 40). Our right subtree is left heavy, meaning it's left subtree (left side) has a taller height.
2. In this case. we'll first do a right rotation on the right subtree.
3. After the right rotation, do a left rotation on 20 (original root)
```
  20
 /  \
10   30
    /  \
   25   40
          \
          50
```
So that's the right rotation on the right subtree, now let's do the left rotation on 20, our original root where we detected the imbalance:
```
     30
    /  \
   20   40
  /  \    \
 10   25   50
```
Nothing is being violated, and the tree is balanced.

---
#### Complex Insertion (Left-Right Rotation)
```
    20  
   /  \
  10  30
 /  \
5   15
      \
      17
```
- Imbalance detected at node 20. Before we rotate 20, check if the left subtree is right heavy. If so, we have the complex case. Here are the steps:
1. Do a left rotation on the left subtree.
2. Do a right rotation on the original root. 
```
        20  
       /  \
      15  30
     /  \
    10   17
   /      
  5       
```
Then the right rotation:
```
        15  
       /  \
      10   20
     /    /  \
    5    17  30
```

---
### Deletion Operations 
- If a node is deleted, and one of the children are missing, return the either child. This may cause an imbalance, but we aren't concerned with that yet
- Else, a node is deleted, and both children are present you can do two things:
  a. Replace the current node with the largest node in the left subtree, or the smallest node in the right subtree. This keeps the order.
  b. Make sure that this node is now the parent with the left and right subtrees of the original node.

After this, the tree should still be a valid BST, but it may be imbalanced. Check for the imbalance and respond with 
one of the 3 cases:
- If you're deleting a node and it has both children, replace the node with the max node in the left subtree or min in the right.
- If you're deleting a node and it has only one child, delete the node and the child takes its place.
- If you're deleting a node that doesn't have children, then just delete the node, no other operations here.

At this point we have a valid bst at least, but it may not be balanced. So akin to how I described in the deletion cases, rotate the tree using one of the four balancing scenarios described in the previous sections.