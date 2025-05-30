# Using HashTables 
There are two data structures that we have to use for KV stores:
- Sorting data structure such as AVL tree, Treap, Trie, and B-tree.
- Two types of hashtables: Open addressing and chaining

Now we need that sorting data structure to nnarrow our search. Then hash tables to achieve an O(1) lookup

## Arrays to hashtables
If your map has keys with unique integers, we can use an array. However we're planing to store many non-integer types like strings structs, etc. Okay here's the plan:

- We'll be able to hash these various values into integers via a hash function. 
- We'll use a hash table that uses separate chaining, with linked lists for each bucket. 
- We'll consider doing resizing and rehashing based on the load factor of our hash function, which will contribute to keeping the collisions low and improve performance.

## Hashtables for Redis (Scalabillity)
The idea of hashtables are simple. The reason we code them ourselves rather than using `std::unordered_map` from C++ is because that will have scalability issues. There are two types of scalability problems:
- Throughput: Often generic easy, solutions, suc has sharding and read-only replicas. 
- Latency: Often domain specific and more complex.

For hashtables, the latency issue will come from insertion as this could trigger an $O(N)$ resize. In many cases, these libraries will automatically resize the hash table when it gets new full, which involves:
- Allocating a new, bigger array.
- Rehashing and copying all entries from the old array into the new one.

The issue with this is that if we all this all it once, the app freezes during that resize. For Redis, a real time system where each command must execute fast (miliseconds or less), this long pause is unacceptable. Most libraries focus on throughput (average-case performance), so using something from a library won't fit our needs.

###  Using Progressive Resizing
Redis doesn't need to resize the entire table in one step. Instead it sperads out the work over time:
1. When resize is needed:
  - Allocate a new larger table (`newTable`)
  - Keep both `oldTable` and `newTable` in memory.
2. Everytime you perform an operation:
  - Move a few entries (fixed number) from the `oldTable` to `newTable`
  - After enoguh operations, all entries from `oldTable` should be migrated.
3. During this time:
  - Reads and writes check both tables. Which will obviously slow things down a bit.

This is called **incremental rehashing**, as you're paying the resize cost gradually, avoiding a big latency spike.

---
### Using calloc() Trick: Zeroing Memory Efficiently
When we allocate a new hash table (the array), we must initialize the slots with values. In this case, we wnat to set all entries to null/zero, as we plan to rehash old entries into these slots. When you use something like `malloc()+memset()` it means that you want to allocate and clear all memory up front rather than gradually. This would be another $O(N)$ operation.

Instead we can use `calloc`, which gives us memory that's already zeroed. For large arrays, the OS gives returns this memory using `mmap()`:
  - Memory is not physically allocated, until the page is accessed. Meaning it only happens until we actually access an index position.
  - This means zeroing happens progressively on demand, so now initialization is incremental.

For smaller arrays, `calloc()` gets memory from the heap, which requires immediate zeroing but at the latency is bounded. Meaning, it'll start with zeros, and resizing is fine since it's small. The threshold between large and small is determined by libc. In

### Separate Chaining with LinkedLists
We'll use separate chaining as it's simpler and it handles collisions more gracefully. Here's why we use linkedlists ove rarrays:

**Latency:** Insertions are $O(1)$. Deletion is $O(n)$ cost for traversing the linked list, but the actual deletion is $O(1)$. Arrays have $O(n)$ deletion in the worse case because it has to shift elements. For linked lists, you only need to adjust pointers, no need to shift elements. 

**Referential Stability:** If we have a reference/pointer to some data stored in a data structure, we want that point to continue to point to that same memory even after operations like resizing. When we resize we usually do this:
1. Create a new larger array that has more slots.
2. Re-insert all existing key-value pairs into the new array, which re-hashes them, etc.
3. Free the memory held by the old array.

In a linked list, each element is individually allocated (dynamic memory). So, pointers to existing nodes remain valid even if the hash table resizes (which may rehash keys but doesn’t move the nodes themselves). Arrays, on the other hand, may get copied to a new memory location during resizing, invalidating references. 

It can also be implemented as a intrusive data structure.

#### Example of Referential Integrity
```C
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int key;
    int value;
} Entry;

int main() {
    int capacity = 4;
    Entry* table = malloc(capacity * sizeof(Entry));
    
    table[1].key = 1;
    table[1].value = 100;

    Entry* ptr = &table[1];  // You hold a pointer to table[1]

    printf("Before resizing: ptr->value = %d\n", ptr->value);

    // Resize the table
    capacity *= 2;
    table = realloc(table, capacity * sizeof(Entry));

    // ptr still points to old memory (possibly freed)
    printf("After resizing: ptr->value = %d (undefined behavior!)\n", ptr->value);

    free(table);
    return 0;
}
```
In C, when we allocate an array with `malloc`, we're given a contiguous block of memory. If you alter need to resize it (e.g. with `realloc`) your system might extend the current block of memory if possible, or move the entire array to a new memory location. This means all the pointers to elements in the original array could become invalid if it moves things instead of extending. 

In linear probing, we just use one array, no nesting. We use malloc to dynamically allocate the array `table` (pointer to an array). Then we use `realloc`. In the worse case this moves the memory block the array is located at, and our ptr still points to that old location so our pointer won't work.

```C
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    int value;
    struct Node* next;
} Node;

Node** create_table(int capacity) {
    Node** table = malloc(capacity * sizeof(Node*));
    for (int i = 0; i < capacity; i++) table[i] = NULL;
    return table;
}

int main() {
    int capacity = 4;
    Node** table = create_table(capacity);

    // Insert a node at bucket 1
    Node* n = malloc(sizeof(Node));
    n->key = 1;
    n->value = 100;
    n->next = NULL;
    table[1] = n;

    Node* ptr = table[1]; // Pointer to the node

    printf("Before resizing: ptr->value = %d\n", ptr->value);

    // Resize the table (reallocating only the table of pointers, not the nodes)
    Node** new_table = create_table(capacity * 2);

    // Rehash (copy the pointers — the nodes stay in place)
    for (int i = 0; i < capacity; i++) {
        if (table[i]) {
            int new_index = table[i]->key % (capacity * 2);
            new_table[new_index] = table[i];
        }
    }
    free(table); // Free old table (but not nodes)
    table = new_table;
    printf("After resizing: ptr->value = %d (still valid!)\n", ptr->value);
    free(ptr);  // Clean up node
    free(table);
    return 0;
}
```
- The outer array (`Node** table`) is just an array of pointers to nodes, each node being separately allocated with malloc.
- When resizing, we allocate a new outer array and just reassign the pointers to those same nodes.
- When freeing table you're freeing that outer array, not the node pointers within. Pretty good part of separate chaining is having that option.

## Generic Collections in C
You don't have to use C++ generics to have our hashtable deal with multiple types. You can use intrusive data structures, often a technique that we see in practical C projects. Let's review common methods for making data structures generic and compare the.

### Method 1: Void Pointers
```C

struct Node {
  void *data; // points to anything
  struct Node *next;
};
```
A pointer-sized value is one that's small enough to fit entirely in a pointer variable, so the same size as a memory address on your system. This would be 4 bytes or 8 bytes on a 32 or 64-bit machine respectively. So when someone says "If the data is pointer-sized or smaller, it can be stored in the pointer". For example if you have an `int` (4 bytes), you might be able to store it directly inside the space used for a pointer, using some special tricks:
```C
void* p = (void*)(intptr_t)42; // Store an int inside a pointer
int value = (int)(intptr_t)p;
```
We store the number 42 inside the pointer. We're not pointing to memory at address 42, we're just using the pointer to hold the bits representing 42, and this only works since 42 is small and fits inside a 64-bit pointer. Of course this is not how you use pointers, but I just wanted to get that across. Okay, back to Redis.

If the data is pointer-sized or smaller, it can be stored in the null pointer. But often the data is larger or variable-sized, likely heap (dynamic) memory. Herre are the draw backs:
- Do pointer indirection (de-referencing) to access the data of the thing its pointing to.
- We have to deal with dynamic memory management
- No type checking, we're literally dealing with void. 

### Method 2: Generate code with C macros, code generators
You can use C++ templates to create functions for each type. You can do the same thing with C macros:
```C
#define DEFINE_NODE(T) struct Node_ ## T {
  T data;
  struct Node__ ## T *next;
}
```
Though from the programmer's point of view, this is hard to debug and maintain, so we won't bother with this.

### Method 3 (Solution): Intrusive Data Structures
Here's the traditional approach:
```C
struct Node {
  void *data;
  struct Node* next;
}
```
Here, `Node` contains a pointer to some data (e.g. `int`, `struct Person`, etc.). The `Node` is a generic container for this data. However the intrusive approach flips things, so that "the structure is inside the data":
```C
struct Node {
  Node *next;
}

struct MyData {
  int foo;   
  Node node; // embedded structure
}
```
We're "intruding" into our data type by embedding the infrastructure for a data structure (e.g. `Node`) inside the data itself. This actually makes things generic:
```C
size_t list_size(struct Node *node) {
  size_t count = 0;
  while (node != NULL) {
    count++;
    node = node->next;
  }
  return count;
}
```
Notice how the function doesn't care what the data is. It just traverses through the `Node` structures, not needing to know about the data type of `MyData::foo`. It becomes "generic" not by hiding the data, but my ignoring it entirely. Now we just have to talk about how to get the data back.
```
[ foo ][ node ] ← This is your struct MyData
           ↑
        pointer to node
```
To get the data we will offset the bytes of the `node` field. We're basically doing pointer arithmetic: 
```C
MyData *pdata = (MyData *)((char *)pnode -offsetof(MyData, node));
```
- Convert `Node*` into `char*`, allowing us to subtract bytes.
- Subtract the byte offset of `node` inside `MyData`.
- Cast the result back to `MyData*`.

To avoid writing that pointer math everytime, it's common convention to use the `container_of` macro:
```C
#define container_of(ptr, T, member) \
    ((T *)( (char *)ptr - offsetof(T, member) ))

MyData *pdata = container_of(pnode, MyData, node);
```
Here we just take a pointer to `MyData::node`, and compute the pointer to the `MyData` struct. The reason this pointer arithmetic works is how structs are laid out in contiguous memory:
```C

// Each field has a fixed offset from the beginning of the struct.
// | id | name[32] | age | age_node |
struct Person {
    int id;
    char name[32];
    int age;
    struct TreeNode age_node;
};

(char *)ptr - 40

```
The macro could return something like 40, meaning `age_node` starts 40 bytes after the beginning of the `Person` struct. Let's say `ptr` points to `age_node`. Since `age_node` lives at offset `+40`, that means the start of the struct can be calculated via the expression above. As a result we've recovered the entire `Person*` from just the `TreeNode*`.



---
### Advantages of Intrusive Data Structures

#### 1. We have faster data access
Using void pointer means that we have to do indirection to access the data. With normal applications that is fine, but with Redis we want to save as much time as possible:
```
         ┌──┐       ┌──┐
         │  ▼       │  ▼
┌Node──┐ │ ┌Node──┐ │ ┌Node──┐
│┌────┐│ │ │┌────┐│ │ │┌────┐│
││next├┼─┘ ││next├┼─┘ ││next├┼──▶ …
│├────┤│   │├────┤│   │├────┤│
││ptr ││   ││ptr ││   ││ptr ││
│└─┬──┘│   │└─┬──┘│   │└─┬──┘│
└──┼───┘   └──┼───┘   └──┼───┘
   ▼          ▼          ▼
 ┌────┐     ┌────┐     ┌────┐
 │data│     │data│     │data│
 └────┘     └────┘     └────┘
```
However with intrusive data structures, we only have one pointer and we don't need to do pointer indirection to get the data. This is because the structure and the data are the same node.
```C
// Intrusive data structure technique
┌Data──┐   ┌Data──┐   ┌Data──┐
│ …    │   │ …    │   │ …    │
│┌────┐│   │┌────┐│   │┌────┐│
││node├┼──▶││node├┼──▶││node├┼──▶ …
│└────┘│   │└────┘│   │└────┘│
│ …    │   │ …    │   │ …    │
└──────┘   └──────┘   └──────┘
// To the compiler, this is functionally equivalent to:
         ┌──┐       ┌──┐
         │  ▼       │  ▼
┌Node──┐ │ ┌Node──┐ │ ┌Node──┐
│┌────┐│ │ │┌────┐│ │ │┌────┐│
││next├┼─┘ ││next├┼─┘ ││next├┼──▶ …
│├────┤│   │├────┤│   │├────┤│
││data││   ││data││   ││data││
│└────┘│   │└────┘│   │└────┘│
└──────┘   └──────┘   └──────┘
```

#### 2. Memory management is minimized
In a traditional linkedlist:
```C
struct Node {
    int value;
    struct Node *next;
};

Node *n = malloc(sizeof(Node));  // You must manually allocate the node
n->value = 42;
```
Each `Node` is a separate piece of memory. If you wanted a linked list of `MyData`, you'd usually wrap `MyData` in a node, which means you not only dynamic allocation for the node, but possibly dynamic allocation for `MyData`. I mean typically we don't know the size of `MyData`, so that's why it could be dynamic. Now with the intrusive setup:
```C
struct Node {
  struct Node *next;
};

struct MyData {
  int value;
  struct Node node; // node is a part of MyData
}

int main() {
  MyData *d = malloc(sizeof(MyData)); 
  d->value = 42;
  d->node.next = NULL;
}
```
We only need to do a dynamic allocation for MyData, not node. The node is already embedded in MyData, and we just did that allocation. Regardless of whether you're allocating memory on the heap or stack, everything should work.

#### 3. Sharing data nodes between multiple collections
A data node can belong to multiple data structures:
```C
// MultiIndexedData is our data node
struct MultiIndexedData {
  Hashnode node1; // embedded structure
  TreeNode node2; // another structure
};
```
For example, a Redis sorted set is indexed both by name and score, needing 2 data structures. With non-intrusive data structures, one index contains the data, the other contains a pointer to the data requiring an extra allocation and indicrection. Whereas with intrusive data structures, you can add many data structures to a single data node. This flexibillity is unique to intrusive data structures.

In any case, I can imagine this being pretty useful if you want to do different operations though. I can't fully imagine the implementation though.

#### 4. Multiple data types in the same collection
Whilst they aren't type safe, intrusive data structures let you use different data types in teh same collection, as long as you have a way to distinguish between them. For example, you can use a different data type as the first linked list item.

### Example: Intrusive Data Structure Motivation
First define our intrusive data structures. These structures just represent like the links and ways of traversal. They're utilites:
```C
struct HashNode {
  struct Hashnode *next;
  // key, hash info ...
};

struct TreeNode {
  struct TreeNode *left;
  struct TreeNode *right;
  // more info ... 
};
```
Then embed those data structures into our data:
```C
struct Person {
  int id;
  char name[32];
  int age;

  struct HashNode id_node;  // to be used in a hash table by ID
  struct TreeNode age_node; // to be used in a binary tree by age 
}
```
The `Person` object has the data and two embedded "hooks" that let it attach itself to multiple data structures. Obviously you'd then have some generic data structure code:
```C

void insert_into_hash_table(struct HashNode *node) {
  // Just deals with pointers like node->next
  // Doesn't care what "Person" is
}

void insert_into_bst(struct TreeNode *node) {
  // Works with left/right pointers
  // Again, doesn't care what "Person" is
}
```
Now let's make sure the same `Person` structure is accessible from both the hash table and the BST:
```C
struct Person *p = malloc(sizeof(struct Person));
p->id = 123;
strcpy(p->name, "Alice");
p->age = 35;

insert_into_hash_table(&p->id_node);
insert_into_bst(&p->age_node);
```
Now the last thing we need to do is to get is the `Person` struct that the node is contained within. This is done with pointer arithmetic:
```C

#define container_of(ptr, T, member) \
    ((T *)( (char *)ptr - offsetof(T, member) ))

// node would be an actual Person::age_node, we're using the Person struct, and age_node 
// is the member we're targeting. 
struct Person *person = container_of(node, struct Person, age_node)
```
