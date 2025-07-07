# Gossip Protocol

Each node has data that it needs to send to other nodes. Think of the gossip protocol like a virus. Node A sends information about it self to nodes B and C. Then B and C send information to the nodes around them, this information not only containing information about themselves, but the nodes that came before them in the chain e.g. A. Then those nodes tell other nodes, and this infection spreads throughout the cluster. In the end, you should have a situation where every node knows almost every other node. This doesn't happen instantly, it's not like one node tells every other node it's information. This is a gradual process, highlighting the eventually consistent nature of Cassandra. Even if you have a 25000 node cluster, the data being sent is actually quite small and doesn't take up a lot of bandwidth.

There are a set number of steps that each node follows when it starts gossiping:
1. Each node starts a gossip session every 1-2 seconds.
2. It chooses 1-3 nodes at random to start gossiping with, but it will prefer nodes that are down or having issues with performance.
3. It doesn't remember the nodes that it has already gossiped with.

The data being sent in these gossip sessions are just metadata about the state of the cluster:
- Heartbeat State:
  - When the node started.
  - The timestamp for when this gossiping information was sent. 
- Application State:
  - The current status of the node, which could be "NORMAL" "LEAVING, "JOINING", etc.
  - Data center we're in, rack we're in, and the schema.
  - How much load this node is currently under.
  - The "severity" which is the IO pressure the node is currently under. The combination of load and severity will give you a good idea of the current performance of the node.

## What Gossiping Looks Like
1. Node 1 initiates a gossiping session with node 2.
2. Node 1 sends information about itself, but all other nodes it knows about to Node 2. Sends the metadata we talked about for each node. 
    - IP for each node it knows about.
    - Heart beat state, which includes the timestamp, when this data was sent.
    - The application state, which kind of represents the health of the node.
    - That's all for this "iteration" of gossip
3. Node 2 stores the IP, heart beat, and application state for itself and all of the other nodes that it knows about. It compares the information about each node that it knows about to the corresponding information about that node given by Node 1. Using the heart beat state, in particular the timestamp, Node 2 is able to resolve which data about a given node is more recent and up to date
    - If Node 2 finds that Node 1's data about a given node was more up to date, it will update itself.
    - Else if Node 2 finds that its own stored data about a given node is more up to date than Node 1's, then it'll tell Node 1 to update its own information. Node 1 will send an acknowledgement to Node 2 once it has updated itself.
    - As a result, both nodes should have the same state/information in the end.
