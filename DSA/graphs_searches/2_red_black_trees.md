# Red Black Tree Notes 


### What is it?
A self balancing binary tree, where each node is colored/painted red or black. It has several properties:
- Root and empty nodes are black
- Children of red nodes are black
- A nodes must have the same black depth. Meaning all paths from that node to a leaf has to have the same number of black nodes.
  The height of a red-black tree is log(n), where n is the amount of nodes in the tree. As a result, all operations, researching, insertion, and deletion take $O(log(n))$ operations.

---
### Inserting into a red-black tree
For all cases, you do some avl rotations, and you result in the same tree:
```
    y(R)
  /   \
x(B)  z(B)
```

Insertion can cause a red-red violation, so we have to balance the tree. Here are the cases:

#### Case 1: Left Rotation
```
x (B)                   y(R)
 \                     /   \
  y (R)      ->      x(B)  z(B)
   \
    z (R)
```
1. Left rotation on x.
2. New root is red, repaint children to be black.

#### Case 2: Left-Right Rotation
```
     z (B)      z (B)
    /          /
   x (R)  ->  y (R)  ->  y(R)    ->   y(R)
    \        /          /   \        /   \
     y (R)  x (R)     x(R)  z(B)   x(B)  z(B)
```
1. Do a left rotation on x (root's left subtree).
2. Then a right rotation on z (root)
3. Root is red, paint direct children as black.

Essentially you do an extra rotation but then reduce this down to case 1. 


### Handling deletion in a red-black tree

#### Base cases
1. If the deleted node is black, and it has only one red child, then color the child black.
2. If the deleted value as a red leaf, which means it has 2 black NIL children, just return an empty black node.
3. If deleted value was a black leaf with two NIL black children, return a double black node. Now bubble up this node to a point where it can be eliminated.

#### Bubbling Up Process
```
  B(1)             DB(2)
  /  \      ->    /   \
B(1) DB(2)      R(0)  B(1) 
```
Here we subtracted one black value from the bottom children and adding 1 to the top. That's the idea, here's another example:
```
   B(1)          DB(2)
  /   \    ->   /     \
R(0) DB(2)    NB(-1)  B(1)
```
Again subtract 1 from children and add 1 to the parent. The only difference is that one of the children was already red, it had a "black value" of zero, and subtracting one black value would bring it to -1 black value.

### Balancing and repainting: Same operations but with double black and negative black
There are 4 rotations needed to after insertion. Deletion requires 6, but 4 of those are identical to the insertion balancing case. We covered these in the insertion section. Now **we'll cover the deletion balancing cases**. Our end result from our operations should be
```
       y(B)
      /    \
    x(B)    z(B)
   /  \     /  \
  a    b   c    d
```
So the same thing as in insertion, but this time we're dealing with double black, and so all 3 nodes are painted black. As opposed to the insertion case where just the root was painted red.

#### Case 1: Right Rotate
``` 
        z(DB)
        /   \
      y(R)   d 
     /   \
   x(R)   c
   /  \
  a    b
```
- Then you just do a right rotation at z.
- This is the same as the rotations in the insertion, it's just now the root is double black. The main difference is, once we rotate, all 3 of the nodes involved are painted black.

#### Case 2: Left-Right Rotate
``` 
    z(DB)
    /   \
  x(R)   d
 /   \
a    y(R)
     /  \
    b    c
```

#### Case 3: Handling double black and negative black
This is really the first really new thing with balancing whilst deleting:
``` 
      z(DB)                    y(B)
     /    \                   /    \
    x(NB)  e                x(B)   z(B)
   /    \         ->       /   \   /  \
  w(B)  y(B)              w(R)  c d    e
 / \    /  \             / \
a   b  c    d           a   b
```
- Repaint: Let z and x be painted as black. Then w is painted as red. That's all the repainting we need, and then we just do the two rotation.
- Do a left rotation on NB. Then a right rotation on z.

### Motivation, Takeaway, and Closing
The reason you'd actually want to implement a red-black tree is because it does have a reasonable speed up compared to a regular AVL tree. However of course, this is quite tough and that's expected.