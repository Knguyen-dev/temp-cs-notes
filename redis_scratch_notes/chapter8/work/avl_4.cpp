#include <assert.h>
#include "avl_4.h"

/**
 * ## Description
 * Returns the maximum height between the two subtrees.
 * 
 * ## Parameters
 * - lhs: Left subtree height
 * - rhs: Right subtree height
 * 
 * ## Return Type
 * A 4byte unsigned integer representing the maximum height of the 
 * subtree. 
 * 
 * Note: 4 unsigned bytes is 2^32, which is more than enough to 
 * represent the maximum height of a subtree.
 */
static uint32_t max(uint32_t lhs, uint32_t rhs) {
    return lhs < rhs ? rhs : lhs;
}

/**
 * ## Description
 * Updates the height and count fields if a given node. This is helpful to make sure all
 * nodes stay updated and are in valid states. 
 * 
 * ## Parameters
 * - AVLNode* node: Node that we're updating.
 * 
 * ## Return Type
 * We don't return anything. 
 * 
 * ## Step by Step
 * 1. Calculate the height of the current node as 1 + the maximum height between its subtrees. 
 * 2. Calculate the count for hte node being 1 + the counts of the left and right subtrees.
 */
static void avl_update(AVLNode *node) {
    node->height = 1 + max(avl_height(node->left), avl_height(node->right));
    node->count = 1 + avl_count(node->left) + avl_count(node->right);
}

/**
 * ## Description
 * Does a left rotation in an AVL tree.
 * 
 * ## Return Type
 * A pointer to the newly balanced AVL tree.
 * 
 * ## Step by Step
 * 1. Get parent P, R (new node), and then RL (inner). 
 * 2. A left subtree is moved to become the right subtree of the original root
 * 3. Regardless if RL existed, we'll update its parent to point at the old root.
 * 4. Make sure the new root has the parent of the old root. Then make sure the old root's 
 *    parent is the new root. 
 * 
 * Note: From what I see, they aren't updating the parent node itself? Okay, when you rotate 
 * the tree, the parent of the node that you're rotating isn't going to get a new size or 
 * height, so you don't need to call avl_update(parent). 
 * 
 * However, I'm noticing that this function isn't responsible for updating the parent 
 * node's left or right subtree to point to the new subtree. This function returns a pointer 
 * to the newly balanced subtree, and then the caller of this function can handle updating the 
 * parent's left or right pointers.
 */
static AVLNode *rot_left(AVLNode *node) {
    // Get the parent, new root, and new root's left subtre 
    AVLNode *parent = node->parent;
    AVLNode *new_node = node->right;
    AVLNode *inner = new_node->left;

    // old root's right subtree is updated. 
    node->right = inner;
    if (inner) {
        inner->parent = node;
    }

    // the new root's parent is updated 
    new_node->parent = parent;

    // new root's left child is the old root.
    // Old root's parent is the new root 
    new_node->left = node;
    node->parent = new_node;

    // Update the data for the new and old roots. Then return the new root
    avl_update(node);
    avl_update(new_node);
    return new_node;
}

/**
 * ## Description
 * Does a right rotation on the tree. Basically a mirror of rot_left, so just refer 
 * to those notes if you need. 
 * 
 * ## Return Type
 * A pointer to the newly balanced AVL tree
 */
static AVLNode *rot_right(AVLNode *node) {
    AVLNode *parent = node->parent;
    AVLNode *new_node = node->left;
    AVLNode *inner = new_node->right;
    // node <-> inner
    node->left = inner;
    if (inner) {
        inner->parent = node;
    }
    // parent <- new_node
    new_node->parent = parent;
    // new_node <-> node
    new_node->right = node;
    node->parent = new_node;
    // auxiliary data
    avl_update(node);
    avl_update(new_node);
    return new_node;
}

/**
 * Do rotations when the left subtree is taller.
 * 
 * Handle fixing the left subtree by doing a single or double rotation if necessary
 * 1. If the height of LL < LR, we'd first do a left rotation.
 * 2. Regardless, do a right rotation right after. 
 */
static AVLNode *avl_fix_left(AVLNode *node) {
    if (avl_height(node->left->left) < avl_height(node->left->right)) {
        node->left = rot_left(node->left);  // Transformation 2
    }
    return rot_right(node);                 // Transformation 1
}

/**
 * When the right subtree is taller. This rebalances it by doing 
 * a single or double rotation. 
 * 1. If RR < RL, then do right rotation first
 * 2. Regardless do a left rotation. 
 */
static AVLNode *avl_fix_right(AVLNode *node) {
    if (avl_height(node->right->right) < avl_height(node->right->left)) {
        node->right = rot_right(node->right);
    }
    return rot_left(node);
}

/**
 * ## Description:
 * This function traverses up from a given node, checking imbalancing issues and doing rotations to fix 
 * those imbalancing issues on its way up.
 * 
 * ## Step by step:
 * 1. While true (while we aren't at the root I guess): 
 *  a. Get pointer to the "fixed subtree"
 *  b. Get the pointer to the parent. If the parent actually exists (which means node is not the root):
 *     1. Assign from to be a double pointer to the parent's left subtree
 *     2. Or assign from to the double pointer to right subtree. 
 *  c. Regardless update hte current node since its data is different?
 *  d. Calculate the height of the subtrees, and if there's a height difference of 2, we'll need to rebalance the tree.
 *     To do this, we'll update the from pointer to point to the fixed subtree after doing rotations on the node in question. 
 *  e. If the parent is undefined, that means we're at the root node, so we'll stop the function and return *from, which 
 *     is the pointer to the balanced binary tree. 
 *  f. Else we aren't at the root, so update node to be the parent, so that we're traversing up! 
 */
AVLNode *avl_fix(AVLNode *node) {
    while (true) {
        AVLNode **from = &node; // save the fixed subtree here
        AVLNode *parent = node->parent;
        if (parent) {
            // attach the fixed subtree to the parent
            from = parent->left == node ? &parent->left : &parent->right;
        }   // else: save to the local variable `node`
        // auxiliary data
        avl_update(node);
        // fix the height difference of 2
        uint32_t l = avl_height(node->left);
        uint32_t r = avl_height(node->right);
        if (l == r + 2) {
            *from = avl_fix_left(node);
        } else if (l + 2 == r) {
            *from = avl_fix_right(node);
        }
        // root node, stop
        if (!parent) {
            return *from;
        }
        // continue to the parent node because its height may be changed
        node = parent;
    }
}

/**
 * ## Description
 * Helper function that supports avl_del by taking care of the logic
 * for deleting a node when it has one child.
 * 
 * ## Step By Step
 * 1. Get the child of the node being deleted (could be null, when no children)
 * 2. Connect child to the parent (Note: parent could be NULL).
 * 3. If parent doesn't exist, we're deleting the root node, so just return the child of the root node.
 * 4. Create a pointer to the parent node's left or right pointer. Target the pointer that points to 
 *    the node we're deleting. Say we're deleting the left child, then from == &parent->left since we want to 
 *    update the parent->left pointer to point to something new. 
 * 5. We can then do *from = child, which is equivalent to parent->left = child. As a result, we're making 
 *    parent->left point to the child node, instead of the node being deleted!
 * 6. The subtree that the parent controls just lost a single node, so we'll need to ensure balancing. 
 */
static AVLNode *avl_del_easy(AVLNode *node) {
    assert(!node->left || !node->right);    // at most 1 child
    AVLNode *child = node->left ? node->left : node->right; // can be NULL (no children)
    AVLNode *parent = node->parent;
    if (child) {
        child->parent = parent; 
    }
    if (!parent) {
        return child;   
    }
    AVLNode **from = parent->left == node ? &parent->left : &parent->right;
    *from = child;
    return avl_fix(parent);
}



// detach a node and returns the new root of the tree
/**
 * ## Description
 * Handles the deleting an AVL node
 * 
 * ## Parameters
 * - node: Node being deleted. 
 * 
 */
/**
 * ## Description
 * Handle detaching a node and returns the new root of hte tree.
 * 
 * ## Parameters
 * - node: The node being detached
 * 
 */
AVLNode *avl_del(AVLNode *node) {
    // 1. If the node has only one or no children, handle the easy case. 
    // 2. At this point both subtrees are defined, so we'll find the inorder successor (victim) of the given node. 
    if (!node->left || !node->right) {
        return avl_del_easy(node);
    }
    AVLNode *victim = node->right;
    while (victim->left) {
        victim = victim->left;
    }

    // 1. Detach the inorder successor from its original position.
    // 2. Victim's pointee now has the data of node's pointee (replace the target node with the inorder successor?)
    // 3. Update L and R so that their parents point to the successor.  
    AVLNode *root = avl_del_easy(victim);
    *victim = *node;    
    if (victim->left) {
        victim->left->parent = victim;
    }
    if (victim->right) {
        victim->right->parent = victim;
    }

    // 1. Create a pointer to the root pointer. 
    // 2. Create pointer that points to the same thing as node->parent
    // 3. If the parent is defined (not the root), let "from" be a pointer to parent->left or parent->right pointer. 
    AVLNode **from = &root;
    AVLNode *parent = node->parent;
    if (parent) {
        from = parent->left == node ? &parent->left : &parent->right;
    }

    // 1. Equivalent to parent->left or parent->right = victim; so now the parent node's pointers are pointing to the inorder successor
    *from = victim;
    return root;
}