## Why The Double Pointer
This has been brought up in past chapters, so I want to illustrate and review why they're using a double pointer and whatnot. Then I'll talk about how this fits into our binary tree logic.

---
### The Problem

In C, a pointer like `AVLNode *node` lets you modify the **contents** of an AVLNode structure. However, you cannot modify the pointer **that points to this node** from within a function that only receives the node pointer.

Consider this scenario: you have a parent node, and you want to replace one of its children. If you only have `AVLNode *node` (the child to be replaced), you cannot modify `parent->left` or `parent->right` to point to a different node.

---
### The Solution: Double Pointers

A **double pointer (`AVLNode **)`** solves this issue because it points to the pointer that points to the node. This gives you the ability to modify the parent pointer, more specifically its inner left and right pointers. Let's see this in action with a deletion example:
```c
AVLNode **from = parent->left == node ? &parent->left : &parent->right;
*from = victim;
```
Here's what's happening:
1. `from` is a pointer to either `&parent->left` or `&parent->right`.
2. `from` is essentially a **double pointer**. It points to the pointer that points to the child being deleted.
3. `node` is the child we want to delete
4. `victim` is the replacement node (like an inorder successor)

---
### How It Works
The key insight is that `from` holds the **address** of the parent's child pointer. It holds the address of the left or right pointers themselves. Now we're getting closer to modifying the parent's child pointers so that they point to something else:

- If we're replacing the left child: `from` points to `parent->left`. This makes sense since you want to modify what `parent->left` points to.
- If we're replacing the right child: `from` points to `parent->right`. Again this makes sense since we want to change what `parent->right` points to. 

Now, when we do `*from = victim`, we're effectively doing:
- `parent->left = victim` (if replacing left child), or  
- `parent->right = victim` (if replacing right child)

Because remember `*from == parent->left` or `*from == parent->right`. In any case, we're updating the corresponding parent pointer's left or right pointers to point at the inorder successor rather than that deleted node! That's the essence of what we're doing.

### Memory Visualization

```
Before:
parent->left -----> node (to be deleted)
    ^
    |
   from points to this address

After *from = victim:
parent->left -----> victim (replacement)
    ^
    |
   from still points to this address
```

## Single vs Double Pointers in Action
### Bad: Looking at the single pointer approach
If you try to modify the parent's child pointesr using a singe pointer, we'll run into the issue that we can't modify what the parent points to.
```C
void delete_node_BROKEN(TreeNode* node_to_delete) {
    // Find inorder successor
    TreeNode* successor = find_min(node_to_delete->right);
    
    // This DOESN'T work - we're only changing our local copy!
    // node_to_delete is just a local copy of the pointer and changing it doesn't 
    // affect the parent's left or right pointers. The parent sitll points to the node we 
    // wanted to delete
    node_to_delete = successor;  // ❌ Parent still points to old node!
}
```
Visually here's what just happened:
```
Before deletion attempt:
    Parent
    /    \
   X      Y
  /
Old Node  <-- Parent->left points here
 
After failed single pointer approach:
    Parent
    /    \
   X      Y        New Node (successor)
  /                    ^
Old Node              /
                node_to_delete now points here, which isn't intended.
                (but Parent->left still points to Old Node!)
```

### Good: Double Pointer Approach
```C
void delete_node_WORKING(TreeNode** from) {
    TreeNode* victim = *from;  // The node to delete

    // from holds the address of the parent's child pointer (parent->left or parent->right)
    // Whilst *from is the actual AVLNode* child pointer itself. SO again:
    // 1. TreeNode** from = &(parent->left); // or &(parent->right)
    // 2. *from == parent->left; 
    // 3. If you now just do *from = other_node_pointer, you can win?
    // 4. Yeah now just do *from = successor_ptr, which is equivalent to parent->left = successor_ptr.
    if (victim->left && victim->right) {
        // Find inorder successor
        TreeNode** successor_ptr = &(victim->right);
        while ((*successor_ptr)->left) {
            successor_ptr = &((*successor_ptr)->left);
        }
        
        TreeNode* successor = *successor_ptr;
        
        // Remove successor from its current position
        *successor_ptr = successor->right;
        
        // Replace victim with successor
        successor->left = victim->left;
        successor->right = victim->right;
        *from = successor;  
        free(victim);
    }
}
```

## Why This Matters for Binary Trees

In tree operations like deletion, you often need to:
1. Find a node to delete
2. Find its replacement (successor/predecessor)
3. **Update the parent's pointer** to point to the replacement

Without double pointers, you'd need to pass the parent node and determine which child to update. With double pointers, you can directly modify the relevant parent pointer, making the code cleaner and more efficient. This pattern is especially useful in recursive tree operations where you need to modify the tree structure as you traverse it.
