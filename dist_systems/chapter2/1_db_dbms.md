# Databases and Database Management Systems


### What is a database? What is a DBMS?
A database is a collection of structured information stored electronically, often on a computer. The database itself is typically managed by a Database Management System (DBMS), which acts as an intermediary between the raw data and the users or applications that access it. The DBMS allows users to retrieve, insert, update, and delete data without needing to understand how the data is physically stored or organized.

To give an example. **MongoDB** is a DBMS (just like MySQL, PostgreSQL, or Oracle DB). It's the software system that manages the actual storage, retrieval and manipulation of data. The database in MongoDB is a specific collection of documents and collections you create (e.g. `blogPosts`, `users`, etc.). MongoDB provides the interface for su to interact with the database:
- Command line shell `mongosh`.
- Drivers for different languages that allow us to interface with it programmatically.
- GUI tools like MongoDB Compass.

Though I'm kind of getting ahead of myself, anyways if you do know databases, I hope this at least refreshed something in your mind.

---
### Components of a database
- Schema: Defines the shape of the data that's used in your database. How the table of `Users` relates to `blogPosts`.
- Table: Think of it like a spreadsheet. But this is a single collection containing one entity or data structure. So the table of all users, table of all instagram posts, and the table of all products in the supermarket.
- Column: Specific attribute for a given table. Price of a product.
- Row: A specific record in a table. A specific product like apples.

### Types of Databases
There are two main types:
1. SQL: These are called relational databases, and they organize your data into tables (think spreadsheets). 
2. NoSQL: Means not only SQL. But this could mean any type of database.
  1. Document-based database; e.g. MongoDB
  2. Graph; for modeling complex relationships
  3. Time-series; for time series data obviously.