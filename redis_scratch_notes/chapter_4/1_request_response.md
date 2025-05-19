# Request-Response Protocol

## Handling Multiple Requests in a single connection
```C
// Not doing concurrent connections right now, just making the server process multiple requests in a single connection using a loop

while (true) {
  struct sockaddr_in client_addr = {};
  socklen_t addrlen = sizeof(client_addr);
  int connfd = accept(fd, (struct sockaddr *)&client_addr, addrlen);
  if (connfd < 0) {
    continue;
  }

  // Loop
  while (true) {
    int32_t err = one_request(connfd);
    if (err) {
      break;
    }
  }

  close(connfd);
}
```
`one_request` reads 1 request and writes 1 response. An application protocol will help it figure out how many bytes to reads, that's its primary function. A protocol has two levels:
1. A high-level structure that splits the byte stream into messages.
2. A structure within the message.

### Simple Binary Protocol: Split byte stream into messages
The request and response are both strings. We split the byte stream into messages, where each message is a 4-byte little endian integer that indicates the length of the request and the variable length of the payload. 

┌─────┬──────┬─────┬──────┬────────
│ len │ msg1 │ len │ msg2 │ more...
└─────┴──────┴─────┴──────┴────────
   4B   ...     4B   ...
Each message is a full piece of communication from one side to another. Remember from client to server, each message represents a request. Then from server to client each message represents a response. A message could represent a GET request or any kind of request or response, depending on how we define the protocol. Each message is just:
1. A 4-byte little-endian header: Length of the payload.
2. A payload: A string e.g. `"GET /hello"` or `"Hello Server"`.

What the payload means or represents is to us and the application layer protocol that we're building. But the main takeaway is think about a message as a full piece of communication. That's about it, but the next challenge figuring out the length of the payload, carefully reading it, and then creating our own application-layer message and sending it as a response.

### Parse the protocol
To read a message, we first read the 4 byte integer, and then the payload. However there are two pitfalls:
- Know that `read/write` can return less than 4 bytes. So you can't just have a while loop reading 4 bytes all the time. 
- A common misconception is that "if the client writes once, I can receive their write in one read. I can just read the number of times they write. Right?". Wrong.
```C
// Client sends two writes 
write(fd, "Hello", 5);
write(fd, "World", 5);
```
Here's what could happen:
- One read to the server might return "helloworld". 
- Or maybe the first read returns "hello" and the second "world"
- Or maybe the first returns "hel", second "lwo", and the third is "rld". 

This is what we mean when TCP doesn't preserve write boundaries. It just delivers a string of bytes, and it's up to us to how it's structured or parsed. We must define our own message boundaries as it allows us to know the start and end of a full piece of communication (a message). That's why **framing** (e.g. using that 4-byte header) is so important and helpful.
```C
static int32_t read_full(int fd, char *buffer, size_t num_bytes) {
  /*
  - Keep calling read() until it reads exactly num_bytes
  - We will advance the buffer pointer, so that the next time mwe use this it doesn't overwrite bytes 
    from previous message payloads.
  - We can't always assume that we read num_bytes bytes. We'll use the number of bytes we read to 
    move our buffer to not overwrite previous bytes (pointer arithmetic).

  Note: This is used to not only read the 4-byte message, but it's also used to read the full payload
  of the message. After we derive the length of the paylaod from the message header, for the next read we'll keep reading until we get the expected size of the payload.
  */
  // While we still have bytes we can read from our limit
  while (num_bytes > 0) {

    // Attempt to read rv number of bytes from the TCP connection socket
    ssize_t rv = read(fd, buffer, num_bytes);
    
    if (rv <= 0) {
      return -1; // error, or unexpected end of file
    }

    // Make sure the number of bytes we read is within our remaining limit
    assert((size_t)rv <= num_bytes);

    // Keep track of the remaining number of bytes we can read
    num_bytes -= (size_t)rv;

    // Advance buffer pointer to point at empty locations in memory.
    buffer += rv;
  }
  return 0;
}

static int32_t write_all(int fd, const char *buffer, size_t num_bytes) {
  /*
  - Writes num_bytes bytes to the TCP connection socket.
  - Advance the buffer pointer to avoid sending bytes that we've already sent.
  - Can't always assume that we're writing num_bytes.

  Same idea as the read function, we're looping until we've sent everything. In a server program, you should use this read_full and write_all functions instead of read and write.
  */
  while (num_bytes > 0) {

    // Write upwards of num_bytes from our buffer to the TCP connection socket; 
    // If the amount we actually wrote is negative, then an error happened.
    ssize_t rv = write(fd, buffer, num_bytes);
    if (rv <= 0) {
      return -1; 
    }

    // 1. Check that we didn't send over our remaining limit.
    // 2. Keep count of the remaining number of bytes we can write to the connection socket
    assert((size_t)rv <= num_bytes);
    num_bytes -= (size_t)rv;

    // Advance our buffer pointer
    buffer += rv;
  }
  return 0;
}

const size_t k_max_msg = 4096;
static int32_t one_request() {
  /*
  Single buffer to read:
  - First 4 bytes = message length
  - Up to 4096 bytes of capacity for a single message.
  */
  char read_buffer[4+k_max_msg];
  errno = 0;

  // Read the first four bytes, which is the header of the message
  int32_t err = read_full(connfd, read_buffer, 4); 
  if (err) {
    msg(errno == 0 ? "EOF" : "read() error");
    return err;
  }

  /*
  Copies 4 bytes into len, interpreting them as a little-endian uint32_t. 

  Note: We're assuming the client sent this data in little-endian format. This is because 
  we're working locally, so the client and server are just different processes on our computer.
  However in the real world, the client sends stuff over the network, we'll receive bytes in 
  big-endian format and we'll just have to do a byte-swap.

  Note: Remember that len is literally the length/size of the message payload! You'll see this later.
  */
  uint32_t len = 0;
  memcpy(&len, read_buffer, 4); 
  if (len > k_max_msg) {
    msg("Too long!");
    return -1;
  }

  // Reads the payload of the message. We're storing this after the 4-byte header.
  // Note: &read_buffer[4] is just a pointer to a spot in the buffer after the header. 
  // So we're just writing the message content starting at index 4.
  err = read_full(connfd, &read_buffer[4], len);
  if (err) {
    msg("read() error");
    return err;
  }

  // Print a string with max length len from the pointer &read_buffer[4]
  printf("Client says: %.*s\n", len, &read_buffer[4]);

  // Start sending a response:
  // - Start preparing the write_buffer
  // 1. Since of the payload is 6 since [w, 'o', 'r', 'l', 'd' '\0']
  // 2. Add 4 bytes to account for us indicating the sizeof the reply itself.
  const char reply[] = "world";
  const char write_buffer[4+sizeof(reply)]; // Just the size of our response I guess?

  // Get the size of the our payload as a 32 bit unsigned integer
  // Note: Obviously in little endian
  len = (uint32_t)strlen(reply);

  // Write the data for the message in the buffer:
  // 1. Copy the length at the start of write buffer
  // 2. Copy reply data to the rest of the buffer
  // - Note: Copy their byte representations obviously.
  memcpy(write_buffer, &len, 4);
  memcpy(&write_buffer[4], reply, len);

  // Write the buffer to the socket.
  return write_all(connfd, write_buffer, 4+len);
}
```

### Client Query Logic
```C
const size_t k_max_msg = 4096;
static int32_t query(int fd, const char* text) {
  uint32_t len = (uint32_t)strlen(text);
  if (len > k_max_arg) {
    return -1;
  }

  char write_buffer[4+k_max_msg];
  memcpy(write_buffer, &len, 4);
  memcpy(&write_buffer[4], text, len);

  int32_t err = write_all(fd, write_buffer, 4+len);
  if (err) {
    msg("write() failed");
    return err;
  }

  // Wait why do we have +1 at the end?
  char read_buffer[4+k_max_msg+1];
  errno = 0;
  int32_t err = read_full(fd, read_buffer, 4);
  if (err) {
    msg(errno == 0 ? "EOF" : "read() error");
    return err;
  }
  
  // Attention: Again we're assuming we're being sent little-endian data. This is because this
  // example runs things locally. If this were real, you'd be converting big-endian to little-endian.
  // But yeah reading the length of the payload.
  memcpy(&len, read_buffer, 4); 
  if (len > k_max_arg) {
    msg("Too long of a message from the server");
    return -1;
  }

  // Read the payload they sent us
  err = read_full(fd, &read_buffer[4], len);
  if (err) {
    msg("read() error");
    return err;
  }

  printf("Server says: %x*s\n", len, &read_buffer[4]);
  return 0;
}

int main() {

  // Prepare socket for creation
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    die("socket()");
  }
  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = ntohs(1234);
  addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);  // 127.0.0.1
  int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
  if (rv) {
      die("connect");
  }

  // Do multiple requests
  int32_t err = query(fd, "hello1");
  if (err) {
      goto L_DONE;
  }
  err = query(fd, "hello2");
  if (err) {
      goto L_DONE;
  }
  err = query(fd, "hello3");
  if (err) {
      goto L_DONE;
  }

L_DONE:
    close(fd);
    return 0;
}

```

### Understanding Reading and Writing: Why we read or write "fully"
When reading a disk file and it returns less than requested, that's an EOF or an error. However a socket can return less data even under normal conditions (still has data and not an error). This is explained with the idea of **pull-based IO** or **pushed-based IO**. Data over a network is ***pushed*** by a remote peer. The remote doesn't need to call `read()` before sending over its data. Remember the OS creates a **receive buffer** to store received data for each TCP connection. Then our `read()` just copies the data from that receive buffer to your user-defined buffers. 
```C
// While you expect 10KB, from this single read, you may get:
// 1. 10KB; lucky 
// 2. 5KB
// 3. 2KB
// 4. Maybe even 1 byte; though unlikely
// Not because something went wrong, but because that's all the data that has arrived so far.
// So read_full is here to keep calling read() in a loop until we've received all the expected bytes.
read(socket_fd, buffer, 10240)
```
Now reading from a file is different. Data from disk is always ready and available, it's all there. The file size is known, so if we request 10KB, we're going to get 10 KB worth of data. The only time we'll get less is when we reach an EOF.

`write()` appends data to an OS-managed buffer, and then the actual network data transfer is given to the OS. However that buffer has limited size, so when it's full, we need to wait for the buffer to drain before sending the rest of the data. However during this wait, the syscall (`write()`) could be interrupted, due to some other OS process, which results in us returning partial data. So to consider this, we'll loop until we send all of the data, which fixes this interruption issue.

`read` can also be interrupted by an OS signal (some other process hapepend) before any data was read. When this happens 0 bytes are read but the return value is -1, and the `errno=EINTR`. In this case, since your read was interrupted, you just retry:
```C
static int32_t read_full(int fd, char *buffer, size_t num_bytes) {
  while (num_bytes > 0) {
    ssize_t rv = read(fd, buffer, num_bytes);

    // So if return rv == -1 and errno=EINTR

    if (rv == -1) {
      if (errno == EINTR) {
        // read syscall was interrupted before we could read data 
        // due to some other thing the OS had to manage. Retry getting our data
        continue;
      }

      // Some other read error
      // Credits: https://man7.org/linux/man-pages/man2/read.2.html
      return -1;
    } 

    if (rv == 0) {
      // Peer has closed the connection (EOF)
      return -1;
    }

    assert((size_t)rv <= num_bytes);
    num_bytes -= (size_t)rv;
    buffer += rv;
  }
  return 0;
}
```


### Last word on protocols
While human-readable protocols like HTTP, JSON, and Plaintext are readable, it isn't machine-readable due to how complex it'd be. It's a lot simpler to use a binary protocol. 

We have a common theme. We have a fixed-size part (header) and a variable-lengthed data (payload). The fixed-size part indicates the length of the variable length part. This is the **length prefixes** pattern, however the other method is the **delimiter pattern**. Use delimiters to indicate the end of a variable-length unit of data. So you keep reading until a delimiter is found, but be careful if the payload contains the delimiter. Now we'd need to work with escape sequences, which is a little more complex. Talking about the real world, the Redis protocol uses both delimiters and length prefixes. Strings are length prefixed, with the length being a decimal number delimiter by a new line. 
```
$5\r\nhello\r\n
```
