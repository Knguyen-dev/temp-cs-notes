# Load Balancers


### What is "load balancing"? Why do we want it?
The idea of distributing incoming network traffic across multiple resources. As a result we can ensure high availability by redirecting requests to resources that are online, or just redirect traffic from servers that are on heavy-loads to ones with lighter loads. This can help reduce the response times, again the idea of maintaining availability, scaling up the system. You achieve load balancer with a load balancer, which could be a device or special software that achieves this distribution of traffic. Load balancing can happen on multiple levels:
- You could load balance HTTP requests to be distributed across servers.
- You could also have a load balancer database queries across different databases. Like maybe your write queries are sent to the leader database, and read quieries are sent to one of the secondary replicas.

In any case, you typically use an existing load balancer solution to do any type of load balancing. In a production setting and in real projects, you shouldn't be creating a load balancer from scratch. 

The reason we have load balancers is because modern websites must handle a load of requests. A general solution to this is to have multiple servers. Then a load balancer can sit in front of these servers and allocate requests, making sure one server isn't handling all the work, but multiple are. If one server goes down, at least we can direct traffic to other servers that are online. This is the best for scalability and availability.

---
### Different workload distributions
We can distribute requests based on different criteria, and that's a core functionality provided by load balancers:
- **Host-based:** Distributes requests based on the hostname.
- **Path-based:** Distributes requests using the entire URL
- **Content-based:** Inspects the message content of a request. So now you can distribute requests baseed on the content e.g. the value of a parameter, contents of the request, cookies, etc.

---
### Layers and Types of load balancers

Load balancers typically operate at two levels:
- **Application layer (Layer 7):** Reads requests in their entirely and performs content-based routing. Usually more efficient since you can make smarter load balancing decision, but yes more expensive.
- **Network layer (Layer 4):** Routes based on network based info suc has IP addresses. These are often hardware-based, and were used in the early days when hardware wasn't as performant. Nowadays, computers are a lot better so they aren't as relevant.

Then they could either be hardware or software based. The former is easier to deploy, cheaper, and a lot more flexible. The latter are a lot more expensive, and not used as much nowadays.

---
### Routing Algorithms
**Dynamic load balancing** uses algorithms to take into account the current state of each server and distribute traffic accordingly. **Static load balancing** distributes traffic without caring about the server's current state..

Static load balancing algorithms:
- **Round-robin** Distributes traffic to a list of servers in rotation using DNS. First request goes to server 1, second request goes to server 2, and so on until it loops back. Quite simple as it doesn't care how busy or fast each server is, it just rotates blindly. 
- **Weighted Round-robin:** Each server gets a weight, which determines how many requests it should receive per cycle. So if $w_{A}=3,w_{B}=1$, then in one cycle server A gets 3 requests whilst B gets 1. The idea being server A is a lot more powerful and can handle a higher load.
- **IP Hash:** Combines the source and destination IP addresses and hashes them. Then based on this hash, we direct the request to a specific server.

Dynamic load balancing algorithms:
- **Least Connections:** Checks which servers have the fewest connections open at the moment and send straffic to those servers. So here we're kind of assuming each server has the same processing power.
- **Weighted Least Connections:** Least connections, but admins can assign weights that represent the processing power of each server. Then it does a ratio, and we route the request to the one with the lowest ratio.
- **Weighted Response Time:** Averages the response time of each server and combines it with the number of connections of each server to determine where to send the traffic. Basically sending traffic to the servers with the quickest respones time.
- **Resource-Based:** Routes based on what resources each server has available. So you'd have specialized software running on each server to measure its resources (CPU and memory). The load balancer then queries for this and distributes traffic based on that info.


### Fault Tolerance and Redundancy
Having a single load balancer is a single point of failure. To overcome this, having a second or `n` number of load balancers can be used. You could also have failure detection, so if the ***active*** load balancer fails, a ***passive*** load balancer will take its place.

### Features of a load balancer
Some features that you should keep in mind:
- **Sticky sessions:** Since we now have multiple servers, we need the ability to assign the same client to the same server to maintain their session. 
- **Caching:** It could also be able to cache responses.
- **Secure:** Should handle HTTPS connections and show TLS and SSL certs.


### Examples of Load balancers
Here are some commonly used load balancing that are used in industry:
- AWS Elastic Load Balancing
- Azure Load Balancing
- GCP Load Balancing
- DigitalOcean Load Balancer
- Nginx
- HAProxy

## Credits
- [Types of load balancing algorithms - Cloudflare](https://www.cloudflare.com/learning/performance/types-of-load-balancing-algorithms/)
