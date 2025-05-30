# Sharding 

### What is data partitioning? What is Sharding?
A technique to break up a database into many smaller parts. It's the process of splitting up a database or a table across multiple machines to improve manageability, performance, and the availability of a database. Let's talk about the types of partitioning:
- **Horizontal Partitioning (Sharding):** The rows of your database are split amongst different "shards" (partitions), with each shard living in a separate database server. For example, users might be divided by geography, so you'd have a database table only for users in South America (one shard), and one for people in Spain (other shard).
- **Vertical Partitioning:** Splits tables by columns. So columns with frequently accessed data could go on one server, while less frequently accessed data goes to another. While this is a way to partitioning data, this isn't considered sharding.

So sharding is horizontal partitioning only. TLDR each shard has the same schema, but it's just a subset of the shared data/rows. The reason for sharding is that after a certain point, it's cheaper and more feasible for horizontal scaling. 

---
### Types of Sharding 
- **Range-based:** An example could be sharding based on the first letter in a customer's first name. So if your first name starts with letters a-i, then your user data goes into the first shard, you get the idea. The downside is that in this case, it's may be more common to have your first name start with a-i rather than t-z, so one of your shards could hold a lot more users in comparison to the others.
- **Hashed:** Based on user info, input it into a hash function, you output a number, and based on that number, the row goes into a certain database. The downside to this is that hash values don't represent any business meaning, making them hard to scale later.
- **Directory:** You'd have a look up table that targets one of the attributes of the rows, and based on said attribute we put the row in a given shard. Just make sure your look-up table is 'correct' in the sense that it is what you want your distributions to be.
- **Geo-location:** Allocate the record based on location. For example a dating site may shard records using this method, as you may have a shard containing all users on the United States West Coast, or East Coast. With this method, the idea is that your shard and related server is closest to the user physically, allowing for a faster response.

---
### Considerations with sharding
- **Cardinality:** Think about the possible values for the shards keys (cardinality). If you choose a 'yes/no' data field for the shard key, you'll only have two shards.
- **Frequency:** Think about the probability of storing a row within a shard. For example, if you choose age to be the shard key on a fitness website, there are going to be a lot more rows in the shard where ages are (22-35) rather than (50-80). As a result, that shard is a hot-spot for queries. If the data distribution isn't uniform, we'll have to rebalance our shards so that data is equally distributed amongst shards.
- **Complexity:** We're increasing the complexity of the system as you're managing multiple databases. Imagine you have to alter the user table, so you have to alter it on all shards now.
- **Cross-shard queries:** It's more difficult and expensive. To join across shards, you need custom application level logic or a distributed query engine.
