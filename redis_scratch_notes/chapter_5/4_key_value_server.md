# Key-Value Server
Let's start making this more Redis-like

### Request Response Message Format
┌─────┬──────┬─────┬──────┬────────
│ len │ msg1 │ len │ msg2 │ more...
└─────┴──────┴─────┴──────┴────────
   4B   ...     4B   ...
This is the message format that we're currently dealing with. We will replace each `msg` with a key-value store with get, set, and del commands. A Redis request is just a list of strings. The list is just a chunk of bytes, which we need to de-serialize. So let's zoom in on a given `msg`, here's what's inside: 

┌────┬───┬────┬───┬────┬───┬───┬────┐
│nstr│len│str1│len│str2│...│len│strn│
└────┴───┴────┴───┴────┴───┴───┴────┘
  4B   4B ...   4B ...


The first 4 bytes, `nstr` indicate the number of strings in the list. Each string item (8 bytes) includes a 4 byte prefix containing the length of the string, and after a 4 byte payload that represents the actual string. For the response, we'll just send a status code and then some data.
┌────────┬─────────┐
│ status │ data... │
└────────┴─────────┘
    4B     ...

### Handling Requests
Here's the plan:
1. Parse the command 
2. Process the command and generate a response.
3. Append the response to the output buffer.

```C
static bool try_one_request(Connection *conn) {
  // ...
  // got one request, do some application logic

  std::vector<std::string> cmd;
  if (parse_req(request, len, cmd) < 0) {
    conn->want_close = true;
    return false; // error, so close the socket
  }

  Response resp;
  do_request(cmd, resp);
  make_response(resp, conn->outgoing);
  // ...
}
```

### Step 1: Parse the Request
```C++
/*
This function reads 4 bytes from a byte buffer (represented by the `cur` pointer)
and stores them as a 32-bit unsigned integer in `out`.

`cur` is assumed to point to a position within a byte array (or buffer),
and `end` is the end of the valid buffer range.

- The check `cur + 4 > end` ensures that there are at least 4 bytes left to read.
  If not, the function returns false to signal that the read would go out of bounds.

- `cur` is passed by reference (`const uint8_t *&cur`) so that the pointer itself can be
  advanced after reading — i.e., we move the pointer 4 bytes ahead so the next read will
  start at the correct position. This is **modifying what the pointer points to**, not the bytes themselves. 
  You could also just rewrite this using a double pointer though and just de-reference once:
  
  static bool read_u32(const uint8_t **cur, const uint8_t *end, uint32_t *out) {  
    if (*cur + 4 > end) {
      return false;
    }
    memcpy(out, *cur, 4);
    *cur += 4;
    return true;
  }

- `out` is passed by reference (`uint32_t &out`) so we can set value of the actual variable passed in.

- `memcpy(&out, cur, 4)` copies 4 bytes from the buffer starting at `cur` into `out`. Agai nwe're coppying the actual values stored in memory, not the memory addresses of the elements themselves. This is a low-level operation and assumes the system is using a compatible endianness. (If byte order matters, you'd usually handle that explicitly.) Think of it as:
  const uint8_t buffer[] = { 0x01, 0x00, 0x00, 0x00, ... }; // so here we're seeing those 4 bytes
  const uint8_t* cur = buffer; 
  memcpy(&out, cur, 4);
  
  // out = 0x00 00 00 01; remember out is a uint32_t, and it's little endian, so the least significant byte goes first.

- After copying, `cur += 4` advances the pointer by 4 bytes so we don't read the same bytes again.
*/
static bool read_u32(const uint8_t *&cur, const uint8_t *end, uint32_t &out) {  
  if (cur + 4 > end) {
    return false;
  }
  memcpy(&out, cur, 4);
  cur += 4;
  return true;
}

/*
Reads n bytes from a byte buffer (pointed to by cur) into the out string.  

Even though cur is a `const uint8_t *`, std::string has constructors and the assign fcuntion takes raw character data. We also know that `uint8_t` is just an alias for `unsigned char` so this is valid. It interprets the data as a sequence of bytes and stores those as is in
std::string. So we're storing the bytes that represents the characters in the string. Pointer arithmetic is being used, but assign() takes two positions in a buffer, and values the elements in between. I mean look at this similar but more understandable example:

const char buffer[] = { 'H', 'i', '!', '\0', 'X', 'Y' };
const char* cur = buffer;
std::string out;
out.assign(cur, cur + 3);  // "Hi!"


*/
static bool read_str(const uint8_t *&cur, const uint8_t *end, size_t n, string &out) {
  if (cur + n > end) {
    return false;
  }
  out.assign(cur, cur+n);
  cur += n;
  return true;
}

/*
This functions parses a single message/request. So again we're parsing the amount of strings in the request's list, and we're parsing each individual string as well. We can reason that *data is a pointer to the start of the msg. Again we should know the size of the message, we know that the header is 4 bytes, and we can derive the number of bytes in the payload from the header. So again:

- *data: A pointer to the start of the message (a byte array)
- size: The size of the entire message (header + payload) included. 
*/
static int32_t parse_request(const uint8_t *data, size_t size, std::vector<std::string> &out) {  
  // 1. Pointer to the end of the byte array. This should be the last element of the byte array.
  // 2. Read 4 bytes from the data buffer and store that into the nstr variable
  // Remember this is the header of the message that represents the number of strings that are in our message.
  const uint8_t *end = data + size;
  uint32_t nstr = 0
  if (!read_u32(data, end, nstr)) {
    return -1;
  }

  // If the number of strings/args bigger than that we've allowed, if so then fail the request
  // Again those 32 bits represent an integer so that's the comparison is happening here.
  if (nstr > k_max_args) {
    return -1;
  }

  // Whilst our output buffer doesn't have the expected number of arguments stored in it.
  while (out.size() < nstr) {

    // Read 4 bytes to get the length of the string and store that in len
    // Note: So I guess len is converted to a size_t interpreting those 4 bytes as an integer.
    // Telling us how many bytes to read to get the actual string payload.
    uint32_t len = 0;
    if (!read_u32(data, end, len)) {
      return -1;
    }

    // Push an empty string object at the back of our vector. Then we'll need 
    // Read len bytes from our byte array and store it in that string object.
    out.push_back(std::string());
    if (!read_str(data, end, len, out.back())) {
      return -1;
    }
  }

  // At this point, we've processed `nstr` strings. Each string (8 bytes of data), we followed the amount of bytes to copy to get 
  // the string. When doing those read_u32 and read_str, those functions advanced the data pointer.
  // If the data pointer isn't pointing to the end, even though we followed the protocol, this message is formatted incorrectly
  // and there's some trailing data at the end so fail the request. We should ideally be pointing at the end if everything was right.
  if (data != end) {
    return -1;
  }

  return 0;
}
```

---
### Step 2: Process the Command
Now we have to process the command
```C++

// The structure of the response that we send back to the user. So we'll have the status of their request.
// Then we have a vector of unsigned bytes that we will send the user.
struct Response {
  uint32_t status = 0;
  std::vector<uint8_t> data;
}

// The map being the key-value store that will be implemented later.
static std::map<std::string, std::string> g_data;


```C
/*
- cmd: Reference to a vector of strings. We'll be able to dierctly affect the original vector since we pass the vector. So These are 
  the arguments from the request?
- out: reference to the response object; same thing directly affect the response object

Algorithm:
  1.If there are two args, adn it's a GET request:
    1. Find key in the map with cmd[1]. take care of sitautions where it's not found.
    - Note: Remember in C++ when searching through a map, the find() function returns an iterator to the element if found and g_data.end() if not.
    So yeah if key exists it points to the element, else the special iterator repressents one past the last element.
    2. In the case where it's in the map, this access the value associated with that key. We'll copy the second value to our out.data vector. 
    Remember that std::string stores bytes (char) and iterating over it gives you char values. However char is a 8-bit signed integer whilst 
    uint8_t is an 8-bit unsigned integer. We're treating the string was raw bytes, which I think works for encoding.
  2. If there were 3 arguments, it's in the form "SET <key-to-update> <value-update-to>".
  3. Else if, we're dealing with a deletion operation we have "del <key-to-delete>".
  4. They're using an unknown command.
*/
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
```

---
### Step 3: Serialize the response
```C++
/*
This constructs the response:
1. Calculate the size of the response. Remember we've created a protocol when we have a message header (4 bytes) and payload (resp.data.size()). 
2. Append the response length to the output buffer (4 bytes).
3. Append the response status (4 more bytes); cast the uint32_t to a pointer to an array of uint8_t
4. Append the response data into the output.
*/
static void make_response(const Response &resp, std::vector<uint8_t> &out) {
  uint32_t resp_len = 4 + (uint32_t)resp.data.size();
  buf_append(out, (const uint8_t *)&resp_len, 4);
  buf_append(out, (const uint8_t *)&resp.status, 4);
  buf_append(out, resp.data.data(), resp.data.size());
}

/*
it is correct that currently, the response data is copied twice, it goes from the key-value pair in the map to Response::data, and 
then from Response::data to Connection::outgoing. Now you want to optimize the code S.T. the response data goes directly to Connection::outgoing?

Well in that case there are a couple ways I can think of. You could replace Response parameter with a connection parameter. But I'm thinking that you want 
to use Response, so I could modify Response to contain a connection object.

*/
```

---
### TLDR Summary Of Everything 
After this chapter, we'll just be implementing data structures in C and also handling doing multithreading for long IO such as disk IO, network requests, etc. which should be done in another thread instead of being something we do in the main thread.
1. First we learned about sockets and setup TCP client server code. We first started off with a server that can do one request at a time. So that's one message with some data. Then we developed our own communication protocol. We use prefixes, so the first 4 bytes indicate the length of the payload (num bytes in payload). Then we have the message body (the payload with the actual data). So this entire message (header+payload) represents one request. 
2. However we wanted to also make it so that the user can send multiple requests through the TCP connection, so we did that. It was simple but now you can send multiple messages per connection. 
3. Then we moved into concurrency. To handle multiple TCP connections, we could use multithreading, but that's really high overhead. Instead we developed an event loop, did non-blocking sockets as well, etc. Here's the breakdown:
  - Be able to monitor all sockets, and also read or write sockets that are ready to do IO without being blocked. This is the core of the event loop
  - The sockets are set as non-blocking. This makes it so they'll raise an error in case they're going to be run and the OS will block them. Prevents us from wasting time.
5. At step 5, we still rely on a standard request-response cycle. The client sends something, and only once they receive our response, they will send another request. We're reaching the limit of the event loop, as there's only so quick we can go. However, a lot of the time being taken is due to the amount of IO we do. We only expect one request when reading. But here we do pipelining, where the client sends multiple requests at a time, not in tandem like we used to. Then we'll now have the potential of reading multiple messages with one read operation, instead of just capping at reading at most 1.


## Credits
- [Chapter 7 Key-Value Server](https://build-your-own.org/redis/07_basic_server)