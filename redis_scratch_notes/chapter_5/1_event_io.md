# Concurrent IO Models


### Multithreading, and why it won't be enough
So far we're only able to manage one TCP connection at a time, but at least we're able to process multiple requests or messages within that TCP connection. Now we just need to be able to handle multiple connections simultaneously, which can we done with multithreading:
```Python
fd = socket()
bind(fd, address)
listen(fd)
while True:
  conn_fd = accept(fd)

  # Process client asynchronously within a subthread; prevents us from blocking the main thread.
  new_thread(do_something_with, conn_fd)
  
  #  Continue to accept the next clienting without blocking the main thread

def do_something(conn_fd):
  while not_quitting(conn_fd):
    req = read_request(conn_fd) # blocks the thread; so blocks the given subthread
    res = process(req)
    write_response(conn_fd, res) 
  close(conn_fd)
```
Most modern server applications use ***event loops*** to handle concurrent IO without creating new threads. Multithreading just has a lot of overhead since your threads manage their own stacks, and that's a lot of memory usage. The overhead creates more latency, and trying to scale this up wouldn't be good. We can do concurrent IO without threading.

### Event-Based Concurrency
- Remember that Linux stores incoming data in an OS-managed buffer, specific to each socket. Then `read` just copies data from it, and when the buffer is empty, `read` makes the code (thread) wait until there's more data.
- Same idea for `write`, each socket as a buffer where all the write data is stored. If it's full, `write()` suspends the calling thread until there's room.
- Multithreading was needed because whilst we're waiting, why not do something else. If one thread is blocked, switch to work on another for a bit. But if we're able to wait on multiple sockets at once, and then `read/write` the ones that are ready, we can do it in one thread.
```Python
while running:
  
  # Get lists of reading and writing sockets (fds)
  want_read = [...] 
  want_write = [...]

  # Wait for sockets to be ready to read and write; this is BLOCKING 
  can_read, can_write = wait_for_readiness(want_read, want_write)

  # 1. Non-blocking (probably because data is ready); only consume from the OS buffer
  # 2. Application logic 
  for fd in can_read:
    data = read_nb(fd)
    handle_data(fd, data)

  # 1. Data produced by our application
  # 2. non-blocking (at this point it's ready), appends to the OS-managed buffer
  # 3. App logic
  for fd in can_write:
    data = pending_data(fd)
    n = write_nb(fd, data)
    data_written(fd, n)

```
This is an event loop, and this is a common alternative paradigm to multithreading. Each loop iterate waits for any "readiness" events, and then reacts to those events without blocking the main thread. The result is that all sockets can be processed without delay. Here are the main OS mechanisms that power this loop:
- **Readiness notification:** Wait for multiple sockets, immediately returns when one of more are ready. "ready". This means that the read buffer isn't empty and write buffer isn't full. 
- **Non-blocking read:** Assuming the read buffer isn't empty, consume from it without interruption. 
- **Non-blocking write:** Assuming the write buffer isn't full, put some data into it without interruption.

While this is concurrency, it's not parallelism. The idea of "doing multiple things at once" comes from our ability to monitor many sockets simultaneously and react immediately when any of them becomes ready. But we still process one ready socket at a time—sequentially—within the loop. This is a big improvement over blocking IO, where we’d be stuck waiting on one specific socket, even if others are already ready. With an event loop and setup to make sure our IO isn't blocked, we avoid that inefficiency. If this were parallelism, then when two sockets are ready, we could process both at the same time—like truly in parallel—using multiple threads or CPU cores.

`wait_for_readiness` is blocking the main thread. However that doesn't matter, since the main concern was being blocked by one socket, whilst others were getting ready. Now we're waiting for any socket (any job) to be ready. It's highly responsive, and its the best and most efficient use for the thread, we don't care about anything else instead of monitoring these sockets. And that's why we say that it's basically non-blocking. 

#### Callback-based programming: JavaScript Event Loop
In JavaScript, we register a callback on some event, and then teh data is delivered to the callback. In JS the eventloop is hidden, but here we're going to code it to see how this really works.

In the Node.js event loop, and asyncio's event loop, they both run on the main thread, they aren't running on a separate thread. 

### Non-blocking IO 

#### Non-blocking vs Blocking read/write
We have blocking and non-blocking versions of our IO functions. Let's compare them:

Both blocking and nonblocking reads will return data when the buffer isn't empty becasue the data is already there. However, when the buffer is empty, the non-blocking read will just return an error, whilst the blocking one waits until there's data. You can use the non-blocking one to repeatedly to drain the read buffer in an efficient way.

Both blocking and non-blocking writes will send data when the write buffer is empty. If the buffer is full, non-blocking returns an error whilst blocking waits until there's room. You can call the non-blocking one multiple times to efficiently fill the buffer.

`accept()` also has a non-blocking mode. Remember that `accept()` extracts a TCP connection from a queue, but when there are no TCP connections it'll just wait. It's blocking stuff, which can be problematic if we also have other sockets to process. Using a non-blocking one will be important:
```Python
for fd in can_read:
  if fd is listening_socket(fd):
    conn_fd = accept_nb(fd)   # non-blocking accept
    handle_new_conn(fd)
  else: # fd is a connection socket
    data = read_nb(fd) # non-blocking read
    handle_data(fd, data)    
```
So I guess the main idea is that the non-blocking accept will handle the case where there aren't any TCP connections left, we don't have to wait, we just continue to the next iteration of the event loop to handle any read sockets.

- Note: A listening socket appears in your can_read list of sockets when there's a new connection. However there's still the chance of failure and this actually blocking. In high-load scenarios or edge cases, even if the socket was ready, calling accept at that time may mean the queue is already empty (a race condition). 

#### Enabling Non-blocking mode
You use the same syscalls for blocking or non-blocking read and writes. You just change the `O_NONBLOCK` flag when you call them I think.

### "Readiness" API
Waiting for when IO is "ready" (when we're able to read or write without being blocked) is platform specific. First we'll talk about Linux `poll()`:
```C

// takes an array of file descriptors, each with an input and an output flag
// - timeout is used to implement "timers" later.
int poll(struct pollfd *fds, nfds_t ndfs, int timeout);

struct pollfd {
  int fd;         // File descriptor for polling; the magic ticket  
  short events;   // type of request we want to do: reading (POLLIN), writing (POLLOUT), or both (POLLIN|POLLOUT)? 
  short revents;  // type of return: ready to read, ready to write? This inidcates the state of the readiness
};
```
- `select()` present in windows and linux, but only supports up to 1024 file descriptors. That's a small number of sockets that we'd be able to monitor so it isn't used.
- `epoll_wait()` is linux specific. The list of fds is managed by the OS and you use `epoll_ctl()` to modify that list. It's more scalable.
- `kqueue()` specific to BSD. It requires fewer syscalls.

You can't use the readiness APIs with disk files. When we read from the socket, the data is ready to read when data is in the buffer (in RAM). The APIs will always show disk files as ready to read, but it doesn't mean all of its data is loaded into RAM for immediate access. Reading from a disk file triggers a disk I/O operation, and the thread will block whilst the disk is reading the data into memory. And disk I/O is a lot slower than accessing RAM. 

The solution to solving this is offloading this disk IO task to a thread pool. The operation needs to happen in a separate thread, not in the main event loop's thread. Because if you read it directly in the event loop, and it blocks you event loop (which it will) you're freezing the loop. As a result:
- You're no longer monitoring all your sockets, events. No existing jobs are being handled, and no new jobs are being taken.
- You're also blocking the main thread since the event loop is in the main thread. Not only in our C example, but in multiple popular iterations of the event loop like Node.js and `asyncio`. That's why you always hear "don't block the event loop" because it's the main thread, and blocking it is equivalent to blocking your main code.

***"Don't block the event loop"*** mean to make sure to not have long-running tasks in your event loop from processing other tasks. Or equivalently, don't do slow/blocking tasks in the main thread. Stuff like network requests, disk IO, and CPU-bound tasks (e.g. complex calculations) are blocking. Make sure to run these things in a separate thread if you want things to be concurrent. As a result your main code and the event loop (within the same thread) can run concurrently like normal, and those separate threads will handle their things like normal as well.


### Summary of concurrent IO techniques and pseudocode
| **Type** | **Method**             | **API**           | **Scalability** |
| -------- | ---------------------- | ----------------- | --------------- |
| Socket   | Thread per connection  | `pthread`         | Low             |
| Socket   | Process per connection | `fork()`          | Low             |
| Socket   | Event loop             | `poll()`, `epoll` | High            |
| File     | Thread pool            | `pthread`         | Medium          |
| Any      | Event loop             | `io_uring`        | High            |


## Credits
- [Event Loop Introduction](https://build-your-own.org/redis/05_event_loop_intro)