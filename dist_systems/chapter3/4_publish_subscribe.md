# Publish-Subscribe Pattern
![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-III/publish-subscribe/publish-subscribe.png)
Similar to message queues, it's a form of service-to-service asynchronous communication. Here, any message published to a topic is pushed immediately to all subscribers of the topic. The subscribers of the message topic are related services that do different things based on receiving the message.

 Here are the main differences with pub/sub:
- The producer sending the message doesn't know who is going to receive their message. With message queues, the producer is aware of the service receiving their message. Also the subscribers don't need to know who produced the message. No one needs to know how the `ORDER_COMPLETED` message was created, you just need to know it was created and everyone who has some logic related to it will do their thing.
- Typically pub/sub has multiple services receiving the message, whilst message queues only has one.
- The message could be persisted inside the topic.

---
### How it works
Unlike message queues, which batches/stores up messages until they are retrieved by the consumer, message topics deliver messages with little or no queuing and pushes them out to all subscribers. Here's how it works:
- A message topic provides a lightweight emchanism to broadcast asynchronous events and it exposes an API that allows other services to connect to the topic in order to send or receive those messages. 
- To broadcast a message, the service broadcasting (known as ***producer***) pushes a message to the topic.
- All components that subscribe to the topic (known as ***subscribers***) will receive every message that was broadcasted to a given topic.

---
### Advantages of Pub/Sub
- Eliminate Polling: Message topics allow instantaneous, push-based delivery, removming the need for message consumers to periodically check (poll) for new info or updates. This emphasizes faster responses and reduced latency. 
- Dynamic and simple communication: Service discover seems a lot easier since now you just need to connect to the topic that facilitates the pub/sub. Subscribers can change, increase, decrease, and it should be a clean change.
- Decoupled and independent scaling: I mean the core thing here is that the producer doesn't need to know about the subscribers and vice versa. They are decoupled and can scale independently. 

---
### Drawbacks and Considerations
- **Delivery failures:** If a subscriber is down, the messages might be lost unless the topic provides persistence.
- **Message ordering:** No guarantee that arrives are going to arrive in the order that they were published. This is kind of annoying if you're scheduling a job.
- **At-least-once** delivery: Messages might be delivered multiple times, requiring subscribers to handle duplicate messages (idenpotency).
- **Debugging complexity:** Overall harder to trace your data, monitor processes, and test stuff.
- **Schema Evolution:** If the message format changes, all subscribers must be adjusted to support the new format.

I'll talk more about the issues of pub/sub and the general event driven architecture in a separate write-up.

---
### Key Features of Pub/Sub
- **Multiple Delivery Protocols:** A topic can connect and deliver its messages to multiple types of endpoints. You could have the message be delivered to a message queue, an HTTP endpoint, a serverless function, etc.
- **Fanout:** The scenario of sending a message to a topic, and then its replicated and pushed to multiple endpoints. 
- **Filtering:** A subscriber can also create a message filtering policy so that it only gets the notifications it wants.
- **Security:** You can have message topics authenticate applications that try to produce a message.

---
### Message Queues vs Pub/Sub: When to Use What
Use message queues for stuff like:
- point-to-point communication: You need exactly one consumer to process each message.
- Guaranteed processing: You need to be sure the message is processed.
- Load balancing: You want to distribute the work across multiple identical workers.

**Example:** Processing payment transactions where each payment should be handled exactly once by a payment processor.

Use pub/sub when:
- Event driven architecture: You need to notify multiple services about events.
- Loose coupling and independent scaling: Services should remain independent and not know about each other. This also facilitates independent scaling.
**Example:** User registration revent that triggers email notifications, analytics tracking, and welcome message delivery.


## Real World Examples/Scenarios

### Example 1: Ecommerce Platform
Let's model when a customer places an order.

- Producer: Order service publishes `ORDER_PLACED` event.
- Subscribers:
  - Inventory service: Updates stock levels.
  - Payment service: Processes payment.
  - Notification service: Sends confirmation email.

We use pub/sub because multiple services need to react to the same event independently. And new services can be added without modifying existing code.

### Example 2: Social Media Platform
Let's model when a user posts new content.

- Producer: Content service publishes `POST_CREATED` event.
- Subscribers: 
  - The feed service updates follower feeds. 
  - The notification service notifies their followers. 
  - The moderation service checks for inappropriate content located in the post.
  - Search service indexes the new content.

You'd use pub/sub to enable real time updates across multiple features without tight coupling.

### Example 3: Finanical Trading System
This system would monitor stock price updates.

- Producer: Price feed service that sees the changes and publishes a message.
- Consumer: Stock order processing service.

Here we use message queues instead of pub/sub because it's important that each price update should be processed exactly once to maintain data consistency.

## Conclusion
Pub/sub is powerful for event-driven architectures where multiple services need to react to the same events independently. It emphasizes loose coupling, real time updates, and dynamic scaling. However you should be prepared for additional complexities in debugging, testing, and ensuring message delivery.

Choose the former when you're broadcasting events to multiple services and want decoupling. Choose the latter if you're dealing with point-to-point communication, message ordering, and guaranteed processing. Of course, real life implementations will probably have additional features and cover deficiencies on both sides, so my advice may not be fully accurate. Anyways a mature system will probably have both patterns being used.