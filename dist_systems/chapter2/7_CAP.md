# Cap Theorem

### Review and Preface
We want to scale out systems as we need our systems to be able to handle more work efficiently. We use this by adding more hardware to increase the load our system can take. However scaling has its downsides, and we need to balance those downsides. This is where the Cap Theorem comes into play.

#### Latency vs Throughput
- **Latency:** The time it takes to do something, or get an answer. 
- **Throughput:** The number of actions we can do or the number of answers we can get per some unit of time.

An assembly line is making cars. It takes 8 hours to make a car and the factory makes 120 cars a day.
- **Latency:** 8 hours, because this is the amount of times it takes to yield a result, make a car.
- **Throughput:** 120 cars per day, or 5 cars an hour. Amount of results in a clear unit of time.

#### What do you mean partitioning? 
Network partitioning is the process of separating a computer network into independent areas or 'subnets' either done by design, or due to an uncontrolled network failure. For example, a group chat where some members lose their internet connections. Now you have two separate groups of people who can talk among themselves, but can't message back and forth between the two groups until the internet is restored.

### CAP/Brewer's Theorem
States that in a distributive system, you can only have two out of 3 guarantees:
1. **Consistency:** All nodes see the same data at the same time. Every read receives the most recent write.
2. **Availability:** A working server/node will return a reasonable response in a reasonable amount of time. Also the idea that that we can access a service even when a node or two are down.
3. **Partition Tolerance:** The system will still work when a network partitions (network breakdowns/outages) happens, which splits the system into disconnected groups of nodes.

The reason you can only choose 2 is because network partitions are inevitable in distributed systems, so you typically must choose between consistency and availability when partitions occur.

### The Three Combinations 

#### CP (Consistency + Partition Tolerance)
These systems sacrifice availability to maintain consistency during network partitions. Some examples are:
- **MongoDB:** When a partition occurs, secondary nodes become read only to prevent inconsistent data. So here we're removing the ability to write, which why we get reduced availability.
- **Redis Cluster:** Stops accepting writes if it can't reach a majority of master nodes.

Users may experience timeouts or errors during network issues, but the data remains consistent on all servers.

#### AP (Availability + Partition Tolerance)
These systems remain available during partitions but may serve stale or inconsistent data. Some examples:
- **Cassandra:** Continues serving reads and writes even when nodes are unreachable, using eventual consistency.
- **CouchDB:** Allows all nodes to accept writes independently, resolving conflicts later.

So yeah the users always get responses, but the data might be outdated or different across nodes temporarily.

#### CA (Consistency + Availability)
These systems work well in a single-node environment, but can't handle network partitions. Here are some examples:
- Traditional RDBMS in a single-node deployment
- File systems on a single machine.
- In-memory databases like Redis for a single-instance node.

We have perfect consistency and availability but the entire system fails if a single node goes down. 

### Real World Considerations
In the real world, distributed systems are going to turn their behavior to achieve some balance rather than making those absolute choices. As well as this we're typically going to be choosing CP versus AP. Notice how CA doesn't even involve a distributed system, it doesn't even count.