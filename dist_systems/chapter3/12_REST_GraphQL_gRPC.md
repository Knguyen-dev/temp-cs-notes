# REST, GraphQL, gRPC

## What is an API
Before we talk about API technologies (like REST, GraphQL, and gRPC), let's fully understand what an API is. An API (Application Programming Interface) is a set of definitions and protocols for building and integrating software. More simply, think of it as a clear list of ways one computer can talk to another computer. If you want to interact with another computer/system e.g. getting data, uploading data, etc. the people who created the system likely posted the API online to show you "Hey this is how you upload a video. Here's how to download a file from our site". 

## REST (Representational State Transfer)
Also known as a RESTful API, it's an API that conforms to the rules of the REST architectural style.  Essentially, here are the rules that an API has to follow to be RESTful.

### RestFUL Constraints
- **Client-server architecture:** We have a client that needs our resources and a server that has the resources. All of this is done through HTTP.
- **Stateless:** All information required to do a request is sent by the client. Every request is a standalone request (self-contained), so the server doesn't need to try to remember a previous request such as store data during client-server communication. If we send two requests, A and B. The server shouldn't need to remember any information from request A, to do stuff for request B. 
- **Cache-ability:** Responses from the server should say whether or not the data can be cached. 
- A **uniform interface** between components so that information is transferred in a standard form. This requires that:
  - Resources are identifiable and distinguishable. So like you'd have unique urls for each resource. For a collection of resources you'd put an Id with each one like `/posts/:postId`
  - The data you get tells you how you can change it. So if you see the `title` field in your data, you're probably going to be able to modify that field with another request.
  - The response tells you what the data is and how to use it. Basically we actually send back data, `Content-Type`, etc.
  - The response includes links for what client can do next. So like you could have a field with the URL endpoint to the comments, endpoint to delete the post, to edit the post, etc. Aka HATEOAS.
- **Layered System:** There are layers to this system. So like we could had load-balancing, proxies, a database layer. This just makes it easier to work with the system.

### HTTP Verbs
HTTP defines a set of request methods to indicate the desired action to be performed for a given resource. Although they can also be nouns, these request methods are sometimes referred to as HTTP verbs. Each of them implements a different semantic, but some common features are shared by a group of them. Below are some commonly used HTTP verbs:
- **GET:** Request a representation of the specified resource.
- **HEAD:** Response is identical to a GET request, but without the response body.
- **POST:** Submits an entity to the specified resource, often causing a change in state or side effects on the server. 
- **PUT:** Replaces all current representations of the target resource with the request payload.
- **DELETE:** Deletes the specified resource.
- **PATCH:** Applies partial modifications to a resource.

### HTTP Response Codes
[HTTP response](https://en.wikipedia.org/wiki/List_of_HTTP_status_codes) status codes indicate whether a specific HTTP request has been successfully completed. There are five classes defined by the standard:
- 1xx - Informational responses.
- 2xx - Successful responses.
- 3xx - Redirection responses.
- 4xx - Client error responses.
- 5xx - Server error responses.

For example, HTTP 200 means that the request was successful.

### Takeaways and Tidbits for REST
- Accept/Respond with JSON: Json is just standard and common for transferring data so don't deviate from it.
- Use nouns instead of verbs in endpoints: We use nouns to represent the thing we're getting. So don't use '/getComments' to get a list of comments. Instead use '/comments', where we just include the noun. Remember following CRUD, we know GET, POST, PUT, and DELETE.
- Use logical nesting: Group stuff that make sense together. If your object contains another object, then cover that endpoint. So a post contains comments, so do `/posts/:postid/comments/:commentid`. However, don't nest too far, as if you're 2 or 3 levels in, you may want to return a URL to those resources. So if you want to return a user who made a comment, you could do `/posts/:postid/comments/:commentid` and return that, or you could send a json response such as `"author": "/users/:userid"`.

## GraphQL APIs

### Introduction to GraphQL
GraphQL is a query language for APIs and a server-side runtime, allowing for a different way for clients to get data. Unlike traditional REST APIs, GraphQL gives clients the power to ask for exactly what they need. So like, instead of fetching an entire social media post from an endpoint, you can choose to only receive certain properties of the social media post that you want. 
- **Type System:** GraphQL uses a type system to define structures of the data and endpoints on the serverside, and then on the client side we define the queries.
- **Single Endpoint:** All requests go through a single URL endpoint.
- **Client-Specified Queries:** Clients define queries, which define the structure of the data that they receive.
- **Hierarchial:** Queries should mirror the structure of hte data they return. 

GraphQL addresses some issues:
- **Overfetching:** REST APIs often return more data than needed. For example, fetching a user profile might return all user fields when you only need the name and email.
- **Underfetching:** Sometimes you need to make multiple REST API calls to get all the required data. For example if you want to get a user and all their videos, maybe you're doing two different GET requests, which leads to the N+1 query issue and more latency.
- **API Versioning:** GraphQL has features that makes it easier or more friendly to evolve your API. In this example, the `name` property for our API is kind of deprecated, we want people to use `fullName` instead. So the client-side tooling will tell the developers interacting with your API that `name` is deprecated and to use `fullName` instead. 
```graphql
type User {
  fullName: String
  nickname: String
  name: String @deprecated(reason: "Use `fullName`.")
}
```

### GraphQL Schema Example
- **Schemas:** Schemas represent entities in your data.
- **Queries:** Queries are created on both sides. Like your endpoints and queries to access those endpoints.
- **Resolvers:** Code that resolves queries.
```graphql
type Creator {
  id: ID!   # Required field
  name: String
  subscribers: Int
  email: String
  
  # A creator can have multiple videos
  videos: [Video]
}

type Video {
  id: ID!
  title: String!
  url: String!
  duration: Int 
  views: Int

  # A video belongs to one creator
  creator: Creator
}

"""
All GraphQL applications will start with "Query" was the root. But in any case, here we give the endpoints for what hte user can get as data. So think of these as GET requests.
""" 

type Query {
  # Get all videos
  videos: [Video]

  # Get a specific creator by ID
  creator(id: ID!): Creator

  # Search videos by title
  searchVideos(query: String!): [Video]
}

# Define what data clients can modify
type Mutation {
  createVideo(title: String!, ur: String!, creatorId: ID!): Video
  deleteVideo(id: ID!): Boolean
}
```
Let's say I was a person on the client side who wants to get some data. I'd craft my own query:
```graphql
query {
  # Just want to query all videos, but we only want the title, url, and the name of the video's creator. 
  videos {
    time
    url
    creator {
      name
    }
  }
}
```
GraphQL obviously has more features, and this writeup won't have enough information to become comfortable with it. So if you're interested, then learn from a tutorial and the docs.

---
### When to use GraphQL
- Reducing bandwidth as a client can query multiple resources in a single query.
- When working with a graph-like data model. I mean if you're using a graph database, you're handling very complex relationships. 
- When you have multiple clients, they're probably going to want different subsets of your data. 


## gRPC (Google Remote Procedure Call) APIs

### Introduction to gRPC and Protobuf
gRPC is a modern open source RPC framework that was developed by Google. It implements RPC, but adds extra optimizations upon it. It abstracts the data exchange mechanism (HTTP, TCP, UDP, etc.). The abstractions and whatnot makes gRPC easier to work with than other RPC implementations.

---
### Similarities and differences between gRPC and REST

#### General Intro
They both are used in a client-server architecture. They both use HTTP, but different versions. REST typically uses HTTP/1.1 whilst gRPC uses HTTP/2. You can implement both in various programming languages. Both support asynchronous communication and stateless design. This reinforces that they are suitable for scalable and distributed systems as you're going to be able to handle concurrent requests and multiple clients. In REST, it's entity/resource oriented. The client interfaces with a resource endpoint `POST /orders`. This is a constraint with REST, but with gRPC, teh client interacts with services/procedures/functions. Then the client uses these functions such as `createNewOrder(customer_id, item_id, item_quantity) -> order_id`. REST is more loosely decoupled since they don't neeed to know about each other's implementations. However gRPC is tightly coupled, as both client and server need to have access to the same proto files to be able to serialize and deserialize gRPC data structures into something that's usable.  

#### Communication Models/Styles
REST APIs follow a traditional request-response model. The client sends a request and waits for a single response from the server. This is called a unary communication pattern. REST servers can handle many concurrent requests, each individual client-server interaction follows this same basic pattern. gRPC supports multiple communication patterns beyond just request-response:
- **Unary:** Traditional one request $\rightarrow$ one response (same as REST).
- **Server streaming:** Client sends one request $\rightarrow$ server sends multiple responses over time.
- **Client streaming:** Client sends multiple requests $\rightarrow$ server sends one final response.
- **Bidirectional streaming:** Both client and server can send multiple messages back and forth.

#### Differences in Transmission
In REST, the data structures are typically sent and received as JSON. E.g. the server constructs some JSON, and then the client will receive that same JSON data. Though we can also pass in XML and JSON. However, gRPC uses Protobuf by default. The server defines a data structure using Protobuf's interface description language (IDL) in a proto-specification file. Then gRPC will serialize the structure into binary and deserialize it for the client. In both cases, data structures are serialized into binary, and then that binary is translated back to a human-readable data structure, but then why is gRPC faster? Well we'd have to look at the transmission process.

**REST API with JSON Transmission Process**
1. **Application layer:** Data exists as objects in memory
2. **JSON serialization:** Objects are converted into JSON strings. Still human readable kind of.
3. **HTTP layer:** JSON text is inserted into HTTP request/response body.
4. **Transport layer:** HTTP message (including JSON text) is encoded into binary for TCP transmission.
5. **Network transmission:** Binary data is sent over the wire.
6. **Receiving end:** Binary data is decoded back into HTTP, and JSON text is parsed back into objects.

**gRPC with Protocol buffers Transmission Process**
1. **Application layer:** Data exists as objects 
2. **Protobuf serialization:** Objects are serialized into a compact binary format. 
3. **HTTP/2 layer:** Binary protobuf data becomes the message payload. 
4. **Transport layer:** The already binary data is prepared for TCP transmission.
5. **Network transmission:** Binary data is sent over the wire.
6. **Receiving end:** Binary protobuf data is directly serialized back into objects.

gRPC is actually faster. Both end up as binary to be sent over the wire, but rather we should think about other factors. The protobuf produces a smaller binary representation than JSON. There are fewer conversation setps as gRPC skips the text representation step entirely. As well as this, HTTP/2 (gRPC) has less protovol overhead than HTTP/1.1 (typical REST). Then think about the fact that it's a lot easier to deserialize binary than to also parse text.

### Why Would You Use gRPC?
gRPC was specifically designed for high performance APIs in a miroservices architecture. It's better sutied for internal systems, system/system communication, that require real-time data streaming and large data loads. This is not something you'd use for public-facing APIs as it's a lot harder to understand, and it's not something you'd use for simple data communication. You'd use REST for a public facing API since it is a lot more readable.

## Credits
- [REST API Explained - RedHat](https://www.redhat.com/en/topics/api/what-is-a-rest-api)
- [GraphQL Explained - Fireship](https://www.youtube.com/watch?v=eIQh02xuVw4)
- [gRPC easily explained ](https://medium.com/@tejasmane485/grpc-protocol-easy-explained-5140d34d892a)
- [Differences between gRPC and REST](https://aws.amazon.com/compare/the-difference-between-grpc-and-rest/)