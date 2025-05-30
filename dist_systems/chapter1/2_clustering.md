# Clustering
At a high level, a computer cluster is a group of two or more computers/nodes that run in parallel to achieve a common goal. As a result, you can distribute a higher number of tasks amongst the cluster (horizontal scaling). 

To create a cluster, your individual nodes should all be connected to a network that enables inter-node communication. Typically one node is the leader, acting as the entrypoint to the cluster. It may distribute incoming work to the cluster and even be the one returning the response to the client.

Ideally it should act as a single system, meaning a client shouldn't need to know that the system is a cluster or an individual machine. 

### Types of Clusters
This will be explained later in the configuration types section:
- Highly available or fail-over
- Load balancing
- High-performance computing

### Configuration Types 
**Active-active** and **active-passive** configurations are going to be the ones that you see most often in high availability clustering setups.


![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-I/clustering/active-active.png)
**Active-active** clusters just involve a cluster services that are running the same kind of service in parallel. The main reason you want this is to introduce the idea of load balancing and scaling. You have more servers serving the same content, increasing your throughput (# of requests you can handle per second) and latency (response time). 

![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-I/clustering/active-passive.png)
**Active-Passive** have a similar idea. You of course have at least two nodes, but one of them is active and actually serving content. The other node, **the failover**, is here in case the active node crashes or something, and if that happens, the failover takes over and starts serving content in its place. This minimizes downtime, and places emphasis on this idea of **high availability**.

In general, you get a lot of benefits from doing clustering. You may increase the availability of your service, as now there's like more fault tolerance. But also you increase the scalability as well since now there's multiple nodes that take do their own fair share of work, instead of just one node that does everything and responds poorly.

### Load Balancing vs Clustering, Challenges with Clustering, and examples
Clustering provides redundancy, gives availability and whatnot. However nodes in a cluster are aware of each other and work together toward a common purpose. With pure load balancing, servers don't need to know each other exist and just focus on the request. Of course you can use them both, I mean look at our active-active diagram, that's using both.

Setting up clustering can be tough. You setup multiple computers, probably to do the same thing. You need monitor resources for each node if you want to do smart stuff if you want to know how to scale. You need to aggregate logs from each node to a centralized place. Having them all connect to the same database could also introduce some complexities due to data synchronization.

Other than that clustering is common and many technologies offer clustering like container services (Kubernetes, AWS ECS), databases like MongoDB, and caches like Redis.