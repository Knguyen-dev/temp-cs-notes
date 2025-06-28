# Balanced Binary Search Tree


## Sorted Sets for sorting
This is kind of early, but let's talk about sorted sets. A sorted set is a collection of sorted key-value pairs e.g. (score, name). The obvious use case is when you want to rank things, such as needing to get teh top-k items from a high score board. Though, a useful data structures needs to be generic so that we can use it for most things.

$(s_{1}, n_{1}) < (s_{2}, n_{2}) \implies s_{1} < s_{2} || (s_{1} = s_{2} && n_{1} < n_{2})$

This is **tuple comparison**, and the idea is to rank pairs by some metric that we denote as the score. We can have name as a byte string. So let's say you wanted to keep a player leaderboard. You'd have the score of the player and the name. Akin to a priority queue, the sets are ordered by their score, with the set having the highest score being at the top. 


**Multi-indexed data:** The score is a 64-bit float, and it doesn't have to be unique. But the name is unique, allowing us to modify a single pair by name. The collection is index in 2 ways, and multiple indices like this are common in databases:
```cpp
struct SortedSet {
  std::set<std::pair<double, std::string>> by_score;
  std::unordered_map<std::string, double> by_name;
}
```
Indexing by score requires a sorting data structures. In databases, you're typically using B-Trees, or LSM-trees, which are somewhat tricky to implement. For in-memory data stores, you have more choices including binary trees. Many databases need to sort data which is why database indices are mostly trees rather than hashtables, even though accessing a tree is slower. Sorting data structures in STL (C++ lib) are backed by a red black tree, and the real Redis application uses skip lists for sorting. For simplicity, we'll use AVL trees instead.

## Tree Data Structures
Almost all sorting data structures are trees, as your data falls into non-overlapping subtrees. With height-balanced trees, you're going to modify the tree after specific operations in order to make sure the height of the tree is at worse $O(logn)$. As a result, that's the search time. Some examples are B, red-black, and AVL trees. However other tree data structures like treap or skiplist, have some randomness in them, and so they don't maintain the worse case but still get the job done.

## Introduction to Balanced Binary Search Trees
```C++
struct Node { 
  Node *left; 
  Node *right; 
};

// *** PseudoCode ***
// Search for a node in the tree that has a given key.
Node **tree_search(Node **from, int32_t (*cmp)(Node *, void *), void *key) {
    Node *node = *from
    if (!node) {
        return from;    // NULL node
    }
    int32_t r = cmp(node, key);
    if (r < 0) {        // node < key
        return tree_search(&node->right, cmp, key);
    } else if (r > 0) { // node > key
        return tree_search(&node->left, cmp, key);
    } else {            // node == key
        return from;
    }
}

// Then in some other point in your program
struct Data {
  Node node;
  T key; // intrusive data structure with a tree nod 
}

Data *new_data = make_some_data();
Node **child = tree_search(&root, cmp, &new_data->key);
if (!*child) { // not found in our tree, so insert something
  *child = &new_data->node; 
} // else: node already exists in the tree, so insertion would be duplication!
```

We use the "incoming pointer" trick, where we use a double pointer. The function returns the address of the pointer to the target node, so if the search ends at NULL pointer, this address can be used for an insertion. We should also talking about "detaching" a node, which just means when we delete a node, please it with its successor node. There are three main cases:
  - Left node: It has no children, so just remove the node.
  - One child: Replace the node with its child.
  - Two children: Replace the node with its in-order successor or in-order predecessor.

## Credits
- [AVL Trees](https://build-your-own.org/redis/10_avltree)