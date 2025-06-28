#pragma once

#include <stddef.h>
#include <stdint.h>

struct AVLNode {
  AVLNode *parent = NULL;
  AVLNode *left = NULL;
  AVLNode *right = NULL;

  // height of subtree. I'm guessing this is 
  // either the height of the larger one, or maybe the height diff. 
  uint32_t height = 0;  
  uint32_t count = 0;
};

inline void avl_init(AVLNode *node) {
  node->parent = NULL;
  node->left = NULL;
  node->right = NULL;
  node->height = 1;
  node->count = 1;
}

// Internal helper functions; we'll use this in the actual CPP file.
inline uint32_t avl_height(AVLNode *node) { 
  return node ? node->height : 0; 
}
inline uint32_t avl_count(AVLNode *node) { 
  return node ? node->count : 0; 
}

// API that we'll expose.
AVLNode *avl_fix(AVLNode *node);
AVLNode *avl_del(AVLNode *node);