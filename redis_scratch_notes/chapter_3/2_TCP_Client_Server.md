# TCP Client and Server 


### Creating a TCP Server
```Pseudocode
fd = socket()
bind(fd, address)
listen(fd)
while True:
  conn_fd = accept(fd)
  do_something_with(conn_fd)
  close(conn_fd)
```

#### Step 1: Get socket handle
```C
int fd = socket(AF_INET, SOCK_STREAM, 0);
```
- `AF_INET` is for IPv4 and `AF_INET6` is for IPv6.
- `SOCK_STREAM` is for TCP and `SOCK_DGRAM` is for UDP.
- 3rd argument is 0 and useless in this case. 
This just determines the socket protocol. Doing `man.socket2` gives you all the flags you can use. However `man ip.7` tells you how to to make TCP/UDP sockets with the socket libraries. Remember the socket has not been created yet, we're just preparing everything.

#### Step 2: Set Socket Options.
```C
int val = 1;
setsockopt(fd, SOL_SOCKET, SO_REUSEADOR, &val, sizeof(val))
```
There are many options that allow us to change the behavior of sockets. The second and third parameters specify what options we're targeting. The `val` is the value of the options we want to select. Then we need `sizeof` so that their functions don't break and overflow. 

- **SO_REUSEADOR (boolean):** Requires some TCP, but essentially we need to set this to 1 so that our server can bind to the same `<IP>:<port>` after a restart. This should be enabled for all listening sockets even if you don't quite get it.

#### Step 3: Bind the socket to an address
```C

// "Internet address"
struct in_addr {
  uint32_t s_addr; // IPv4 in big-endian 32 bits (4 bytes), that makes sense.
};


// This struct represents the address that a socket will bind to. Since we have <IPv4>:<PORT>
struct sockaddr_in {
  uint16_t sin_family; // AF_INET; Type of IP address v4 or v6
  uint16_t sin_port;   // port in big-endian 
  struct in_addr sin_addr;  // HOST IP; "socket internet address"
};

struct sockaddr_in addr = {};

// 1. Set it to IPv4 like planed
// 2. The we need our port in big-endian; Port 1234
// 3. Host IP 0.0.0.0; this is a special IP address, as it's a wildcard, which means listen on all interfaces
// Regardless of the IP address the host has, accept incoming connections. The reason this is even used is becasue server apps like
// HTTP servers, redis, usually use 0.0.0.0. It's very connect so clients can connect through LAN, internet, localhost.
addr.sin_family = AF_INET; 
addr.sin_port = htons(1234); 
addr.sin_addr.s_addr = htonl(0);

// Binding; preparing parameters; 
// Reminder in C: We're casting. So addr is a struct sockaddr_in and we're casting it to a pointer 
// that points to const struct sockaddr; So the pointer can point to something else but the data it points to is read-only. Basically we won't mess with your struct.
// NOTE: rv stands for return value, if it's 0 then we successfully associated socket fd with the address addr.. Else < 0 then it failed.
int rv = bind(fd, (const struct *)&addr, sizeof(addr));

if (rv) { 
  die("bind()"); 
};
```

Here's also the same setup but we're using IPv6. So we're setting up the code for an IP v6 listening socket
```C
// Struct representing IPv6 address; 
// We have an array of length 16, each element containing a byte. 
// 16 bytes is 128 bits, checks out.
struct in6_addr {
  uint8_t s6_addr[16];
}

struct sockaddr_in6 {
  uint16_t sin6_family;      // Type of IP address, should be AF_INET6
  uint16_t sin6_port;        // Port we're binding the socket on
  uint32_t sin6_flowinfo;    // ignore
  struct in6_addr sin6_addr; // IPv6 
  uint32_t sin6_scope_id;    // ignore
}

struct sockaddr_in6 addr = {}
addr.sin_family = AF_INET6; 
addr.sin_port = htons(1234); 
addr.sin_addr.s_addr = htonl(0); // you could also use in6addr_any apparently? 

addr.sin6_flowinfo = 0;
add.sin6_scope_id = 0

// 1. Obviously sockaddr_in6 has a different size than our sockaddr_in, so we have to pass in the size of the struct
// 2. We cast the sockaddr_in6 to a generic sockaddr pointer because that's what bind expects.
int rv = bind(fd, (struct sockaddr *)&addr, sizeof(addrlen))
if (rv < 0) {
  // handle error
}
```

#### Step 4: Listening and Accepting
Remember that the previous steps were just passing parameters, then we actually create the socket when calling `listen()`. 

- OS handles messy TCP stuff: The handshake (SYN, SYN-ACK, and ACK), reordering, re-transmission, and buffering. It creates the TCP connection and pushes the reference into a **back-log queue** of completed connections. As well as this for each established connection socket, there's a **receive buffer** (managed by the OS) that holds incoming TCP data.
- If your program is accepting connections fast enough, this queue should never fill up, so the exact backlog size isn't that improtnat. however if the server is slow to call `accept()`, or gets flooded with too many connections, the queue can fill up and incoming connections may get lost.
- What accept actually does. `accept()` merely pulls the next completed TCP handshake connection from the queue and creates a new socket file descriptor to interact with it. 
```C
// Listening:
// Create socket with handle fd (our magic ticket); Second arg is the size of the queue, and SOMAXCONN=4096 on linux. 
// Apparently this argument doesn't matter because "accept()" is not a bottleneck? What do you mean "bottleneck" and why does that eliminate the need for SOMAXCONN?
// - Puts our socket in listening mode, where SOMAXCONN is the maximum number of pending connections that the queue can have
rv = listen(fd, SOMAXCONN);
if (rv) {
  die("listen() failed");
}

// Accepting: 
// The server would enter a loop (kind of like a game loop) that accepts incoming client connections and processes them by doing something.
while (true) {
  struct sockaddr_in client_addr = {};
  socklen_t addrlen = sizeof(client_addr);
  
  // Yeah the OS itself actually handles doing the TCP handshake it seems. We just have a continuous 
  // event listener that sees if client connections sockets are actually being created.
  // - Extracts the first connection in the queue of pending connections, and this creates a connection socket; also creating a fd for that connection socket
  // - So this is like the server 
  int connfd = accept(fd, (struct sockaddr*)&client_addr, &addrlen);
  if (connfd < 0) {
    continue; // Skip processing if we couldn't create the TCP connection socket
  }

  // Pass the connection socket to process it
  do_something(connfd);
  close(connfd);
}
```
Now let's define the `do_something()`. This allows us ot talk about reading and writing. Remember `read/write` and `send/recv` being equivalent? This is that:
```C
static void do_something(int connfd) {
  char read_buffer[64] = {};

  // Buffer only has room for 63 characters since last one is always reserved for null termination character. A byte per character
  // Just like the OS handles a queue of TCP connection sockets, the OS is likely also handling the data packets sent in those connection
  // sockets, and we'll read that data here. Once a TCP connection is established, any data sent by the client is stored in an OS buffer 
  // and your program reads from that buffer. Our program should see a clean byte stream.

  ssize_t n = read(connfd, , sizeof(read_buffer) - 1);

  // Is this the status of the read?
  // n is the number of bytes we'll need to read. 
  // - If it's negative then that's an error
  // - If 0, the close has closed the connection

  if (n < 0) {
    msg("do_something: read() error");
    return;
  }
  printf("Client Says: %s\n", read_buffer);

  // - Sending data to the client
  // - Right now we have ignored the return value of write; i'm you could also do error handling later
  char write_buffer[] = "world";
  write(connfd, write_buffer, strlen(write_buffer)); 
}
``` 

### Creating a TCP Client
```C
// Attempt to create fd for a socket; OS now remembers we're going to create a IPv4 TCP socket later
int fd = socket(AF_INET, SOCK_STREAM, 0);
if (fd < 0) {
  die("socket()");
}

// Setup the <IPv4>:<Port> combination that the client is trying to connect to. So this is the server's IP.
struct sockaddr_in addr = {};
add.sin_family = AF_INET; 


// Here we define the server IP address and port in little endian. 
// But why? When defining and working with sockets and networks, we should define stuff in big endian?
// ATTENTION: I think it's becasue since you're running it locally, both client and server are the same 
// computer, so both are in little endian. Technically no need to convert to big-endian since both sides 
// interpret raw bytes in the same way. Of course I don't think this will work if you actually send this 
// over a real network.
addr.sin_port = ntohs(1234); // Port 1234
add.sin_addr.s_addr = ntohl(INADDR_LOOPBACK) // 127.0.0.1

// Attempt to connect to the server IP and port.
// Attempt to create the TCP connection socket with our fd and socket address we defined
// NOTE: if -1, connecting to the server failed.
int rv = connect(fd, (const struct sockaddr*) &addr, sizeof(addr));
if (rv) {
  die("connect");
}

// Write data to our socket
char msg[] = "hello";
write(fd, msg, strlen(msg));
char read_buffer[64] = {};
ssize_t n = read(fd, read_buffer, sizeof(read_buffer) - 1);
if (n < 0) {
  die("read() failed");
}
printf("Server says: %s\n", read_buffer);
close(fd);
```
- `INADDR_LOOPBACK`: Equals `0x7f000001` which represents the `127.0.0.1` IP address.


### Extra Stuff

#### More on SocketAPI
```C
struct sockaddr {
  unsigned short sa_family; // AF_INET or AF_INET6
  char sa_data[14];         // ignore
};

// IPv4:port
// IPv4:port
struct sockaddr_in {
    sa_family_t     sin_family;     // AF_INET
    uint16_t        sin_port;       // port number, big-endian
    struct in_addr  sin_addr;       // IPv4 address
};

// IPv6:port
struct sockaddr_in6 {
    sa_family_t     sin6_family;    // AF_INET6
    uint16_t        sin6_port;      // port number, big-endian
    uint32_t        sin6_flowinfo;
    struct in6_addr sin6_addr;      // IPv6 address
    uint32_t        sin6_scope_id;
};


// Can store both sockaddr_in or sockaddr_in6
struct sockaddr_storage {
  sa_family_t ss_family; // Either AF_INET, or AF_INET6
  char __some_padding[__BIG_ENOUGH_NUMBER];
}
```
The SocketAPI defines a lot of useless types:
- `struct sockaddr` is never used, we always just cast one of the concrete structs as it.
- `struct sockaddr_storage` can just be replaced with a union of the two concrete types.
- Structs like `sin_addr` or `sin6_addr` don't need to be structs, they're just fields.
- The `*_family` structs are just 16-bit integers, they don't need a type.

Ideally, the API should use this:
```C
struct fictional_sane_sockaddr {
  uint16_t family; // AF_INET, AF_INET6
  uint16_t port; 
  union {
    struct { uint8_t ipv4[4]; }
    struct { uint8_t ipv6[16]; }
  }
}
// Warning: Of course you can't actually use this or cast it to struct sockaddr*
```

#### Syscalls, APIs, libraries
Each socket function is a syscall wrapper in libc. The socket API is called BSD socket and is supported by all major sockets. The windows API is mostly the same, though has minor differences.

You can use `bind()` before `connect()` to specify the source IP address. Without this the OS will automatically select one. Binding is useful if you have multiple IP addresses you want to use. Also if the port you pass to `bind()` is zero, the OS will automatically pick a port.

If you're using wildcard IP address or letting the OS pick the port, you don't really know the exact address your computer is listening on. Here's some help:
```C

// Get the local address; IP:port of host machine
int getsockname(int fd, struct sockaddr *addr, socklen_t *addrlen);

// Get's the remote address; IP and port of the client making the request.
int getsockname(int fd, struct sockaddr *addr, socklen_t *addrlen);
```

#### DNS Stuff
`getaddrinfo()` resolves a domain name inot a IP address.

#### Sockets and Inter-process communication (IPC)
We already know processes on the same machine can communicate with each other via Unix domain sockets, pipes, etc. 

Unix domain sockets have the same API as network sockets. You can create a packet-based or byte-stream based socket. However a Unix domain socket is created with different flags when you use `socket()` and it uses `struct sockaddr_un`, but the rest is the same. You can read more on `man unix.7`.

A pipe is a one-way byte stream, so you'll need a protocol like TCP socket to work with it.


## Comments
- [Endian-ness explained](https://www.freecodecamp.org/news/what-is-endianness-big-endian-vs-little-endian/)
- I mean Phillip Moore did say this was pretty hard. I guess if I ever complete it I'd want to send it to him. It could be proof that I actually understand things that are happening here.