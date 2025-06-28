# Tree Traversals: Complete Guide

A tree traversal refers to the process of visiting/processing each node of a tree exactly once in a certain order. There are multiple techniques, but they fall under two main umbrellas:

- **Recursion (DFS)**: Includes pre-order, in-order, and post-order traversals.
- **Level-order traversal (BFS)**: Visits nodes level by level.

For all examples, we'll use this binary tree:
```
        1
       / \
      2   3
     / \
    4   5
```

## Pre-order Traversal (Root → Left → Right)

**Order**: Visit root first, then left subtree, then right subtree.

**Result**: `[1, 2, 4, 5, 3]`

```python
def preorder(root):
    if not root:
        return []
    
    result = []
    result.append(root.val)          # Visit root
    result.extend(preorder(root.left))   # Traverse left
    result.extend(preorder(root.right))  # Traverse right
    return result
```

**Traversal Path**:
```
1 (root) → 2 (left) → 4 (left of 2) → 5 (right of 2) → 3 (right of 1)
```

## In-order Traversal (Left → Root → Right)

**Order**: Visit left subtree first, then root, then right subtree.

**Result**: `[4, 2, 5, 1, 3]`

```python
def inorder(root):
    if not root:
        return []
    
    result = []
    result.extend(inorder(root.left))    # Traverse left
    result.append(root.val)              # Visit root
    result.extend(inorder(root.right))   # Traverse right
    return result
```

**Traversal Path**:
```
4 (leftmost) → 2 (parent of 4) → 5 (right of 2) → 1 (root) → 3 (right of 1)
```

**Note**: For Binary Search Trees, in-order traversal gives nodes in sorted order!

## Post-order Traversal (Left → Right → Root)

**Order**: Visit left subtree first, then right subtree, then root.

**Result**: `[4, 5, 2, 3, 1]`

```python
def postorder(root):
    if not root:
        return []
    
    result = []
    result.extend(postorder(root.left))   # Traverse left
    result.extend(postorder(root.right))  # Traverse right
    result.append(root.val)               # Visit root
    return result
```

**Traversal Path**:
```
4 (leftmost leaf) → 5 (right leaf) → 2 (parent) → 3 (right leaf) → 1 (root)
```

## Traversal Successors and Predecessors
When working with trees, especially binary trees, you'll hear about terminology such as the **inorder successor** or the **inorder predecessor**. For a given value in a inorder seqeunce, the predecessor is the element that was processed before the element, whilst the successor is the following element in the sequence. 

For example if you need to find the pre-order successor of a given value, you'll return the element after the value in the pre-order traversal sequence.

## Level Order Traversal (BFS)

**Order**: Visit nodes level by level, left to right.

**Result**: `[1, 2, 3, 4, 5]`

```python
from collections import deque

def level_order(root):
    if not root:
        return []
    
    result = []
    queue = deque([root])
    
    while queue:
        node = queue.popleft()
        result.append(node.val)
        
        if node.left:
            queue.append(node.left)
        if node.right:
            queue.append(node.right)
    
    return result
```

**Level-by-Level Breakdown**:
```
Level 0: [1]
Level 1: [2, 3]
Level 2: [4, 5]
```

## Why Recursion Matters

Tree traversals demonstrate the power of recursion:

1. **Base case**: Empty node (`if not root: return`).
2. **Recursive case**: Process current node and recurse on children.
3. **Self-similarity**: Each subtree is processed the same way.

This recursive thinking is fundamental for:
- **Dynamic Programming**: Breaking problems into subproblems.
- **Tree algorithms**: Every tree operation uses these patterns.
- **Divide and conquer**: Solving by combining solutions to smaller parts.

## Credits
- [Tree Traversals inorder, preorder, and postorder - geeksforgeeks](https://www.geeksforgeeks.org/tree-traversals-inorder-preorder-and-postorder/)