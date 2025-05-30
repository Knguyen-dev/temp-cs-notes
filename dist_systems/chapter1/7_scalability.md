# Scalability 
The measure of how well a system responds to changes by adding or removing resources to meet demands. Mainly used when talking about whether a system can grow bigger. 

### Vertical Scaling 
Vertical scaling (also known as scaling up) would mean adding more power to an existing machine. So this would be upgrading the hardware such as upgrading the CPU, adding more RAM or storage, or even replacing your entire server with a more powerful one

- **Disadvantages:** The drawbacks of this are that at some point you'll either exhaust your financial resources, or you'll reach the current limit in technology. In the sense that, hey there's no CPU more powerful than the one you currently have, so you have to look for another solution. In any case, you're either going to hit a technological and financial ceiling, which isn't good.

### Horizontal Scaling (With Load Balancing)
Horizontal scaling (also known as scaling out) expands a system's scale by adding more machines. It improves the performance of the server by adding more instances to the existing pool of servers, allowing the load to be distributed more evenly.

Of course, remember you'd want the to make your server's IP addresses private, and route people to the public IP address of the load balancer. In general horizontal scaling is the preferred approach, as it's more cost effective, and also you reduce the likelihood of those hitting those common ceilings we talked about. Though remember, the setup is more complex and you need to deal with data inconsistency.