# Search Algorithms


### Depth First Search (Stack)
procedure:
  1. let S be a stack
  2. S.push(start)
  3. while S is not empty:
    - current = S.pop()
    - if current is visited, skip it. Else label it as visited.
    - For all neighbors, add neighbors to stack S.

This is a iterative implementation of DFS. Notice that we skip nodes that have already been visited, which allows us to avoid having to process the same nodes again. With DFS, you go as deep as you can into the graph until you hit a dead end. You've already done something similar to DFS when you traversals. Now BFS is just the same thing as DFS, but we use a queue instead of a stack.

NOTE: You should probably only add all non-visited neighbors onto the stack. As a result, you avoid wasting time by adding visited neighbors onto the stack. This is the same for Bfs as well

### Breadth First Search (Queue)
Literally the same algorithm, but we use a queue instead. As a result, we're not going deepest into a layer, but rather doing it layer by layer.

procedure:
  1. let Q be a queue
  2. Q.push(start); push starting element to the end of the queue
  3. while Q is not empty:
    - current = Q.pop()
    - if current is visited, skip it. Else label it as visited 
    - For all unvisited neighbors, add them to the queue. 

#### Goal Oriented Searching
With goal oriented searching, instead of traversing the entire tree, you're going to be traversing until you find the goal node. This is commonly implemented by stopping when the current node being expanded is the goal.



### Dijkstra: Shortest Path Algorithm
The motivation behind Dijkstra is that it finds from start to the goal node. It does this by finding the shortest path from the start node to any other node. The idea you're going to have a start node and the cost for that node is zero, whilst the cost to everything else is infinity since it's not known yet. The reason they do this is with the algorithm you want to add the minimum 

You're going to keep the priority queue of nodes, with the value of a given node in the queue being the cost to get from the start to that given node. 

The idea with dijkstra is that it will expand the node that has the cheapest cost from the start. As a result, with dijkstra, once you visit a node you will not visit it again. However there's this idea of "finding a cheaper path" in Djikstra. This happens when you're expanding a node and comparing the costs of unvisited neighbors. You may find that the cost of going to a neighbor, through your curent node, is cheaper the previous cost of getting to that neighbor (presumably through another path). As a result, you're going to update the the cost of that neighbor in the cost map.

Let's start the algorithm:
- **parentMap:** A map in with the key being a node, and the value being the parent of that node. The parent is the node we had to expand to get the current node. Using this data structure we'd be able to iterate over it to reconstruct the path from start to end.
- **costMap:** A map with key being the node, and the value being the shortest known cost from the start to a given node. 
- **visitedList:** Keeps track of visited nodes in order for us to not cisit them again
- **toVisit:** A priority queue (min-heap) containing the nodes that we are planning to visit. The nodes are ordered by the cost it takes from the start to themselves. Initialize this with with the start node, a cost of 0.
1. While toVisit is not empty:
  - current = toVisit.getMin(); pop minimum node off stack.
  - if current == goal:
    a. Stop the algorithm
  - Label the current node as visited
  - For all unvisited neighbors:
    a. neighbor_cost = cost of start to curerent + cost of current to neighbor
    b. If neighbor isn't visited yet OR if neighbor_cost < costMap[neighbor] (if cheaper path):
      1. parentMap[neighbor] = current 
      2. costMap[neighbor] = neighbor_cost
      3. Add the neighbor to the priority quue with its new cheaper cost. Since it's cheaper, it will be placed higher than previous versions of itsself that are in the queue. As a result, its visited first, and more expensive versions are ignored.

- **Note on infinity:** Setting all other nodes to infinity isn't a random choice, but it's an indicator that something hasn't been added to the queue yet. Regardless of the current distance from start to neighbor n, if neighbor n has never even been added to the queue, then having its value be infinity will guarantee that it's added to the queue. And that's what we want with Dijkstra's.
- **Note on visiting:** It's been mathematically proven, but you can reason that once a node has been visited by Djikstra's algorithm, we have found the cheapest cost from start to that node n. Furthermore, you don't even need 

### Beyond: More Pathfinding Algorithms 
There are actually a few other search algorithms:

#### Greedy Best First Search
The general idea of greedy algorithms is that we have a graph that has some type of weight or cost to traverse a node. They're trying to minimize some distance or optimize some value. In common AI search problems, you'll have your edge weight (a known distance from a to b), but then each node also has a value called the "heuristic", which is typically an approximation of how far a node n is away from the goal. This idea of a heuristic gets a lot more important in classical AI, but for this specific algorithm, all you need to know is that it always picks the node with the cheapest heuristic value.

#### **UCS:** 
In classical AI/Pathfinding you'll probably hear of an algorithm called Uniform Cost Search, which is a variant of Djikstra's but with one major difference. With Djikstra, the motivation is to calculate the shortest distance from a start to every other node. On the other hand UCS is rooted in AI/Search problems, and it's used when you want the shortest path from a start node to a specific goal node. TLDR, Djikstra keeps going until it finds the shortest path from start to any other node n in the graph, whilst UCS will stop once it visits the goal node. When putting a node on the min-heap, Djikstra calculates the distance as $f(n) = g(n)$, where g is the distance from the start to the current node n. 

- **NOTE:** Actually the algorithm we showed for Dijkstra, is also UCS since it's trying to find a goal node.

#### **A-star**
A pathfinding algorithm used to find the shortest path between two points. It's a combination of:
- Djikstra's: Finds shortest path from start to node n so far .
- Greedy Best First Search: Takes advantage of a heuristic, typically the estimated cost from the goal. 

UCS has a priority heap that orders nodes based on their actual distance from the start to node n. The equation can be represented as $f(n) = g(n)$, where g(n) is the distance from start to node n. However with A*, it uses the equation $f(n) = g(n) + h(n)$, where $h(n)$ is the heuristic value of node in. So instead of ordering them by their actual distances, it's ordering nodes by their estimated distance towards the goal node. The algorithm is pretty simple now as its a copy and paste of UCS:
- parentMap: A map for path reconstruction.
- g_score_cost_map: Contains actual costs from start to node n. Set the cost map of the start to 0, and all else to infinity.
- f_score_cost_map: Contains approximated costs from start to goal, through node n. Initialize with start's heuristic value, all else infinity.
- visitedList: List of all visited nodes
- toVisit: Your priority queue that compares nodes based on their f_score values.
- While toVisit is not empty:
  - current = toVisit.getMin(); pop minimum node off stack.
  - If the current node has already been visited: Skip it
  - if current == goal:
      a. Stop the algorithm
  - Label the current node as visited
  - For all neighbors:
    a. neighbor_cost = costMap[current] + edge_cost(current, neighbor); 

    
    b. If not visited or neighbor_cost < g_score_cost_map[neighbor] (if never added to frontier or cheaper actual path):
      1. parentMap[neighbor] = current 
      2. g_score_cost_map[neighbor] = neighbor_cost
      3. f_score_cost_map[neighbor] = neighbor_cost + heuristic(neighbor)
      3. Add the neighbor to the priority queue with its new cheaper cost. Since it's cheaper, it will be placed higher than previous versions of itsself that are in the queue. As a result, its visited first, and more expensive versions are ignored.

**Note:** In AI we have the idea of an admissible vs a consistent heuristic value:
  - **Admissible:** The heuristic value from a node underestimates the actual cost to get form the node n to the end. This is required to guarantee that A* will work, and actually return the optimal shortest path. Without this, you're going to get a non-optimal answer, and that ruins the whole part of these shortest path algorithms.
  - **Consistent:** For every node N and its successor (child) of N, called P, a heuristic is consistent when this condition holds: $h(N) \leq c(N,P) + h(p)$ and $h(G) = 0$. So if the heuristic of N is less than or equal to the cost to get from N to P PLUS the heuristic of P, then it's a consistent heuristic. As well as this, a consistent heuristic is also admissible. The reason this matters is that if your heuristic is consistent, then when expanding a node, it guarantees A* has found the optimal path from start to node n. In the event that your heuristic is not consistent, you may find a cheaper cost to a visited node n, and you'll need to update the cost AND add it to the frontier for re-expansion. Yeah that's about it, with a non-consistent heuristic, it's just less efficient. You can also reason that if your heuristic is consistent, this conditional `neighbor_cost < g_score_cost_map[neighbor]` will always be false. Think about it, if it's consistent, when you're expanding a node, you've found it's optimal path to that node, but you've also kind of found the shortest path to those neighbors as well. So on that first time you see the neighbors, you're going to record their cheapest costs ever.



