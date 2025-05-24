
# Linked List, Stack, and Queue Notes
Some simple notes about basic data structures

---
### Linked List

* **Definition:** A dynamic data structure consisting of nodes where each node contains data and a reference (pointer) to the next node.
* **Advantages:**

  * Dynamic size — no fixed capacity like arrays.
  * Efficient insertions/deletions (especially at the head or tail).
* **Disadvantages:**

  * Access time is O(n) since you must traverse nodes sequentially (no random access like arrays).
* **Types:**

  * **Singly Linked List:** Nodes point only to the next node.
  * **Doubly Linked List:** Nodes have pointers to both next and previous nodes.

    * Allows efficient insertions/removals at both head and tail.
    * Slightly more complex due to extra pointer management.

---
### Stack

* **Definition:** A LIFO (Last In, First Out) data structure.

* **Operations:**

  * **Push:** Add an element to the top.
  * **Pop:** Remove the top element.
  * **Peek/Top:** View the top element without removing.

* **Implementation:**

  * Can be implemented using arrays or linked lists.
  * Linked list stack allows dynamic sizing without resizing overhead.

* **Motivation:**

  * You'll see stacks used extensively in search and backtracking algorithms later on.

---
### Queue

* **Definition:** A FIFO (First In, First Out) data structure.

* **Operations:**

  * **Enqueue:** Add element to the back.
  * **Dequeue:** Remove element from the front.
  * **Peek/Front:** View the front element.

* **Implementation:**

  * Can be implemented using arrays, linked lists, or two stacks. I mean you can already tell some of the issues. Using arrays has fixed length, linked list means dynamic size, but ideally you'd want a doubly linked list to access both ends in constant time. Two stacks makes things easy and is also efficient.
  * Linked list is a natural fit as it supports efficient insertion at tail and removal from head.

You'll also hear of a double ended queue, also called a "Dequeue". Basically the queue has a left and a right pointer which determines where we insert new elements. 

## Credits and TLDR
I'll be real, these are basic data structures, and you'll probably see them again. Obviously there are different implementations of the data structures like I mentioned queue has a single stack vs double stack version and whatnot. Of course you should have an inkling of what each brings. It would be very helpful as knowing your data structures is very helpful for stuff like optimization work.