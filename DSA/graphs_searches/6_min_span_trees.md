# Minimum Spanning Trees

### What is a minimum spanning tree?

Given an undirected graph with weighted edges, the minimum spanning tree is a subset of the edges in the graph which connect all the vertices together (without creating cycles) whilst minimizing the total edge cost required to do so.

In english, pick the edges of the graph that minimize the cost of the graph, but also keep the graph connected and acyclic.

### What is "Prim's Algorithm"?
An algorithm that creates a minimum spanning tree. The process is basically Dijkstra, but when evaluating the costs to get to a node, we only care about the cost between current and neighbor, rather than the summed cost from the start to node n.


- mst_vertices: Set of vertices that we're including in the MST. When we visit a node, that means we're including it in hte MST.
- edge_list: List to store edges in the mst
- Q: priority queue that stores tuples (v, w, weight), basically stores the edge between two nodes and their weight. Initialize it with the starting node.
- While priority_queue isn't empty AND len(mst_vertices) < len(graph):
  1. Pop current_edge from Q. You should have from_vertex, to_vertex, and weight.
  2. If to_vertex in mst_vertices, it means the to_vertex is already in the MST, already connected by the cheapest edge weight that we already found for it. Basically its already been visited, and once a vertex has been visited by prim's, we can guarantee it already has the minimum edge connecting it to the rest of the mst.
  3. Else, we haven't visited the to_vertex yet, so label it as visited by including it in mst_vertices. 
  4. Add current_edge to mst_edges as well.
  5. Iterate through all neighbors of to_vertex, our newly added vertex:
    a. If neighbor is not in mst_vertices, then add the edge from to_vertex to neigbhor into the priority queue.
    - NOTE: Essentially we just added the edges for all unvisited neighbors of to_vertex.

## TLDR and Takeaway
Pretty intimidating, but on paper you're using the same tracing algorithm you used when doing Dijkstra. You can probably see the motivation behind Prim's algorithm, as you'd want to connect all the nodes using the least amount of cost. I've actually used this algorithm to create mazes and whatnot, and it's been pretty helpful.