# Comparing SQL and NoSQL

## Surface Level Differences  
Pick the technology that picks the requirements better:
For SQL:
- Structured data with strict schema
- Relational data
- Need for complex joins
- Transactions
- Lookups by index are very fast

For NoSQL:
- Dynamic or flexible schema
- Non-relational data
- No need for complex joins
- Very data-intensive workload
- Very high throughput for IOPS

## Scalability between SQL and NoSQL
This is kind of it's separate writeup, but I'll talk about it here. I've mentioned how SQL is harder to horizontally scale, whilst NoSQL is more suited to horizontal scaling. Now let me explain why. This is going to definitely introduce concepts from later write-ups, but that's fine, I just want to get the idea clear and actually explain the motivation and reasoning.

### Why SQL Databases Struggle with Horizontal Scaling (Overview)
SQL databases are built around ACID, and to maintain these properties, there's some overhead:
- **Atomicity:** Transactions must be all or nothing. When data is spread across multiple servers, maintaining atomic transactions becomes really complex since you need to coordinate commits or rollbacks across multiple machines. This is the idea of ***distributed transactions***, which are transactions happening across multiple databases.
- **Consistency:** All nodes must see the same data at the same time. You need an expensive coordination mechanism like two-phase commit, or some kind of distributed consensus protocol.
- **Isolation:** Concurrent transactions shouldn't interfere with each other. Maintaining isolation across multiple servers requires sophisticated locking mechanisms that can create bottlenecks.

### Relational Integrity Challenges 
SQL databases enforce referential integrity through foreign keys and constraints. Now let's say you have foreign key constraints split across databases from multiple servers:
- Cross-server joins become expensive network operations.
- Now your foreign key constraints require checking data on multiple machines.
- Maintaining consistency across these relationships is a lot harder.

### Leveraging the CAP Theorem
The CAP theorem states you can only guarantee two of the three properties: Consistency, Availability, and Partition tolerance. SQL databases prioritize consistency and availability, which makes it less suitable for distributed (partitioned) environments.

### How SQL Databases Scale Vertically
SQL databases can leverage more powerful hardware:
- CPU scaling: More cores handles more concurrent queries.
- RAM scaling: Larger memory reduces disk IO through caching.
- Storage scaling: Faster SSD and more storage capacity.

Again vertical scaling has physical and economic limits. You can only make a single machine so powerful before hitting diminishing returns (technology limit), or prohibitive costs (higher tech costs a lot more).

### Why NoSQL Databases Scale Horizontally Better

#### Relaxed Consistency Models 
A lot of NoSQL databases follow looser consistency models (i.e BASE) and use eventual consistency instead of strong consistency:
- Changes propagate to all nodes overtime, instead of immediately.
- Reduces the need for expensive coordination.
- Allows for better partition tolerance.

#### Denormalized Data Models
NoSQL databases often store denormalized data, which means:
- Reduces the need for complex joins as related data is stored together. Could also reduce the need for cross-server queries too.
- We accept data redundancy in exchange for better performance.

#### Partition Friendly Architecture 
Some NoSQL databases are built with partitioning in mind (i.e MongoDB):
- Data can be easily sharded across multiple nodes.
- Built-in replication (clustering) and failover mechanisms.

### Real World Example: E-commerce Platform
TechMart is an e-commerce platform that started small but has rapid growth:
- Started with 1000 products and 100 daily orders
- Currently has 100,000 products and 10,000 daily orders.
- Expected to have 1 million products and 100,000 daily orders.

#### Initial Database Design
The platform initially used PostgreSQL with a normalized schema:
```sql
-- Users table
CREATE TABLE users (
    user_id SERIAL PRIMARY KEY,
    username VARCHAR(50),
    email VARCHAR(100),
    created_at TIMESTAMP
);

-- Products table
CREATE TABLE products (
    product_id SERIAL PRIMARY KEY,
    name VARCHAR(200),
    description TEXT,
    price DECIMAL(10,2),
    -- Omitting this for example's sake
    category_id INTEGER REFERENCES categories(category_id)
);

-- Orders table
CREATE TABLE orders (
    order_id SERIAL PRIMARY KEY,
    user_id INTEGER REFERENCES users(user_id),
    order_date TIMESTAMP,
    total_amount DECIMAL(10,2)
);

-- Order items table
CREATE TABLE order_items (
    order_item_id SERIAL PRIMARY KEY,
    order_id INTEGER REFERENCES orders(order_id),
    product_id INTEGER REFERENCES products(product_id),
    quantity INTEGER,
    unit_price DECIMAL(10,2)
);
```

#### The Scaling Problem Emerges 
As the application grew, several issues became apparent:
```sql
-- This is a query to get the user's order history
-- This query becomes increasingly slow as data grows
SELECT u.username, o.order_date, p.name, oi.quantity
FROM users u
JOIN orders o ON u.user_id = o.user_id
JOIN order_items oi ON o.order_id = oi.order_id
JOIN products p ON oi.product_id = p.product_id
WHERE u.user_id = 12345
ORDER BY o.order_date DESC;
```
There could also be hardware limitations:
- Database server CPu usage consistently above 80%
- Storage IO (database reads and writes) becoming a bottleneck during peak hours.

TechMart probably tried some vertical scaling attempts: 
- Upgraded from 8 to 16 cores
- Increased RAM
- Used better SSDs

While these helped, the improvements were diminishing and the costs were exponential. The next hardware upgrade could cost $50,000 for like a 20% improvement. When they tried horizontal scaling their PostgreSQL setup, it was hard:
```sql
-- 1. Attempting to shard by user_id
-- Users 1-100000 go to Server 1
-- Users 100001-200000 go to Server 2
-- etc. This is just really complex in general, but let's say they solve this.

-- 2. Cross-Shard Query Problems
-- This query would need to hit multiple shards (multiple servers!)
SELECT p.name, SUM(oi.quantity) as total_sold
FROM products p
JOIN order_items oi ON p.product_id = oi.product_id
JOIN orders o ON oi.order_id = o.order_id
WHERE o.order_date >= '2024-01-01'
GROUP BY p.product_id, p.name
ORDER BY total_sold DESC
LIMIT 10;
```
**Foreign Key Complexity:** When orders and users are on different shards, maintaining referential integrity becomes complex. Your application must handle the integrity checks, and there's an actual risk of creating orphaned records (records where their referenced foreign key doesn't even exist). This wouldn't happen if it was a single database.

**Transaction Complexity:**
```sql
-- This transaction would span multiple shards
BEGIN;
UPDATE users SET last_order_date = NOW() WHERE user_id = 150000; -- Shard 2
INSERT INTO orders (user_id, order_date, total_amount) VALUES (150000, NOW(), 99.99); 

-- Then in Shard 2 you need to do this
UPDATE products SET stock_quantity = stock_quantity - 1 WHERE product_id = 5000; -- Could be any shard
COMMIT;
```
Managing distributed transactions requires two-phase commits, which is complex, slow, and can even fail, which leaves the system in an invalid state.

---
#### Solution: NoSQL
TechMart decides to migrate their product catalog and order history to MongoDB, whilst just keeping in authentication in PostgreSQL.
```json
// Denormalized product document
{
  "_id": ObjectId("..."),
  "name": "Wireless Bluetooth Headphones",
  "description": "High-quality wireless headphones with noise cancellation",
  "price": 199.99,
  "category": {
    "id": 15,
    "name": "Electronics",
    "parent": "Audio"
  },
  "specifications": {
    "brand": "TechSound",
    "color": "Black",
    "battery_life": "30 hours",
    "features": ["noise_cancellation", "wireless", "foldable"]
  },
  "inventory": {
    "stock_quantity": 150,
    "warehouse_location": "US-West-1",
    "last_restocked": ISODate("2024-05-15")
  },
  "reviews_summary": {
    "average_rating": 4.7,
    "total_reviews": 1247,
    "five_star_count": 856
  }
}

// Denormalized Orders 
{
  "_id": ObjectId("..."),
  "order_id": "ORD-2024-051234",
  "user": {
    "user_id": 150000,
    "username": "john_doe",
    "email": "john@email.com"
  },
  "order_date": ISODate("2024-05-20T10:30:00Z"),
  "status": "shipped",
  "shipping_address": {
    "street": "123 Main St",
    "city": "San Francisco",
    "state": "CA",
    "zip": "94105"
  },
  "items": [
    {
      "product_id": "prod_5000",
      "name": "Wireless Bluetooth Headphones",
      "price": 199.99,
      "quantity": 1,
      "category": "Electronics"
    },
    {
      "product_id": "prod_7500",
      "name": "USB-C Cable",
      "price": 15.99,
      "quantity": 2,
      "category": "Accessories"
    }
  ],
  "total_amount": 231.97,
  "payment": {
    "method": "credit_card",
    "last_four": "1234",
    "transaction_id": "txn_abc123"
  }
}
```
MongoDB handles sharding automatically. It distributes data across multiple servers based on a shard key:
```json
// Shard products by category for even distribution
sh.shardCollection("ecommerce.products", { "category.id": 1 })

// Shard orders by user_id for user-centric queries
sh.shardCollection("ecommerce.orders", { "user.user_id": 1 })
```
The queries are less complex and more efficient:
```javascript
// This query runs on a single shard and returns complete data
db.orders.find({
  "user.user_id": 150000,
  "order_date": { $gte: ISODate("2024-01-01") }
}).sort({ "order_date": -1 });

// Product search with all needed data in one document
db.products.find({
  "category.name": "Electronics",
  "price": { $lte: 300 },
  "inventory.stock_quantity": { $gt: 0 }
});
```

#### Trade-offs and Considerations
We gained:
- Linear scalability and cost efficiency
- Availability (built-in replicaiton) and failover

What we lost:
- Strong consistency: We use eventual consistency in our model
- Complex queries: Our model has limited join capabilities
- Data integrity: We don't have references

#### When to Choose Each Approach
Choose SQL and Vertical Scaling When:
- Strong consistency is critical (financial transactions)
- Complex relational queries are predictable
- Data size and traffic is predictable and manageable.

Choose NoSQL + Horizontal Scaling When:
- Rapid growth and unpredictable scaling needs.
- Data structure may vary and evolve frequently.
- Geographic distribution is important.
- Availability is more critical than consistency. 

#### TLDR and Conclusion
The choice between SQL and NoSQL databases isn't just about the query language or data model—it's fundamentally about how you plan to scale. SQL databases excel at vertical scaling and provide strong guarantees about data consistency and integrity, making them ideal for applications where these properties are critical.

NoSQL databases sacrifice some of these guarantees in exchange for better horizontal scalability, making them ideal for applications that need to handle massive scale or unpredictable growth patterns.

The key is understanding your specific requirements: Do you need the strong consistency and complex querying capabilities of SQL, or do you need the scalability and flexibility of NoSQL? Often, the best solution involves using both, as TechMart did, leveraging each database type for what it does best