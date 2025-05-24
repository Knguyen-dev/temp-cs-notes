# Recursive DFS, Topological Sorting Order, and Kosaraju

### Topological Sort: Also DFS 
Allows us to model the dependencies of a system. E.g. A depends on B, and then B depends on C. So in order to complete things, you need to finish C first, then B, and finally you can do A. First you do DFS. Then keep track of the nodes being returned and exited. 
```bash
A ---> B
|   /  |
v  /   v
C v--> D
|      |
v      v
F <--- E
```
First traverse using DFS, and we'll keep track of some things:
- visit(A):
  - visit(C):
    - visit(F)
    <!-- Exit: [F] -->
    - exit(F)
    - visit(D)
      - visit(E)
        - revisit(F); it's a re-visit we just skip it instead of adding it to other stuff
      - exit(E); immediate exit of parent since we revisited, exit(f) is implied, but we want to keep things clean.
      <!-- Exit [D, E, F] -->
    - exit(D)
    <!-- Exit [C, D, E, F] -->
  - exit(C)
  - visit(B)
    - revisit(D); Just skip since revisit
  - exit(B)
- Exit(A)

So the exited list goes: [A, B, C, D, E, F]. Where F was the first one exited, and A was the last thing exited. This list here can be referred to as the **topological sort** of the graph. Thinking of the graph as a list of dependencies, it's evident that the last thing that we're able to get done would be F, as it depends everything else to be completed. However the first thing we can get done is A. Then we can complete B, since A is the only thing it depends on. We can complete C after, it only depends on A. D depends on B and C, but they are now completed. And so on.

- **NOTE:** Topological sort doesn't have a unique and one size fits all answer. In my DFS, we choose to expand C over B, but it was equally valid to choose B over C. And as a result, the topological sort order would have been different.

- **Note about cycle detection:** It's a lot simpler than the iterative implementation. If you revisit a node before exiting out of that node, then that's a cycle. Now it doesn't count if we exit out of node F and then after we revisit it. That doesn't count. It only matters when we revisit a node before we even had the chance to exit out of it.

### Strongly Connected Components and Kosaraju's Algorithm

#### What is a strongly connected component
```bash
0 < --- 3 ----> 4 ----> 7
|       ^       | \ __  ^
|       |       |     \ |
|       |       v      \|
1 ------|       5 ----> 6

# Pretend there's a directed edge from 6 to 4.
```

It's just a special kind of directed graph. For any two vertices A and B in a SCC, there exists a path from A to B and B to A. Here are a list of the SCCs:

- [0,1,2]
- [3]: It's a SCC with a single node.
- [4,5,6]
- [7]: Another individual SCC.

In total, there are four strongly connected components.

#### Introduction and intuition behind Kosaraju's Algorithm

Ideally, if you start from 0, you can visit all the nodes using the path: [0,1,2,3,4,5,6,7]. Now let's break down the graph into just traversing through our strongly connected components:

```bash

# You can reason that if you're able to traverse form scc1 to scc2, that means that you should be able to visit all of the nodes in scc1 and then travel to scc2. However, there's a better way to do this.
scc1 -> scc2 -> scc3 -> scc4

# Reverse the graph, switches the directions of the edges.
# Know this:
# 1. If you have a SCC, reversing the direction of its edges will still make it an SCC, so it maintains that property. 
# 2. However, the idea is that if we reverse the directios of the graph, we won't be able to traverse to the next SCC from the previous, but lets us  do separate depth first searches on each SCC.
scc1 <---- scc2 <---- scc3 <---- scc4.
```

#### Introducing Kosaraju's Algorithm
1. Find the topological sort of our original graph
2. Reverse the edges
3. Do a dfs, starting from the latest exit time. So just in top-sort order.

So first do a DFS on this tree, and then by tracking the order that things finish, we'll be able to get the topological sorting order. After backtracking we got the sequence: [0,1,2,3,4,5,6,7], where 7 exited first and 0 exited last.

Now we should reverse the graph, so all directed edges are reversed. Now we do a dfs. We start with node 0, since that was the last one to finish. As we visit stuff, we cross from out of our sequnece. 

- DFS on SCC1: Visit 0, then visit 2, then visit 1. 1's neighbor, 0, is already visited, so we stop there. [0,2,1]. Our next node to visit is 3 since it's the next unvisited node in our exit list, and that should indicate a new SCC.
- DFS on SCC2: Visit 3, then we try to visit 2, which is from SCC1, but that's already visited, so we stop. [3]. Our next node to visit is 4.
- DFS on SCC3: Start with 4, go to 6, go to 5. [4,6,5]. We stop since 5 goes back to 4, which is already visited so we stop. Now our last vertex to visit is 7
- DFS on SCC4: Start at 7, go to 4, that's already visited, so stop. [7]

Notice that now we've visited all of our nodes, and we've only actually had to start with 4 different nodes. As a result, we can reason that there are only 4 different strongly connected components in our graph.

## Credits:
- [Strongly Connected Components and Kosaraju's Algorithm](https://youtu.be/R6uoSjZ2imo?si=0oUheAtrAFRKqxTD)