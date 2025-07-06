# Introduction to Cassandra
Apache Cassandra is an open source NoSQL distributed database that focuses on scalability and high availability. Here are some characteristics about it:
- It's a wide column store.
- It's fault tolerant and distributed. Cassandra uses peer-to-peer architecture, which means there's no master database. As a result, Cassandra is really fault tolerant as there's not a single point of failure. Data can be replicate across multiple nodes, so if one node goes down, its data can be accessed by another node.

Cassandra has really fast writes, and is widely used in big data, especially in IoT, where there's a lot of data that needs to be streamed. Again it's a **wide-column store**, so instead of storing rows with all of their attributes, Cassandra stores each column in a separate file. The idea is that this is really efficient if you only need a certain attributes, you only need to go through specifics column. Rather than a regular Postgres database, you'd have to search through all columns for each row. Here's how the `name` column would look like in a Cassandra database. Each instance in a column is a key-value pair:
```Python
name_column = {
  "name": "Brian",
  "name": "Brian",
  "name": "Brian",
  "name": "John",
  "name": "Bob",
  "name": "Phil",
}
```

There are two main main optimization Cassandra makes:
- Queries don't look at entire row, but only the columns you specify.
- Columns optimize space. If multiple consecutive key-value pairs have the same value, then Cassandra tries to optimize space by writing an expression. This saves memory on disk.
```Python
name_column = {
  # The next 3 rows, including this one, are all of name="Brian".
  "name": "Brian * 3",
  "name": "",
  "name": "",
  "name": "John",
  "name": "Bob",
  "name": "Phil",
```
One other thing that differentiates Cassandra from an SQL database is that let's say we had a column called `age`. In an SQL database, you need to fill that row with a value or NULL. In Cassandra you don't even have to put a value there.

## How Cassandra Relates to the Cap Theorem
Let's review the CAP theorem:
- **Consistency:** If consistent, all nodes have the data for the most recent write. Else it's not consistent, which can happen when it takes time for the data to replicate
- **Available:** Any non-failing node will return the data in a reasonable amount of time. In essence, our database should always be available, we should always be able to run queries on it.
- **Partition Tolerance:** System continues to function during a network partition or failure. A network partition is when some of the nodes can't talk to each other, or maybe one group can't communicate to another group.

Network partitions are events we can't control and so most systems have to be partition tolerant. So that's one choice down, so we're going to need to pick between consistency or availability for the last choice. SQL databases like Postgres and MySQL learn more towards consistency and availability. Cassandra opts for more consistency. However these choices aren't binary. In the revised Brewer's theorem, it's said that it's not black and white, and you could have a mix both availability and consistency.