# When To Choose Cassandra

## Leader-follower vs P2P Architecture in Database Systems
Cassandra works really well if you want to write and store a large amount of data in a distributed system. You don't care much about ACID, you care about availability and performance. Again Cassandra is a distributed database. Most databases nowadays, even traditional RDBMS can run in a distributed environment, but the difference is how Cassandra makes this easy to do and configure.

It's uses P2P architecture, a masterless architecture, which means it doesn't have a single point of failure. In most RDMBS systems, you use database replication where we have a master (leader) database handling writes and read databases handling reads, all of them handling the same data. By adding more slaves (followers), you're distributing the read load which is great for read heavy applications. You can then distribute writecapacity by having multiple master database. 

However a single master and multiple replicas is a single point of failure, and you aren't really scaling write perofrmance. Adding more master nodes allows you to scale writes, but now you have to deal with complex issues such as "what if two users changed the same row at the same time on different masters?". You have to create some resolution logic. Or maybe you're also sharding and now you somehow need to do a join on tables that are in two different databases. Sharding, vertical partitioning, and replication are hard. In general scaling traditional SQL is hard and having to follow ACID and distributed transactions makes things even tougher.

To support ACID in a distributed environment, every node will constantly need to exchange data should be other to all be in sync. If we add a new row, we must wait until all nodes are updated to hold that row, and the update has to happen all together or not at all. This can be a little time consuming if your nodes are kind of far away from each other. Essentially maintaining ACID in a distributed system with many thousands of requests per second and databases being far away with each other, it's not efficient. Cassandra doesn't have strong consistency or ACID, but it does have eventual consistency. You're actually able to set/configure the level of consistency you want, and as a result, balancing consistency versus availability.

## Lack of Relational Schema
You can't use joins or foreign keys in Cassandra. If you're wanting to represent a complex schema multiple relationships, then it's going to be difficult in Cassandra.

## Single Points of Failure and "Elasticity"
In general, it's not good to have a single point of failure. Most of the time your instance is going to go offline due to a network partition or soemthing else, and it's going to be a headache. Also having a single node isn't very elastic. I mean, when it's prime hours and everyone's using your website, you are justified to use all of your instances. However using all of your instances on a Monday when traffic is significantly less is just wasting computing resources and money. To be elastic is to be able to adjust based on the volume of traffic, and a single node isn't able to do this.

## High Write Performance
You can have a lot of nodes and each node is capable of handling writes, so Cassandra has really good write performance. Netflix were able to do 1 million writes per second whilst slowing down. Such performance simply isn't possible with a variant of the leader-follower architecture.

## Linear Scaling 
Most databases actually scale in a non-linear manner, meaning that doubling the amount of nodes doesn't actually mean doubling the performance. If running 2 units gave you 2 units of performance, then running 4 would give you about less than 4 units of performance for example.

This is because performance in SQL databases also depend on how much data they hold rather than the operation they want to do. Postgres databases may need to validate unique keys in the row, ensure foreign mappings, ensuring strong consistency, etc. But also that depends on how complex the relational model is, but in general there's a lot of overhead for any operation you do in a traditional database. 

Cassandra doesn't have to worry about this stuff, it scales linearly. This means doubling the nodes in the system or doubling the computing power in the system would double the performance of the system.

## When To Not Use Cassandra
- When you want a lot of different types of queries or you can't predict how your data will be used. Cassandra is query-first, if you don't even know what queries your application needs, you're going to have a hard time even creating a schema. If you have a lot of queries that means a lot of different tables and this could be complex as you may need to write application-level logic for consistency.
- When a single node works. Distributed systems are complex and hard to get right. Using a distributed database like Cassandra on a simple applet would be overkill.
- When you want strong ACID compliance. Casandra and most Dyanmo-style databases trade consistency for availability/scaling. Whilst Cassandra does offer some level of ACID, it's not as strict as relational databases. Look elsewhere for ACID compliance.
- If you have complex relationships, it's going to be difficult to keep them consistent due to Cassandra's denormalized nature. For example if you have a lot of many-to-many mappings (join-tables), Cassandra is most likely not the play.
- If you think individual items in your talbe should be flexible and have different columns, then look into a document database like MongoDB.

