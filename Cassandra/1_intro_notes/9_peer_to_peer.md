# Peer to Peer Architecture


## Explaining Leader-Follower Architecture
In a typical relational database, we have a client-server architecture. If we wanted to start replicating our data and have failover, which is just the idea of being able to switch automatically to a reliable backup system. In this case, we'd transition to a leader-follower architecture. One leader database that takes in write operations, and replicates/syncs those changes to the followers.

When the leader fails, it typically takes some time for failover to occur. This architecture sucks at scaling the storage amount. One approach to improve storage is doing vertical scaling on your leader and followers. Another approach is sharding, which is arguably probably the better approach to horizontally scale. However eve nweh nyou're sharding, for a given shard of data, you would still have a leader-follow cluster managing that shard. Then if the leader goes down, we'd still have to wait a bit for failover, so our original problem isn't fully solved.

## Explaining Peer-To-Peer (P2P) Architecture

In P2P architecture, there is no leader, all nodes are equal and independent from each other. As a result, they don't rely on each other, and when one node goes down, it doesn't affect the other nodes. When we send a read or write request, any node in the cluster can receive it. The node that receives it is called the **coordinator node**. A coordinator is an orchestrator, its job is to:
- Receive the client request. Let's say it's a read request.
- Determine which node should participate based on the partition key. So let's say the consistency level is 3, which means at least 3 nodes must respond with the data before Cassandra concludes that it's a successful read request. 
- The coordinator foward requests to the appropriate replica nodes.
- Collect responses and return the result to the client when the enough nodes have responded in order to meet the consistency requirements.

Note that the nodes that actually respond with data are determined by the partitioning scheme, replication strategy, consistency level, and not by which node happens to be the coordinator. In english, the coordinator may or may not be one of the nodes that holds a replica and responds with data. It may or maynot be the node that actually needs to store a write replica, etc. I wanted to clear this up so that people don't think that the coordinator is somehow ruled out for responding with data or storing a replica, pretty simple.

Based on our replication factor and consistency levels, we can determine how responsive Cassandra can be in response to network partitions. If we want to be very tolerant to network partitions, you'd loosen the conditions for replication and consistency levels. As a result, even when there's a network partition, if we only need to read data from a minimum or one node we're going to be very fault tolerant at the cost of risking data inconsistency since we don't have other nodes to back it up. The same idea is for writes.

We can reduce the fault tolerance of our application and raise data consistency by increasing the replication and consistency levels. As a result, it takes more nodes to complete a request, and during a network partition (where the network is fractured), this could be a little tougher to satisfy the required number of nodes needed. However we would be really good for data consistency.
