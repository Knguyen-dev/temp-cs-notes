# Event Loop

### App code vs event loop code
```Python
while running:
    want_read = [...]           # socket fds
    want_write = [...]          # socket fds
    can_read, can_write = wait_for_readiness(want_read, want_write) # blocks!
    for fd in can_read:
        data = read_nb(fd)      # non-blocking, only consume from the buffer
        handle_data(fd, data)   # application logic without IO
    for fd in can_write:
        data = pending_data(fd) # produced by the application
        n = write_nb(fd, data)  # non-blocking, only append to the buffer
        data_written(fd, n)     # n <= len(data), limited by the available 
```
In some libraries the event loop runs application code via callbacks, and the application code you write interacts with the event loop via a well defined API (e.g. putting in new tasks). But there's is definitely a separation.

### Storing state for each TCP Connection
A job can stay in the event loop for multiple iterations. We want to store state for that connection/job that indicates what type of operation we want to do (read, write, both?) and data such as what we're going to respond with. 
```C
struct Connection {
  int fd = -1;

  // Application's intentions for this job in the event loop, whether we're 
  // wanting to check if read or write is ready.
  bool want_read = false;
  bool want_write = false;
  bool want_close = false; // tells the event loop to destroy the connection

  // Buffers for IO; 
  std::vector<uint8_t> incoming; // incoming data to be processed by the app
  std::vector<uint8_t> outgoing; // outgoing data we're sending as responses
}
```

Previously, when reads were blocking, we could just call `read_full()` and wait until all `n` bytes were received. But with non-blocking reads, we can't wait, we can only read whatever data is available right now, and move on to the next connection. You're not guaranteed to read the full message in one go, so in the loop we will do:
1. Do a non-blocking read 
2. Append whatever bytes we get to the connection's `Conn::incoming` buffer.
3. Try parsing the buffer:
  - If we don't have enough data for the full message, skip processing for this iteration
  - If we do have enough data, process the message and clear the `Conn::incoming` buffer.

With blocking write,s we could send all our data in one go to `write_full` as it would wait until the OS was ready to accept everything. But now since writes are non-blocking, we can only send data when the socket is ready. This means:
- A single large message may take multiple event loop iterations to send all its data.
- We'll need to keep track of hte unsent portion of the message, of each connection.
- Since we're handling multiple connections concurrently, each connection needs its own `Conn::outgoing` buffer to store the remaining data that still needs to be sent.

### Checking the statuses before IO 
Do `poll()` to return a fd list, and then create a connection object for each one. Since an fd is just the smallest possible non-negative integer they can give pack, you could store these in a efficiently packed array, where fd is just the index. Then use a readiness API to pass in the list of fds we want to do IO on, and we'll be returned a list of fds ready for IO:
```C
// ndfs is just the size of the fds array
// timeout is set to -1 to indicate no timeout

int poll(struct pollfd *fds, nfds_t nfds, int timeout);
struct pollfd {
  // File descriptor of the resource to check
  int fd;

  // What we WANT to check (input to poll); This is almost an array, it's actually a bitmask, a sequence of bits representing flags.
  // We can combine multiple flags like pfd.events = POLLIN | POLLOUT, where POLLIN = 0x0001 and POLLOUT = 0x0004 (just an example), which would set both of 
  // those bits. As a result, pfd.events is a single number that encodes multiple readiness conditions.
  short events; 

  // What IS ready (output from poll function); Again a bitmask that indicates which if our things are ready.
  short revents;
}
```
Again, we use the readiness API to check the readiness of the socket to read or write. Think of it as quickly getting the status so that we're doing a read/write that makes us wait unnecessarily, this is called pollling. So the idea is:
1. We ask the OS: "For these file descriptors, tell me which ones are ready to read/write".
2. You pass in an array of `pollfd` structs to `poll()`.
3. The kernel fills out the `revents` field for each one, telling us what is ready



### Actual Code and Implementation
#### Main Event Loop code 
Here's the code for the event loop
```C
// Main with event loop
int main() {  
  // Setup and create the listening socket that will be used for accepting new connections.
  // ...

  /*
  - **fd_to_connection:** Array of connection objects representing TCP connection sockets. These are defined outside the event loop, which means that a given TCP connection may need multiple event loops to be "completed" (for the request to be fully read and response to be fully written). This makes sense because we may need to multiple loops to fully read their message, and of course multiple writes to do it as well. Each connection object of course has the file descriptor (mapped to the socket/open resource), but also booleans indicating our intention with the socket, whether we wnat to read, write, or even close the socket (due to error or if we're actually done processing it).
  
  - **poll_args:** Remember that each socket has a sender and receiver buffer managed by the OS. If the buffer is empty and we try to read from it, then linux will make our main thread wait until there is data. Again this is blocking IO, as whilst we're waiting for data, there could be other sockets that are already ready to go to read or write. This poll_args array is part of the solution, as it's here to detect which sockets are ready for reading or writing, allowing us to avoid that blocking aspect, where the OS makes us BLOCKS for the receive buffer to have data or the send buffer to not be full. 

  Of course, the other part of the solution is to make sure all of our sockets are marked as nonblocking. If the socket is marked as non-blocking and reading data from the socket would actually block the system, it'd raise an error (set errno) to EAGAIN or EWOULDBLOCK to warn us that the read would block our main thread. This is very helpful as it helps us avoid being blocked. However moving back to the poll objects, the first check we're doing is checking the poll::revents bitmasks to see if a socket is ready for reading or writing. You'll see later that in our handle_read() and handle_write() will have those errno=EAGAIN as the second check. I'm guessing that this is needed because those microseconds between the poll() and the handle_read() there could be some interruptions or things in the internal OS that could cause that previously approved non-blocking read, to temporarily become blocking. I guess we're just dealing with race conditions.

  The zeroth index of this will be the listening socket that checks whether we have new TCP connections that we can accept. Remember that TCP connections are stored in an OS-managed queue (for that particular listening socket). If we call accept() AND our listening socket is marked as BLOCKING, and the queue of pending is empty, it'll BLOCK the main thread and wait until there's a new connection. Obviously that's not good as some connection sockets could be ready, but we're still waiting on them. However, this is solved, we'll set the socket to non-blocking, so if there are no connections, we'll get an EAGAIN error. 

  Back to poll_args, the zeroth index contains a poll structure for tracking the status of the listening socket. Then all other poll objects will contain the statuses of the TCP connection sockets to see if they're ready for reading, writing, or whether the socket ran into an error. The idea with this is that on every iteration of the event loop, we're going to create new poll objects for all our sockets, and then monitor all of those sockets for their readiness states. This is the only time our code is BLOCKED, but it's blocked efficiently as there's nothing else we need to do. As soon as it finds a socket that's ready for IO, stop waiting. Lastly we have two things we want to do:
    1. Check the results of the poll object for the listening socket to see if it's ready for reading, meaning there's a TCP connection socket that we can accept. Then of course handling it.
    2. Check the results of the poll objects for each of the TCP connection sockets. If the poll indicates the socket is ready for IO, then perform IO on the socket.
  Once that's done the next loop begins. Note that on each loop we reset the poll_args array. This makes sense as you're starting off with a clean slate for the statuses, then track those statuses, etc. 

  
  - **The Event Loop:** Then start the event loop. Here's the basic overview of the event loop:
  While running:
    1. Clear poll args state. Then populate it with the poll objects for the listening socket and the connection sockets.
    2. Wait for readiness, monitor all of those sockets; EFFICIENT BLOCKING
    3. At this point, at least one of the sockets have been marked as ready for IO. So let's processing:
      1. Process the poll object for the listening socket in case its status indicates that it's able to accept a pending TCP connection from the backlog queue. 
      2. Process poll objects for the TCP connection sockets. If their statuses indicate that they're able to read or write, then do that reading or writing.
      3. For any poll objects that got an error when checking the IO statuses for a socket OR the connection object has been marked to be closed, then close/delete the connection. This involves not only using close() to close the socket associated with that fd, nullify it in the array of connections, and freeing the memory associated with the struct. 
  */
  std::vector<Connection *> fd_to_connection;
  std::vector<struct pollfd> poll_args;
  while (true) {

    // 1. Clears poll objects, and creates new ones
    poll_args.clear();
    struct pollfd poll_fd = {fd, POLLIN, 0};
    poll_args.push_back(poll_fd);
    for (Connection *conn: fd_to_connection) {
      if (!conn) {
        continue;
      }
      struct pollfd pfd = {conn->fd, POLLERR, 0};
      // Adds flags to the poll object in case the connection object intends ot.
      if (conn->want_read) {
        pfd.events |= POLLIN;
      }
      if (conn->want_write) {
        pfd.events |= POLLOUT;
      }
      poll_args.push_back(pfd);
    }

    // 2. wait for readiness; monitor all sockets; return immediately if IO is ready for at least one socket
    int rv = poll(poll_args.data(), (nfds_t)poll_args.size(), -1);
    if (rv < 0) {
      if (errno == EINTR) { // interrupted, just do retry
        continue
      }
      // Actual error during the poll()
      die("poll() failed")
    }

    // 3. ATP, at least one socket is ready for IO so it won't be blocked.
    // - First check the poll obj for the listening socket to see if it can accept a TCP connection.
    if (poll_args[0].revents) {
      if (Connection *conn = handle_accept(fd)) {
        if (fd_to_connection.size() <= (size_t)conn->fd) {
          fd_to_connection.resize(conn->fd + 1);
        }
        assert(!fd2conn[conn->fd]);
        fd_to_connection[conn->fd] = conn;
      }
    }
    // - Lastly handle the poll objs for the connection sockets to see if they can read or write
    for (size_t i = 1; i < poll_args.size(); ++i) {
      uint32_t ready = poll_args[i].revents;
      if (ready == 0) {
        continue;
      }
      Connection *conn = fd_to_connection[poll_args[i].fd];
      // Bitwise AND Evaluates to true when we have a non-zero value; 
      // Think of this as checking if the POLLIN bit is set in the ready bitmask.
      if (ready & POLLIN) {
        assert(conn->want_read);
        handle_read(conn); 
      }
      if (ready & POLLOUT) {
        assert(conn->want_write);
        handle_write(conn);
      }
      // Close the socket when it has an error or when our application 
      // has marked the connection for deletion.
      if ((ready & POLLERR) || conn->want_close) {
        (void)close(conn->fd);
        fd_to_connection[conn->fd] = NULL;
        delete conn;
      }
    }
  } // end of the event loop
  return 0;
}
```

#### Handling reads, writes, and trying a single request
There are other functions `handle_accept()`, `fd_set_nb()`, `handle_read()`, `handle_write()`, and `try_one_request()`, which are the application callbacks. Here's the code for those;
```C
/*
+ Application callback when listening socket is ready:
So what we mean by this is that this is the code we call when the listening socket 
is ready to be read. The idea is to create the TCP connection socket by extracting a pending TCP connection from the listening socket's backlog. The main idea here is to construct and return a Connection object that satisfies 2 conditions:

1. It contains the fd for the TCP connection socket, allowing us to refernce it later when doing read or write operations
2. We want the want_read flag to be set as true. This needs to be set initially because in order to process a request, we need to read data from that request. Notice that we leave want_write = false, because we don't want to write yet. Of course, this affects the flags on the poll obj, and what statuses we query for.

So the main takeaway is that these application callbacks are going to be directly working with these connection objects. Changing the values in the incoming and outgoing user defined buffers, changing the values on the booleans as well.
*/
static Connection *handle_accept(int listening_socket_fd) {
  struct sockaddr_in client_addr = {};
  socklen_t addrlen = sizeof(client_addr);
  int connfd = accept(listening_socket_fd, (struct sockaddr *)&client_addr, &addrlen);
  if (connfd < 0) {
    msg_errno("accept() error");
    return NULL;
  }
  uint32_t ip = client_addr.sin_addr.s_addr;
  
  // Doing some application logic of printing the client IP address.
  fprintf(stderr, "new client from %u.%u.%u.%u:%u\n",
      ip & 255, (ip >> 8) & 255, (ip >> 16) & 255, ip >> 24,
      ntohs(client_addr.sin_port)
  );
  fd_set_nb(connfd);
  Connection *conn = new Connection();
  conn->fd = connfd;
  conn->want_read = true;
  return conn;
}

/*
This code sets the socket in non-blocking mode. So reads and writes, whether blocking or non-blocking are the same sys calls. However if we set the socket to non-blocking, we actually affect the internal behavior of those syscalls.

If read-buffer isn't empty, both blocking/non-blcoking will return data. However when empty, reading on a blocking socket will make the socket wait for more data. However if you're reading with the non-blocking socket, the read call will raise an error if it detects that proceeding with the read call will cause blocking. This is very helpful as it prevent us from reading and blocking the MAIN thread. 

A similar idea can be applied when writing. If the the OS-sender buffer isn't full, both blocking/non-blocking writes will be able to copy data into the buffer. However if the buffer is full, calling write() with a blocking socket will wait until the buffer isn't full and has room for that data, blocking the main thread for a bit. On the other hand, calling write() with a non-blocking socket will simply raise an error if it detects that the write will block the main thread. As a result we prevent wasting time.
*/
static void fd_set_nb(int fd) {
  errno = 0;
  int flags = fcntl(fd, F_GETFL, 0);
  if (errno) {
    die("fcntl error");
    return;
  }
  flags |= O_NONBLOCK;
  errno = 0;
  (void)fcntl(fd, F_SETFL, flags);
  if (errno) {
    die("fcntl error");
  }
}

/**
 * Shift 32 to the left by 20 by 20 bits. This makes the thing 32 MB. You can easily calculate 
 * this by doing 32 * 2^{20}. 
 * 
 * When a socket is created the OS creates a socket receive and socket send buffer for incoming and outgoing data.
 * We've covered this, however the typical buffer sizes on many systems is about 64 KB to 512 KB, which is a lot less
 * than 32 MB. So we'll be storing data from that OS buffer into our user-defined buffer, and our user-defined buffer is like 
 * many times bigger.
 * 
 * The motivation behind making the max message size so big, is that we want the application to handle very large messages, even 
 * if the kernel only able to at most give us a part of the message at a time. So the receive buffer could be fill, we send it to our 
 * user-defined buffer, and maybe that has to happen a couple of times for the entire message to be received. It's a big indicator that 
 * our application is ready to handle partial reads and writes.
 */
const size_t k_max_msg = 32 << 20;

/*
+ try_one_request: Tries 1 request (tries to read one message, and send one response) if there's enough data. This function is used in `handle_read()` as a helper function. Anyways this function returns a boolean indicating whether this operation was successful.
*/
static bool try_one_request(Connection *conn) {
  /*
  Again all of the data here is inside the Connection buffer, so it's in our user-defined buffer. Attempt to read the message header for the size of the payload:
  - If we don't even have enough data for the message header, skip.
  
  - Validate that the size of the payload isn't too big for our protocol. If it is indeed too big, we're not going to be able to process the request, so we want to mark the connection to be closed at the end of the current iteration of the event loop. 
  
  - Knowing the full size of the message (header + payload), we'll do an early return when the full payload isn't there. So wait until the full payload is stored in the connection's incoming buffer before continue to process. This makes sense, as before 
  sending a response to a message, we want to get the full data. 
  */
  if (conn->incoming.size() < 4) {
    return false;
  }
  
  uint32_t len = 0;
  memcpy(&len, conn->incoming.data(), 4);
  if (len > k_max_msg) {
    msg("too long");
    conn->want_close = true;
    return false;
  }

  if (4+len > conn->incoming.size()) {
    return false;
  }
  const uint8_t *request = &conn->incoming[4];
  // got one request, do some application logic
  printf("client says: len:%d data:%.*s\n",
    len, len < 100 ? len : 100, request);
  
  /*
  We've parsed their request, now let's construct a response. For the response, we're 
  just going to echo their message back to them. Appending the message header and payload. Then return true to indicate that the operation was successful. 

  After doing this, we want to delete the message data from our incoming buffer. We don't want to close the connection after processing just one message, remember that there could be multiple messages/requests that the client can send through one TCP connection. Since we know that there could be other messages, the motivation for deleting the data from incoming buffer is to avoid re-reading this data from the incoming buffer that we've already seen.
  
  Q: Why not empty the entire incoming buffer? Why just just empty the message payload
  for this particular message? The answer to this is related to the idea of "pipelining".
  */
  buf_append(conn->outgoing, (const uint8_t *)&len, 4);
  buf_append(conn->outgoing, request, len);
  buf_consume(conn->incoming, 4+len);
  return true; 
}

/*
+ handle_write(): Application callback when the socket is writable.

1. Write data from our outgoing buffer to the socket, or the OS-managed send buffer.
2. If rv < 0, then there's an issue when writing. The error number EAGAIN happens when we pass in a non-blocking socket to our write. It just means that if the OS had continued with the write, it would have been a blocking write, so it returned an error to prevent us from doing that. However for all other errors, it would bea real error.

So after this, at this point we can assume that we wrote the data from our outgoing buffer to the socket. Anyways remove the sent bytes from the outgoing buffer and if we sent all of our data (buffer's size is 0), then we need to update our intentions for the TCP connection, which will take place on the next event loop iteration. Our connection has no data, so we want to make sure we read on the next event loop, and not write. 

However there's a chance that we did a partial write, meaning that we didn't write the 
entire outgoing buffer to the socket, and in that case, it's implied that the socket 
stays with its want_write = true to indicate that we still have data to we want to write to the socket.

Note: In that implied else case, I don't think want_read is set to false explicitly.
*/
static void handle_write(Connection *conn) {
  ssize_t rv = write(conn->fd, &conn->outgoing[0], conn->outgoing.size());
  if (rv < 0) {
    if (errno == EAGAIN) {
      return;
    }
    msg_errno("write() error");
    conn->want_close = true;
    return;
  }
  buf_consume(conn->outgoing, (size_t)rv);
  if (conn->outgoing.size() == 0) {
    conn->want_read = true;
    conn->want_write = false;
  }
}

/* 
+ application callback when the socket is readable (ready to be read):


*/
static void handle_read(Connection *conn) {
  /*
  Q: This buffer is kind of a random size? 64 KB?
  
  Here we're reading bytes from the socket (OS-managed incoming buffer) and we're 
  putting it in the buffer we define here. We're handling the cases where we 
  got an error:
    - EAGAIN: OS realizes that continuing to read from the buffer would be blocking so it stops us and raises this error. In this case the good course of action is to early return, and wait until the next iteration of the event loop to try to read again.
    - Or we could have gotten a real read error. In this case, we should mark the connection to be closed.

  We also handle the case when we the client closes the connection. rv = 0 indicates an EOF. We can reason that if our incoming buffer has 0 data, then the client is done talking. Else, we can reason that we had an unexpected EOF.
  */
  uint8_t buffer[64*1024];
  ssize_t rv = read(conn->fd, buffer, sizeof(buffer));
  if (rv < 0) {
    if (errno == EAGAIN) {
      return;
    }
    msg_errno("read() error");
    conn->want_close = true;
    return;
  }
  if (rv == 0) {
    if (conn->incoming.size() == 0) {
      msg("client closed")
    } else {
      msg("unexpected EOF");
    }
    conn->want_close = true;
    return;
  }

  /*
  Add data that we got from the buffer into our connection's incoming buffer. Then in the while loop, we want to try to do a single request-response cycle. What I mean is we're just processing one message and sending one response. The last thing that we want to do is update the connection's intentions.

  If we still have data in our outgoing buffer after writing, then we want to keep writing the data. We've already done one request, now we'll call handle_write to write more data. We'll set the want_write flag and unset the want_read flag to make sure that on the next iteration in the while loop (assuming the socket is ready for IO) that we continue writing. Of course the handle_write() could modify the flags again though.

  Q1: Why are we calling this in a loop? See the explanation of "pipelining"
  */
  buf_append(conn->incoming, buffer, (size_t)rv);
  
  while (try_one_request(conn)) {}
  if (conn->outgoing.size() > 0) {
    conn->want_read = false;
    conn->want_write = true;
    return handle_write(conn);
  }
}
```

### State transitions between request and response
In a request-response protocol the program is either reading a request or writing a response. At the end of `handle_read()` and `handle_write()`, we'll need to switch between the states. Of course this isn't always true, some proxies and messaging protocols aren't request-response, and can read/write simultaneously.

### TLDR: What we learned?
- Blocking IO wastes time waiting on a socket when another socket might already be ready for IO.
- Non-blocking IO (non-blocking sockets) + readiness APIs (poll) let's us efficiently do work. Monitor everything and act on sockets that are ready, read and write efficiency, if an operation will block, skip it and work on it in a future iteration.


- Callback-based programming: Reacting to events of readiness instead of blocking the thread.
- Handling input with non-blocking IO: Keep data in a buffer until there's enough to proceed and actually do stuff.
- The boundary between the event loop setup and application code. While the event loop is still non-trivial, separate the generic event handling and monitoring code, from the actual application code (our application-callbacks).
- Persist application between between loop iterations, we do this by having the array of connections outside the event loop. Also alternate between read and write for the request-response protocol.

## Credits
- [Event Loop Part 1](https://build-your-own.org/redis/06_event_loop_impl)