# CQL, Keyspaces, and Tables 

## The Hierarchy and keyspaces
```
Cluster (Ring of Nodes)
├── Keyspace (Equivalent to a database/schema)
│   ├── Table 1
│   ├── Table 2
│   └── Table N
├── Keyspace 2
│   ├── Table 1
│   └── Table 2
└── Keyspace N
```
A keyspace is the Cassandra's equivalent of a database or schema that you'd find in traditional SQL databases. It's the top-level container that groups related tables together and defines how data is replicated across the cluster/ring. Each cluster has one or more keyspaces, and again they define different database schemas essentially.

## Real World Keyspace Example 
```bash
# Separate keyspaces for different business domains
CREATE KEYSPACE user_management WITH replication = {'class': 'SimpleStrategy', 'replication_factor': 3};
CREATE KEYSPACE inventory WITH replication = {'class': 'SimpleStrategy', 'replication_factor': 3};
CREATE KEYSPACE orders WITH replication = {'class': 'SimpleStrategy', 'replication_factor': 3};
CREATE KEYSPACE analytics WITH replication = {'class': 'SimpleStrategy', 'replication_factor': 2};
```
When a keyspace defines a replicaiton strategy, each table is going to follow that strategy. Herer are things to remember:
- All nodes know about all tables in the keyspace.
- Each row is stored on 3 nodes since `replication_factor=3`
- Different partitions go to different nodes.

## Keyspace Management Commands
```bash
# Create a keyspace
CREATE KEYSPACE ecommerce 
WITH replication = {'class': 'SimpleStrategy', 'replication_factor': 3} 
AND durable_writes = true;

# List all keyspaces
DESCRIBE KEYSPACES;

# Switch to a keyspace (like "USE database" in SQL)
USE ecommerce;

# Drop a keyspace (careful - this deletes everything!)
DROP KEYSPACE ecommerce;
```

## Understanding Tables and Primary Keys
Tables in Cassandra are somewhat similar to SQL tables. For primary keys, they still serve same fundamental purpose of uniquely identifying each row, but they have some differences. A primary key in Casandra has two parts:
- **Partition Key:** Determines which node in our ring/cluster stores the data. You can only query via the value of the partition key.
- **Cluster Columns:** Clustering columns serve these critical purposes.
  - **Sorting:** They determine the physical order of the data on disk, how things are sorted on the partition. One thing, data is sorted in the order you define the clustering columns. When doing read queries and ordering, to order the result set by a given attribute, you'll need to also order by all columns that come before it. 
  - **Filtering:** You can use them in WHERE clauses. However you need to have a WHERR clause with the partition key first, and then you can have one with the clustering key.
  - **Uniqueness:** They help make the primary key unique when the partition key alone isn't enough.

If you're comfortable with creating unique composite keys, then this should be easy. Notice that you should be able to fetch a single row by querying by partition key and using all clustering columns in your query.

### Example 1: Diving into Cassandra Tables

Let's create an "Employee by ID Table". Here we remember that the where ID is the primary key (UID) and the partition key. But since it's a UID, it's more of a primary key:
```bash

# Create the table
CREATE TABLE employee_by_id (
  id int PRIMARY KEY,
  name text,
  position text
);

# See all tables we have 
DROP TABLE employee_by_id;
DESCRIBE TABLES;
```
Now let's create the `employee_by_car_make` table. Note that car make can't be a UID, so we need a different approach:
- Create a composite key, and set it as the primary key
- The composite key is composed of "Car Make" as the partition key and we apply a number of "clustering columns" to make the primary key unique.

```bash

CREATE TABLE employee_by_car_make (
  car_make text,
  id int,
  car_model text,
 
  # PRIMARY KEY(partition key, clustering_column); again remember that the clustering column will affect how the data is organized/sorted on disk.
  
  # Based on our primary key, one node will contain all rows for employees who 
  # have the "BMW" car make. Then on that node, the rows are sorted based on id.
  # This is because we specified id as the clustering column here.
  PRIMARY KEY(car_make, id)
);

# Gives information about a specific table
# - Such as the primary key and the clustering order which is how the data is 
# ordered per node on disk. 
DESCRIBE TABLE employee_by_car_make


# You can specify multiple clustering columns. So for a given node
# your rows are going to first be sorted by age, and then id.
CREATE TABLE employee_by_car_make_sorted (
  car_make text,
  age int,
  id int,
  name text,
  PRIMARY KEY(car_make, age, id)
);

```
You can have multiple columns make up the partition key. Then you can have a single (or multiple) clustering columns. Then the combination of your partition and clustering keys make up the primary key. The reason you'd even think about having multiple columns compose your partition key is when you realize one of your nodes is overloaded with data, and you realize the way you partitioned things isn't efficient.

For example, if almost all of your employees are using BMW, and your partition key is on car make, then that single node handling "Bmw" car make is going to have a lot of data. As a result, most of your queries are going to that node whilst your other nodes are remaining until

You can split the data based on car make and model. As a result, only individual models for a specific car make will be stored for a given node. So let's say the car make "BMW" had different models like the "Jaguar" and "Swordfish", then you'd have a node specifically for "BMW Jaguar".

Let's see this in action to better:
```bash 

# Here our primary key is the car make, model, and id of the employee 
# - Set the partition key of the table to be a composite of the car make and model
# - For each node, order the rows by id
CREATE TABLE employee_by_car_make_and_model (
  car_make text,
  car_model text,
  id int,
  name text,
  PRIMARY KEY((car_make, car_model), id)
);
```



### Example 2: Real World Tables and Patterns
Let's build a social media platform to see how this works in practice:
```SQL

-- Select the "social_media" keyspace. Again  
-- think of it as the schema for our social media site.
USE social_media;

-- Example 1: User profiles (simple primary key)
CREATE TABLE users (
  user_id uuid PRIMARY KEY,
  username text,
  email text,
  created_at timestamp,
  bio text
);

-- Example 2: Posts by user (composite primary key)
CREATE TABLE posts_by_user (
  user_id uuid,  -- Our partition key
  post_id uuid,  -- Clustering Column 
  content text,  -- Content of the post
  created_at timestamp, -- When the post was created 
  likes_count int, -- The amount of likes on the post.

  -- The primary key for the row is the combination
  -- Order matters remember so the first one is the 
  -- partition key and the second is the partition key.
  PRIMARY KEY(user_id, post_id)
);

-- Example 3: Post by hashtag (for discovery-like feature)
CREATE TABLE posts_by_hashtag (
  hashtag text, -- Actual hashtag of the post
  created_at timestamp, -- When the post was created
  post_id uuid, -- ID of the post
  user_id uuid, -- ID of the user who created the post
  content text, -- Content of the post

  -- hashtag is the partition key, so all posts with the same hashtag
  -- are going to be s tored on the same node. Then our clustering 
  -- columns are the created_at and post_id fields. This means that 
  -- within that node those posts are sorted by created_at date and then
  -- the post_id.
  PRIMARY KEY (hashtag, created_at, post_id)
) WITH CLUSTERING ORDER BY (created_at DESC, post_id ASC);

CREATE TABLE posts_by_time (
  year_index int,
  month_index int,
  created_at timetsamp,
  post_id uuid,
  user_id uuid,
  content text,
  -- You know that there's going to be a massive amount of posts per year.
  -- Through operations like creating posts, updating posts, etc. 
  -- To reduce the hotness of nodes, we'll partition based on the month
  -- and year, which seems a lot more reasonable. I'll review this 
  -- more in another example.
  PRIMARY KEY((year_index, month_index), created_at, post_id)
);
```
Again notice that multiple tables have the same data. For example, we have the full data for a social media post in different tables the `posts_by_user`, `posts_by_hashtag`, `posts_by_time`. Our schema is denormalized, and there's no single source of truth for a post's information. We talked about this before when learning about the data model in Cassandra. Remember this is typical in Cassandra, and similar situations happens when you're dealing with SQL and large-scale infrastructure.


```bash
# E-commerce order table (Composite Partition Key)
CREATE TABLE orders_by_customer_and_year (
    customer_id uuid,
    year int,
    order_id uuid,
    order_date timestamp,
    total_amount decimal,
    status text,
    PRIMARY KEY((customer_id, year), order_date, order_id)
);
```
Each customer's orders are distributed across different nodes by year. Within each partition, the orders are sorted by date. Then multiple orders on the same date are distinguished by order ID. Let's look at a common schema for time-series data:
```bash
CREATE TABLE sensor_readings (
  sensor_id uuid,
  year int, 
  month int,
  reading_time timestamp,
  temperature float,
  humidity float,
  PRIMARY KEY((sensor_id, year, month), reading_time)
) WITH CLUSTERING ORDER BY (reading_time DESC);
```

## Using Cassandra Query Language
```bash

# Insertion 
INSERT INTO users (user_id, username, email, created_at) VALUES (
  uuid(), 'john_doe', 'john@example.com', toTimestamp(now())
);

# Insertion without some columns
# Note: Notice how here email and created_at are simply left out. They'll return as 
# null of you try to query that row. If you don't insert a value, it doesn't exist in storage so Cassandra just doesn't store 
# that column at all.

# NOTE 2: You are required provide the primary key columns (partition + clustering). 
INSERT INTO users (user_id, username) VALUES (uuid(), 'jane_doe');

SELECT * FROM posts_by_user WHERE user_id = 123;

UPDATE users SET bio = 'I am John' WHERE user_id = 123;

DELETE FROM posts_by_user WHERE user_id = 123;
```
Notice the limitations with CQL versus SQL:
- **No JOINs:** Cassandra doesn't allow you to do JOINs and connect data between tables. No foreign keys, so design your tables to for specific read queries.
- **Limited WHERE clauses:** You can only filter via the partition partition key and clustering columns.
- **No complex aggregations:** You don't have the ability to do aggregate functions, so use application-level logic or separate analytics tools.

## Best Practices, Common Commands, etc.
- Design tables for queries 
- Avoid hot partitions 
- Use appropriate data types:
```bash
CREATE TABLE user_preferences (
  # Note: We're using UUIDs here, which is definitely different from autoincrementing integers
  # in SQL. I'll explain more on this in a later chapter.
  user_id uuid PRIMARY KEY,
  settings map<text, text>,        -- Key value pairs
  favorite_tags set<text>,         -- A set of unique values 
  recent_searches list<text>,      -- Ordered values
  profile_data frozen<user_profile> -- Complex nested type that we can define
)
```
- Here are some common table inspection commands:
```bash
DESCRIBE TABLES;
DESCRIBE TABLE posts_by_user;
DESCRIBE CREATE TABLE posts_by_user
SELECT COUNT(*) FROM posts_by_user
```
- Maintain data consistency: One of the challenges regarding Cassandra is its denormalized approach and lack of a single source of truth. If you were to update the title of a social media post, and that social media post's title is in multiple different tables, then you'd need to update that data in all tables. 

### Example 3: Blog Platform
Let's create the Cassandra schema for a real world Blog Platform:
```bash

# Create a schema/keyspace for the blog platform and select it for use
CREATE KEYSPACE blog_platform WITH replication = {'class': 'SimpleStrategy', 'replication_factor': 3};
USE blog_platform;

CREATE TABLE authors (
  author_id uuid PRIMARY KEY,
  name text,
  email text,
  bio text,
  created_at timestamp
);

# Posts by author (for author dashboard)
CREATE TABLE posts_by_author (
  author_id uuid,
  published_at timestamp,
  post_id uuid,
  title text,
  content text,
  tags set<text>,
  PRIMARY KEY (author_id, published_at, post_id)
) WITH CLUSTERING ORDER BY (published_at DESC);

# For browsing/search feature
# Note: Now you're wondering why I didn't include the post content here. We'll again we're designing query-first
# and basically trying to predict what a feature would look like. When selecting posts by categories, you can imagine something
# similar to a Youtube search where you see hte title of the video, the author, but we don't need the entire content of the post.
# We'll just include a short excerpt from the post. Of course this isn't perfect as maybe you'd want to display author name as well, but you get the idea
# maybe you'd include that, or maybe you'd be using application level logic.
CREATE TABLE posts_by_category (
  category text,
  published_at timestamp,
  post_id uuid,
  author_id uuid, 
  title text,
  excerpt text,
  PRIMARY KEY(category, published_at, post_id)
);

CREATE TABLE comments_by_post (
  post_id uuid,
  comment_id uuid,
  author_email text,
  content text;       # Content of the comment; yeah make sure your documentation is good so you don't confuse things.
  created_at timestamp,
  PRIMARY KEY(post_id, created_at, comment_id)
) WITH CLUSTERING ORDER BY (created_at DESC);
```

- Primary keys are still there to uniquely identify a row right? I'm reading that we can query by category, but just filter by clustering column. This implies that clustering column is not only used for sorting, but also it's where we can define the filters for the queries we're doing I think. 
- What's the importance of the clustering column? I know they organize the data on the disk or in the nodes, but is that the main idea?
- It seems a node stores more than one table, but multiple tables. Let's say I create a post, and it's replicated to 3 different nodes. that means those 3 different nodes have copies of the same table? Let's say I create ae keyspace, do all of the nodes have the same tables? Different "partitions" of the same table.
- What if you wanted to update a post's content? Would you need to update the post contents in all tables and then let eventual consistency handle the rest? 
- We should add some information but, why are we using uuids? Why can't we use integer primary keys like in SQL? Brief explanation on what uuid is because from what I hear it's an extremely random and practically unique long string. 






## CQL
Similar to SQL. They both have select, insert, update. However CQL has no joins. 


```bash

# In Cassandra Shell
# We want our data to be replicated 3 times using simple strat since we only have a one node thing
# - You could set durable writes to false, but that increases our risk of data loss 
CREATE KEYSPACE test_keyspace WITH replication = {'class': 'Simple', 'replication_factor': '1'} AND durable_writes = 'true';

# List all keyspaces in Cassandra cluster
DESCRIBE KEYSPACES

# Drop a key space 
Drop KEYSPACE test_keyspace

# Select a keyspace to be used so that following commands are relative to this keyspace
USE test_keyspace;
```

- A keyspace can have many **tables**. A table contains a set of rows that can contain many key-value pair columns. 
- We're often dealing with rows that contain a primary key and a number of columns. 
- In Cassandra every row has a primary key that we use to access the data. The PK is actually a composite key that's composed of the partition key AND a number of "clustering columns"
- Clustering columns are used for sorting and order data on disk.


## TLDR
- A cluster can have multiple keyspaces. Your keyspaces are the outermost container.
- A keyspace is the thing that contains your tables. 



## Credits 
