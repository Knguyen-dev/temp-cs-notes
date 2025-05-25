# Data Consistency
There are different patterns of data consistency, or how data like updates when doing distributed systems. I mean, do you really think that when you one letter in an instagam post's description, that this update is reflected for all of Instagram's servers across the world? Or maybe do the changes happen over time, or using some other pattern that's more efficient. That's what we're talking about today.

### Strong Consistency
In strong consistency, read operations performed on **any** server must always retrieve the data that was included in the latest write operation. So if a write happens, every subsequent read must reflect teh changes in that write. Literally the most up to date as possible. For this to happen, data is typically replicated synchronously across multiple servers.

The benefits are better data consistency across the board and data durability. However the drawbacks are that it's resource intensive, more latency, etc. Anyways popular use cases of the strong consistency model are file systems, relational databases, financial services, semi and fully distributed consensus protocols like 2PC and Paxos. 

---
### Eventual Consistency
When a write operation is executed against a server, the immediate subsequent read operations against **other** servers don't necessarily return the latest written data. The system will eventually converge to the same state and the latest data will be returned by other servers on subsequent read operations. Here data is typically replicated asynchronously across multiple servers, and it happens over time. Think like a reasonably short amount of time.

Eventual consistency can be implemented through multi-leader or leaderless replication topology. When I mean "leader" I mean like a master database that accepts writes whilst the followers (slaves) are read-only and accept changes from the leader. 

In any case, the pattern is simple, scalable, and low latency, but you have weaker data consistency. Some examples of eventual consistency are comments or posts on social media platforms.

---
### Weak Consistency
When a write operation is executed against a server, the subsequent read operations against other servers may not return the latest written data. Essentially, the distributed system must meet various conditions (i.e the passing of time) before the latest changes are returned.

An example of weak consistency is the write-behind cache pattern. So like after the cache receives the data, we send that data to a message queue (an async job). And after that async job finishes the changes are reflected in the database.

**Note:** If the system promises that all nodes will eventually agree, it's eventual consistency. If the system doesn't make that promise, and its up to timing, chance, or retries, then it's weak consistency.

## Credits
- [Consistency patterns in system design](https://systemdesign.one/consistency-patterns/)