# Event Loop Part 2: More Advanced

### Introducing Pipelining
A typical request-response application can handle 20k-200k requests per second for simple operations (e.g. Redis GET, SET, DEL). That's basically the limit for the number of IO events a single thread can handle, assuming the application code isn't that complex.

- **Non-pipelined:** The client sends one request, and waits for the response, then repeats.
- **Pipelined:** A client sends multiple requests in a row, then waits for all responses together.

The benefit of using a pipelined model is that now we're able to use less IO operations to process more requests and write more responses. Remember in a non-pipelined model, we read a single request per read event, we're assuming that the client is sending at most 1 request at a time in the TCP connection. However, in our pipelined model, we have the potential of reading multiple requests within one read operation. 

Think of it like this: Without pipelining, we were doing things one request at a time. But with pipelining, we're actually treating our TCP connection as an actual stream of bytes and information. Don't worry, as the server will still handle requests and send responses in order.

```C
/*
+ handle_read() without pipelining: 
After it processes/reads 1 requests worth of data, it sets flags for writing. Then we're passing it onto handle_write() which will write one response, and then set it back to read. This is great, but the big assumption we're making is that the client is sending us at most one request. What if the client sent multiple requests, and maybe we can processes multiple messages whilst we're in this state of non-blocking for this read operation? To emphasis, the idea with pipelining is to be able to collect multiple requests/messages within one read operation, not just one!

Alright, now that we've seen the small inefficiency here, let's update this to use pipelining!
*/
static void handle_read(Connection *conn) {
  // ...
  try_one_request(conn);            // Read one request; assumes at most one requests worth of data
  if (conn->outgoing.size() > 0) { // if we got data; so write 
    conn->want_read = false;
    conn->want_write = true;
  }
}
static void handle_write(Connection *conn) {
  // ...
  if (conn->outgoing.size() == 0) {     // One response written; go back to reading
    conn->want_read = true;
    conn->want_write = false;
  }
}

/*
+ handle_read() and setup with pipelining:
Here we use a while loop, which just indicates, whilst IO is non-blocking, we'll try to read as many requests as we can and store as much output in our outgoing buffer. 
*/
static void handle_read(Connection *conn) {
  // ...
  // try_one_request(conn);         // wrong
  while (try_one_request(conn)) {}  // correct, keep consuming until there's nothing left
  // ...
}

/*
When consuming input, only remove the bytes for the message that we just processed using buf_consume(). You don't want to do conn->incoming.clear() because that clears everything. This is ideal for partial read and pipelined messages. So we're only clearing the current header and payload, and leaving the rest attached.
*/
static bool try_one_request(Connection *conn) {
  // ...
  // conn->incoming.clear();
  buf_consume(conn->incoming, 4+len);
  return true;
}
```

---
### Debugging and testing 
- Test the case where we pipeline requests, so sending multiple strings.
- Test when we have a really large message that's going to take multiple iterations to process.
- Use strace to overify we're testing correctly `strace ./server >/dev/null`

---
### Optimistic non-blocking writes
In the normal request-response protocol:
- Client sends request
- It waits for a response before sending the next request
- So when the server receives a request, it can assume "THe client must have already received (and thus read) my last response.". 

This implies that the socket is likely writeable, meaning the client's. However we can't assume this with a pipelined client:
- Client sends multiple requests in a row without waiting for the previous response.
- The server may receive multiple requests before the client has read any responses. This breaks the assumption that "receiving a request means the client has read the previous response". So if you try to write a response, your TCP send-buffer could be full since the client hasn't started reading yet. Remember that in TCP, we send data over, they send an acknowledgement back, and so we can delete that data from our OS-managed send-buffer to free up space and allow new data to be written.

```C

// Non-pipelined version
static void handle_read(Connection *conn) {
  // ...
  if (conn->outgoing.size() > 0) {
    conn->want_read = false;
    conn->want_write = true;
    return handle_write(conn); // The socket is likely ready to write the next request
  }
}

// Here's the adjust you need to make for a pipelined version
static void handle_write(Connection *conn) {
  ssize_t rv = write(conn->fd, &conn->outgoing[0], conn->outgoing.size());
  if (rv < 0 && errno == EAGAIN) {
    return; // We aren't ready to send.
  }
  if (rv < 0) {
    conn->want_close = true; 
    return; // Actual error
  }
  // ...
}
```


## Additional Exercise: Better buffer handling
Currently with our buffer handling, we append to the back and remove from the front. We're using it like a FIFO data structure. We have a dynamic array meaning it resizes itself. 
- Random access: $O(1)$
- Insertion or removal from the end: Amortized constant $O(1)$. 
- Insertion at the beginning or middle: Typically $O(n)$. Imagine you already have an array of data. If you're goint to insert a new element at the beginning or middle, you'd need to shift all elements to the right, and that's about $n$ elements. 



That's the common thing with an array, so a better thing would be having a buffer that's efficient at both ends. The idea being removing from the front is just advancing a pointer:
      ┌────────────┬────────────┬────────────┐
      │   unused   │    data    │   unused   │
      └────────────┴────────────┴────────────┘
      ⇧            ⇧            ⇧            ⇧
buffer_begin   data_begin   data_end   buffer_end

In our new data structure here's how it works:
- To delete `n` bytes from the buffer, just increase our pointer by n.
- To add `n` bytes to the back of the buffer. We can either reallocate to the like a dynamic array or move the rest of the data to the front to make room.
```C
struct Buffer {
  uint8_t *buffer_begin;
  uint8_t *buffer_end;
  uint8_t *data_begin;
  uint8_t *data_end;
}

static void buf_append(struct Buffer *buf, uint8_t bytes, size_t n) {
  // ... 
}

static void buf_consume(struct Buffer *buf, size_t n) {
  buf->data_begin += n;
}
```


## TLDR Takeway
With pipelining, we can read multiple with one read operation. However this is out of the ordinary for the request-response protocol, so we would adjust a bit. 


## Credits 
1. [Event Loop Part 2](https://build-your-own.org/redis/06b_event_loop_more)