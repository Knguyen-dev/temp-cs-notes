# WebSockets

### Premise and Motivation
When we do an http request, we send a request to the server, the server agrees and sends us the data, and then finally the connection is closed. So if you want more data you'd have to open up a connection again. Imagine an online game where the top score was updated in 'real-time'. How would we broadcast the most updated information? The user could refresh their pages frequently, we could use a timer to update the page. The solution is WebSockets!

---
### Why not polling?
Remember that with polling, we're sending an http request every 5-10 seconds hoping that the server has an answer. This creates a lot of overhead considering that most of hte http responses are empty responses as the server may not have the data immediately. Long polling is a little more efficient, doing one http request and keeping that connection until the server has an answer. After the server returns a response, the client will do the process over again, maybe waiting a couple of seconds before doing another long poll. However the long poll request can get timed out so it's still resource intensive. The goal of polling is get as close to a real time connection as possible. The freshest updates and data as we can. 

---
### What are websockets
WebSockets are long-lived TCP connections that enable bi-directional, real-time communication between client and server. Officially, you're using the WebSocket protocol, as opposed to the HTTP protocol (for standard traffic). You'll see the prefixes `ws://` for unsecure websocket communication, and `wss://` for secure websocket communication as it's encrypted with TLS/SSL, just like their HTTP counterparts. You'll also hear the word **duplex** being applied with WebSockets, which just means two way data transmission since both client and server can both send and receive data.


1. Client sends request to the server (HTTP request with upgrade ), starting a websocket handshake.
2. Server sends response back, accepting the handshake.
3. WebSocket connection replaces the handshake.
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
### How data is represented in WebSockets:
Data is represented as a series of **frames**. Each frame as a specific structure, so let's go throught it:

- **FIN:** If this bit is 1, it's the final fragment, else 0 means it's not the final fragment.
- **RSV1, 2, and 3:** Bits reserved for future features in websocket protocol.
- **opcode:** 4 bits indicating the type of data being sent.
- **mask:** Bit indicating whether the data is masked. The goal is masking is to ensure that data is smoothly sent through infrastructure that wasn't designed with WebSockets in mind. Masking is there to ensure other infrastructures and computers differentiate websocket data from normal HTTP traffic. Otherwise any network infrastructure equipment which is not yet upgraded to understand the WebSocket protocol can mistake it for normal http traffic, causing various issues. There's even security exploits that rely on this, so it's also a security issue as well.
- **payload length:** Length of the payload data.
- **Masking key:** Used to obscure payload data
- **Payload data:** The actual data being delivered.

Data for a large message is split up as fragments. So we have the gradual delivery of large amounts of data. 
### Takeaway
Enables real time communication in web apps, chatting, real time updates for stock values, etc.
## Credits:
- [ByteMonk](https://www.youtube.com/watch?v=G0_e02DdH7I)
- [WebSockets API - MDN Docs](https://developer.mozilla.org/en-US/docs/Web/API/WebSockets_API)
