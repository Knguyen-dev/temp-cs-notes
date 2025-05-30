# Proxies
A proxy server is an intermediate piece of hardware/software that sits between the client and backend server. receives requests from client and relays them to the origin servers. Proxies are used to filter, log, and sometimes transform requests (by adding/removing headers, encrypting/decrypting, etc.).

### Types of proxies:

#### Forward Proxy
![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-I/proxy/forward-proxy.png)
A forward proxy, often called a proxy, proxy server, or web proxy is a server that sits in front of a group of client machines. When those computers make requests to sites and services on the internet, the proxy server intercepts those requests and then communicates with web servers on behalf of those clients, like a middleman.

#### Benefits of a forward proxy
- **To block access to certain content:** They can be setup to block a group of users from accessing certain sites on the Internet. For example, a school network is probably configured such that anyone connected has to go through the proxy. As a result, the proxy can be configured to refuse requests to certain sites such as Facebook and Instagram.
- **Avoid state or institutional browsing restrictions:** Some governments, schools, etc. use firewalls to limit users access to the internet. A forward proxy lets users bypass this.
- **Identity Protection:** Regular Internet users may want more online anonymity. For example, they could live in places where posting about the government is pretty illegal. Using a forward proxy to connect to a website to talk about it is a lot safer, as you aren't given the IP address of the user's machine, but a proxy's IP.

#### Reverse Proxy
![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-I/proxy/reverse-proxy.png)
A reverse proxy sits in front of one or more web servers, intercepting requests from clients. When clients send requests to the origin server, those requests are intercepted by the reverse proxy. This prevents clients from directly communicating with the origin servers.

#### Benefits of a reverse proxy
- **Increased Security:** With a reverse proxy, the IP addresses of the origin servers aer never revealed to the public. This makes it more difficult to do a targeted attack such as a DDoS attack as attackers will only be targeting the reverse proxy itself. Also reverse proxy services nowadays have built in mechanisms to detect suspicious traffic and prevent DDoS attacks from taking down your origin servers.
- **SSL or TLS support:** Can be configured to decrypt incoming requests and encrypt all outgoing requests. This not only reduces computationally expensive operations from your origin server, but it's just extra security against things like 'man in the middle' attacks.
- **Increased scalability (load balancing):** A reverse proxy also acts as a load balancing solution, distributing traffic evenly amongst your different servers.
- **Caching:** They can also cache content. For example, a user in Paris connects to a site with origin servers in Los Angeles. The request would probably go to a Paris reverse proxy that would communicate with the LA origin servers, then cache the results of the response. As a result, subsequent Parisian users will also get the locally cached version from the load balancer, which increases performance on both sides.

### Load balancer vs Reverse Proxy, examples of proxy solutions
- **Load balancer:** Useful when we have multiple servers. Used when routing traffic to a set of servers that all serve the same function.
- **Reverse proxy:** Can be useful when we have just one or two servers. Remember that a reverse proxy can be a load balancer, but not the other way around.

Some examples are Nginx, HAProxy, etc.