# Implementing the Hashtable

## Starting with Fixed Size hash tables
### Step 0: Choosing a hash function
- There are various different types of hash functions. Cryptographic, checksum, and hash functions for hash tables. We'll use that last one (FNV, Murmur, etc.)

### Step 1: Define intrusive linked list node
```C
struct HNode {
  HNode *next = NULL;
  uint64_t hcode = 0; // hash value
}
```
An intrusive linked list node. The intrusive hashtable won't care about the data, but we need this hash value to calculate the index of the bucket this belongs to.

### Step 2: Define the fixed-size hashtable
```C
struct HTab {
    HNode **tab = NULL; // array of slots
    size_t mask = 0;    // power of 2 array size, 2^n - 1
    size_t size = 0;    // number of keys
};

static void h_init(HTab *htab, size_t n) {
    assert(n > 0 && ((n - 1) & n) == 0);    // n must be a power of 2
    htab->tab = (HNode **)calloc(n, sizeof(HNode *));
    htab->mask = n - 1;
    htab->size = 0;
}
```
The expression `hash(key) % N` maps to an index in range [0, N-1]. Modulo or division are considered slow, so it's common to use bitwse operations. So you'd just 
need the array size to be a power of 2. Then you can just do `hash(key) & (N-1)`

### Step 3: Linked List Insertion
```C
static void h_insert(HTab *htab, HNode *node) {
    size_t pos = node->hcode & htab->mask;  // node->hcode & (n - 1)
    HNode *next = htab->tab[pos];
    node->next = next;
    htab->tab[pos] = node;
    htab->size++;
}
```
New nodes are inserted at the front of hte linked list, making this $O(1)$. Linked list are the most common collection in large C projects, and almost all practical linked lists are intrusive. Non-intrusive linked lists like `std::list` are rarely useful.

### Step 4: Hashtable lookup
```C
static HNode **h_lookup(HTab *htab, HNode *key, bool (*eq)(HNode *, HNode *)) {
    if (!htab->tab) {
        return NULL;
    }
    size_t pos = key->hcode & htab->mask;
    HNode **from = &htab->tab[pos];     // incoming pointer to the target
    for (HNode *cur; (cur = *from) != NULL; from = &cur->next) {
        if (cur->hcode == key->hcode && eq(cur, key)) {
            return from;    // Q: Why not return `cur`? This is for deletion.
        }
    }
    return NULL;
}
```
The reason we're passing a function pointer is because our generic search function knows nothing about the data, due to its generic nature. we aren't returning the pointer of the node (target pointer), but instead we're going to return the address of its parent pointer. This is because we need the address of the parent pointer to delete the current pointer.

### Step 5: Hashtable deletion
```C
// Remove a linked list node by changing what the parent pointer points to. 
// When detaching a node, if it's an empty slot
// You can just simply return the address of the to-be-updated pointer, regardless of its an empty slot or not.
static HNode *h_detach(HTab *htab, HNode **from) {
    HNode *node = *from;    // the target node
    *from = node->next;     // update the incoming pointer to the target
    htab->size--;
    return node;
}

// In another function...
HNode **from = &htab->tab[pos];     // incoming pointer to the target
for (HNode *cur; (cur = *from) != NULL; from = &cur->next) {
    if (cur->hcode == key->hcode && eq(cur, key)) {
        return from;                // may be a node, may be a slot
    }
}
```
## Dealing with progressive rehashing 
```C
struct HMap {
    HTab newer;
    HTab older;
    size_t migrate_pos = 0;
};

// Normally Hmap::newer is used whilst HMap::older is unused. When the load factor is too high, HMap::neer is moved to Hmap::older, and TMap::newer is replaced with a hashmap that's double the size
static void hm_trigger_rehashing(HMap *hmap) {
    hmap->older = hmap->newer;  // (newer, older) <- (new_table, newer)
    h_init(&hmap->newer, (hmap->newer.mask + 1) * 2);
    hmap->migrate_pos = 0;
}

// For a chaining hashtable, the load factor should be greater than 1 since each slot is intended to hold multiple items.
void hm_insert(HMap *hmap, HNode *node) {
    if (!hmap->newer.tab) {
        h_init(&hmap->newer, 4);    // initialized it if empty
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

// Notice that the key doesn't actually need to be an Entry.
// It could be other types that embed an HNode. As long as you update the implementation of your comparison function things should work 
static void do_get(std::vector<std::string> &cmd, Response &out) {
    // a dummy `Entry` just for the lookup
    Entry key;
    key.key.swap(cmd[1]);
    key.node.hcode = str_hash((uint8_t *)key.key.data(), key.key.size());
    // hashtable lookup
    HNode *node = hm_lookup(&g_data.db, &key.node, &entry_eq);
    if (!node) {
        out.status = RES_NX;
        return;
    }
    // copy the value
    const std::string &val = container_of(node, Entry, node)->val;
    assert(val.size() <= k_max_msg);
    out.data.assign(val.begin(), val.end());
}

// Finally we should know that Entry is just a KV pair, but our data for a redis entry will get larger as we go. So you can define a leaner struct for lookups

struct LookupKey {
  HNode node;
  std::string
}
```