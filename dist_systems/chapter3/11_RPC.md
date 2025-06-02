# Remote Procedure Call (RPC)

## What is RPC?
The concept of RPC is about allowing a program to execute a procedure (block of code) in another address space (usually on a remote server) as if it was a local procedure call (as if it were local code). So it abstracts away the complexities of network communication, allowing us to call remote software as if we have it on our local machine. This lets us write distributed applications without worrying about the underlying networking details.

A **remote procedure** is a block or code or function located in a different address space or on a remote system that we (the client) can call/execute. Here are the key characteristics of a remote procedure:
  - **Located on a remote system:** A remote procedure resides on a different machine compared to the client that calls it. It's implemented somewhere else. 
  - **Callable by remote clients:** Clients can execute this procedure remotely as if they were calling a local function/method. 
  - **Abstracted Communication:** The communication between client and the remote procedure is abstracted. So you should be able to call the function, without extensive low-level networking knowledge.
  - **Transparent Invocation:** The client calling the remote procedure is going to use a programming language that makes calling the remote procedure seem like we're just executing a regular function.

## Key Components of RPC
- **Client:** The program that initiates the RPC process by requesting a service from a remote server.
- **Server:** The program that provides that requested service. It listens for incoming RPC requests and executes the corresponding procedures.
- **Stub:** AKA the ***client proxy***. It's the local representation of a remote service. The client interacts with the stub as if it were the actual service. The proxy handles communication details.
- **Skeleton:** AKA the ***server proxy***. It's the server's interface to the client. It's the one receiving RPC unpacks, unpacking parameters and invoking the requested procedures on the server.
- **Communication Protocol:** Defines the rules and formats for exchanging messages between the client and server. Protocols like HTTP and TCP/IP help send and receive RPC messages.

So far, it's a way for a server to expose some services, bare bones functions in this case, and then the client can call those remotely. It's a little different from REST, which is a different architectural style with extra rules, but whether we're targeting endpoints (which map to route handlers) or barebones functions, we requesting resources from the server.


## How does RPC work? An example of workflow?

### RPC Workflow Step by Step
When a client invokes a remote procedure, here's what happens:
1. **Client Stub Invocation:** The client calls a procedure via the stub. The stub prepares an RPC request, including info about the procedure to execute and its parameters. 
2. **Marshalling and request:** The parameters and method info are serialized so that they can be sent over the network. The client then sends the RPC request to the server using the chosen communication protocol.
3. **Server Skeleton Processing:** The server's proxy receives the request and unpacks the data (de-marshalling), and determines the requested procedure.
4. **Local Procedure Execution:** The server executes the requested procedure with the parameters.
5. **Server Response:** Server marshals the response and sends it back to the client.
6. **Client processing:** The client stub receives the response, demarshals it, and gives the result to the client, as if it was the result of calling a local function.

---
### Example Scenario of RPC
Imagine a scenario where a client program needs to calculate the square of a number using a remote procedure hosted on a server:
1. **Client Request:** Client sends a request to the server, asking it to calculate the square of a particular number, say 5.
2. **Remote Procedure Execution:** The server receives the request, locates the `calculate_square(x)` procedure. It does the calculation and prepares a response.
3. **Response Transmission:** The server sends back the response (25) to the client.
4. **Client Retrieval:** The client receives the response and can continue using the result obtained from the RPC process.



## RPC and Distributed Systems? Motivation for RPC and Alternatives

### Types of RPC
- **Synchronous vs Asynchronous:** Synchronous RPC waits for a response before continuing whilst asynchronous RPC allows the client to continue working.
- **Blocking vs Non-blocking:** Think more about the thread, whether the thread is paused whlist waiting for the response or if the thread can do other work.


---
### Common RPC Frameworks
- **gRPC:** Developed by Google, gRPC is an open-source RPC framework, an actual implementation. It uses HTTP/2 for transport and Protocol Buffers for serialization. We will talk about this later.
- **Apache Thrift:** An RPC framework developed by Facebook that supports multiple programming languages and transports.
- **Java RMI (Remote Method Invocation):** A Java-specific RPC mechanism for remote communication between Java objects. 

---
### Alternatives to RPC, Considerations, when to use which?

An alternative to RPC is REST APIs or other HTTP-based communication. RESTful APIs use HTTP methods to access resources, where those resources/data entities are available via URLs (api endpoints). But RPC focuses more on remote method invocation, literally executing functions in a remote server. 

REST API use text-based formats like JSON or XML to exchange data, whilst RPC uses binary serialization formats. With JSON, you have a `User` object, serializing it into a JSON string, which is of course converted into binary when sending over the network. Then on the receiver side, the binary is converted into a JSON string, and then it's parsed into a javascript object. However with binary formats you're probably going to be working with a protobuf object, but any transmission converts that object into or from binary. 

Honestly, REST APIs are pretty common and have been proven to be pretty useful for most cases. For web, yeah a RESTful API is preferred, to expose a human-readable interface, use REST as those resource urls are pretty good. General stateless communication yes. 

## Credits
- [Beginner's Guide to Remote Procedure Call](https://mobterest.medium.com/demystifying-remote-procedure-calls-rpc-for-beginners-a-comprehensive-guide-7e639c92ea17)