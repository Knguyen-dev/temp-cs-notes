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
#include <string>
#include <vector>

static void msg(const char *msg) {
  fprintf(stderr, "%s\n", msg);
}

static void die(const char *msg) {
  int err = errno;
  fprintf(stderr, "[%d] %s\n", err, msg);
  abort();
}

// Same thing for reading until all data is sent
static int32_t read_full(int fd, uint_t *buf, size_t n) {
  while (n > 0) {
    ssize_t tv = read(fd, buf, n);
    if (rv <= 0) {
      return -1; // error or unexpected EOF
    }
    assert((size_t)rv <= n);
    n -= (size_t)rv;
    buf += rv;
  }
  return 0;
}

// Writing until all data is sent;
static int32_t write_all(int fd, const uint8_t *buf, size_t n) {
  while (n > 0) {
    ssize_t rv = write(fd, buf, n);
    if (rv <= 0) {
      return -1;
    }
    assert((size_t)rv <= n);
    n -= (size_t)rv;
    buf += rv;
  }
  return 0;
}

// append to the back; abstracting the tedious operations out
static void buf_append(std::vector<uint8_t> &buf, const uint8_t *data, size_t len) {
    buf.insert(buf.end(), data, data + len);
}

const size_t k_max_msg = 32 << 20; 

static int32_t send_request(int fd, const uint_8 *text, size_t len) {
  // Failed since the length of data is bigger than we allow
  if (len > k_max_msg) {
    return -1;
  }

  // 1. Construct the byte buffer with the message header and body. 
  // 2. Then write the data to the socket.
  std::vector<uint8_t> write_buffer;
  buf_append(write_buffer, (const uint8_t *)&len, 4);
  buf_append(write_buffer, text, len);
  return write_all(fd, write_buffer.data(), write_buffer.size());
}

static int32_t read_response(int fd) {

  // Create a byte array for our read buffer. First we want only 4 bytes
  // to read the message header; handle any failure
  std::vector<uint8_t> read_buffer;
  read_buffer.resize(4);
  errno = 0;
  int32_t err = read_full(fd, &read_buffer[0], 4);
  if (err) {
    if (errno == 0) {
      msg("EOF");
    } else {
      msg("read() error");
    }
    return err;
  }

  // Read that 4 byte message header to get payload size and validate it
  // Note: Assuming that all bytes are being transferred in little endian rather than conventional big endian
  unint32_t len = 0;
  memcpy(&len, read_buffer.data(), 4); 
  if (len > k_max_msg) {
    msg("too long");
    return -1;
  }

  // Parse the message payload or "reply body"
  read_buffer.resize(4+len);
  err = read_full(fd, &read_buffer[4], len);
  if (err) {
    msg("read() error");
    return err;
  }

  // process data and return successful process
  printf("len:%u data:%.*s\n", len, len < 100 ? len : 100, &rbuf[4]);
  return 0;
}

int main() {

  // Prepare IPv4 connection socket for 127.0.0.1:1234 
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    die("socket()");
  }
  struct sockaddr_in addr = {};
  add.sin_family = AF_INET;
  addr.sin_port = ntohs(1234);
  addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK); 
  int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
  if (rv) {
    die("connect failed");
  }

  // Prepare to send multiple requests within this one TCP socket connection
  // Note: 1 char = 1 byte, we'll have a message with the maximum size
  std::vector<std::string> query_list = {
    "hello1", "hello2", "hello3", std::string(k_max_msg, 'z'), "hello5"
  };

  // Send requests and then read the responses
  // Note: If we get some failure when sending or responding then we'll just end the program.
  for (const std::string &s : query_list) {
    int32_t err = send_request(fd, (uint8_t *)s.data(), s.size());
    if (err) {
      goto L_DONE;
    }
  }
  for (size_t i = 0; i < query_list.size(); ++i) {
    int32_t err = read_response(fd);
    if (err) {
      goto L_DONE;
    }
  }

L_DONE:
  close(fd);
  return 0;
}