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

static void do_something(int connfd) {
  char read_buffer[64] = {};
  ssize_t num_bytes = read(connfd, read_buffer, sizeof(read_buffer) -1);
  if (num_bytes < 0) {
    msg("read() error");
    return;
  }
  fprintf(stderr, "client says: %s\n", read_buffer);
  char write_buffer[] = "world";
  write(connfd, write_buffer, strlen(write_buffer));
}

int main() {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    die("socket() failed");
  }

  // Needed for most server applications; so that the server can restart
  int val = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

  // Bind the socket to a IPv4:Port combo. This is where our server is listening.
  // Just listening to port 1234, regardless of the IP address we're givne.
  // Note: Just preparing parameters, socket creation comes next.
  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  add.sin_port = ntohs(1234);
  addr.sin_addr.s_addr = ntohl(0);
  int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
  if (rv) {
    die("bind() failed");
  }

  // Attempt to create a TCP listening socket using the ticket we got
  rv = listen(fd, SOMAXCONN);
  if (rv) {
    die("listen() failed");
  }

  // Create loop to accept all established TCP connections in our OS's queue.
  while (true) {
    struct sockaddr_in client_addr = {};
    socklen_t addrlen = sizeof(client_addr);

    // Attempt to create TCP connection socket 
    // Note: Still just an fd or magic ticket that we can use to interact with 
    // this connection later. Something we do in do_something
    int connfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);
    if (connfd < 0) {
      continue;
    }

    // Does something to the connection and closes it after we're done
    do_something(connfd);
    close(fd);
  }

  return 0;
}