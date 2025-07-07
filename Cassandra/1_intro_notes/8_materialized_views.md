# Materialized Views


## What are Materialized Views? What they solve?

Materialized Views are additional tables that Cassandra automatically creates and maintains for you. They contain the same data as your base table, but with a different primary key structure, allowing you to query by columns that weren't part of the original primary key.

Remember that in Cassandra, you can normally only query efficiently via:
- Partition key 
- Partition key + clustering columns (in order).

If you want to query by other columns, we'd need to:
- Use secondary indexes, which are unperformant.
- Manually create and maintain duplicate tables.

## How Materialized Views Work
You can think of them as pre-computed queires that Cassandra keeps up to date automatically:

```bash
CREATE TABLE employees (
    id uuid PRIMARY KEY,
    name text,
    department text,
    salary int
);
```
The problem is that you want to find all employees in a specific department, but department isn't part of the primary key. The solution is to create a materialized view with respect to the department column:
```bash

CREATE MATERIALIZED VIEW employees_by_department AS SELECT * FROM employees 
WHERE department IS NOT NULL
  AND id IS NOT NULL
PRIMARY KEY (department, id);

# Now you can do a query like this 
SELECT * FROM employees_by_department WHERE department = 'Engineering';
```
Let's talk about **key rules**: 
- All base table primary key columns must be included in the materialized view's primary key.
- All columns in the WHERE clause must be NOT NULL.
- Cassandra automatically keeps the view in sync with the base table.
- You can also do read operations from materialized views, no write operations.

## Costs, Benefits, and Considerations 
No application code needed to maintain sync, but we take a slight performance hit with writes since Cassandra needs to sync data between views and base table. Of course you could still try manual table duplication, secondary indexes, or application-level denormalization.