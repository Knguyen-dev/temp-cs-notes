
# Hash Tables

### Hashing
* **Definition:** Process of converting a key into an integer index using a **hash function**. E.g. we'd take a letter like "A" and do `hash("A")` which outputs the index that we want to place that key in. You'd want to insert the key and the index there for checking later on.
* **Goal:** Map data (keys) to locations (buckets/slots) efficiently for quick access.

---
### What is a Hash Table?
Normally with an array, if we want to look for an item. We'd have to loop through the array (seq search). This could take a long time, but if we also knew the index of the thing we're looking for? Hash tables solve this issue as if we know the data, we can calculate its index and retrieve the data from said index. 

A hash table (or hash map), is a data structure that associates a key with a value. An example of hash tables are python dictionaries. The idea here is that the access time should be reduced to about $O(1)$. If it wasn't obvious already, a hash table is implemented using an underlying array.

### What are collisions and how to handle them?
- **Collisions:** Two elements try to position themselves at the same index. This is common so we have to have a way to deal with it.
- **Load factor:** Number of items stored / size of the array. Keep this reasonably low so that open-addressing and linear probing can work reasonably well. 
- **Clustering:** Keys bunching up very closely in the list, this isn't good since we kind of want to have a uniform distribution when looking at data.

There 2 main ways of doing collision resolution

#### Chaining (Closed Addressing)
This is the idea that each array slot (bucket) holds a linked list, where each element in the linked list are entries that hash to the same index. So new entries are append to the linked list, and lookup requires traversing the chain. 

This is quite simple to implement and is flexible as you're not going to need to resize the array containing your buckets. The main thing is that this is a lot less efficient than probing (opening addressing). This is because overtime your linkedlist are going to get really long, and so your operations are going to get slower.

#### Probing (Open Addressing)
Instead of storing multiple items per bucket, try to find another open slot within the array when a collision occurs. So if index = 4 is taken, maybe try index 5, and so on. Thoug hthe specific probing sequence will determine how you find the next slot.

Note that you'll resize the array once your load factor hits a certain ratio as you don't want to fill up the underlying array. Also when doing probing, assume you havea circular array so if you're at the last index, go back to the first. Let's talk about the different types of probing:

**Linear Probing:**
If you get a collision, check next slot sequentially (index + 1, +2, +3...). Simple but can cause clustering.

**Quadratic Probing:**
Use a quadratic function to jump around $(index + 1^{2}, +2^{2}, +3^{2}...)$.
This reduces clustering compared to linear probing. 

**Double Hashing**
Uses a second hash function to jump around: $j = h_{1}(k) + i \cdot h_{2}(k) mod C$, where C is the size of the table and k is your key. This is really good at reducing clustering nad gives you a more uniform spread. Of course it's a little more computationally expensive.

### Hashmap Example 1: Chaining
```
0       [ ]
1       [ ]
2       [ ]
3       [ ("Bob", 25) → ("Ron", 40) ]   ← Collision handled by chaining
4       [ ]
5       [ ]
6       [ ]
7       [ ("Amy", 32) ]
8       [ ]
9       [ ]
```
Let's try to find amy. You'd do `find("Amy")` to be returned index 7. Then you'd iterate through the array in that slot, returning the result where `item[0] == "Amy"`, matching the keys. Then you return the value of that result which is `32`. This is more apparent because if hash "Bob" or "Ron" they both output index 3. To differentiate them, we need to have the key with them!


### Hashmap Example 2: Linear Probing
Assume we have a hash table with linear probing. Then let's say we do some operations:
- put("Amy", 32) = 2, so we place the (key, value) tuple at index 2.
- put("Bob", 25) = 5, do the same thing.
- put("Ron", 40) = 5. 5 is already taken, so see if 6 is taken. It isn't, so put it at 6.
```
Index   Slots
-------------------------
0       [           ]
1       [           ]
2       [ "Amy":32  ]     ← Hashed here
3       [           ]
4       [           ]
5       [ "Bob":25  ]     ← Hashed here
6       [ "Ron":40  ]     ← Placed here due to collision
7       [           ]
8       [           ]
9       [           ]
```
Now if you want to find the value for "Amy", you'd do `find("Amy")`, which returns index 2. We check the key stored at index 2 to make sure it's "Amy", and it is. Now just reutrn the value 32. However let's try to do this for Ron. Do `find("Ron")` which returns 5. We look at index 5, check the keys, at index 5 it's "Bob" not "Ron". So you look in the next place at index 6 to see the tuple ("Ron", 40). The keys match, so return the value 40. Again this illustrates the process and shows why we store key and value.


## TLDR
That's about it. Of course, the experience is a lot different when learning theory and then implementing the real thing. I'll link my own implementation of a hashmap [here](https://github.iu.edu/CSCI-C343-Spring2025/nguyekev-submission/tree/main/A5)

