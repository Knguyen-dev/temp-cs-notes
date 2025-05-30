# Message Brokers

### What is Messaging?
Before diving into message brokers, let's udnerstand what messaging means in software systems. Messaging is a method of commuincation between different parts of a distributed system where information is exchanged through structured data packets/objects called "messages".

Think of it like a postal system. Instead of application communicating and calling each other directly (like making a phone call), they can send messages through an intermediary sysetm (like dropping letters in a mailbox). The recipient can pick up and process these messages when they're ready.

#### Restaurant Analogy
Imagine a busy restaurant during dinner rush. Without a system in place, it would be like: 
- Customers walk in and place orders. Customers will wait until the orders are finished.
- Whilst these initial customers are waiting for their order to finish, they block other customers from ordering. This is pretty inefficient as its blocking other orders from being scheduled, it's BLOCKING.

Instead we'll have a better system:
- Customers give orders to waiters (producers).
- Waiters write orders on tickets and place them in the kitchen queue (message broker).
- Kitchen Staff (consumers) pick up orders when they're ready. Multiple orders can be processed simultaneously.
- If the kitchen falls behind, your orders are just going to be waiting in the queue. We'll still be accepting new orders, so things are non-blocking.

#### Why Not Just HTTP? 
Why do we need message brokers when we have classic stuff like HTTP? HTTP uses the request-respones process, it's tight coupling as services must respond quick, it's blocking as the sender is waiting and expecting an immediate response and we're not serving other users whilst we're doing this. This is great for real-time interactions as things are synchronous.

However message brokers are based on asynchronous communication. In situations where we're using message brokers, the client sends a request, but isn't waiting on an immediate response. The client can go and do other stuff, the job is already scheduled. Then they can come back later to see if things are finished. So this is base for background processing, data pipelines, and event-driven applications. 

Relating to the restaurant analogy, a request-response way of communicating is normal, but since our task is long running, it's a bad experience for the user. The user doesn't want to wait there for it to be done (probably ends in a request timeout), and it's even worse knowing that a couple of these long running requests is going to block oru server code, preventing other users from being served. Using synchronrous communication in this situation will drastically reduce the availability of our app. Messaging and message brokers are here to schedule tasks to be done asynchronously, in the background, without blocking other orders from coming in.

---
### What is a Message Broker
A message broker is middleware software that lets applications communicate with each other by exchanging messages. It acts as an intermediary that receives messages from sender applications (producers) and routes them to receiving applications (consumers). Here are the key characteristics of message brokers:
- **Decoupling:** Applications don't need to know about each other directly. You're going to have applications that receive your messages. You odn't need to know what applications do that, don't need to know if they're even active, or how many there are.
- **Reliability:** Messages can be stored and delivered even if systems are temporarily unavailable.
- **Scalability:** Can handle high volumes of messages and multiple consumers.
- **Protocol Translation:** There are actually multiple different messaging formats and protocols, so the broker needs to be able to convert between these to allow for maximum compatibility.

---
### Messaging Patterns
Let's talk about messaging patterns to fully understand the different ways the communication can work:
- **Message Queues:** We have two application talking to each other. One app (producer) sends messages to be stored in a queue, which schedules the job. Then the other pulls from that queue to process jobs.
- **Publish Subscribe (Pub/Sub):** A producer publishes their message to a topic. Then we have multiple consumers subscribe to topics to listen and receive new messages. Think of a topic like a newsletter, and people who are subscribed will receive notifications for new posts. 

**Note:** Message broker is the entire post office whilst the message queue is just one mailbox in the office. You can spin up one messaeg broker instance that manages multiple queues or topics for different processes.

---
### Message Brokers vs Event Streaming 
Message brokers can support two or more messaging patterns including messages queues and pub/sub. Here messages are stored and consumed, it's a one and done thing for messages and the focus is on routing/delivering messages. Some examples are RabbitMQ, Amazon SQS, and ActiveMQ.

Event streaming platforms only offer pub/sub. You'd use this when you have a high volume of messages (or events if you want) that are sent in streams. These message are sent into cateogries called **topics** and stored there for a predetermined amount of time. Here, rather than being being consumed and then removed, your messages are persisted temporarily and can be consumed multiple times. Unlike message brokers, event streaming platforms can't guarantee that messages have been delivered and you can't trace what consumers received the message you sent out. In any case, you see event-streaming platforms for real-time analytics, data pipelines, etc. Examples are Apache Kafka, Amazon Kinesis, etc.

Think of message brokers as the postal service (messages-delivered once) versus event streaming as a newspaper subscription (events can be read multiple times).

---
### Message brokers vs Enterprise Service Bus (ESB)
![](https://www.karanpratapsingh.com/_next/image?url=%2Fstatic%2Fcourses%2Fsystem-design%2Fchapter-III%2Fenterprise-service-bus%2Fenterprise-service-bus.png&w=3840&q=75)
- Heavyweight: Complex infrastructure/architecture with many built-in features
- Centralized: Single point of control and potential failure
- Feature-Rich: Includes routing, transformation, orchestration
- Complexity: Requires significant configuration and maintenance

ESBs were popular in enterprise applications back in the day, but have been largely replaced by microservice architectures that uses simpler message brokers. So you won't see these as often anymore, but it's important to realize that they were related to messaging and asynchronous communication.

---
### Why Not Multithreading or something? 
Let's say we have an application where we're executing long-running and complex simulations. You're right, one solution is to use multithreading and let the simulation be done in a separate thread. This prevents blocking our main thread, we can quickly send a response saying that we schedule the task to be done. However there are some limitations with threads:
- Threads are resource intensive. There's a reason why the event loop exists, creating multiple threads will create multiple stacks, causing a lot of memory overhead. This doesn't scale.
- If our app crashes, our task is lost. There's no built-in retry, logging, or persistence.

Message brokers solve these issues with queues, acknowledgments, and retries. 

--- 
### Core Motivation and Use Cases

#### 1. Decoupling Systems
If we have multiple services that need to communicate, direct connections create tight coupling. If Service A calls B directly, both have to be online. Message brokers eliminate this. For example, let's talk about an e-commerce order processing system:
- Order service receives customer order.
- Instead of calling inventory, payment, and shipping services, the order services publishes a `OrderCreated` message to the broker.
- Each service listening, pulls that message from the broker and processes the order independently.

#### 2. Handling Traffic Spikes
During high traffic periods, some services might be overwhelmed whilst others are idle. For example, take Black Friday sales:
- Thousands of orders are coming in simultaneously.
- Order service publishes these to a queue.
- The processing service consumes messages at their own pace. If it didn't do this, and tried to process all orders at once as quickly as possible, it may lose some messages due to not having enough space/time to respond. With the message queue, orders are processed at their own pace and nothing is lost even if things are going to be a little delayed now due to the asynchronous nature of the work.

#### 3. Reliability and Fault Tolerance
Message brokers can persist messages as we mentioned before, ensuring that they're not lost if parts of our system crashes. For example, let's say we were talking about financial transaction processing:
- Payment messages are stored in broker until they have been confirmed as processed.
- If the payment service crashes, our messages wait safely in the queue with the message broker.
- The service restarts, coming back online, and it can process the remaining messages.

#### 4. Event Driven Architecture
Enable systems to react to events without knowing who generates them. For example, let's talk about a user registration system:
- User registers and the service publishes a `UserRegistered` event. In this case we're using pub/sub, as we expect multiple services to react to this event.
- Email service listens and gets `UserRegistered` event.
- Analytics service updates user metrics.
- Recommendation service initializes user profile.

All of this happens independently and asynchronously.

---
### TLDR and Takeway
Consider message brokers when you need:
- Asynchronous processing of tasks. Especialy if you have long running tasks. Or if you want to regulate traffic spikes 
- Building event-driven architectures
- Integrating systems that can't communicate with each other directly
- If you want improved system reliance and decoupling. I mean if a system is offline, the idea is that at least the message broker will store the message or events until the system gets back online.

