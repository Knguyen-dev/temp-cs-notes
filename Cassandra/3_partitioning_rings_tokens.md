# Partitioning, Rings, and Token

## Partition Keys
We want to be able to get data in the most efficient way possible. In Cassandra, this means searching the fewest nodes as possible per query. 
- Reads: Ideally know exactly what node contains the data we want to read
- Writes: Write to a single table, and eventual consistency handles replicating that data over.

We partition data via **Partition Keys**, and in Cassandra you can only query data by its partition keys. In the "Employee By Car Make" table, the partition key was the make of the car. Partition keys don't have to be in that key-value form like all of the other columns. Conceptually, all rows with the make "BMW" will hash to and be stored on the same node. Then it's stored on the "BMW" partition that's managed by that node. Then the car make "Jaguar" may hash to a different node, or the same node, and so all employees with the car make "Jaguar" are stored on the node's partition that's resopnsible for storing "Jaguar" rows. In any case, when you query to get all BMW employees, Cassandra can easily narrow things down to what nodes need to be searched and what partitions, making things really fast.

A similar situation happens with the "Company Car By ID" table, as the partition key is actually the ID of the car. So each query of the company car should return only one record. Since the partition key is a UID for each row, having a node for each row is dumb. Instead, the data can be distributed across all nodes without that much loss in performance. This is just a case where your partition key just acts as a UID, rather than something that puts rows in a specific node.

Note that partition keys are different from primary keys. Look at the "Employee By Car Make" table. Partition keys are that first column, and partition keys don't have to be unique per record. However the employee ID column is more akin to the primary keys that we see in SQL databases. Again, you can only query by the partition key, so if you wanted to query by employee id, you would need a different table. We'll fully go into depth on this when we talk about keyspaces, so don't worry about that.

## Cassandra Token Ring Distribution

**Basic Process:**
1. Partition key (e.g., "BMW") → hash function → token (64-bit integer)
2. Token determines which node stores the data

**Token Ring Structure:**
Nodes form a ring where each node owns a token range. Each node stores data for tokens in its assigned range.

**Example with 5 nodes:**
- Node 1: tokens (0, 10]
- Node 2: tokens (10, 20] 
- Node 3: tokens (20, 30]
- Node 4: tokens (30, 40]
- Node 5: tokens (40, 0] (wraps around)

**Data Placement Examples:**
- "BMW" → hash = 15 → Node 2 (since 15 ∈ (10, 20])
- "Audi" → hash = 43 → Node 5 (since 43 ∈ (40, 0])

**Key Points (Takeaway):**
- Same partition key always hashes to same token → consistent placement
- All rows with same partition key stored on same node
- Ring topology handles distribution across the cluster




## Virtual Nodes
In our example, we obviously know that in the real Cassandra the token values as decided by a 64-bit integer, and so those token ranges were kind of small. Cassandra doesn't actually want to create such a large token range. 

In Node 1's range from (0, 10], Cassandra subdivides this range into smaller ranges. It's almost like we have nodes in-between that create smaller ranges like (0,2], (2,4], ..., (8, 10]. These ranges are associated with **virtual nodes**. Now don't worry, rows that land in one of these virtual nodes, will still be associated with node 1. However, this feature of virtual nodes helps Cassandra (behind the scenes) add more nodes to the cluster for greater throughput and data storage.

It also helps us if we know that our nodes have different storage capacities. Let Node 1 have 256 GB of storage, but Node 2 only has 64 GB. Since Node 1 has more capacity, we'll assign it more virtual nodes, say 8 of them, to let that node store and process more data. Then Node 2 may only handle 2 virtual nodes, so it handles less data, but at least it's appropriately scaled.