# Data Serialization

## Motivation and Datatypes
So far the response is a string but Redis commands retunr other data types. For example, `dbsize` returns the number of keys as an integer, `keys` returns a list of strings, etc. 

Simple data types are string, integer float, boolean, and null. You could represent all of these are strings, but strings are ambiguous and require more work parsing. Some things need to be interpreted as other data types. Many serialization formats let you use integers, and other types, whilst XML only usees strings, which is why XML fell off.

Complex data types are array, map, struct. They are collections of other data types. These are commonly encoded as a map or array.

## Schema or Schema-less

Formats like XML or JSON are ***self-describing***, meaning they can be fully parsed without extra information besides the data itself. 

A schema ensures the data has a certain shape, but it has costs:
- Schemas require extra libs or tools like code generators.
- Schemas are extra moving parts.

Self-describing means schemas are optional, you can still add them. However with something like Protobuf, the schema is required. Here's a workflow:
```
                 encode
schema + input ──────────► bytes
                 decode
schema + bytes ──────────► output
```

---
## JSON as a reference
JSON is a good format. It's simple, the objects can be converted to native arrays, maps, structs. Also it's self-describing, schemas are optional.

## Serialization Formats

### Delimited Formats
Delimiters are used to determine the length of strings, arrays, etc. JSON this to identify 3 data types:
- Strings are delimited by quotes `"foo"`.
- Arrays are delimited by brackets and commas `[1, 2, 3]`
- Maps are delimited by braces, commas, and colons: `{"k":"v", "foo":"bar"}`.

The only hard part is dealing with parsing, and even large libraries have the issues. Stuff like ooptional white space, complicated escaping rules, and ambiguous string representations makes it difficult. However we can avoid all of these issues by not making our data format textual.

---
### Tag-Length-Value
Most binary serialization formats use the **Tag-Length-Value (TLV)** technique. Here's how it works:
1. "Tag" or "Type" comes first, making the data self-describing.
2. "Length" describes the length of strings, arrays, or maps. It removes the need for delimiters.
3. "Value" is the encoded data.

Example: Let's convert `[123, "foo"]` into TLV format.

 array   2    int         str    3
┌─────┬─────┬─────┬─────┬─────┬─────┬─────┐
│ tag │ len │ tag │ 123 │ tag │ len │ foo │
└─────┴─────┴─────┴─────┴─────┴─────┴─────┘

1. It's first tagged as an array with length of 2.
2. Then we tag the first element as an integer and we show the integer.
3. The last element is tagged as a string, with its length of 3. Then we show the actual string value. 

### Comparing Serialization Formats
Here's a clean table based on the data you provided:

| **Name**        | **Text** | **Self-describing** | **Simple Types**                      | **Complex Types**        |
| --------------- | -------- | ------------------- | ------------------------------------- | ------------------------ |
| **XML**         | Yes      | Yes                 | `str`                                 | `markup`                 |
| **JSON**        | Yes      | Yes                 | `null`, `bool`, `number`, `str`       | `array`, `map`           |
| **MessagePack** | No       | Yes                 | `null`, `bool`, `int`, `float`, `str` | `array`, `map`           |
| **Thrift**      | No       | Yes                 | `bool`, `int`, `float`, `str`         | `array`, `map`, `struct` |
| **Protobuf**    | No       | No                  | `bool`, `int`, `float`, `str`         | `struct`                 |

Most binary formats use TLV with different encodings. Many projects, including the real Redis, have rolled their own serialization format due to how easy it is to od so.

---
### Constructing a Simple Format
We'll support the following data types, so a response data is of one of these types.
```
enum {
    TAG_NIL = 0,    // nil
    TAG_ERR = 1,    // error code + msg
    TAG_STR = 2,    // string
    TAG_INT = 3,    // int64
    TAG_DBL = 4,    // double
    TAG_ARR = 5,    // array
};
```
Each array element can be of any type including nested arrays, but we're not doing maps for now

### Binary Format
The tag is 1 byte, length is four bytes. Strings are arbitrary bytes and array length is the number of array elements:
 nil       int64           str                   array
┌─────┐   ┌─────┬─────┐   ┌─────┬─────┬─────┐   ┌─────┬─────┬─────┐
│ tag │   │ tag │ int │   │ tag │ len │ ... │   │ tag │ len │ ... │
└─────┘   └─────┴─────┘   └─────┴─────┴─────┘   └─────┴─────┴─────┘
  1B        1B    8B        1B    4B   ...        1B    4B   ...

Integers and lengths are encoded in little-endian. 

### Output the serialized data
How about discarding the `struct Response` and storing serialized response data directly to `Conn::outgoing`. 
```C
/*
Note: `Buffer` is still just an std::vector, and we could replace it with something better apparently. Appending to theback is efficient for dynamic arrays is efficient since it doesn't "reallocate" cost. However removing from the front of dynamic arrays is not efficient because it has to move the rest of the data each time. Apparently this is O(N^2).
*/
typedef std::vector[uint8_t] Buffer;

// Note: The buffer appending functions are good places to handle endian-ness 
static void buf_append_u8(Buffer &buf, uint8_t data) {
  buf.push_back(data);
}

static void buf_append_u32(Buffer &buf, uint32_t data) {
  buf_append(buf, (const uint8_t *)&data, 4);           
}

/*
This models the scenario where Redis found nothing so we output
TAG_NIL to indicate that the response data is nothing, no response.
*/
static void out_nil(Buffer &out) {
    buf_append_u8(out, TAG_NIL);
}

/*
This models the situation where we have to send back string data after a Redis operation. Here's the workflow:
1. Output 1 byte (8bits unsigned) for the string tag.
2. Output 4 bytes for the message header (payload size, length of string). Cast size_t to an unsigned 32 bit integer.
3. Write the data 's' into the buffer, treat it as a buffer of bytes.
*/
static void out_str(Buffer &out, const char *s, size_t size) {
    buf_append_u8(out, TAG_STR);
    buf_append_u32(out, (uint32_t)size);
    buf_append(out, (const uint8_t *)s, size);
}

/*
When the output (data) is an integer
1. Append one byte for the integer tag.
2. Append the 8 bytes to the buffer that represent the integer value. 
*/
static void out_int(Buffer &out, int64_t val) {
    buf_append_u8(out, TAG_INT);
    buf_append_i64(out, val);
}

/*
When the output is an array:
1. Append 1 byte for the tag array.
2. Append 4 bytes to indicate the array data? 
*/
static void out_arr(Buffer &out, uint32_t n) {
    buf_append_u8(out, TAG_ARR);
    buf_append_u32(out, n);
}

// You can use those functions like this:
static void do_get(std::vector<std::string> &cmd, Buffer &out) {
    // ...
    return out_str(out, val.data(), val.size());    // string value
}
static void do_set(std::vector<std::string> &cmd, Buffer &out) {
    // ...
    return out_nil(out);                // nil
}
static void do_del(std::vector<std::string> &cmd, Buffer &out) {
    // ...
    return out_int(out, node ? 1 : 0);  // the number of deleted keys
}

/*
+ Handle the message header:
Now that we're outputting the response body to Conn::outgoing. 4 bytes in the response header that's going to indicate the response size.
*/
static bool try_one_request(Conn *conn) {
  // ...
  size_t header_pos = 0;
  response_begin(conn->outgoing, &header_pos);
  do_request(cmd, conn->outgoing);
  response_end(conn->outgoing, header_pos);
  // ...
}

/*
Add the response header to indicate payload size:
1. Store the position of the reserved header in header_pos 
2. Append 4 bytes of the output, but we're appending the bytes representing 0
*/
static void response_begin(Buffer &out, size_t *header) {
  *header = out.size();
  buf_append_u32(out, 0);
}

/*
Calculate the size of the response? Actual payload?
*/
static size_t response_size(Buffer &out, size_t header) {
  return out.size() - header - 4;
}

/*
It seems this function is responsible for constructing the message header.

Step by step:
1. Calculate the size of the response, the message we're sending the user.
2. If the size of our response isn't bigger than k_max_msg:
  a. Resize the output buffer to be the size of the header + 4? The header is 4 bytes, so total of 8 bytes?
  b. Probably append to the response that the response is too big.
  c. Re-calculate the message size. 
3. Cast size_t into 32 bit integers for the message size. 
4. Copy the 4 bytes for len into the output buffer's header position.
*/
static void response_end(Buffer &out, size_t header) {
    size_t msg_size = response_size(out, header);
    if (msg_size > k_max_msg) {
        out.resize(header + 4);
        out_err(out, ERR_TOO_BIG, "response is too big.");
        msg_size = response_size(out, header);
    }
    // message header
    uint32_t len = (uint32_t)msg_size;
    memcpy(&out[header], &len, 4);
}

/*
A helper function to do_keys(), which lists all of the key's in your Redis store. Then hm_foreach() invokes the cb_keys function on each HNode* in our hashmap:
1. Cast our void* into a Buffer*. 
2. Do pointer arithmetic to get the entry struct for each HNode* and extract the key.
3. Copy the key into the output buffer and return true that the operation succeeded I guess.
*/
static bool cb_keys(HNode *node, void *arg) {
    Buffer &out = *(Buffer *)arg;
    const std::string &key = container_of(node, Entry, node)->key;
    out_str(out, key.data(), key.size());
    return true;
}

/*
So this represents the "keys()" command in Redis. For each hashmap, iterate through all entries.
*/
static void do_keys(std::vector<std::string> &, Buffer &out) {
    out_arr(out, (uint32_t)hm_size(&g_data.db));
    hm_foreach(&g_data.db, &cb_keys, (void *)&out);
}
```

## Takeaway
