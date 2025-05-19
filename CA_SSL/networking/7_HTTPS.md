# Intro to HTTP and HTTPS

### Basic History of HTTP
A communication protocol, built ontop of TCP/IP, to standardize ohw clients and servers talk to each other. Let's quickly go through the history of it:

**HTTP/1.0 (1996)**
- Handles various response formats like images, videos, html, etc.
- Multiple methods suc has GET< pOST, and HEAD
- HTTP headers for requests and responses, stataus codes, caching, character sets
- However you can't have multiple requests per connection. If the client needs something from teh client, a new TCP connection will need to be created, and then closed after successful request. If you had to go on a webpage with 10 images, that's a total of 10 separate connections being done. That's really inefficient.

**HTTP/1.1 (1997)**
- Persistent connections are now possible. Instead of closing the connection immediately after the request is fulfilled, now connections are kept open which allowed for subsequent requests. Clients would also be able to close the connection after sending a request with header `Connection: close`.
- Chunked Transfer: Send data in chunks when the size is unknown, good for dynamic data.

The main issue is this idea of head-of-line blocking, where a slow request gets stuck in the pipeline/connection and therefore blocks the requests behind it. The previous request must be finished before the next one is started.

**HTTP/2 (2015)**
Focuses on low latency stuff:
- **Multiplexing:** Multiple async requests over a single connection. Now a heavy request isn't going to block others.
- **Binary Protocol:** Faster at parsing data.
- **Server push**, meaning we can send them extra data. Before if the client requested `index.html`, then that's all we would respond with. Now we can send `index.html` alongside `styles.css`. Server proactively sends resources.
-  **Header Compression (HPACK):** Makes the headers of requests and responses smaller. ata.

### Intro to HTTPS
#### Why is it important?
With regular HTTP, when you send stuff over, it's sent in plaintext. So if a someone were to intercept that http request, they would be able to view the data in it. This gets really bad when you send over critical information like passwords or critical card info. With HTTPS, that data is encrypted via TLS. So if encrypted data is intercepted, the hacker can only see jumbled data. So SSL/TLS does two main things: 
  1. Encrypt traffic between client and server
  2. A form of authentication to show the server is who they say they are; preventing a malicious from masquerading as the server in the case of a man in the middle attack.

---
#### What is SSL (Secure Sockets Layer), TLS (Transport Layer Security), and "SSL/TLS"?
SSL, the Secure Sockets Layer, is an internet security protocol. It is the predecessor to the more modern version TLS, so when you see "SSL/TLS" just know people are trying to use it interchangeably, as their core idea remains the same: A site that implements SSL/TLS uses HTTPS instead of HTTP, meaning communication between client and server is encrypted. Officially though, the last version of SSL (SSL 3.0) was updated in 1996, and it's no longer in use my most modern web browsers due to its many security vulnerabilities. Now they just use the more modern TLS, but again people use the name interchangeably nowadays

---
#### How does it work? (TLS Handshake)
1. Client and server do a TCP handshake first.  
2. Client sends a "Hello" to the server, which contains the following info:
  - TLS versions the client can support. 
  - The cipher suites the client supports. This is just a set of encryption algorithms.
3. Server chooses the TLS version and cipher suite, then sends back a "hello" alongside a **SSL/TLS certificate**. This certificate proves the server's identity, but we'll talk about that later. The certificate includes the public key of the server. As a result the client can encrypt data using that key, send it over, and the server can decrypt using its own private key.
4. The client generates a "session key", which is just a **shared encryption key (symmetric encryption)**. The client will encrypt this session key using the server's public key, and then send it to the server to decrypt. Now they both have these symmetric keys and can now encrypt and decrypt the traffic they send to each other. There we go, encrypted and secure communication. 

Nowadays they may use more optimized methods where there are less interactions, or is more secure. But the core idea and motivation is still the same


### Intro to Certificate Authorities
---
#### Vulnerability with HTTPS: Man in the middle attacks
The asymmetric encryption is good, but there's one flaw. When the client try to connect to the server, their request can be intercepted by a malicious actor. As a result, your client may think it's doing a TCP handshake with the server and setting up TLS, but it's actually connected to the hacker. Now the malicious actor will send over their own public key. Your client thinks this public key is from the server, and then encrypts sensitive data with this key and sends it back. 

The hacker will intercept it again, and use their own private key to encrypt the data, effectively stealing your info. The way to stop this is to ensure that we're connecting with the actual server, rather than being intercepted by some unchecked malicious actor. So we need to be able to encrypt traffic, but also be able to verify that we're talking to trusted websites.

---
#### What are Certificate Authorities (CA):
A third party organization trusted by the browser that issues these  **SSL/TLS certificates** to websites, businesses, and individuals. These certificates are then used to verify the identity of the site that they're issued to. CAs are responsible for this verification process. Certificate authorities have their own public and private keys as well.

1. Client sends request to server.
2. Server sends its public key to the CA. The CA creates the certificate, which is essentially just a text file that contains info such as:
  - The site that the certificate is issued to and the CA that issued it.
  - Server's public key that was sent to the CA.
  - **Encrypted server's public key (signature):** The CA uses it's own private key to encrypt the server's public key. To decrypt this, you'll need the CA's public key.
3. CA sends the certificate back to server.
4. Server sends certificate to the client.
5. The client is now looking at the cert., mainly the encrypted server's public key. To ensure that this is actually the server's public key that's been encrypted, we'll get the help of the CA:
  - Request the CA's public key. 
  - Use the CA's public key to decrypt the "encrypted server's public key".
  - If the decryption result matches the server's public key, then we can reason that the server's public key was indeed encrypted by the CA's private key. And in order to be encrypted by the CA's private key, we had to have gone through the certification process.

---
#### SSL/TLS Certificates
Akin to an ID card that proves someone is who they say they are. They are issued by **certificate authorities** (a trusted third party), and it shows important info for verifying the owner of a website and also it helps with setting up data encryption between the client and server. 

---
#### CAs and Chain of Authority
Extra little tidbit, but a lot of the time, the client/server isn't directly asking for a public key from one of those 8 or so main CAs. Rather than requesting from CyberTrust (Root CA), a website is going to reach out to Cloudflare to ask for information. Then Cloudflare reaches out to the Root CA. This is very useful:

If the root CA is somehow hacked or the private key is somehow leaked, then everything is over. But if an intermediate CA has a problem, then it's less serious since intermediates only affect issuing certs created by the root. If something went wrong with issuing, that's not as bad as something going wrong with creating a certificate, which could affect everyone. Also it creates a barrier against cyber-attacks. You can't just attack a Root CA, you'll need to go through an intermediate site.  Also it's more scalable.

---
## Credits: 
1. [HTTP 1 vs HTTP 2 vs HTTP3](https://youtu.be/UMwQjFzTQXw?si=gO8p3mEi5ghLGIfZ)
2. [SSL, TLS, HTTPS explained - Byte Byte Go](https://youtu.be/j9QmMEWmcfo?si=VtxNf2CshfK6_I0G)
3. [SSL, TLS, and CA tutorial](https://www.youtube.com/watch?v=EnY6fSng3Ew)
4. [SSL/TLS Explained - CloudFlare](https://www.cloudflare.com/learning/ssl/what-is-ssl/)