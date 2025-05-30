# Event Sourcing

Instead of storing the current state of the data. Let's create a history of the full series of actions that all lead up to the final state. An "append-only" store which acts as a record, and in order to read that final data state, you can just replay the sequence of events in the order they occurred.

![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-III/event-sourcing/event-sourcing.png)

I mean it can be pretty good, as you know the context of the change, "what", "when", "why", and "who" changed something.

---
### Example: Customer Order

1. A customer places an order which is an `OrderEvent.` Their order has status of 'pending" and have an outstanding balance of $200.  
2. We receive `OrderPaymentReceived` event with info:
  - Order number 1220
  - Payment amount 200 dollars
  - We would append this event to our append-only log.
3. As a result our log contains these 2 events. If you want to know the the current data about the order, replay all of these events in sequence.

Another example is that after they placed their order, we could have applied a `DiscountApplied` event that gives them a $50 discount. Then when replaying the order, we'd see that they only owe $150.

### Benefits of event sourcing
- Auditing: You're storing data as a series of immutable events over time. You're basically already logging/auditing.
- Time-travel: Since all state changes are kept, you can reconstruct past states and do temporal queries like "What was their balance last Friday?"
- Observability: Generally easy to trace stuff out and see what's happening.
- Asynchronous First: Event sourced systems ideally want to minimize synchronous interactions. You still have to set boundaries for consistency, so it's not fully async though.

### Event Native Store, Streams, and other terms
They use event-native databases. These are databases that are specifically focused on storing events and facilitating event-sourced applications. Different from relational, graph, document stores, etc. These are specifically designed to store a history of changes, the state is represented as an append-only log of events

The events themselves are immutable, so if you want to change something, change it by adding a new event after the most recent one. 

We also have streams. For a particular domain object, akin to a record in a relational database, all the events for that object are stored in a stream. Then we can retrieve the state of that domain object by reading all the stream events and applying them one by one in order of appearance (replaying). A stream will have a UID to represent that specific object, and each event has its own unique position in the stream.


## Command and Query Responsibility Segregation (CQRS)
It's good practice to separate our databases S.T. any read requests/queries are directed to databases that are dedicated to processing reads, and any writes are directed to other databases. You probably already know about this from database replication. The benefit of doing this is that, now you can independently scale up if you want to handle more reads or writes respectively. This is called CQRS. However you should know that this is different from database replication, as CQRS could involve completely different schemas for the read and write databases.

Though you'll need to have some kind of replication so that after every write, the changes are propagated to your read databases. However, maybe your changes aren't that critical, I mean it's not that important for people to see the changes in a user's profile a couple seconds after the change has been made. This is why CQRS is listed as 'eventual consistency'.

### CQRS Example
Imagine you have an e-commerce platform that you know has a lot more read requests than write requests.

Here's an example of your write database. It's normalized to avoid redundancy and maintain integrity
```
Tables:
1. Users
   - id (PK)
   - name
   - email
   - address

2. Products
   - id (PK)
   - name
   - description
   - price
   - stock_quantity

3. Orders
   - id (PK)
   - user_id (FK)
   - order_date
   - status

4. Order_Items
   - id (PK)
   - order_id (FK)
   - product_id (FK)
   - quantity
   - price_at_order_time
```
The read database is designed for fast queries, so it may have some denormalization. So here we'll use MongoDB, a NoSQL technology, with this schema:
```
Collections/Tables:
1. ProductCatalog
   {
      "id": "123",
      "name": "Smartphone",
      "description": "Latest model",
      "price": 799.99,
      "stock_quantity": 45,
      "coverImageUrl": ...,
      "productImageList": [...]
   }

2. OrderSummaries
   {
      "id": "456",
      "user_id": "789",
      "user_name": "John Doe",
      "order_date": "2024-11-25",
      "status": "Delivered",
      "items": [
         {
            "product_id": "123",
            "name": "Smartphone",
            "quantity": 2,
            "price": 799.99
         },
         {
            "product_id": "456",
            "name": "Wireless Charger",
            "quantity": 1,
            "price": 29.99
         }
      ]
   }
```
So the OrderSummaries provides a complete view of an order with minimal joins. The ProductCatalog would be optimized for product browsing. We may even include things such as the images related to the product as well.






## Takeaway
- Immutable history
- Auditability
- Time-based reconstruction such as being able to query "What was the user's balance last friday". You're able to build past states
- Complex workflows modeled as a series of events.
- However you're probably going to use this for more complex applications.

Again don't forget 



## Credits:
- [Event Sourcing Article](https://www.kurrent.io/event-sourcing#:~:text=Event%20Sourcing%20is%20an%20architectural,in%20the%20order%20they%20occurred.)