#include <assert.h>
#include <stdlib.h>     // For calloc(), free()
#include "hashtable_1.h"

/**
 * This code allocates memory for that hashmap with chaining. It 
 * uses calloc so that we can quickly get zeroed memory. We also 
 * put in the mask and default size of the hashmap. 
 */
static void h_init(HTab *htab, size_t n) {
  assert(n > 0 && ((n - 1) & n) == 0);
  htab->tab = (HNode **)calloc(n, sizeof(HNode *));
  htab->mask = n - 1;
  htab->size = 0;
}


/**
 * This inserts a node into the hashmap:
 * 1. Use precompute hash and mask to determine the index of the bucket.
 * 2. Get the pointer to the head of the linked list associated with the bucket. 
 * 3. Make the current node the new head of the bucket. Also update array to contain pointer to new head.
 * 4. Increase the size of the hashmap that's for sure.
 */
static void h_insert(HTab *htab, HNode *node) {
  size_t pos = node->hcode & htab->mask;
  HNode * next = htab->tab[pos];
  node->next = next;
  htab->tab[pos] = node;
  htab->size++;
}

/**
 * Parameters:
 * - htab: Underlying hash table data structure. This should exist.
 * - key: Hash node that represents the key. It contains the hashcode we'll use to get an index.
 * - eq: This is a pointer to a function that returns boolean after comparing two different hash node pointers.
 * 
 * 
 * Algorithm
 * 1. If the actaul table pointer isn't allocated, then early return.
 * 2. Calculate the index that the key relates to. 
 * 3. Get a pointer to the first node in the bucket
 *  - htab->tab[pos] is HNode*, a pointer to the first node in the linked list.
 *  - Then &htab->tab[pos] is a pointer to that pointer HNode **.
 *  - This allows our motivation to return not the node itself (HNode*), but a reference to the 
 *    node, so that the caller can access, modify, or even delete it later.
 * 4. Iterates through the list. cur should be a pointer to the current node which is de-referenced from "from"
 *    We update the *from pointer as we move on.
 *    - If the hashes of the current node and the node we're looking up match, we'll return a pointer that 
 *      pointers to the node that matches.
 * 
 * Return:
 *  - It could be a pointer to `htab->tab[pos]`, the head of a list. 
 *  - It could be a pointer to cur->next, somewhere in the middle or tail.
 *  - Return when not found.
 * 
 * Note: Motivation to use a double pointer (HNode **from) 
 * You'd use this because it allows your calling function to modify the pointer that points to HNode*. Demonstrate:
 *   a. HNode **from = &htab->tab[pos]; A pointer to HNode*
 *   b. for (HNode *cur; (cur = *from) != NULL; from = &cur->next); since we have a double pointer,
 *      we can update from to be a pointer to the next HNode*. If we didn't use a double pointer, we would 
 *      need to keep track of the previous node during traversal.
 *  
 */
static HNode **h_lookup(HTab *htab, HNode *key, bool (*eq)(HNode *, HNode *)) {
  if (!htab->tab) {
    return NULL;
  }
  size_t pos = key->hcode & htab->mask;
  HNode **from = &htab->tab[pos];
  for (HNode *cur; (cur = *from) != NULL; from = &cur->next) {
    if (cur->hcode == key->hcode && eq(cur, key)) {
      return from; // may be a node, may be a slot; 
    }
  }
  return NULL;
}

/**
 * Removes a node from our hashtable. Specifically it removes the head node from a given linked list.
 * 
 * Algorithm:
 * 1. One level of de-reference since we want to access the node's data. 
 * 2. Update the head of the linked list to point at the node after the head; effectively removing entry.
 *   So now that inner HNode* points to a new memory.
 * 3. Decrement size of the hash table 
 * 
 */
static HNode *h_detach(HTab *htab, HNode **from) {
  HNode *node = *from;
  *from = node->next;
  htab->size--;
  return node;
}



const size_t k_rehashing_work = 128;

/**
 * A helper function for progressive rehashing
 * 
 * 
 * 
 */
static void hm_help_rehashing(HMap *hmap) {
  size_t nwork = 0;
  while (nwork < k_rehashing_work && hmap->older.size > 0) {
    // find a non-empty slot
    HNode **from = &hmap->older.tab[hmap->migrate_pos];
    if (!*from) {
      hmap->migrate_pos++;
      continue;
    }
    // Move the first list item to the newer table
    h_insert(&hmap->newer, h_detach(&hmap->older, from));
    nwork++;
  }

  // Discard the old table if we'er done migrating keys 
  if (hmap->older.size == 0 && hmap->older.tab) {
    free(hmap->older.tab);
    hmap->older = HTab{};
  }
}

static void hm_trigger_rehashing(HMap *hmap) {
  assert(hmap->older.tab == NULL);
  // (newer, older) <- (new_table, newer)
  hmap->older = hmap->newer;
  h_init(&hmap->newer, (hmap->newer.mask+1) * 2);
  hmap->migrate_pos = 0;
}

HNode *hm_lookup(HMap *hmap, HNode *key, bool (*eq)(HNode *, HNode *)) {
  hm_help_rehashing(hmap);
  HNode **from = h_lookup(&hmap->newer, key, eq);
  if (!from) {
    from = h_lookup(&hmap->older, key, eq);
  }
  return from ? *from : NULL;
}

const size_t k_max_load_factor = 8;

void hm_insert(HMap *hmap, HNode *node) {
  if (!hmap->newer.tab) {
      h_init(&hmap->newer, 4);    // initialize it if empty
  }
  h_insert(&hmap->newer, node);   // always insert to the newer table

  if (!hmap->older.tab) {         // check whether we need to rehash
      size_t shreshold = (hmap->newer.mask + 1) * k_max_load_factor;
      if (hmap->newer.size >= shreshold) {
          hm_trigger_rehashing(hmap);
      }
  }
  hm_help_rehashing(hmap);        // migrate some keys
}

HNode *hm_delete(HMap *hmap, HNode *key, bool (*eq)(HNode *, HNode *)) {
  hm_help_rehashing(hmap);
  if (HNode **from = h_lookup(&hmap->newer, key, eq)) {
      return h_detach(&hmap->newer, from);
  }
  if (HNode **from = h_lookup(&hmap->older, key, eq)) {
      return h_detach(&hmap->older, from);
  }
  return NULL;
}

void hm_clear(HMap *hmap) {
  free(hmap->newer.tab);
  free(hmap->older.tab);
  *hmap = HMap{};
}

size_t hm_size(HMap *hmap) {
  return hmap->newer.size + hmap->older.size;
}