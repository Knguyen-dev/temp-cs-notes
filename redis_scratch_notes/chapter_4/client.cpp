#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>

static void msg(const char *msg) {
  fprintf(stderr, "%s\n", msg);
}

static void die(const char* msg) {
  int err = errno;
  fprintf(stderr, "[%d] %s\n", err, msg);
  abort();
}

static int32_t read_full(int fd, char *buffer, size_t num_bytes) {
  while (num_bytes > 0) {
    // Read up to rv bytes from the TCP connection (os managed buffer by the way)
    ssize_t rv = read(fd, buffer, num_bytes);
    if (rv == -1) {
      if (errno == EINTR) {
        // read syscall was interrupted before we could read data 
        continue;
      }
      // Some other read error
      return -1;
    } 
    if (rv == 0) {
      // Peer has closed the connection (EOF)
      return -1;
    }
    // Keep track of the remaining bytes we need to read
    // Advance the buffer pointer forward to avoid reading bytes that we've already read.
    assert((size_t)rv <= num_bytes);
    num_bytes -= (size_t)rv;
    buffer += rv;
  }
  return 0;
}

static int32_t write_all(int fd, const char *buffer, size_t num_bytes) {
  while (num_bytes > 0) {
    // Same idea as before; all interrupts are handled with while loop for write scenario
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
static int32_t query(int fd, const char *text) {

    // Calculate the length of our message we're sending to the server
    uint32_t len = (uint32_t)strlen(text);
    if (len > k_max_msg) {
        return -1;
    }

    // 1. Store our message head and payload in the write buffer
    // 2. Send our data to the client (sends data to OS-managed buffer for writing)
    // Note: Assume little endian, so we aren't converting into Network Byte Order since testing locally
    char wbuf[4 + k_max_msg];
    memcpy(wbuf, &len, 4); 
    memcpy(&wbuf[4], text, len);
    int32_t err = write_all(fd, wbuf, 4 + len);
    if (err) {
        return err;
    }

    /*
    Prepare to read from the server:
    - 4 bytes for the message header, which indicates the size of the payload
    - k_max_msg bytes for the payload, so this is the max-capacity
    - +1 is likely for a null terminator. This is needed when we treat our 
    payload like a string.

    Remember that read() doesn't convert anything. It's literally reading raw bytes 
    from that file descriptor. if we're interpreting data as a string we're responsible for 
    ensuring that it's null terminated. Remember that char data type can also be used as a

    */
    char rbuf[4 + k_max_msg + 1];
    errno = 0;
    int32_t err = read_full(fd, rbuf, 4);
    if (err) {
      // If errno is 0, it's simply the end of the response, else it's a real read error
      msg(errno == 0 ? "EOF" : "read() error");
      return err;
    }

    // Obtain length of response and check that it's within reason
    // Note: so it seems k_max_arg is our maximum payload size.
    memcpy(&len, rbuf, 4);  // assume little endian
    if (len > k_max_msg) {
        msg("too long");
        return -1;
    }

    // Read the server's response 
    err = read_full(fd, &rbuf[4], len);
    if (err) {
        msg("read() error");
        return err;
    }
    // do something
    printf("server says: %.*s\n", len, &rbuf[4]);
    return 0;
}

int main() {
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

  // multiple requests
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