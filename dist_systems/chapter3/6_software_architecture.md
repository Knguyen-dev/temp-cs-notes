# Software Architecture

## What do we mean by software architecture?
What we mean is how we setup different components of our system to talk to each other. This means like not only how are the servers set up? Are we using load balancers? Or a proxy? Are we using database replication? How about N-Tiered-Architecture? But we also ask things like: Is the code for the calendar service and the chat service in the same app? Or are they separated into their separate modules/servers that communicate with each other? How do your backend services communicate with each other? Are you using HTTP-style communication, or are you using message brokers? 

**A service** is a self-contained module/component within an app that focuses on a specific area of functionality or business logic. It handles a distinct set of requirements, typically reusable, and can operate independently of other services to support the broader application. Like your Order service, an api for handling orders, or maybe your Redis service, which is a running Redis instance for caching. Or maybe it's your PostgreSQL service, which is your database.

## Monolithic Apps
![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-III/monoliths-microservices/monolith.png)
An architecture that combines all of the services in one application. For example, you're building an e-commerce app that takes orders from customers, verifies inventory and available credit, and ships them. It also has a frontend UI component. You would stick all of those component into one app.

**Advantages:**
- Simple to develop or debug.
- Fast and reliable communication.
- Easy monitoring and testing.
- ACID transactions are a lot easier. Everything is in one place, using one shared database.

**Disadvantages:**
1. App tends to get really big over time and the codebase is going to seem daunting. 
2. Scaling up the application is lot harder. If you need to scale up your shipping services, you're going to be spinning up more instances of your entire app. This is wasted resources such as CPU memory, and infrastructure costs. This is where microservices come in as they allow you to scale a given service whilst not affecting others.

## Microservices (Important)
![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-III/monoliths-microservices/microservices.png)
An architecture where each service has a separate codebase and is its own application. Typically, each service is maintained by a separate team. services can be deployed independently, so you can redeploy a service without needing to restart the entire app.

---
### Characteristics of Microservices
- **Loosely coupled:** Services should be loosely coupled so that they can be independently deployed and scaled. This will lead to the decentralization of development teams and thus, enabling them to develop and deploy faster with minimal constraints and operational dependencies.
- **Small but focused:** It's about scope and responsibilities and not size, a service should be focused on a specific problem. Basically, "It does one thing and does it well". Ideally, they can be independent of the underlying architecture.
- **Resilience & Fault tolerance:**  Services should be designed in such a way that they still function in case of failure or errors. In environments with independently deployable services, failure tolerance is of the highest importance. 
- **Highly maintainable:** A service by itself should be easy to maintain and test because services that cannot be maintained will be rewritten.
- **Database-Per Service:** Each service has its own database that only it can directly access. While this sounds great for scaling, it can get really complex as you're now dealing with distributed databases.

---
### Advantages, Disadvantages, Considerations
Things are loosely coupled, allowing for independent scaling and fault tolerance. Also there's the idea that you can have multiple technology stacks in your system as one service could be in one language but another service in another language. This could be a benefit since you aren't committed to a given technology, or a curse since members have to learn more stuff.

The disadvantage is that now you're dealing with the complexity of a distributed system. Testing across services is hard, and inter-service communication is a little trickier. You're probably dealing with distributed transactions as wel.

In general, you should make sure that if a service fails, it shouldn't destroy your entire system. Try not to let errors cascade. Services should communicate through well-defined APIs, again decoupling. Also ensure that API changes are backward compatible. A good way to do this is to just make sure that you're dealing API versioning. It's also common to use NoSQL databases, and mix them in for certain services. This is also the idea of not going all in on ACID, but using some BASE as well.

---
### Microservices vs Monolith (Suggestion):
Even Netflix started off as a monolith. If you're just getting started, it's probably better to start with a Monolith, as you're just trying to get your product to market. Building a distributed system is pretty hard, so you can worry about scaling to a million users when you get there. Also you'd usually do it if you have a large team of people.

We usually draw inspiration from places like Netflix, but then we overlook the fact that we aren't Netflix. They won't through many iterations before they had a good solution, and through that they found this architecture. Microservices are solutions to complex concerns, and if your business doesn't have complex issues, then you don't need them.



## SOA (Service Oriented Architecture)
A precursor to microservices. We still define software components as 'services'. Each service provides a feature, or satisfies some business requirement that the app needs to have. Services should be able to communicate each other across platforms and languages. Here are some key characteristics though:
- Communication: ESB Heavily relies on ESB for communication. It also uses stuff like SOAP, XML, etc. Kind of old school in that sense.
- Usually they all mess with a shared database, rather than having a database per service.
- The technology stack is usually standardized rather than having diversity.

## Serverless
The idea of using servers provided by a **cloud service provider** (CSP) instead of your own. So the idea is that developers can focus on writing application code, and deploying it to containers managed by the CSP. So the CSP handles scaling as needed, infrastructure maintenance, OS updates/patches, security, system monitoring and more. As well, you only pay for the time your servers are running.

Examples of serverless services are AWS Lambda, Azure Functions, or Google Cloud Functions. 


# Credits
1. [Monolithic Architecture](https://microservices.io/patterns/monolithic.html)
2. [Monolithic vs Microservice Architecture](https://youtu.be/NdeTGlZ__Do?si=ShYkjeHQFC8iio4P)
3. [Service Oriented Architecture](https://aws.amazon.com/what-is/service-oriented-architecture/#:~:text=you%20implement%20microservices%3F-,What%20is%20service%2Doriented%20architecture%3F,other%20across%20platforms%20and%20languages.)
4. [What is serverless computing? - IBM](https://www.ibm.com/topics/serverless)

