# NoSQL
A broad category that includes nay database that doesn't use SQL as the query language. These are also called non-relational databases, data stored here doesn't have to conform to a pre-defined schema. Meaning for a given record, you can add an extra field, or you may not know all of the fields that exist within a given record. 


TODO:
- I should probably have an example or reasonable use-case for each one, a schema, why you'd use it. Maybe some code snippets.

### Document Store
The idea of storing 'documents' (XML, JSON, binary, etc.) where a document stores all information for a given object. Very similar to a key-value store. For example, let's say you have a Blog application and you're using MongoDB:
```JSON
post = {
  authorId: 1,
  title: "My First Post",
  content: "This is a great blog",
  tags: ["Comedy", "Fantasy", "SCI-FI"]
}
```
The unstructured aspect comes from the `tags` field as one post could have different tags from another post. You can't predict the amount of elements/strings in that `tags` field. Of course you can also add a field like `post_description` to a given post object, and other post objects may not have that field, and that's allowed. Of course, typically you're going to of course want some structure, so that idea probably won't be used, but the idea with the `tags` is valid.  

You have a flexible schema, nested objects/arrays, and easy horizontal scaling. If you were using a relational database, you'd probably have a join table. Of course your tag array could be an array of UIDs too, so you still grapple with this idea of normalization.

---
### Key-Value
Like a hash table. They store data as a collection of key-value pairs, in which a key serves as a unique identifier. So like maybe we have a collection of products
```
{
  {1, bookID}: {book title, author, date, etc.},
  {2, albumID}: {album title, band name, release year, genre}
}
```
I mean it's very scalable and follows object oriented design, like every developer is comfortable with a hashmap. As well as this read and writes are O(1), which is a lot faster than relational databases. So examples of a key-value store are Memcached or Redis. Your use-cases are storing sessions, caching etc.

Key-value stores don't support complex queries though, so there are limitations. Typically the key-value store as a cache rather than the main form of storage. Remember that key-value stores don't enforce a set schema, so things are kind of loose. So a team has to have a plan to standardize and let everyone know what kind of schema they want.

Things are just extremely fast, simple to use, and really good for caching and reducing load. Examples are Redis, Memcached, Amazon DynamoDB, and Aerospike.

---
### Graph
A graph database is designed for very complex relationships. Nodes represent entities and edges represents relationships. The use-case are social networks, recommendation engines, knowledge graphs, etc. An example is Neo4j's database:
```Cypher
CREATE (a:User {name: "Alice"})
CREATE (b:User {name: "Bob"})
CREATE (a)-[:FRIEND]->(b)
```
Here we're creating a one way relationship of type `FRIEND` between Alice and Bob. So Alice is friends with Bob. You'd use graph-based databases for highly interconnected relationships or when relationships between entities are crucial, again very limited cases and niche applications. You're going to get efficient traversal of relationships this way and it's pretty intuitive. Examples are Neo4j, Amazon Neptune, etc.

---
### Time-Series 
Optimized for storing timestamped data, often used for metrics, events, or logs. Some use cases could be IoT devices and financial market related situations. Good for storing and retrieving time-based data. Examples are InfluxDB and Apache Druid.

---
### Wide Column
Data is stored in "column families" rather tha nrows and columns. It's really scalable and ideal for real time big data apps, but the drawback comes in write time.

Examples are Google BigTable, Apache's Cassandra, and ScyllaDB.

---
### Multi-model Technique
The multi-model databases technique combines different database models (i.e relation, graph, key-value, document, etc.) into your backend. So you can accomodate for various scenarios. This is more suitable for complex projects and it gives you more flexibility. Of course the drawback is that things are going to be a little more complex.