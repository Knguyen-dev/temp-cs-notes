# Timestamps, TTLs, Collections, etc. 

## Timestamps
You can check the time data was last time a specific column for a row was given:

```bash

# Get the car make, model, and the most recent write time for the car model 
SELECT car_make, car_model, writetime(car_model) FROM employee_car_make;

# Remember we can update a specific row's column (simple)
# Here we set the car model to truck for the row with car make of "BMW" and id of 1
UPDATE employee_by_car_make SET car_model='TRUCK' WHERE car_make='BMW' AND id=1
```

## TTLs
This is used to expire data that's no longer needed:
```bash
UPDATE employee_by_car_make USING TTL 60 SET car_model='TRUCK' WHERE car_make='BMW' AND id='2';
```
For 60 seconds, the car_model is 'TRUCK'. However after those 60 seconds it will be deleted (set to nulled). The reason you might even think about using this is when data is only valid for a specific period of time. For example, a session stored in the database may only be valid for 24 hours, and after those 24 hours we want that data gone.


## Collections 
You can have a column that holds a collection of values. For example, what if you want an employee to have multiple phone numbers.
```bash

ALTER TABLE employee_by_id ADD phone set<text>;

# Giving some employee 2 phone numbers; this sets the field and overwrites previous values.
UPDATE employee_by_id SET phone = {'343', '565'} WHERE id = 1;

# This adds on to the field;
UPDATE employee_by_id SET phone = phone + {'555'} WHERE id=1;

# This removes from the field
UPDATE employee_by_id SET phone = phone - {'555'} WHERE id=1;

# Remove all phone numbers
UPDATE employee_by_id SET phone = {} WHERE id=1;
```

## Secondary Indexes
If you try to query using something that isn't the primary key (partition key especially), you aren't going to get data.

However you can use the `ALLOW FILTERING` setting to query based on attributes that aren't a part of the primary key. Cassandra supports this, but it isn't recommended because it's highly inefficient since you could be accessing data across a bunch more nodes.

Another option is adding a secondary index, which again allows you to query based on the attribute that you declared as the index. This is not recommended, and is mainly useful when you started off with a terrible database schema, and need to work with it.
```bash

# Create index on name column
CREATE INDEX ON employee_by_id (name);
```

## UUIDs and Counters 

In database systems, we often want to create unique values to uniquely identify rows in our database. In non-distributed databases, this is done by incrementing a numerical value. For example in SQL, you often use an integer that you increment e.g. `id=14`, and you can reason that by incrementing, the UID should continue to stay unique. Well this is partly because there is only one database server.

However in distributed systems, it's extremely difficult to keep track of this integer and ensure it's uniqueness. Instead we'll use a **Universally Unique Identifier (UUID)** to uniquely identify each row. According to UUID version 4 (UUIDv4), which based on randomless:
- There are $2^{122}$ possible UUIDs 
- That's about $5.3 \times 10^{36}$ unique combinations.

The probability of generating a duplicate UUID is so astronomically low it doesn't matter. A normal UUID is used for when you store data and just need a unique value. A time uuid just uses the time component of a uuid to sort the data chronologically.

```bash

# Using regular uuid. As a result, all of the uuids are completely different, each part 
# is random, there's no pattern.
CREATE TABLE employee_by_uuid (id uuid PRIMARY KEY, first_name text, last_name text);
INSERT INTO employee_by_uuid (id, first_name, last_name) VALUES (
  uuid(),
  "Tom",
  "Hanks"
);

# Using timeuuid. You'll notice that the latter sections of the uuids for your rows may match
# whilst your beginning section is the only one that's different.
CREATE TABLE employee_by_timeuuid (id timeuuid PRIMARY KEY, first_name text, last_name text);
INSERT INTO employee_by_timeuuid (id, first_name, last_name) VALUES (
  now(),
  "Tom",
  "Hiddleston"
);
```

**A counter** is a special type of column that represents an integer that only changes through increments and decrements. Some usecases would be tracking the inventory on a certain product, counting the views on a youtube video, etc. Keeping track of counters in a distributed database introduces some challenges, so Cassandra has to put some restrictions:
- Can only be created in dedicated tables
- Can't be assigned to a column that is a part of the primary key.
- Can't index or delete a counter column.

```bash

# For each customer we want to keep track of how many purchases that they have made.
CREATE TABLE purchases_by_customer_id (
  id uuid PRIMARY KEY,
  num_purchases counter
);


# -- This is now valid syntax (equivalent to UPDATE)
INSERT INTO purchases_by_customer_id (id, num_purchases) VALUES (uuid(), 1);

# Outdated info:
# Data is updated rather than inserted into counter columns, and counter columns also can't be set.
# In order to add a new row, we have to use the update command rather than insert. Here's
# the command to insert a row with some uuid and the purchase count = 1.
UPDATE purchases_by_customer_id SET num_purchases = num_purchases+1 WHERE id=uuid(); 

# Update the counter to increment
UPDATE purchases_by_customer_id SET num_purchases = num_purchases+1 WHERE id=<your-uuid>; 
```

- Regular integers: Can be set to any value, can be part of the primary key and can be indexed. 
- Counters: Only increment/decrement, dedicate dtables only, optimized for concurrent and distributed updates.

Use counters when you need to track values that are frequently updated concurrently. Use regular integers when you need full control over the value and don't have high concurrent update requirements.

## Importing and Exporting Data
Assume we have a CSV file.
- The table must already exist in Cassandra. The columns in the CSV must match at least the primary key columns in the database for things to work. For example of the table's primary key = {car_make, car_model, start_year}, you need to ensure the CSV has those columns as well. For all other columns, if they're missing, they'll just be nulled for that row.
- The limit is about 2 billion rows, but if you need more, use batch importing.

```bash

CREATE TABLE test_csv_import (
  car_make text, 
  car_model text, 
  start_year int, 
  id int, 
  first_name text, 
  last_name text, 
  department text,
  PRIMARY KEY (car_make, car_model, start_year, id)  
);

COPY test_csv_import (
  car_make text, 
  car_model, 
  start_year, 
  id, 
  first_name, 
  last_name, 
  department
) FROM '<path_to_csv>' WITH DELIMITER=',' AND HEADER=TRUE;

COPY test_csv_import TO '<path_to_new_csv>' WITH DELIMITER=',';
```