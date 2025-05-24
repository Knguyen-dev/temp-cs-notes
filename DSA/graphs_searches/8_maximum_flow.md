# Maximum Flow 

### What is it?
With an infinite input source, what's largest amount of flow that we can send through the network, whilst not exceeding the capacity for any given edge? So the maximum flow is a bottleneck value that your network can handle. With a maximum flow algorithm, we calculate the flow that each edge should receive to achieve the maximum flow of the network. There could be multiple ways to achieve network maximum flow, but we should always be getting the same network maximum flow.

- **Edges:**
  - Edge max capacity: Maximum amount of flow that can pass through this edge.
  - Edge flow value: Units of flow that are passing through that edge. 
- **Source node:** where the flow or water is coming from
- **Sink node:** Where the flow or water is draining to.
- **Flow graph:** A directed graph where each edge (aka arc) has a certain capacity which can receive a certain amount of flow. The flow running through an edge is at most its capacity. 


### Ford Fulkerson Algorithm:
- Find a path from $S \rightarrow T$
- Find the bottle neck for that path. This is the most flow we're allowed to send through the path, which is calculated by finding the smallest edge in our path from the list of edges.
- Subtract the bottleneck from all edges in the path. 
- Create edges in the opposite direction for all nodes on the path (reverse edges) and have their weights be the bottle weight.
- Count the current bottleneck to the maximum flow. Then keep doing this algorithm until you can't find a path from start to end.

Once you're done, in the most complex cases you'll have flow in a real directed edge being conflicted with flow from an artifical edge that didn't exist in the graph. You want to calculate the net flow for that given edge.

**Note:** As long as you use that reverse edge and backtracking rule, it doesn't really matter what path you choose from start to end. Sometimes on a test you'd be asked to get the shortest path or a BFS path, etc.

## TLDR and Takeaway
Maximum flow can get really messy on paper, especially if you end up doing multiple iterations. Despite sounding so academic, but is an algorithm that has real and interesting uses. Stuff like network routing, image segmentation in computer vision, or transportation and logistics.