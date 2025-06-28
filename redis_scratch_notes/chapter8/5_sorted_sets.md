# Sorted Set

## 11.1 Sorted Set Interfaces
Redis has many sorted set commands, but they are just interfaces of 3 types of queries:
1. Point query: Find a single pair by name.
2. Range query: 
  - Seek to the nearest pair by (score, name).
  - Iterate from a pair in sorted order.
3. Rank query:
  - Find the nth pair in sorted order.
  - Find the position of a given pair in sorted order.

Hashtables only manage the point queries. Databases can do point and range queries because they're based on trees. Rank queries are unique to Redis, and they require you to modify the tree data structure. There are two types of update-related operation:
  1. Insert a new pair.
  2. Query for an existing pair to update or delete. 

The real Redis has a lot of sorted set comamnds. Some commands only use score, others only use name or even no parameters at all. We'll replace all of these with a simpler interface:
```
ZQUERY key score name offset limit
```

### Time Complexities and Efficiencies
| Operation               | Average  | Worst    | Data Structure          |
| ----------------------- | -------- | -------- | ----------------------- |
| Add or remove a pair    | O(log N) | O(N)     | Hashtable & tree insert |
| Point query by name     | O(1)     | O(N)     | Hashtable lookup        |
| Seek by (score, name)   | O(log N) | O(log N) | Tree lookup             |
| Iterate in sort order   | O(1)     | O(log N) | Tree iteration          |
| Find the rank of a pair | O(log N) | O(log N) | Augmented tree          |
| Seek by rank            | O(log N) | O(log N) | Augmented tree          |

The time complexities should be self explanatory for the most part. Hashtables for look ups, whilst comparison based sorting is done by the balanced trees.

The rank queries are new, and they're used for this idea of **offsetting**. To remind you, **offsetting** means skipping a number of items before we start to return results. The offset is the number of elements to skip. Offsetting a range query is at least $O(d)$ since we're skipping $d$ items one by one. It's also why paginations with offsets is kind of inefficient in SQL. However, Redis has modified data structures and can find the $d$-th item in $O(logN)$, making it a much more efficient operation when you offset by a large number.

## 11.2 Sorted Set Data Types
```C
// pseudo code
struct SortedSet {
    std::set<std::pair<double, std::string>> by_score;
    std::unordered_map<std::string, double>  by_name;
};

// This seems to be the sorted set implementation?
struct ZSet {
    AVLNode *root = NULL;   // index by (score, name)
    HMap hmap;              // index by name
};

struct ZNode {
    // Again this is an intrusive data structure so this ZNode is the only copy of the data. Then we have our data structures that let us quickly lookup the node or put the data in a balanced tree.
    AVLNode tree;
    HNode   hmap;

    // Here's the actual data that we're dealing with
    double  score = 0;
    size_t  len = 0;
    
    char    name[0];        // flexible array
};

// Then we'll implement the following point query and update function

// Insert something into our key-value store, look up something from the key-value store, delete something from the key-value store.
bool   zset_insert(ZSet *zset, const char *name, size_t len, double score);
ZNode *zset_lookup(ZSet *zset, const char *name, size_t len);
void   zset_delete(ZSet *zset, ZNode *node);
```

### Flexible Arrays in C

I'm going to put the write up separately, so this focuses more on why we use a flexible array here.


```C
static ZNode *znode_new(const char *name, size_t len, double score) {
    ZNode *node = (ZNode *)malloc(sizeof(ZNode) + len); // struct + array
    avl_init(&node->tree);
    node->hmap.next = NULL;
    node->hmap.hcode = str_hash((uint8_t *)name, len);
    node->score = score;
    node->len = len;
    memcpy(&node->name[0], name, len);
    return node;
}

static void znode_del(ZNode *node) {
    free(node);
}
```








## 11.3 Sorted set lookup, insert, and deletion

## 11.4 Sorted set range query

## 11.5 Sorted set range query

## Credits
- [Sorted Set](https://build-your-own.org/redis/11_sortedset)