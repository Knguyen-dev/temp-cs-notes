# Consistency, Inserts, and Selects 


## Consistency Explained in Cassandra 
In Cassandra, you can actually control the level of data consistency. There are a variety of consistency levels, all of which will have different effects on your read and write operations. For all of these levels, let's assume that we have a standard replication factor of 3. This is just standard when learning Cassandra and it 

- **Any**:
  - Only available on writes, and ensures things are written to at least 1 replica before returning to the client.
  - In our situation, when creating the record, we know we're going to create 3 replicas of it. However, with this consistency level, as soon as it's confirmed that we've written to at least 1 node, Cassandra will return the write operation as successful. As soon as that happens, you're free to move on to your next operation.

- **One|Two|Three (Integer Consistency Level):**
  - The second consistency level is just an integer that we can choose. Imagine we set it to "Two". In our situation, Cassandra will return that the write was successful as soon as the record was stored on two nodes. If the consistency level was set to "Three", we'll have to wait until Cassandra writes the record 3 nodes, and then it will return a success. You can quickly notice that, the higher we set the level, the more we'd have to wait, but the higher the guarantee that things actually replicated and worked out.

  - You can also set the consistency level for read queries. If you set the consistency level to 1, we return data as soon as we get that data from one node. If we set it to two, Cassandra will wait until it got that data from two nodes before returning our data. Now the motivation behind this is that, one of your nodes could have outdated data whilst the other has the most up to date data. In the case of level 2 consistency, Cassandra will do the work to resolve which data is more recent. When increasing the consistency level on reads, yes you'll likely be taking waiting a little bit longer, but you're reducing the chances of getting outdated data.

- **Quorum:**
  - Ensures that a read or write was received by the majority of nodes before completing, returning data, etcetera.
  - The number of nodes queried during a quorum operation is denoted by $\frac{R+1}{2}$, where $R$ is the replication factor. In this case $\frac{3+1}{2} = 2$, so we must query at least two nodes.

Here's the workflow for a quorum write operations:
- We write the data to some node in the cluster. The partitioner determines what nodes we need to replicate to, here we're replicating to 3.
- The data is copied to the first node, the node will respond indicating that it has saved the data.
- The data is copied to the second node, and that node will respond also. 
- At this point, 2 nodes have confirmed that they've replicated the record, which is enough for the quorum to indicate that the operation was successfully completed.

A similar workflow happens for quorum read operations. Cassandra waits until 2 nodes have responded, and it will return the data with the most recent timestamp.

### Example 1: Working with Consistency and Review on Tables
```bash
# In Cassandra Shell

# Shows the consistency level
CONSISTENCY

# Set the consistency to quorum
CONSISTENCY QUORUM;
```