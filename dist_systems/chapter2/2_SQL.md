# SQL Databases

### Introduction to SQL databases
An SQL (or relational) database is a collection of data that has relationships. The data is organized as tables, columns, and rows.
- **Field:** A single piece of data. Such as a 'name', 'age', 'username', etc.
- **Record:** A single object or entity in the database. A 'person', 'user', or 'item'. Each record has a UID called a primary key.
- **File:** A collection of related records. So like a file could contain data about the students enrolled at a certain school. 

Also they follow the ACID consistency model, meaning they support ACID transactions, which will be explained more later.

---
### Types of Keys 
- **Primary key:** Attribute or sometimes combination of attributes that uniquely identify a row/record in the database.
- **Foreign key:** A key from a different 'foreign' table.
- **Composite key:** Key composed of multiple attributes. Note that an attribute that's used to compose a key is a 'key attribute'. Useful as identifiers of weak entities, or when you have a composite entity (joint table).
- **Superkey:** Key that uses multiple columns to identify a record. It may have redundant data that isn't necessary for uniqueness.
- **Candidate key:** A super key with the fewest attributes possible, so it has no redundant data.

#### Example: Key and Schema

```sql
-- STUDENTS Table
+------------+------------+-----------+
| student_id | first_name | last_name |
+------------+------------+-----------+
| 101        | Alice      | Smith     |
| 102        | Bob        | Lee       |
+------------+------------+-----------+

-- COURSES Table
+-----------+----------------------+
| course_id | course_name          |
+-----------+----------------------+
| CS101     | Intro to Programming |
| MATH101   | Calculus I           |
+-----------+----------------------+

-- ENROLLMENTS Table
+------------+-----------+----------+
| student_id | course_id | semester |
+------------+-----------+----------+
| 101        | CS101     | Fall     |
| 101        | MATH101   | Fall     |
| 102        | CS101     | Fall     |
+------------+-----------+----------+
```

---
##### 1. **Primary Key**
* **Students:** `student_id` is the **primary key** — uniquely identifies each student.
* **Courses:** `course_id` is the **primary key** — uniquely identifies each course.
* **Enrollments:** The combination of (`student_id`, `course_id`, `semester`) can serve as a **composite primary key**. The combination can uniquely identify a row within the table. It could also make sense in a business context since one student can take the same course again in a different semester.

```sql
PRIMARY KEY (student_id, course_id, semester)
```

---
##### 2. **Foreign Key**

* `student_id` in `Enrollments` → references `Students(student_id)`
* `course_id` in `Enrollments` → references `Courses(course_id)`

```sql
FOREIGN KEY (student_id) REFERENCES Students(student_id),
FOREIGN KEY (course_id) REFERENCES Courses(course_id)
```

---
##### 3. **Composite Key**

* In `Enrollments`: (`student_id`, `course_id`, `semester`)
* Uniquely identifies each row since the same student can retake the same course in a different semester.

---

##### 4. **Superkey**

Any set of attributes that uniquely identifies a row. Examples in `Students`:

* `{student_id}`
* `{student_id, first_name}`
* `{student_id, last_name, first_name}`
  All are superkeys, but only `{student_id}` is minimal.

---

##### 5. **Candidate Key**

* A **minimal superkey** — no unnecessary attributes.
* In `Students`, `{student_id}` is a **candidate key**.
* If we had another unique identifier like `email`, then both `{student_id}` and `{email}` would be candidate keys.

---

##### Summary Table

| Concept       | Example                                 |
| ------------- | --------------------------------------- |
| Primary Key   | `student_id` in `Students`              |
| Foreign Key   | `Enrollments.student_id` → `Students`   |
| Composite Key | (`student_id`, `course_id`, `semester`) |
| Superkey      | `{student_id, last_name}` in `Students` |
| Candidate Key | `{student_id}` in `Students`            |

---
### Indexes
Essentially, it's a data structure that allows a DBMS to quickly locate rows within a table, based on the values of one or more columns. Akin to indexes in a book, where we can find a specific topic (the index), rather than scanning through the entire book. 

For example, you can create an index for the `author` column, allowoing us to quickly filter books by author. In general you'll index columns that appear in WHERE, JOIN, or ORDER BY clauses, especially if they're frequently used or slow to query without an index.

And yes, try not to use too many indexes because you'll quickly see diminishing returns or negative impacts to latency.

---
### N+1 Query Problem
This problem happens when your data access layer executes `N` additional SQL statements to fetch the same data that could have been retrieved when executing some simple query. Imagine loading a list of entities and then separately loading related data for each entity. 

You often see this when you're using GraphQL-based APIs or ORMs, where you have less control over the actual queries being used. You could address this by optimizing the SQL query, or for GraphQL use a dataloader that batches requests internally. 

#### Example: N+1 Query In Java
```Java

// Author can have many books but a book can only reference one author
@Entity
public class Author {
    @Id
    private Long id;
    private String name;

    @OneToMany(mappedBy = "author")
    private List<Book> books;
}

@Entity
public class Book {
    @Id
    private Long id;
    private String title;

    @ManyToOne
    private Author author;
}
```
Then here's the code that causes the N+1 queries:
```Java
// Do 1 query for all authors; 
List<Author> authors = entityManager
    .createQuery("SELECT a FROM Author a", Author.class)
    .getResultList();

// DO n queries to get the book for each author.
for (Author author : authors) {
    List<Book> books = author.getBooks(); // This triggers a query per author!
    System.out.println(author.getName() + " has " + books.size() + " books.");
}
```
You don't have to do this to get the data. Then the solution to this to just do a JOIN, reduce this into one query:
```Java
// Do one query to get all the data.
List<Author> authors = entityManager
    .createQuery("SELECT a FROM Author a LEFT JOIN FETCH a.books", Author.class)
    .getResultList();

for (Author author : authors) {
    System.out.println(author.getName() + " has " + author.getBooks().size() + " books.");
}
```
In general, we want to reduce the amount of queries we do. To identify N+1 and unnecessary queries is to monitor the query count and check ORM documentation.

---
### Optimizing SQL Queries

1. Limit rows returned; you don't want millions of rows
2. Don't use select * unnecessarily
3. Subqueries can often be re-written as JOINS, just do that.
4. Indexes make fast reads but slow down writes. Use them wisely
5. Filter early. So use `WHERE` to reduce the number of rows, and put filters before `GROUP BY` or `JOIN`.
6. For AND, place the filter most likely to be false first. For OR place the one most likely to be true first. In theory this works, but in the modern era this may be done automatically.
## Database Design

### Data Dictionary
In real applications, there could also be a **data dictionary**, which stores info about the design of the database. It answers why certain decisions were made, the relationships, what each field means, etc. This is very helpful to a database designer and also developers:
- **Data element names:** Names of individual data elements, such as field names within a table such as 'username', 'password', or objects for a table 'User'.
- **Description:** Detailed explanations of the objects in the database and elements in the database. What a 'email' is for a user and why it's here.
- **Data types, constraints, validation:** Specifies data types for fields. Constraints such as length of the field, the domain of acceptable values, etc.
- **Relationships:** Information about how data elements relate to each other, such as foreign key relationships.
- **Source of data:** Can include the origin of the data, such as what system or process creates a piece of data.
- **Usage notes:** Additional notes or instructions for using or interpreting the data elements.

---
### Database Relationships
If you're making an application, you're going to design the database. A common approach is to use an [entity relationship diagram](https://www.freecodecamp.org/news/crows-foot-notation-relationship-symbols-and-how-to-read-diagrams/). 

Let's talk about relationships. There are 3 main ones, 1:1, 1:M, M:N (this we will fix). Of course you can probably have some granularity. I think 2:m could exist, like two users own a thing? But yeah.

#### 1. **One-to-One (1:1)**
> Each record in **Table A** corresponds to *at most one* record in **Table B**, and vice versa.
* A **person** has *one* **passport**.
* A **passport** is assigned to *one* **person**.

```sql
CREATE TABLE Person (
    person_id INT PRIMARY KEY,
    name VARCHAR(100)
);

CREATE TABLE Passport (
    passport_id INT PRIMARY KEY,
    person_id INT UNIQUE,
    FOREIGN KEY (person_id) REFERENCES Person(person_id)
);
```

---
#### 2. **One-to-Many (1\:M)**
> A record in **Table A** can have (be referenced by) *many* matching records in **Table B**, but each record in **Table B** belongs to (references) *only one* record in **Table A**.

* A **user** can write multiple **posts**, but each post is written by only *one* user.
```sql
CREATE TABLE Users (
    user_id INT PRIMARY KEY,
    name VARCHAR(100)
);

CREATE TABLE Posts (
    post_id INT PRIMARY KEY,
    user_id INT,
    content TEXT,
    FOREIGN KEY (user_id) REFERENCES Users(user_id)
);
```

---
#### 3. **Many-to-Many (M\:N)** — **Not directly supported**
> Each record in **Table A** can relate to *many* in **Table B**, and vice versa. This is actually not supported directly, it just wouldn't work. However you can still represent it indirectly, using a **join table**. This table goes in between tables A and B, and turns that many to many relationship, into 2 one-to-many relationships.

* **Students** enroll in many **courses**.
* Each **course** has many **students**.
```sql
CREATE TABLE Students (
    student_id INT PRIMARY KEY,
    name VARCHAR(100)
);

CREATE TABLE Courses (
    course_id INT PRIMARY KEY,
    title VARCHAR(100)
);

-- 🎯 Join table for M:N
-- Now we have 2 one-to-many relationships. 
-- 1. Student to Enrollments is 1:M, as a student can be referenced in many enrollment records.
-- 2. Courses to Enrollments is 1:M, as a course can be referenced in many enrollment records. 
-- Also look it's a composite key of student and course.
CREATE TABLE Enrollments (
    student_id INT,
    course_id INT,
    semester VARCHAR(20),
    PRIMARY KEY (student_id, course_id),
    FOREIGN KEY (student_id) REFERENCES Students(student_id),
    FOREIGN KEY (course_id) REFERENCES Courses(course_id)
);
```
> This join table **Enrollments** holds the relationships between `Students` and `Courses`.

---
### Database Normalization
Database normalization is a design decision where we remove aspects of data redundancy in our database. It's measured in levels called "normal forms", with each form becoming less redundant:

#### First Normal Form (Level 1, 1nf)
- All columns contain atomic (indivisible) values.
- No duplicate records.

#### Second Normal Form (Level 2, 2nf)
- All requirements from First normal form.
- No partial dependencies, meaning non-key attributes must depend on the entire primary key. So to determine the value of non-key attributes, I should need our entire primary key.

Assume the composite primary key: (`student_id`, `course`)
```bash

# A student's age only depends on student_id. You can identify a student's age just based on the id of student. You don't need the course code to do it at all, it's unnecessary. A partial dependency detected.
Enrollments
+------------+----------+------------+
| student_id | course   | student_age|
+------------+----------+------------+
| 1          | Math     | 20         |
| 1          | CS       | 20         |
| 2          | Physics  | 22         |
+------------+----------+------------+

# Solution: Split it into two tables
Students
+------------+------------+
| student_id | student_age|
+------------+------------+
| 1          | 20         |
| 2          | 22         |
+------------+------------+

Enrollments
+------------+----------+
| student_id | course   |
+------------+----------+
| 1          | Math     |
| 1          | CS       |
| 2          | Physics  |
+------------+----------+
```

#### Third Normal Form (Level 3, 3nf)
- Must be in 2nf
- No transitive dependencies, which means non-key attributes must not depend on other non-key attributes.
```bash

# City depends on ZipCode, not directly on ID, violates 3nf
Students
+----+--------+---------+-----------+
| ID | Name   | ZipCode | City      |
+----+--------+---------+-----------+
| 1  | Alice  | 12345   | New York  |
| 2  | Bob    | 67890   | San Diego |
+----+--------+---------+-----------+

# Solution: Split it into 2 tables
# Now every non-key attribute just depends on the primary key.
Students
+----+--------+---------+
| ID | Name   | ZipCode |
+----+--------+---------+
| 1  | Alice  | 12345   |
| 2  | Bob    | 67890   |
+----+--------+---------+

ZipCodes
+---------+-----------+
| ZipCode | City      |
+---------+-----------+
| 12345   | New York  |
| 67890   | San Diego |
+---------+-----------+
```

Note: You may hear of **functional dependency**, which is when the value of one attribute (or a set of attributes) determines another. This is everywhere and is normal, sometimes! That's why we have the rules.

#### Additional normal forms? Denormalization and performance?
There are additional normal forms like **Boyce-Codd Normal Form (BCNF, 3.5NF)**, which is a stricter version of 3NF. Then 4NF and 5NF deal which are really rare. This [video](https://www.youtube.com/watch?v=GFQaEYEc8_8) perfectly explains everything. Most applications are perfectly fine using up to 3NF. With normalization, we benefit:
- Reduces data redundancy
- Better data design
- Increases data consistency
- Enforces referential integrity.

However it does make our design more complex, makes us require more joins, and gives us slower performance.

The idea of **denormalization** is when we purposely introduce redundancy in our database schema. It's a database optimization technique that helps us avoid costly joins in a relational database. So we're attempting to improve read performance at the cost of write performance, increased data inconsistency and redundancy, etc. 

Once your data becomes distributed with techniques like federation and sharding, doing joins across the network further increases complexity. Using denormalization may avoid the need for doing such complex joins.

#### Database Denormalization Example
You're building an e-commerce app and you want to store the order history for customers:
```bash

# Normalized database schema
Customers
---------
id | name | email

Products
--------
id | name | price

Orders
------
id | customer_id | date

OrderItems
----------
id | order_id | product_id | quantity
```
However if you want to show a user's order history, you must:
- Join `Customers` with `Orders`
- Join `Orders` with `OrderItems`
- Join `OrderItems` with `Products`

This is a little more expensive than usual. And you can imagine that scaling this for a lot of users could prove to have a noticeable load. You may denormalize the database to a single `OrderItems` table:
```bash
OrderItems
----------
id | order_id | product_id | product_name | product_price | quantity
``` 
Despite being normalized, this could be a better design:
- Product names/prices can change, this lets you record what the user actually saw and paid. So this ensures historical accuracy as the `Product` table can change without affecting past orders.
- Improves performance as we have less joins. 

## Pros and Cons of SQL, Examples
It's simple, accurate, accessible, has good consistency, and is generally flexible. You can easily represent relationships. 

However, it's difficult to scale as it has poor horizontal scalability. Essentially it has to do with the fact that SQL databases have to be ACID compliant. When data is spread across machines, ensuring the results of a transaction is consistent across multiple nodes is kind of tough. For example:
- A transaction that touches rows on 3 differrent machines, needs some kind of coordination. This causes some latency and also complex failure handling.
- Joins across machines are a little tougher as now you have to do it over the network.

That said, vertical scaling can go a long way and amny companies have scaled very far with relational databases. Using replicas, caching layers and clever engineering, you have a distributed system. Just takes a little more work and knowledge.