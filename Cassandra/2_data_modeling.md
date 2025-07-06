# Data Modeling in Cassandra

## Top to Bottom Architecture 
- Node: Every "node" is a Cassandra database server/instance. 
- Every node is responsible for its own partition of data. They all have the ability to read and write, no master-slave replication like in SQL. 
- Nodes are grouped in a cluster, which is commonly referred to as a "ring". This will make more sense when we talk about consistent hashing.
- Data is typically replicated on multiple nodes. So if you create a row, you can choose that row to be stored on the partitions of other nodes. This is that fault tolerance I was talking about earlier.
- The behavior of the replication and the schema is defined via a keyspace:
  - A keyspace is equivalent to a database schema in SQL. Each keyspace contains 1 or more tables.
  - Your tables have a schema, but technically it's flexible so a row may not even have a value for a column and that's fine.
  - Each node in a Cassandra cluster is aware of the schema (keyspaces and tables), but it only stores the data for the partitions it is responsible for. It also stores the replicated records of other partitions as determined by the replication factor.

I know a lot of this won't make since for now, but I'll explain this in due time. Just come back to this later when you have questions.

## SQL Databases: Relational Modeling 

**Employee Table**
| Column Name  | Data Type | Description                        |
| ------------ | --------- | ---------------------------------- |
| id           | INT       | Primary key                        |
| firstName    | VARCHAR   | First name of the employee         |
| surname      | VARCHAR   | Surname of the employee            |
| companyCarId | INT       | Foreign key referencing CompanyCar |
| salary       | DECIMAL   | Salary of the employee             |

**Car Table**
| Column Name | Data Type | Description                  |
| ----------- | --------- | ---------------------------- |
| id          | INT       | Primary key                  |
| make        | VARCHAR   | Manufacturer of the car      |
| model       | VARCHAR   | Model of the car             |
| cost        | DECIMAL   | Purchase cost of the car     |
| engine      | VARCHAR   | Engine type or specification |

- Every table represents a real world entity
- We have a number of columns, the ID is a primary key that uniquely represents a row in that table.
- Relational databases have relationships, we literally have a foreign key to another table in our `Employee` table.
- Relational databases use JOINs. Joins are good when our database has a single node, but when our database is distributed this can be somewhat inefficient. This is because joins create a lot of locks, and once our nodes are locked, we aren't able to serve other requests. A single transaction could end up tying up a lot of nodes on our network. For a system that requires high throughput, this isn't going work out.

## Cassandra: Query First Approach
Cassandra gets a lot of its speed in reads and writes due to the fact that it doesn't need to do joins on the database. In Cassandra, it's impossible to do joins. Instead of a relational model approach, we take a **query-first** approach, where we create our data model around specific db queries (read operations) that we think we'll need. 

For example, you're going to need a query that gets all orders for a customer sorted by order date. Also for simplicity, assume all we need is the customer's id, order date, and order id. You'll create a table like this:
```SQL
CREATE TABLE orders_by_customer (
  customer_id UUID,
  order_date TIMESTAMP,
  order_id UUID,
  PRIMARY KEY (customer_id, order_date)
);
```
As a result, you'll just need to query this table to get all of that information. If this was an SQL database, we may have organized that information into different tables, and did joins, which would have been a little less efficient. Notice the downsides though, the fact that there is some data redundancy in our table, it's denormalized. The `order_date` is normallly an attribute that can be obtained purely through the `order_id` in a separate table, but now it's all in the same table with the customer information. Whilst it may seem weird to design a database schema based on the queries you run, this actually happens a lot in SQL databases that are high load. At scale, sometimes we need to denormalize our schema due to having too many joins and the latency genuinely adding up. Denormalizing would reduce latency, and I'm just making the point that Cassandra's query-first idea may already be familiar to you.

### Example 1: Query First and Cassandra Design

**Employee by Car Make**
| Make     | Employee ID | First Name | Surname | Salary |
| -------- | ----------- | ---------- | ------- | ------ |
| BMW      | 1           | Elvis      | Presley | 30,000 |
| BMW      | 7           | Elton      | John    | 20,000 |
| BMW      | 4           | Selena     | Gomez   | 30,000 |
| Audi     | 2           | David      | Bowle   | 40,000 |
| Audi     | 5           | Mariah     | Carey   | 30,000 |
| Subaru   | 3           | Kylie      | Jenner  | 60,000 |
| Subaru   | 6           | Justin     | Bieber  | 50,000 |

**Company Car by ID**
| ID | Make     | Model    | Cost   | Engine |
| -- | -------- | -------- | ------ | ------ |
| 1  | BMW      | 5 Series | 50,000 | 1.8    |
| 2  | Audi     | A6       | 55,000 | 1.6    |
| 3  | Subaru   | C-Class  | 60,000 | 1.6    |
| 4  | Subaru   | A-Class  | 30,000 | 1.4    |
| 5  | BMW      | 3 Series | 35,000 | 1.6    |

Let's say that you predict you're going to need two queries:
1. Getting an employee by the make of their car. One table would be the "Employee By Car Make" table. Before you'd have to do a join between the employee and car table, with a where clause on the `make` column. 
2. Getting a car by its ID. The other table would be the "Company Car By ID" table. No that different from the Postgres version, we'll still support getting a car via its id.

Of course that's the raw versions of the table the real version will have several changes:
- Optimizations with several consecutive column values being the smae.
- Empty columns. Here David doesn't have a surname and Justin Bieber doesn't have a salary.

- **Note:** The column values are key-value pairs.  

**Employee by Car Make (Real One)**
| Make     | Employee ID | First Name          | Surname            | Salary           |
| -------- | ----------- | ----------          | -------            | ------           |
| BMW*3    | {id: 1}     | {firstName: Elvis}  | {surname: Presley} | {salary: 30,000} |
|          | {id: 7}     | {firstName: Elton}  | {surname: John}    | {salary: 20,000} |
|          | {id: 4}     | {firstName: Selena} | {surname: Gomez}   | {salary: 30,000} |
| Audi*2   | {id: 2}     | {firstName: David}  |                    | {salary: 40,000} |
|          | {id: 5}     | {firstName: Mariah} | {surname: Carey}   | {salary: 30,000} |
| Subaru*2 | {id: 3}     | {firstName: Kylie}  | {surname: Jenner}  | {salary: 60,000} |
|          | {id: 6}     | {firstName: Justin} | {surname: Bieber}  |                  |

## TLDR and Takeaway
When designing our database in Cassandra, make sure that each table corresponds with a single query that you have in mind. If we need to support queries that we haven't created individual tables for, it might be somewhat tricky and inefficient. Though despite this, we're still going to be performant, save disk, etc.