# Endian-ness Explained

### What is Endian-ness
Endianness is the byte order used to represent multi-byte data (like integers) in memory. Let say we want to store a 4-byte hex number:
```bash

# Hex representation; 0x means the number after 0x is a hexadecimal literal
0x12345678

# In binary; remember each hex is just four bits; use that to convert
# But how are these 4 bytes ordered? Why does it even matter? Just order them like how we calculated?
00010010 00110100 01010110 01111000
```

| Byte Order        | Meaning                            | Memory Order (lowest address first) |
| ----------------- | ---------------------------------- | ----------------------------------- |
| **Big-Endian**    | Most Significant Byte (MSB) first  | `12 34 56 78`                       |
| **Little-Endian** | Least Significant Byte (LSB) first | `78 56 34 12`                       |

With Big-endian, the byte `12` that contributes most to the decimal sum comes first. This is how we originally saw that hexadecimal. However with little-endian, the byte that contributes the least to the decimal value comes first.

Also reversing the byte order is called a ***byte swap***.

### Why this even matters?
Different use-cases will need you to know how to order the bytes you send over:
- Networking (TCP/IP): Uses big-endian (network byte order), and you often need to convert with `htonl()` and `ntoh()` in C.
- Cross-platform file formats: Must specify endianness so everyone interprets the data the same way.
- Binary protocols (e.g. Redis, DNS)

### History Lesson: Why is this even a problem?
Early mainframes used big endian since it was intuitive. It matched the hex and decimal representations, making it feel natural.

However Intel's 8086 processor used little endian due to better performance and simplicity in lower level operations. Now we have both bit and little-endian machines, which results in a compatibility nightmare when exchanging binary data across platforms. This is a problem because if you send `0x1234` and they read it as `0x4321` those are going to evaluate to different decimal numbers. That's bad.

Now imagine early developers compiling their code to binary, on a big-endian machine, and executing that code on a little-endian machine. It wouldn't work, it'd be garbage unless byte-swapping is done.

### How to Byte Swap
Functions `htonl()`, `htons()`, `ntohl()`, and `ntohs()` are used in network programming in C to handle byte order (endianness) when sending or receiving binary data over a network

| Function  | Meaning                            | Use case                         |
| --------- | ---------------------------------- | -------------------------------- |
| `htonl()` | **Host TO Network Long** (32-bit)  | Convert `uint32_t` to big-endian |
| `htons()` | **Host TO Network Short** (16-bit) | Convert `uint16_t` to big-endian |
| `ntohl()` | **Network TO Host Long** (32-bit)  | Convert from network to host     |
| `ntohs()` | **Network TO Host Short** (16-bit) | Convert from network to host     |

Remember that network protocols use big-endian (also known as network byte order), but most PCs (like those with Intel or AMD CPUs) use little-endian.

These functions (`htonl`, `htons`, etc.) will swap the byte order if necessary, based on your system's native endianness.

When byte-swapping, you're literally reordering the bytes, which changes the way the value is represented in memory — and therefore its decimal interpretation if read directly.

However, the goal is for both machines to interpret the bytes the same way. So if each system converts values to network byte order before sending, and then back to host byte order after receiving, they will agree on the correct value, regardless of their native endianness.

### One Last Example
Client wants to connect to:
- IP: `192.168.1.100`
- Port: `1234`

```C
 // stays as 0x04D2 → stored as [0xD2, 0x04] (little-endian)
addr.sin_port = 1234;          

// stored as [0x64, 0x01, 0xA8, 0xC0] (little endian)
addr.sin_addr.s_addr = 0xC0A80164; 
```
The network infrastructure interprets the IP and port different because it interprets the IP as:
- IP: `100.1.168.192`
- Port: `53764`

Completely wrong thing.