# API Gateway
An API Gateway is a component that sits between a client and your backend services. It acts as the single entrypoint that traffic from the broader internet can enter through your internal group of applications. It also has other responsibilities as handling authnetication, monitoring, load balancing, caching, throttling, logging, etc.
![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-III/api-gateway/api-gateway.png)

Other features include:
- authentication, security enforcement
- load balancing ciruict breaking
- protocol translation and service discovery

### Why do we need an API Gateway
If it wasn't obvious


### Backend For Frontend (BFF) Pattern

---
### API Gateway Workflow
1. Client sends request to API Gateway
2. API Gateway begins some checks.
3. Validates parameters
4. Checks IP address against an allow or denylist.
5. The request goes through authentication and authorization. Here your API Gateway will talk to an IDP (external orl internal), possibly mapping a user to the request.
6. It can also perform basic rate limiting based on IP address or request headers. Or even rate limiting based on the user session, depends on how far you'd want to go.
7. With the help of a service discovery tool, hte API Gateway helps route the request to the correct service.
8. It transforms the request into the desired protocol for the backend service. Then when it gets a response back, it sends that response back to the client.
  


## Credits
- [What is an API Gateway - ByteByteGo](https://www.youtube.com/watch?v=6ULyxuHKxg8)