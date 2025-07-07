# MongoDB vs Cassandra

## MongoDB Architecture and Characteristics
In terms of architecture and data model:
- Document-oriented NoSQL database
- Uses B-Trees for storage, similar to traditional SQL databases. As a result though, whilst writes can be slightly slower, your reads are going to be faster.
- Leader-follow architecture, with sharding support.
- Supports ACID transactions and distributed transactions.

**Key Characteristics:**
- Schema flexibility with document structure.
- Strong consistency by default.
- Complex queries and joins supported.
- Better for applications requiring data integrity and consistency.

## Cassandra Overview 

**Architecture and Data Model:**
- Wide Column store.
- Leaderless replication architecture; P2P, again no single point of failure. 
- Uses LSM Trees + SSTables for storage. Your writes are initially in memory before they're batched to disk, allowing for faster writes. However reads can be a bit slower as you may need to check multiple SSTables. In any case, this is optimized for write-heavy workloads.

**Key Characteristics:**
- Optimized for write-heavy workloads.
- High availability and partition tolerance.
- Limited query flexibility.
- Single partition access pattern. 

Cassandra is designed so that most operations should target a single partition to achieve optimal performance. Remember that data is distributed across nodes based on a partition key, and each partition lives on specific nodes (though they are replicated to other nodes also). The partition key determines which nodes store your data. There are multiple reasons for single partition access:
- When you read/write to one partition, you're hitting specific nodes directly.
- Multi-partition operations require coordination between nodes, which Cassandra does not support and avoids. 
- Within a partition, the data is sorted by clustering columns for efficient range queries.
- Only have row-level locking. You should really only be modifying one row at a time, rather than trying to do some modifying operation involving multiple rows. We don't have ACID transactions.

**Data Consistency:**
You can do quorums, which means you're writing and reading to the majority of nodes. You can tune your level of consistency on how strong your level of eventual consistency should be for your application. Cassandra has mechanisms that help keep data fresh between nodes as well:
  - **Read repair:** During a read request, if we're getting conflicting data back, from our nodes, the coordinator node would get the full data, figure out what is the latest, and update the nodes that had the outdated data. You can find the article [here](https://cassandra.apache.org/doc/latest/cassandra/managing/operating/read_repair.html). Remember that it queries full data from one node, queries a hash from another, and they compare the hashes in order to see if data is the same. We hinted about that before.
  - **Write Conflict Resolution:** Cassandra prioritizes the most recent write (last write wins), but we can't trust timestamps in distributed systems. If my write happens a couple miliseconds earlier than the other guy's, then my write essentially disappears, it's lost. Clocks aren't perfect, and so that makes them somewhat unreliable in distributed systems. Cassandra just tells you to keep things as in sync as possible. We're just emphasizing that we don't really have that much data integrity.

## Conclusion
- Use MongoDB for when you need the data integrity and consistency that's close to relational database plus and the schema flexibility of a document-store. If you want ACID transactions or distributed transactions, then MongoDB does support it.

- You're going to be reading from one partition at a time. You're not perofrming any joins across different tables. However you're going to get good write and read throughput, but poor data guarantees. Writes can get dropped, and trying to do ACID or distributed transactions won't be a good idea. Cassandra is really good for write heavy applications, and you could use a hybrid database approach. A good example is a chat application, where you're storing the users, sessions, and chat ids in the relational database, but storing the actual messages contents in Cassandra. Your partition key could be the group chat id and the sorting can be done with then someone sent a chat message. Very good organization, and also we're just reading/writing from one partition at a time. 

