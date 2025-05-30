# Transactions 

## Introduction to Transactions
### What are transactions? Payment Example!
At dinner Alice offers to pay the bill, and the Bob will just transfer money Alice's account to compensate Alice for paying the bill. In this case, it'd be bad if one of the operations didn't work such as, Alice pays the bill, but Bob's account doesn't send money to Alice's due to some error. We need a way to make sure either both of these operations happen, or the whole thing is cancelled. This is a transaction, either all operations happen or none of them do. More formally, a **transaction** is a series of operations that are executed as a single, atomic unit to ensure data integrity and consistency. 

---
### ACID Consistency Model
Properties that should be maintained during transaction processing:
1. **Atomic:** All operations will either succeed or fail together.
2. **Consistency:** All changes are consistent with database constraints. For example, if an account balance can't go below zero, then a transaction will fail if one of its operations violates this condition.
3. **Isolation:** Multiple transactions can happen simultaneously without affecting the outcomes of other transactions.
4. **Durability:** All changes made by operations will persist (written to disk), and will remain in the system even in the event of a system failure.

---
### Base Consistency Model
The BASE consistency model for NoSQL databases. Okay for some domains and situations, ACID is a lot more pessimistic and overprotective of the data, meaning it's a lot more worried about the data's safety than that's really required. NoSQL often uses BASE, which is less strict, but it aligns with horizontal scaling for performance.
- **Basic Availability:** Database works most of the time.
- **Soft State:** Stores don't have to be write-consistent. Replicas don't need to be mutually consistent all the time.
- **Eventual Consistency:** The data is all the same/consistent not immediately, but over time.

#### When is BASE used?
It's evident in NoSQL database like DynamoDB, Cassandra, Hadoop, etc. Remember the idea is that we don't offer guaranteed consistency, but the data will become consistent in the future or when someone does a read and forces it to become consistent.  

In the real world, BASE-compliant databases are typically used by large companies that are storing and processing several terabytes of data every day. It implies that at this point, there's so much data that trying to enforce ACID compliance creates so much overhead that it actually hurts our performance noticeably.

### Examples of ACID Transactions
Should be used in a scenario where you have multiple operations that need to happen together. Examples are transferring money from two accounts, the idea of buying an item which also reduces stock, etc. You don't use a transaction when doing reads, when it's not mission critical, or if it's one operation.


```sql
-- Important SQL Operation; if a step fails, all of them fail.
BEGIN;
UPDATE accounts SET balance = balance - 100 WHERE id = 1;
UPDATE accounts SET balance = balance + 100 WHERE id = 2;
COMMIT;
```