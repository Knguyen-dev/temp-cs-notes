# Event Driven Architecture (EDA)
About using events as a way to communicate within a system. Generally, leveraging a message broker to publish and consume events asynchronously. The publisher is unaware of who is consuming an event and the consumers are unaware of each other. EDA is simply a way of achieving loose coupling between services in a system.

## What is an Event?
An event is an object of data, indicating state schanges in a system (**"something new happened"**). It shouldn't actually specify what should happen or how the change should modify the rest of the system, but rather it will purely notify the system of the state change. When a user makes an action, they trigger an event.

For example, in an airport system one service could send a `FlightArrivedEvent`, which contains data:
- flight number: `UA123`
- arrival time: `10:45 AM`
- gate: `B12`

Notice how this event doesn't tell the system what to do, it just tells us what happened. Then you could have services that update the arrival boards, send an SMS to waiting passengers, logging stuff, etc. The point is that the systems that need to know about this, will purposely listen for these events.

## Components and Patterns
![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-III/event-driven-architecture/event-driven-architecture.png)
***Note:*** Dots in the diagram represent different events being routed.

EDAs have three key components:
- **Event producers:** Publishes an event to the router.
- **Event routers:** Filters and pushes the events to consumers.
- **Event consumers:** Uses events to reflect changes in the system.

There are several patterns and ways to implement it such as saga, pub/sub, event sourcing, and CQRS. The latter 2 will be talked about later. 


## Challenges with Pub/Sub Event-Driven Architecture
While Pub/Sub and Event-Driven Architecture (EDA) offer significant benefits like loose coupling and scalability, they introduce complex challenges that can make systems harder to develop, debug, and maintain. 

### 1. Debugging and Observability Challenges
In traditional synchronous systems, yo ucan follow a clear request-response flow. With events, the flow becomes distributed and asynchronous, making it more difficult to trace what happened when something goes wrong.

```JavaScript
// ******Traditional synchronous flow - easy to debug******
function processOrder(orderId) {
    const order = getOrder(orderId);           // Step 1
    const payment = processPayment(order);     // Step 2
    const shipment = createShipment(order);    // Step 3
    return { order, payment, shipment };       // Clear result
}

// Event-driven flow - context scattered across events
// OrderService publishes: ORDER_CREATED
// PaymentService consumes: ORDER_CREATED → publishes: PAYMENT_PROCESSED  
// ShippingService consumes: PAYMENT_PROCESSED → publishes: SHIPMENT_CREATED
// Where did it break? Which service? What was the state?

// *****Synchronous - immediate error feedback*****
try {
    const result = paymentService.charge(order);
} catch (error) {
    // Handle error immediately
    return { error: "Payment failed" };
}

// Event-driven - errors happen later, elsewhere
eventPublisher.publish('ORDER_CREATED', order);
// Payment failure happens in PaymentService minutes later
// How does OrderService know payment failed?
// How does the user get notified?
```
With the distributed tracking complexity, events flow to multiple services, there's no single place to see the complete transaction and logs are scattered amongst different services. If a customer says "my order is stuck" we have to check multiple services, cross-reference events using IDs, and try to reconstuct the timeline manually.

### 2. Event Ordering and Consistency Issues
Events can arrive out of order, be processed multiple times, or be lost entirely, leading to inconsistent system states. 

#### Example: Out-Of-Order Processing
Events published in this order:
1. USER_CREATED { userId: 123, email: "old@email.com" }
2. USER_EMAIL_UPDATED { userId: 123, email: "new@email.com" }

But processed in this order due to network/processing delays:
1. Consumer receives: USER_EMAIL_UPDATED first, which won't work.
2. Consumer receives: USER_CREATED second

The result could be that the user email is "old@email.com" instead of "new@email.com"

#### Example: Duplicate Event Processing
In message-based systems like pub/sub and EDA, an event handler may receive the same event more than once, often unitnentionally. If your event handling logic isn't idempotent, then we could create multiple records for the same thing, send duplicate emails, and deduct inventory more than necessary.
```JavaScript
// A network retry causes duplicate events
function handleOrderCreated(event) {
    // This might be called twice for the same order
    const order = createOrderInDatabase(event.orderId);
    sendConfirmationEmail(event.userEmail);
    decrementInventory(event.productId, event.quantity);
    
    // Without idempotency:
    // - Two orders created
    // - Two confirmation emails sent  
    // - Inventory decremented twice
    // Solution with idempotency: Just check if a order with that id already exists
    // in our database, and if it does, it means we aleady processed it.
}
```
There are multiple cases for why and when events are processed twice. One main one is network timeouts/retries. A producer sends an event, but doesn't get an acknowledgement, so they send it again. This is more in-line with how the message queue pattern behaves, but just be careful. Most message brokers guarantee at least once delivery, which also implies that we may get the message more than once. 

#### Example: The Saga Pattern Problem
Distributed transaction across multiple services
1. ReserveFunds → SUCCESS
2. ReserveInventory → SUCCESS  
3. CreateShipment → FAILURE

Now need to compensate by undoing:
4. ReleaseInventory → What if this fails?
5. ReleaseFunds → What if service is down?

System left in inconsistent state


### 3. Message Delivery and Reliability Issues
It's hard to guarantee that events are delivered exactly once to all subscribers. It's not something we typically do.

#### Example: At-Least-Once Delivery
```JavaScript
// Reliable but requires idempotency
function handlePaymentProcessed(event) {
    // You may have multiple of the same event, so you'll just need this.
    // 
    if (alreadyProcessed(event.correlationId)) {
        return; // Skip duplicate
    }
    
    updateOrderStatus(event.orderId, 'PAID');
    markAsProcessed(event.correlationId);
    
    // What if system crashes between updateOrderStatus and markAsProcessed?
    // Order status updated but not marked as processed
    // Next retry will attempt to update again.
}
```

#### Example: At-Most-Once Delivery
```JavaScript
// Fast but unreliable
// Events might be lost during failures
publishEvent('ORDER_CREATED', order); // Fire and forget
// If network fails, event is lost forever
// Order created but inventory never decremented
```

### 4. Schema Evolution and Versioning
If your message schema changes, you need to make sure none of your subscribers will break:
```javascript
// Version 1 event
{
    "eventType": "USER_CREATED",
    "userId": 123,
    "email": "user@example.com"
}

// Version 2 event - breaking change
{
    "eventType": "USER_CREATED", 
    "userId": 123,
    "contactInfo": {  // email field moved and renamed
        "primaryEmail": "user@example.com",
        "phone": "+1234567890"
    }
}

// Old consumers expect 'email' field - will crash or ignore events
// New consumers expect those additional fields
// **** Backwards Compatibility Challenges ****
// Publisher needs to support multiple schema versions
function publishUserCreated(user) {
    // Which subscribers expect which schema version?
    // Need to publish multiple event versions?
    
    const v1Event = { userId: user.id, email: user.email };
    const v2Event = { userId: user.id, contactInfo: { primaryEmail: user.email }};
    
    eventPublisher.publish('USER_CREATED_V1', v1Event);
    eventPublisher.publish('USER_CREATED_V2', v2Event);
    
    // Topic proliferation nightmare
}
```
There are some ways for handling this though. Instead of using multiple topics, how about you have a single schema that includes the version in one of the fields:
```json
{
  "version": 1,
  "eventType": "USER_CREATED",
  "data": {
    "userId": 123,
    "email": "user@example.com"
  }
}

{
  "version": 2,
  "eventType": "USER_CREATED",
  "data": {
    "userId": 123,
    "contactInfo": {
      "primaryEmail": "user@example.com",
      "phone": "+1234567890"
    }
  }
}
```
This allows consumers to check the version field first, and then handle the message accordingly. You could also design for backward compatibility. This typically means:
- Don't modify existing fields without planning. Your old services rely on this.
- Only add new optional fields. Old services don't care, but new services need these to work.

You'll see tools like **Apache Avro** or **Protobuf**, and pair these with a schema registry to manage versions, define schemas formally, and programmatically validate backwards compatibility.

## Conclusion
The decision to adopt event-driven architecture should be made with full awareness of these challenges and a plan to address them. Many organizations underestimate the operational complexity and end up with systems that are harder to maintain than traditional synchronous architectures. Some widely used technologies are **Apache Kafka**, **Google PubSub**, and **Amazon SNS**.

**Tips for mitigating issues:**
- Observability: Use centralized logging.
- Reliability: Handle duplicate processing. Try to prevent cascading failure.
- Schema Management: Carefully develop schema, you may have to also do event versioning, maybe even API versioning.

**When EDA Makes Sense:**
- High-scale systems with clear decoupling needs
- Organizations with mature DevOps practices
- Teams experienced with distributed systems
- Clear business value from loose coupling

**When to Avoid EDA:**
- Simple CRUD applications
- Small teams without distributed systems experience
- Systems where consistency is more important than availability
- When debugging and operational simplicity are priorities

