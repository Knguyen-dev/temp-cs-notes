#include <assert.h>
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

static void die(const char *msg) {
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}

const size_t k_max_msg = 4096;


static int32_t read_full(int fd, char *buf, size_t n) {
    while (n > 0) {
        ssize_t rv = read(fd, buf, n);
      
        if (rv == -1) {
          if (errno == EINTR) {
            // Read sys-call was interrupted before it could actually read; retry!
            continue;
          }
          
          // Actual read error
          return -1;
        }

        if (rv == 0) {
          // Client has closed the connection (EOF)
          return -1;
        }

        // Advance buffer and keep track of how much left we need to read.
        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buf += rv;
    }
    return 0;
}

static int32_t write_all(int fd, const char *buf, size_t n) {
    // Continue to write data from the buffer into the TCP connection socket
    // Note: Technically, write is sending this binary data to an OS-managed write buffer
    // specific to this TCP connection.
    while (n > 0) {
        ssize_t rv = write(fd, buf, n);
        if (rv <= 0) {
            return -1;  // error
        }
        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buf += rv;
    }
    return 0;
}

static int32_t one_request(int connfd) {
    // 4 bytes for message header, but payload has a max cap of k_max_msg bytes
    char rbuf[4 + k_max_msg];
    errno = 0;
    int32_t err = read_full(connfd, rbuf, 4);
    if (err) {
        // Check if we got an EOF from the client, or a real read error.
        msg(errno == 0 ? "EOF" : "read() error");
        return err;
    }

    // Obtain length of the payload and check that it's within size
    uint32_t len = 0;
    memcpy(&len, rbuf, 4);  // assume little endian
    if (len > k_max_msg) {
        msg("too long");
        return -1;
    }

    // Obtain the payload bytes themselves
    // Again it's a raw array of char bytes. However we'll treat it as a string when printing
    // Since there is no null terminator, we'll use len (an integer representation for the number of bytes the payload is)
    // to indicate how many of the read_buffer we should print out.
    err = read_full(connfd, &rbuf[4], len);
    if (err) {
        msg("read() error");
        return err;
    }

    // Pritn out what the payload says
    fprintf(stderr, "client says: %.*s\n", len, &rbuf[4]);

    // reply using the same protocol; the same idea of fixed message header and payload
    // This is the standard we created, that works well.
    // 1. Create buffer of appropriate size
    // 2. Get the length of our reply and store it in the write buffer. Store the payload in the write buffer
    // 3. Finally send the write buffer 
    const char reply[] = "world";
    char wbuf[4 + sizeof(reply)];
    len = (uint32_t)strlen(reply);
    memcpy(wbuf, &len, 4);
    memcpy(&wbuf[4], reply, len);
    return write_all(connfd, wbuf, 4 + len);
}

int main() {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    die("socket()");
  }

  int val = 1;
  setsockopt(fd, SOL_SOCKET< SO_REUSEADDR, &val, sizeof(val));

  // Bind IPv4 socket
  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = ntohs(1234);
  addr.sin_addr.s_addr = ntohl(0);    // wildcard address 0.0.0.0
  int rv = bind(fd, (const sockaddr *)&addr, sizeof(addr));
  if (rv) {
      die("bind()");
  }

   // listen
   rv = listen(fd, SOMAXCONN);
   if (rv) {
       die("listen()");
   }

   while (true) { // While we're still processing a given TCP connection socket
      // accept
      struct sockaddr_in client_addr = {};
      socklen_t addrlen = sizeof(client_addr);
      int connfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);
      if (connfd < 0) {
          continue;   // error
      }
      while (true) {
          // While we're still processing the message(s) sent through this TCP connection
          // This allows us to do multiple messages within a TCP connection
          // Although it's a different protocol than our binary protocol, imagine 
          // how multiple http requests can be sent through a single TCP connection.
          
          int32_t err = one_request(connfd);
          if (err) {
            // If err
              break;
          }
      }
      close(connfd);
  }
}