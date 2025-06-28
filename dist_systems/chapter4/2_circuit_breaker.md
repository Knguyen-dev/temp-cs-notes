# Circuit Breaker Pattern, Scaling, and More

## What is the Circuit Breaker Pattern
It's a design pattern used in microservices architecture to prevent service failures from impacting an entire system (preventing cascading failures). The idea is that a circuit breaker will monitor your services. When it detects an increase in failures, due to things like timeouts, server overloads, or network issues, it activates/trips by halting traffic to the failing service. This gives time for the system to recover. A circuit breaker operates in three primary states:
1. **Closed:** Requests pass through normally. Relating to electronics, think of this as the circuit being in a closed state, so current passes through fine.
2. **Open:** Requests are halted, nothing passes through. Think of it as the circuit being open, so current can't pass through. 
3. **Half-open:** Here we send a small amount of test requests to determine if the service has recovered. If things are good, the breaker returns back to the closed state, else it transitions into open where it'll be given time to recover.

Again the idea is that we want fault tolerance and system resilience. This pattern emphasizes the idea that when one service fails, we want to minimize the damage that causes, such that other services failure because of the first one, and it snowballs into something disastrous. 
The motivation with circuit breaker is that we want to 

## The Problem
An application may not get a response from the database in an expected timeframe due to a temporary network disruption (or other factors). This can actually increase your load as your maintaining connections for a little longer, you may have retry mechanisms, you may handle this higher load using the backpressure queuing technique, etc. High volumes of concurrent database operations of course contribute to the load, and at worse your system just stops working.

Consider a standard request-response flow for a HTTP-based API:
1. Client sends HTTP request to application.
2. Application sends read query to the database. 
3. Database sends data to application.
4. Application sends response to the client. 

Now we could have a network failure that results in us not being able to use the database. 
- **Connection Churn:** Creating a new connection to the database, attempting to use it (which fails), closing it, and repeating for the next request.
- **Read Query Retry:** Applications often retry read operations against replicas, so if a read operation against Database A doesn't work, we'll do the query on Database B.
- **Timeout and Connection Churn:** If a network disruption occurs, our request could timeout and we'd have to wait for a lent time (e.g. 15+ seconds) or it may not even finish, making our app wait forever. By implementing a timeout mechanism, we can make it so that if the backend doesn't respond in a fixed amount of time, we cancel the request. This fails fast, and we can take action like retrying the request, logging, etc. 

![](https://images.ctfassets.net/drk57q8lctrm/3ZZ9ZQm6y1ddjHNR0kqWMR/bbcb6c8c1b7c8eb4b61ddbd82f012ba4/circuit-breaker-pattern-http-client-db-node.webp)
Let's walk through a real scenario:
1. App sends a DB read request
2. Network failure, so the app doesn't get that data and can't respond in time.
3. We setup a timeout mechanism and the app will close the connection because of this.
4. The app open handles a new connection, and this is the idea of re-trying this request for the user. 
5. App is configured for high availability, so it'll retry the read query on a replica (another DB server) instead of the one that previously failed. This retry could also timeout, and cause us to churn the connection, meaning steps 3-5 may be repeated. Repeated until we fail their requests after a certain number of iterations, or return their data like they wanted.

This design is somewhat concerning because as you scale up, the way we're maintaining high availability is kind of expensive. This assumes the worse case where most steps fail, but as you scale up, these types of failures become more common:
  - More requests means more chances to fail.
  - Also increases the number of retries, increasing pressure on our backend.
  - Increased number of connections being processed or churned. Most apps use TLS connections, which are expensive.
  - One failure can cause multiple retries, adding more load on your database replicas.

There's also the idea of creating an error feedback loop. The idea that as errors increase, those errors are being re-tried, which further puts load on the system. The additional load then causes more requests to fail, snowballing, or at least persisting even after the initial requests that failed have already been processed. 

## How the Circuit Breaker Pattern Works
The Circuit Breaker pattern, typically implemented as some software component, will "trip the breaker" when the number of failures within a given time period exceeds a predefined threshold. 

For example, assume an app is doing 1,000 read queries per second and a network failure occurs and lasts for 2 seconds. During this outage, we would get 1,000 network errors and so that's 1,000 churned connections. With circuit breaker, we could set it up so that at maximum there could be 100 failed connections (that connect to the database), but after this threshold we immediately raise an exception for the remaining 900 operations instead of trying to connect to the database server.

- Without the circuit breaker: 1000 request per second, 1000 wasted connections that are created and destroyed per second.
- With Cirucit Breaker: Only 100 requests actually attempt the connection (our threshold), whilst the other 900 fail fast wtih an immediate exception so they don't even connect to the database.

The rationale is that if 100 database attempst are failing, we can reason that our database is likely down or unreachable. We can assume that the remaining 900 requests in that short timeframe will probably fail as well. So instead of wasting resources on 900 more doomed database connections, we immediately fail the request without ever interacting with the database on that connection. The benefits are that we're not wasting resources doing TCP setup, we reduce latency as we don't wait for the connection to timeout instead we just tell the user we failed, and we reduce the load on our infrastructure; giving the database a chance to recover instead of being bombarded. 

## Credits
- [The Circuit Breaker Pattern Intro - Micah Carrick](https://aerospike.com/blog/circuit-breaker-pattern/)
- [Circuit Breaker and Graceful Degradation Best Practices (Technical)- AWS Architecture](https://docs.aws.amazon.com/wellarchitected/latest/reliability-pillar/rel_mitigate_interaction_failure_graceful_degradation.html)
