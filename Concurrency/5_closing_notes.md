

Reinforce concepts:
- What is blocking the event loop?
- Multithreading and concurrency
- Thread pools shared resources, event loops, backpressure and queues, failure handling.


### Projects
Here are some projects I recommend doing if you're still feeling unsure about multithreading and concurrency, and whatnot. You could easily research a lot of these to understand what it would take to get started and follow along with their tutorial. A lot on build your own x, and just online. This is probalby not the highest priority, but it's something to think about.

1. Multithreaded Web Crawler
Concepts: Thread pools, producer-consumer, I/O blocking

Fetch URLs in parallel using multiple threads

Use a shared queue of URLs to crawl (with a lock or concurrent queue)

Detect and avoid visiting the same page twice (shared visited set)

✅ Learn: Thread coordination, blocking I/O (HTTP), URL scheduling

2. 

Peer-peer note taking platform for university students. Peer to peer is when resources are retireved from peers rather than a 
central server. Users can register their files to the platform, put filters on them like the class, the professor, etc.
They use a central server to store metadata about the files and ip addresses. So it's not fully peer to peer, it's a lot harder to 
do that, but it's pretty close. When you want to download something it creates a TCP connection between the two computers and sends the data 
over. Pretty good project for network programming.


2. Chat Server with Clients
Concepts: Sockets, multithreading, blocking vs non-blocking I/O

Build a TCP chat server that can handle multiple clients

Use a thread per client (blocking) or select()/epoll (non-blocking)

Add user IDs and chat rooms

✅ Learn: Network I/O, event loops, threading models

3. Threaded File Processor
Concepts: Task parallelism, CPU vs I/O-bound work

Watch a folder for new files (log files, CSVs)

Spin up threads to parse, analyze, and write output concurrently

✅ Learn: File I/O, blocking disk reads, parallel task execution

4. Job Queue System (Mini RabbitMQ)
Concepts: Message passing, producer/consumer, retry logic

Clients publish jobs (like "resize image")

Workers pull jobs from a queue and process them

Include retries, dead-letter queues, job acknowledgments

✅ Learn: Queuing, thread-safe data structures, reliability

5. Key-Value Store with Replication (Mini Redis)
Concepts: State sharing, TCP servers, replication, race conditions

Build a simple in-memory key-value store

Add master-slave replication with periodic sync

Add a simple command interface (GET, SET, DELETE)

✅ Learn: Shared state, concurrency, consistency, serializatio


7. Rate Limiter Service
Concepts: Token bucket/leaky bucket, time-based concurrency

Limit requests per user to X per minute

Use threads to simulate users

Use shared data structures for tracking rate limits

✅ Learn: Time-aware data structures, thread safety, system boundaries