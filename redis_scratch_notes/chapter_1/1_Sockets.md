## Chapter 1: Intro
Redis is a popular in-memory key-value store that we use mainly for caching. It’s literally a map<string, any value>. We’ll follow this book to build it with C and some C++. This requires us to implement the data structures and network programming. 

## Chapter 2: Socket Programming

### TCP Byte Stream
TCP sends over a continuous stream of bytes with “no internal boundaries”. An Application Protocol will process/make sense of this byte stream, and split it into Messages. 

### Data Serialization
We send over strings, struct, and lists, however computer networks can only send over binary. So we want to convert these high level objects into binary and this process is called serialization. We’ll do this by manipulating bits.

### Concurrent Programming
Server-side is complicated since we’re dealing with multiple connections concurrently. Even if most are idle, it’s still hard. This is called the C10k problem. Software needs to be able to fully make use of modern hardware. Nowadays the solution is event-based concurrency with event loops, which is the core of scalable software like NGINX, Redis, etc. We’ll learn by doing

### Layers of Protocols
Network protocols are divided into layers, with a lower layer containing a higher layer. The physical contains IP (network), IP contains transport, etc.

IP: When downloading a large file, it’s split into IP packets. Then a higher layer, TCP, reassembles things.

Port number (multiplexing): Many apps live within a computer. Each process/agent claims an unused port before it’s able to send or receive data, and the computer uses a 4-tuple to identify how info flows (src_ip, src_port, dst_ip, dst_port).

TCP (ordered and reliable bytes): TCP reassembles and orders those packets. It goes application -> transport(TCP/IP) -> IP layer -> Data link. TCP is kind of on a higher level since it does more, whilst UDP is just “IP + port number”.

In reality, we only care about src and dst IP addresses. We can ignore data link layer, as we only care about the TCP and UDP since apps make their own or use an established means.

### Packet vs Stream
TCP gives a byte stream but most apps expect a clear cut message. You’ll add a message layer to TCP OR add reliability and order to the UDP. The former is a lot easier so most apps use TCP  with a well known protocol on top of it or by making their own. TCP and UDP are insanely different, so that’s the first thing to decide when working in networked apps.

### What is a socket?
A socket is just a ***handle*** to refer to a connection, or something else. A handle is an integer that refers to things that cross an API boundary. 

- **Opaque integer:** Integer that has no meaning to the programmer, only has meaning to the system or API. It’s like a ticket number, you don’t know what it actually means or what it represents, you can’t 
change it either. But if you give it to the OS, it can read from it. It’s opaque, the meaning is internal, but we can use it to do run some API functions. It's like an access token in that sense.
- **API Boundary:** The point where your code talks to something external, like the OS, a library, or another web service. If you say “I want to open this file”, the OS replies “Here’s handle 42”. You’re 
using something else’s services (the API) from your program. The handle is your receipt/reference to see what the OS is managing on your behalf. It doesn’t matter how the OS manages that file internally, you just care about being able to get your data back
- **File descriptors:** It is a meaningless name. It could represent a socket, pipe, terminal, or other resource (doesn’t have to be a file). The is a handle to some system resource.

`socket()` allocates memory for a socket and returns a socket fd (the handle), and you later use this to create connections. Just close the handle when you're done with it to free the resources on the OS side. 

### Listening and connecting
Listening means telling the OS that an app is ready to accept TCP connections form a given prot. The OS will return a socket handle for apps to refer to that port. Apps can retrieve (accept) incoming TCP connections, which is also represented as a socket handle. So there are two types of socket handles: listening sockets and connection sockets.

Creating a listening socket:
1. Get socket handle via `socket()`
2. Set the socket to an `<IP_Addr>:<Port>` using `bind()`
3. Create the listening socket `listen()`

How a client creates a connection:
1. Obtain a socket handle `socket()`
2. Create a connection socket with `connect()`.

**Note:** `socket()` creates a typeless socket, and the type is defined when you do `listen()` or `connect()`.

### How Reading and Writing Works
TCP and UDP share the same socket API. For message-based sockets (UDP), each send/recv corresponds to a single packet. However byte-stream based sockets (TCP), each send/recv adds to or takes from the byte stream. On Linux send/recv are variants of the more generic `read/write` syscalls which are used for sockets, pipes, etc. 

- **syscall:** Low-level functions that allow your program to interact with the operating system. If you're using a syscall, you're doing something that basically interacts with the hardware programmatically. Doing this without the help of the OS is dangerous, so use syscalls. These are the lowest functions can get.


### Last Resources
If you get really stuck. You'll need to get some help. So here's a good book that contains probably all you need. 
- [Beej's book on network programming](https://beej.us/guide/bgnet/)

You may also want to use the linux man pages that give you the docs for commands, system calls etc.

```bash
# Shows the manual for the ls command
man ls

# 
man [section][topic]

# Shows the man page for the socket() syscall. 
man socket.2

# shows an overview
man socket.7 

```