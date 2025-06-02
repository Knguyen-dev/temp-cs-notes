# API Gateway
An API Gateway is a component that sits between a client and your backend services. It acts as the single entrypoint that traffic from the broader internet can enter through your internal group of applications. It also has other responsibilities as handling authentication, monitoring, load balancing, caching, throttling, logging, etc.
![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-III/api-gateway/api-gateway.png)


### Why do we need an API Gateway
Think of "granularity" like the size of puzzle pieces. Microservices typically provide fine-grained (small piece) APIs, while clients often need coarse-grained (big piece) functionality.

**Example Scenario (Without API Gateway):** You're building an e-commerce application and you want to display a user's order history. Without an API gateway, your client does:
1. Calls User Service to get user details.
2. Calls order service to get order list.
3. Call product service to get product details.
4. Calls payment service to get payment status.
5. Call shipping service to get delivery status.

Your mobile app has to make about 5 separate API calls, handle different responses, pass their authentication token to each service, and handle error scenarios for each service. Your client is tightly coupled with your backend, almost like it needs to know the implementation and all of these details in order to get the order history. It'd be better if things we're behind a single endpoint, and more decoupled. If you change the logic of the ordering process, you may break the client applications that are very tightly relying on your code.

**With an API gateway:** You make one call to `/api/user-orders/123` and the gateway handles calling all of those services for you. It then combines hte data, handling error scenarios, handling authenticating your request to all services. Finally it returns exactly what your mobile app needs, **encapsulating** and hiding the unnecessary complexities of the operation. Instead of the client team having to understand and integrate with various different services and endpoints (each with their own docs, authentication, rate limits, etc.) they only need to follow the API gateway's interface. This is not something you simply fix by adding an all-encompassing endpoint for order history. You still have to address some other issues, and simply an API gateway has addressed said issues and has gone beyond:
- **Authentication Issue:** Differing services may use JWT, OAuth2.0, API keys, etc. Instead of making the client manage 4 different types of credentials and failure scenarios, the client will authenticate once with the gateway. Then the gateway handles all downstream authentication internally. 
- **Clearer Intent:** Even if you started creating all-encompassing endpoints, your client team still has to dig through the docs of your microservices. The API gateway let's you define routes, and here it'll handle orchestrating calls to backend services.
- **Service Discovery:** 
```bash
# Without service discovery, client devs need to know these DNS+ports
userservice-prod-1.internal.company.com:8080
orderservice-prod-2.internal.company.com:8081
paymentservice-prod-1.internal.company.com:8082

# Here's how you access it with a gateway
api.company.com
```

---
### Features of an API Gateway
- Authentication and Authorization
- Service discovery
- Reverse Proxy
- Caching
- Security
- Retry and Circuit breaking
- Load balancing
- Logging, Tracing
- API composition
- Rate limiting and throttling
- Versioning
- Routing
- IP whitelisting or blacklisting

---
### Advantages and Disadvantages
I mean this encapsulates your API and simplifies client code and interactions. It's also good security, and generally having a reverse proxy like this is pretty good. The only thing that could seem bad is how it could be a single point of failure. Also it could be challenging to configure and setup.

---
### Backend For Frontend (BFF) Pattern
Here, you create separate backend services that are used by specific frontend applications. The main idea is that we want ot get the required data from the appropriate services, format it ,and send it to the frontend. Very minimal. 

![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-III/api-gateway/backend-for-frontend.png)

One thing is that data from microservices is not in the right format, so we have a middle component (api gateway) that does this. 

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
  
### TLDR and Takeaway
Generally API gateways and having a component that sits in front of your origin servers is good. Examples of widely used gateway technologies are Amazon API Gateway, Azure API Gateway, and Kong API Gateway.

## Credits
- [What is an API Gateway - ByteByteGo](https://www.youtube.com/watch?v=6ULyxuHKxg8)