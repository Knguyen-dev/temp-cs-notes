# Real Time Data: Polling, WebSockets, and SSEs.
Imagine you're building a stock trading dashboard. Users need to see live price updates as they happen, not just when they refresh the page. Or consider a chat application where messages should appear instantly without users having to manually check for new ones. These scenarios require real-time communication between server and client. 

The challenge is that traditional web architecture follows a request-response model: clients ask for data, servers respond, and that's it. But what happens when the server has new information to share? How do we push updates to clients who might not know they need to ask for them? This is where various real-time communication patterns come into play.. Each approach has different tradeoffs though.


## Polling
Polling is the simple approach of repeatedly querying the server for data. Akin to checking your mailbox every few minutes to see if new mail arrived.

### Short Polling
The client sends requests to the server at regular intervals and immediately receives a response (even if there's no new data). Typically you're frequently sending requests, so you're waiting a short interval before sending the next request.
```
Client → "Any new stock prices?" → Server
Client ← "Nope, same prices" ← Server
[Wait 5 seconds]
Client → "Any new stock prices?" → Server  
Client ← "Yes! AAPL is now $150" ← Server
[Wait 5 seconds]
Client → "Any new stock prices?" → Server
Client ← "Nope, same prices" ← Server
```
Though, short polling isn't efficient. It's really wasteful as there's most of the time you're using a lot of requests that aren't resulting in new data. Also you're increasing the server load with the amount of requests you're sending, and you can imagine how bad this can get if you're scaling this up to more users. In general, short polling is almost always a bad idea.

---
### Long Polling
The client sends a request, but the server holds onto it until there's actually new data to send back. If no data arrives within a timeout period, your server responds with "no data" and the client immediately sends another request
```
Client → "Any new stock prices?" → Server
[Server waits... waiting... waiting...]
[New price update arrives!]
Client ← "Yes! AAPL is now $150" ← Server
[Client immediately sends next request]
Client → "Any new stock prices?" → Server
[Server waits again...]
```
A lot closer to real time than short polling. However maintaining a lot of persistent connections will cost more server resources. The much more modern alternative for real time data is websockets! In general, this is really useful and can scale up well.

---
### Why Not Polling in some real-time data use-cases?
Remember that with polling, we're sending an http request every 5-10 seconds hoping that the server has an answer. This creates a lot of overhead considering that most of the http responses are empty responses (or the same data) as the server may not have the data immediately. Long polling is a little more efficient, doing one http request and keeping that connection until the server has an answer. After the server returns a response, the client will do the process over again, maybe waiting a couple of seconds before doing another long poll. However the long poll request can get timed out and maintaining open connections makes it resource intensive. The goal of polling is get as close to a real time connection as possible. The freshest updates and data as we can. 

## WebSockets 

### What are websockets
![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-III/long-polling-websockets-server-sent-events/websockets.png)
WebSockets are long-lived TCP connections that enable bi-directional, real-time communication between client and server. Officially, you're using the WebSocket protocol, as opposed to the HTTP protocol (for standard traffic). You'll see the prefixes `ws://` for unsecure websocket communication, and `wss://` for secure websocket communication as it's encrypted with TLS/SSL, just like their HTTP counterparts. You'll also hear the word **duplex** being applied with WebSockets, which just means two way data transmission since both client and server can both send and receive data.
1. Client sends request to the server (HTTP request with upgrade ), starting a websocket handshake.
2. Server sends response back, accepting the handshake.
3. WebSocket connection replaces the handshake.

Generally WebSockets are the preferred protocol for real-time data due to it not only having a real-time bi-directional communication, but also it's less overhead as well.

---
### How websockets work
1. Client sends an HTTP GET request to the server with the header information to indicate that it wants to establish a websocket connection.

```
GET /chat HTTP/1.1
Host: server.bytemonk.com
<!-- Indicates the client wants to upgrade the connection to use websocket protocol -->
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Key: <some-string>
Origin: http://bytemonk.com
Sec-WebSocket-Protocol: chat, superchat
Sec-WebSocket-Version: 13
```
  - **Upgrade:** Indicates the type of upgrade the client wants. Here the client wants to upgrade the connection to use the WebSocket protocol, so we want to use a websocket connection rather than a regular http connection.
  - **Connection:** This header indicates to the server that the client wants the upgrade.
  - **Sec-WebSocket-Key:** A base64 encoded string unique to this specific handshake. This is used to verify the websocket handshake later.
  - **Sec-WebSocket-Protocol:** Specifies any subprotocols.
  - **Sec-WebSocket-Version:** Specifies the specific version of the websockets protocol that they want to use.
2. Server sends a response to indicate that it accepts:
```
<!-- Indicates that it's good and we're switching to websockets -->
HTTP/1.1 101 Switching Protocols
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-accept: <some-other-string>
Sec-WebSocket-Protocol: chat
```
The server verifies the presence `Upgrade: websocket` and `Connection: upgrade`. Then we have to go through a process to make sure we're not being man in the middled:
  - Obtain `Sec-WebSocket-Key` and concatenate this with a fixed GUID. Now hash it with some cryptographic hash function. This creates our `Sec-WebSocket-Accept`.
  - The server will send the HTTP 101 response with this hash that was generated. The client will somehow validate this?
  - **NOTE:** Don't worry about the GUID, it's value is "258EAFA5-E914-47DA-95CA-C5AB0DC85B11", and is the value that all websocket implementations use. It was hardcoded into the WebSocket protocol, and basically it just prevents things like proxies or caches from accidentally responding to the WebSocket upgrade request.

---
### How data is represented in WebSockets:
Data is represented as a series of **frames**. Each frame as a specific structure, so let's go through it:

- **FIN:** If this bit is 1, it's the final fragment, else 0 means it's not the final fragment.
- **RSV1, 2, and 3:** Bits reserved for future features in websocket protocol.
- **opcode:** 4 bits indicating the type of data being sent.
- **mask:** Bit indicating whether the data is masked. The goal is masking is to ensure that data is smoothly sent through infrastructure that wasn't designed with WebSockets in mind. Masking is there to ensure other infrastructures and computers differentiate websocket data from normal HTTP traffic. Otherwise any network infrastructure equipment which is not yet upgraded to understand the WebSocket protocol can mistake it for normal http traffic, causing various issues. There's even security exploits that rely on this, so it's also a security issue as well.
- **payload length:** Length of the payload data.
- **Masking key:** Used to obscure payload data
- **Payload data:** The actual data being delivered.

Data for a large message is split up as fragments. So we have the gradual delivery of large amounts of data. 

## Server-Sent Events
![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-III/long-polling-websockets-server-sent-events/server-sent-events.png)
A way of establishing long-term communication betwee client and server that enables the server to proactively push data to the client. It's unidirectional, meaning the client sends one request, and can only receive responses from the server over the remainder of that connection. It can't make another request on that same connection.

### How SSE Works
Let's look at the workflow:
1. The client makes a request to the server.
2. The connection between client and server is established and it remains open.
3. The server sends responses or events to the client when new data is available.

### Considerations and Examples
It's simple to implement, and widely supported by browsers. However, that unidirectional nature can be limiting in situations where you want to respond to the server.



## Credits:
- [ByteMonk](https://www.youtube.com/watch?v=G0_e02DdH7I)
- [WebSockets API - MDN Docs](https://developer.mozilla.org/en-US/docs/Web/API/WebSockets_API)
- [Explaining short polling, long polling, and websockets](https://www.youtube.com/watch?v=ZBM28ZPlin8)