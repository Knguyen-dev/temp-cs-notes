#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>

static void die(const char* msg) {
  int err = errno;
  fprintf(stderr, "[%d] %s\n", err, msg);
  abort();
}

int main() {

  // Prepare to create a IPv4 TCP socket 
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    die("socket() failed");
  }

  // 1. Create <IPv4>:<Port> address that our client is going to connect to.
  // 2. Attempt to create TCP socket connection with the server.
  // Note: Typically you want to make sure the port and host address are in network byte order
  // however notice our server IP is "127.0.0.1". We're basically communicating with our own computer
  // so both client and server work in little endian. Not sending anything over a network, so it's good.
  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = ntohs(1234);
  addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK); 
  int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
  if (rv) {
    die("connect() failed");
  }

  // Write to the server using our TCP socket connection
  char msg[] = "hello";
  write(fd, msg, strlen(msg));
  
  // We expect to get data from the server so read it.
  char read_buffer[64] = {};
  ssize_t num_bytes = read(fd, read_buffer, sizeof(read_buffer) - 1);
  if (num_bytes < 0) {
    die("read() failed");
  }
  printf("Server says: %s\n", read_buffer);
  close(fd);
  return 0;
}