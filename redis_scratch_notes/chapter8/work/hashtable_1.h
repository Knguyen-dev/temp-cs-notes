#pragma once
#include <stddef.h>
#include <stdint.h>

/**
 * Hash table node that's embedded in the payload (intrusive data structure).
 * We're using hash tables with chaining, so this is a linked list node containing 
 * the next node if it exists.
 * 
 * - hcode: A hashcode for the key associated with this node. Notice that the hashcode 
 *  isn't the index position that the node is at in the hashtable. Let me explain to you the workflow:
 * 
 * 1. Given a string, we can safely treat that string as a pointer to a buffer of 8-bit integers (char = 8bits) (sequence of bytes)
 * 2. We can hash that sequence of bytes, which effectively hashes our string. This outputs our hashcode. 
 * 3. We do bitwise AND with this entry's hash code and the hashtable's bit-mask, which yields the bucket index. 
 * 
 * Note: Hash codes aren't cryptographic, they aren't designed to be hard to crack because they don't need to be.
 * They just need to be designed for speed. 
 */
struct HNode {
  HNode *next = NULL; 
  uint64_t hcode = 0;
};

/*
A simple fixed-size hashtable; Underlying supporting structure.
- tab: Array of slots
- mask: Equals size-1. Remember that the valid indices would be in range [0, size-1] and this bitmask is used to do the hash function calculatiosn quickly. However this works only if the table size
 is a power of 2. For example, let size of the table N=8, then mask=2^{3}-1=7=0b111. So any hash code is mapped with hcode & 0b111, which is a value between [0,7] for the indices, which
 should be valid. Just an efficient way of calculating the indices. We'll also use mask+1 in our code to represent the current maximum capacity of our hashmap. 
- size: Current Number of entries in the hash table.
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
