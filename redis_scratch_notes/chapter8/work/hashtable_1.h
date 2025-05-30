#pragma once
#include <stddef.h>
#include <stdint.h>

/**
 * Hash table node that's embedded in the payload (intrusive data structure).
 * We're using hash tables with chaining, so this is a linked list node containing 
 * the next node if it exists.
 * 
 * 
 * - hcode: A hashcode for the key associated with this node. The plan is 
 * to store it and avoid the idea of re-calculating the hash. This 
 * speeds up lookups as hash values aren't changing.
 * 
 * This probably isn't the index, I'm guessing hashcode also differentiates entries 
 * in the case of collisions.
 */
struct HNode {
  HNode *next = NULL; 
  uint64_t hcode = 0;
};

// 
// 
/*
A simple fixed-size hashtable; Underlying supporting structure.
- tab: Array of slots
- mask: A bitmask used to map a hash code to a bucket index. h(hashCode) = hashCode & mask. The reason we aren't 
using modulo is because bitwase AND is a lot faster. However this works only if the table size is a power of 2.

For example, let size of the table N=8, then mask=2^{3}-1=7=0b111. So any hash code is mapped with hcode & 0b111, which is 
a value between [0,7] for the indices, which should be valid. Just an efficient way of calculating the indices.

- size: Number of entries in the hash table.
*/
struct HTab {
  HNode **tab = NULL;
  size_t mask = 0;
  size_t size = 0;
};


/*
Hash table interface that we'll expose. It uses 2 hashtables for progressive rehashing. 
- newer: The current bigger hashtable that we're progressively storing our linkedlist pointers in.
- older: The current smaller hashtable that we're migrating from.
- migrate_pos: index position that we're currently migrating. 
*/
struct HMap {
  HTab newer;
  HTab older;
  size_t migrate_pos = 0; 
};

// *** Here are the functions we'll expose to our actual program ***

HNode *hm_lookup(HMap *hmap, HNode *key, bool (*eq)(HNode *, HNode *));
void   hm_insert(HMap *hmap, HNode *node);
HNode *hm_delete(HMap *hmap, HNode *key, bool (*eq)(HNode *, HNode *));
void   hm_clear(HMap *hmap);
size_t hm_size(HMap *hmap);
