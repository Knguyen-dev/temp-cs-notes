# Cycle Detection

### What do you mean?
We're going to talk about how we can detect cycles in a graph. Of course there are other ways, but this is just one way of doing it.

In this idea we're going to be focusing on the stack. The idea is that when we visit some node, we're not done visiting it (ongoing) until we've finished visiting its children. WE'll keep track of the nodes that are ongoing by keeping them in a list called **inPath**. So all the nodes in the inPath are nodes that we're still processing because we haven't visited their immediate children yet.

We know that we have a cycle when the current node that we are visiting, is also found in the **inPath** list. The idea is that, "Hey we're supposed to be currently processing this node's children, but now we found this node again?". You'll expand the node again, and get its children. You'll never be able to finish processing its children because you'll expand the node again before it. And as a result, you'll never finish processing that node, and therefore we have a cycle.

This technique relies on marking the nodes in a special way:
- Marked
- Neighbor
- Empty

### Example 1:
```
    A
   / \
  B   C <--
       \ /
        D
        |
        E
```
- Just assume we have edges going from A to B and C. Then assume that C goes to D, and D loops back to C. Also from D, you can get to E.

1. Expand A:
  - Stack: [A, B, C]
  - inPath: [A]
2. Expand C, mark it as marked as we can only pop it off after we're done visiting it. And to be done visiting it we must have visited its children.
  - Stack: [A, B, C, D] 
  - inPath: [A, C]
3. Expand D:
  - Stack: [A,B,C, D, C, E]
  - inPath: [A, C, D]
  - Cycle detection: We're processing node C again even though we are currently processing its children. As a result it's a cycle. In an ideal case, we'd have been after to finish visiting D, which would allow us to pop the marked node C off the stack.


## TLDR and Credits
TLDR: If you expand a node and then find the node again when you're not done processing its children, then we have a cycle. You can use this information to indicate "hey there's a cycle in this graph". Though if you're traversing on paper, then you can just say "Hey I'mma backtrack" so that you can actually traverse your entire graph.

Here I'll link some code for some graphing and cycle detection stuff which is fairly accurate:
- [Search algorithms, cycle detection, etc.](https://github.iu.edu/CSCI-C343-Spring2025/nguyekev-submission/tree/main/A12)