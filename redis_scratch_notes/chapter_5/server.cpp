// stdlib
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
// system
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
// C++
#include <vector>


static void msg(const char *msg) {
  fprintf(stderr, "%s\n", msg);
}

static void msg_errno(const char *msg) {
  fprintf(stderr, "[errno:%d] %s\n", errno, msg);
}

static void die(const char *msg) {
  fprintf(stderr, "[%d] %s\n", errno, msg);
  abort();
}

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

const size_t k_max_msg = 32 << 20;
struct Connection {
  int fd = -1;
  bool want_read = false;
  bool want_write = false;
  bool want_close = false;
  std::vector<uint8_t> incoming;
  std::vector<uint8_t> outgoing;
}

// append bytes to the back of a buffer.
// Note: These are important for latter because we're going to be doing partial reads and writes.
// If you're partially writing, you can imagine the buf_consume can be used to eliminate data that has already be sent or something similar.
static void buf_append(std::vector<uint8_t> &buf, const uint8_t *data, size_t len) {
    buf.insert(buf.end(), data, data + len);
}

// delete bytes from the front of a buffer.
static void buf_consume(std::vector<uint8_t> &buf, size_t n) {
    buf.erase(buf.begin(), buf.begin() + n);
}

static Connection *handle_accept(int fd) {
  // Create TCP connection socket using the fd we had; we'll record client IP address.
  struct sockaddr_in client_addr = {};
  socklen_t addrlen = sizeof(client_addr);
  int connfd = accept(fd, (struct scokaddr *)&client_addr, &addrlen);
  if (connfd < 0) {
    msg_errno("accept() error");
    return NULL;
  }
  uint32_t ip = client_addr.sin_addr.s_addr;
  fprintf(stderr, "new client from %u.%u.%u.%u:%u\n",
    ip & 255, (ip >> 8) & 255, (ip >> 16) & 255, ip >> 24,
    ntohs(client_addr.sin_port) 
  );

  // 1. Make sure this TCP connection socket is used in a non-blocking manner.
  // 2. Create a connection object, we intend to read incoming data so set that flag
  fd_set_nb(connfd);
  Connection *conn new Connection();
  conn->fd = connfd;
  conn->want_read = true;
  return conn;
}


const size_t k_max_args = 200 * 1000;

// Read 4 bytes from a buffer (pointed by cur pointer) and stores them as 32 bit uint in out
static bool read_u32(const uint8_t *&cur, const uint8_t *end, uint32_t &out) {  
  if (cur + 4 > end) {
    return false;
  }
  memcpy(&out, cur, 4);
  cur += 4;
  return true;
}

// Read n bytes from a byte buffer (pointed by cur) itno the out string
static bool read_str(const uint8_t *&cur, const uint8_t *end, size_t n, std::string &out) {
    if (cur + n > end) {
        return false;
    }
    out.assign(cur, cur + n);
    cur += n;
    return true;
}

static int32_t parse_request(const uint8_t *data, size_t size, std::vector<std::string>> &out) {
  const uint8_t *end = data + size;
  uint32_t nstr = 0;
  if (!read_u32) {
    return - 1;
  }
  if (nstr > k_max_args) {
    return -1;
  }
  while (out.size() < nstr) {
    uint32_t len = 0;
    if (!read_u32(data, end, len)) {
      return -1;
    }
    out.push_back(std::string());
    if (!read_str(data,end,len,out.back())) {
      return -1;
    }
    if (data != end) {
      return -1;
    }
  }
  return 0;
}

enum {
  RES_OK = 0,
  RES_ERR = 1, // error
  RES_NX = 2,  // key not found 
};

struct Response {
  uint32_t status = 0;
  std::vector<uint8_t> data;
};

static std::map<std::string, std::string> g_data;

static void do_request(std::vector<std::string> &cmd, Response &out) {
  if (cmd.size() == 2 && cmd[0] == "get") {
    auto it = g_data.find(cmd[1]);
    if (it == g_data.end()) {
      out.status = RES_NX; // not found
      return;
    }
    const std::string &val = it->second;
    out.data.assign(val.begin(), val.end());
  } else if (cmd.size() == 3 && cmd[0] == "set") {
    g_data[cmd[1]].swap(cmd[2]);
  } else if (cmd.size() == 2 && cmd[0] == "del") {
    g_data.erase(cmd[1]);
  } else {
    out.status = RES_ERR; // unrecognized command
  }
}

static void make_response(const Response &resp, std::vector<uint8_t> &out) {
  uint32_t resp_len = 4 + (uint32_t)resp.data.size();
  buf_append(out, (const uint8_t *)&resp_len, 4);
  buf_append(out, (const uint8_t *)&resp.status, 4);
  buf_append(out, resp.data.data(), resp.data.size());
}

/**
 * Process 1 request if there is enough data. We return a boolean indicating whether we successfully
 * read the request or not. 
 * 
 */
static bool try_one_request(Connection *conn) {
  // Not enough data; the message header alone should be at least 4 bytes
  if (conn->incoming.size() < 4) {
    return false;
  }

  // Read the first four bytes to get the message header. If the payload is too long for us, then
  // we fail this request and also indicate that we want to close this TCP connection on the next iteration
  // of the event loop.
  uint32_t len = 0;
  memcpy(&len, conn->incoming.data(), 4);
  if (len > k_max_msg) {
    msg("too long");
    conn->want_close = true;
    return false;
  }

  // If the full size of the payload hasn't been received, then we aren't going to process things yet.
  if (4+len > conn->incoming.size()) {
    return false;
  }

  // Get pointer to the start of the message payload; 
  const uint8_t *request = &conn->incoming[4];
  std::vector<std::string> cmd;
  if (parse_request(request, len, cmd) < 0) {
    msg("bad request");
    conn->want_close = true;
    return false;
  }

  Response resp;
  do_request(cmd, resp);
  make_response(resp, conn->outgoing);

  // We processed one request, so we'll remove the data from the header and payload
  buf_consume(conn->incoming, 4+len);
  return true;  
}

// Application callback when the socket is writable.
static void handle_write(Connection *conn) {
  assert(conn->outgoing.size() > 0);

  // 1. Attempt to write data to the client; it is assumed that this is non-blocking.
  // If it works, consuming data from our outgoing buffer to keep track of the remaining data we need to send.
  ssize_t rv = write(conn->fd, &conn->outgoing[0], conn->outgoing.size());
  if (rv < 0) {
    // A signal interrupted our write before it could happen. The error is just telling us to try again.
    if (errno == EAGAIN) {
      return;
    }
    
    // Actually an error, this connection failed, so mark this connection to be closed on the next iteration.
    msg_errno("write() error");
    conn->want_close = true;
    return;
  }
  buf_consume(conn->outgoing, (size_t)rv);

  /**
   * + Update readiness status:
   * Update the readiness of the connection. If all data has been read, then we'll want to start reading 
   * instead of writing.
   * 
   * Else, we'd want to write, but that's implicit.
   */
  if (conn->outgoing.size() == 0) {
    conn->want_read = true;
    conn->want_write = false;
  }
} 

static void handle_read(Connection *conn) {

  // Attempt to read data from buffer. Consider the case where we were interrupted and the case where 
  // we got an actual error that forces us to close the connection socket.
  uint8_t buffer[64 * 1024];
  ssize_t rv = read(conn->fd, buffer, sizeof(buffer));
  if (rv < 0) {
    if (errno == EAGAIN) {
      return;
    }
    msg_errno("read() error");
    conn->want_close = true;
    return;
  }

  // EOF or client closed; regardless we want to mark the connection to be closed.
  if (rv == 0) {
    if (conn->incoming.size() == 0) {
      msg("client closed");
    } else {
      msg("unexpected EOF");
    }
    conn->want_close = true;
  }

  /**
   * Here you parse your requests and generate responses to each request for the current client. The reason 
   * we call this in a loop is related to the idea of "pipelining".
   * TODO: Explain pipelining
   */
  buf_append(conn->incoming, buf, (size_t)rv)
  while (try_one_request(conn)) {}

  /**
   * + Update the readiness status after writing.
   * If we still have data that we should send, the connection intends to write instead of reading.
   * So I guess that kind of lets you infer that we will write until we're empty.
   * 
   * Else, the read should be set to true.
   */
  if (conn->outgoing.size() > 0) {
    conn->want_read = false;
    conn->want_write = true;
    return handle_write(conn);
  }
}

int main() {
   // ***** Setup the listening socket; already seen this stuff before
   int fd = socket(AF_INET, SOCK_STREAM, 0);
   if (fd < 0) {
       die("socket()");
   }
   int val = 1;
   setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
   struct sockaddr_in addr = {};
   addr.sin_family = AF_INET;
   addr.sin_port = ntohs(1234);
   addr.sin_addr.s_addr = ntohl(0);    // wildcard address 0.0.0.0
   int rv = bind(fd, (const sockaddr *)&addr, sizeof(addr));
   if (rv) {
       die("bind()");
   }
   fd_set_nb(fd);
   rv = listen(fd, SOMAXCONN);
   if (rv) {
       die("listen()");
   }
   
  //  ***** Main event looop stuff, this is new! *****
   std::vector<Connection *> fd_to_connection;
   std::vector<struct pollfd> poll_args;
   while (true) {

    // 
    poll_args.clear();
    struct pollfd poll_fd = {fd, POLLIN, 0};
    poll_args.push_back(poll_fd);

    for (Connection *conn: fd_to_connection) {
      if (!conn) {
        continue;
      }
      // We always want to poll to for an error; it just lets us see if we got an error or not.
      // Then let's poll based on the application's intentions for the given connection object.
      struct pollfd pfd = {conn->fd, POLLERR, 0};
      if (conn->want_read) {
        pfd.events |= POLLIN;
      }
      if (conn->want_write) {
        pfd.events |= POLLOUT;
      }
      // Then record that poll object in the poll_args array
      poll_args.push_back(pfd);
    }

    // wait for readiness; monitor all sockets
    int rv = poll(poll_args.data(), (nfds_t)poll_args.size(), -1);
    if (rv < 0) {
      if (errno == EINTR) { // interrupted, just do retry
        continue
      }
      // Actual error during the poll()
      die("poll() failed")
    }

    // At this point, at least one of the sockets are ready for I/O, making our IO non-blocking.
    // - First handle the listening socket, which is at position 0.
    // - If we sucessfully create a new TCP connection, then add it to our list of connections.
    if (poll_args[0].revents) {
      if (Connection *conn = handle_accept(fd)) {
        if (fd_to_connection.size() <= (size_t)conn->fd) {
          fd_to_connection.resize(conn->fd + 1);
        }
        assert(!fd2conn[conn->fd]);
        fd_to_connection[conn->fd] = conn;
      }
    }

    // 2. Handle the connection sockets; if any are ready for IO
    for (size_t i = 1; i < poll_args.size(); ++i) {
      
      // If the socket isn't ready to do anything, then skip it
      uint32_t ready = poll_args[i].revents;
      if (ready == 0) {
        continue;
      }

      // Handle reading and writing as long as the socket is ready to do it.
      Connection *conn = fd_to_connection[poll_args[i].fd];
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

    }
  }   

  return 0;
}