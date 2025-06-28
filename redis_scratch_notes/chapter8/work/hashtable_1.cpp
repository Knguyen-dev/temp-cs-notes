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
 * Return parent pointer that points to the node.
 * 
 * Parameters:
 * - htab: Underlying hash table data structure. This should exist.
 * - key: Hash node that represents the key. It contains the hashcode we'll use to get an index.
 * - eq: This is a pointer to a function that returns boolean after comparing two different hash node pointers.
 * 
 * Algorithm:
 *  1. Get index associated with key.
 *  2. Get a parent pointer to the head of the linked list; using double pointer makes traversal logic easier. 
 *  3. Iterate through the linked list, only return our parent pointer, when the key of the current node matches
 *     the inputted key. Parent pointer points to current node pointer so that works out.
 * 
 * Return:
 *  - It could be a pointer to `htab->tab[pos]`, the head of a list. 
 *  - It could be a pointer to cur->next, somewhere in the middle or tail.
 *  - Return when not found.
 * 
 * Note: Motivation to use a double pointer (parent pointer of the target node) (HNode **from) 
 * You'd use this because it allows your calling function to modify the pointer that points to HNode*. Demonstrate:
 *   a. HNode **from = &htab->tab[pos]; A pointer to HNode*
 *   b. for (HNode *cur; (cur = *from) != NULL; from = &cur->next); since we have a double pointer,
 *      we can update from to be a pointer to the next HNode*. If we didn't use a double pointer, we would 
 *      need to keep track of the previous node during traversal.

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
 * 1. Deferrence to access node pointer
 * 2. Update parent pointer to point to the next list in the chain. 
 * 3. Decrement hash table's size and return the pointer of the removed ndoe
 * 
 * Returns removed node
 * 
 * Note: This doesn't actually delete the node from memory. This function can be 
 * used for moving stuff around.
 */
static HNode *h_detach(HTab *htab, HNode **from) {
  HNode *node = *from;
  *from = node->next;
  htab->size--;
  return node;
}

/**
 * A helper function for progressive rehashing.
 * 
 * 
 * Algorithm: 
 * - nwork: Tracks how much "work" or rehashing we've done. The number of entries migrated. 
 * - While we haven't reached rehashing limit and the old hashmap still has entries
 *    1. Get a double pointer to a head of the linkedlist in the old map; 
 *    2. If the actual node pointer is null, it's an empty slot so skip it and increase the index position
 *    3. Move a node from the "older" hashtable bucket to the "newer" one. 
 *    4. Increase the nwork value by 1 to indicate we moved one item.
 *  
 * Note: Double pointer is likely needed since insert or detach updates **from to be a pointer to the next node pointer.
 * - If the older hashmap is defined and has zero keys, we're done with it:
 *    1. Free the double pointer 
 *    2. Make our older hash table point to a zero initialized HTab instance (C++) syntax.
 */
const size_t k_rehashing_work = 128;
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
  // Discard the old table if we're done migrating keys 
  if (hmap->older.size == 0 && hmap->older.tab) {
    free(hmap->older.tab);
    hmap->older = HTab{};
  }
}

/**
 * Create new table for the hashmap and ensures older and new references are properly shifted and initialized.
 * is a bigger table than table2. 
 * 
 * At this point, I'm assuming all entries are in our current newer hashtable.
 * 1. Our current new table becomes the old table; 
 * 2. Update the new table to point at a newly initialized hashtable.
 * 3. hmap.newer.mask+1 (equivalent to the size of the hashmap) create a hashtable that's double the size.
 * 4. Reset the migrate position index to prepare for progressive re-hashing
 */
static void hm_trigger_rehashing(HMap *hmap) {
  assert(hmap->older.tab == NULL);
  // (newer, older) <- (new_table, newer)
  hmap->older = hmap->newer;
  h_init(&hmap->newer, (hmap->newer.mask+1) * 2);
  hmap->migrate_pos = 0;
}


// **** Start creating the functions that actually going to be exposed by our API/Library ****

/**
 * Give a node with a given key, return a pointer to the HNode that matches that key. 
 * 
 * Algorithm: 
 * 1. Start migrating entries (entire buckets)
 * 2. Search the newer hashmap for the key-value pair
 * 3. If we didn't find it, from the newer hashtable, then search the older for the key-value pair
 * 4. If our parent pointer is defined, return a pointer to the node, else return null. 
 */
HNode *hm_lookup(HMap *hmap, HNode *key, bool (*eq)(HNode *, HNode *)) {
  hm_help_rehashing(hmap);
  HNode **from = h_lookup(&hmap->newer, key, eq);
  if (!from) {
    from = h_lookup(&hmap->older, key, eq);
  }
  return from ? *from : NULL;
}

/**
 * Inserts a node somewhere into the hashmap. 
 * 
 * Algorithm: 
 * - If our newer hash table isn't defined, define it with a size of 4. 
 * - Then insert into the newer table. We always want to insert into the newer table. 
 * - If older table doesn't exist, compute a threshold:
 *    1. Our threshold = size * max load factor; a common threshold, the load factor is really high actually usually it's around 0.75 in java
 *    2. If the number of entries exceeds the threshold, trigger rehashing
 * 
 */
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

/**
 * Deletes a node associated with the key node in the hash map. 
 * 
 * 1. Migrate some keys.
 * 2. Attempt to find key-value pair in the newer hash table. 
 * 2. Attempt to find key-value pari in older hash table. 
 * 3. At this point it wasn't in either tables, so return NULL. 
 */
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

/**
 * Clear the hashmap by freeing the memory associated with the older and newer hashtables. 
 * Also zeroes the memory of the entire hashmap pointer. 
 * 
 * Note: They're only freeing the memory of the HNode**, the parent pointers. Does that mean 
 * that each HNode* is not dynamic memory? The only other time we use free is in hm_help_rehashing?
 */
void hm_clear(HMap *hmap) {
  free(hmap->newer.tab);
  free(hmap->older.tab);
  *hmap = HMap{};
}

/**
 * Get the size of the hashmap by summing up the sizes of the older and newer hashtables.
 */
size_t hm_size(HMap *hmap) {
  return hmap->newer.size + hmap->older.size;
}