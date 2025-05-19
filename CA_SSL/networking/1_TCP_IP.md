# TCP/IP

### What is IP (Internet Protocol) and IP Addresses?
It's a set of rules, for routing/sending and addressing packets of data, so that these packets can across across networks and reach the right destination.

Data traversing the internet is divided into smaller pieces called **packets**. In each packet, it has information on the source and the destination IP addresses, helping routers process and send these packets to the right place. 

#### What are IP addresses?
Every device connected to the Internet is assigned an IP address, think of it as the mailing address (unique identifier) where packets can be sent to. Each IP address is a series of characters (e.g. 192.168.1.1). Via DNS resolvers, these IP addresses are translated into human-readable domain names, allowing us to type in something like `www.walmart.com`, and be directed to the right website. 

- **IPv4:** Old standard for representing IP addresses. However just like there are only so many license plates, there's only a limited amount of IPv4 addersses.
- **IPv6:** A new standard that has more characters and can represent more IP addresses. However this is not completely adopted as most domains and devices still have IPv4 addresses.

#### "IP packets" and more on packets specifically?
The official name for these data packet sare IP packets. Beyond storing the data to be sent, packets have a header:
- Source and destination IP addresses
- Transport protocol being used (TCP or UDP)

Among other information. Then there's the actual payload of the packet which contains the actual data being sent. 

#### How IP routing works?
The internet is made of large networks called **autonomous systems (AS)**. Basically routers have "routing tables" that indicate which networks the packets should travel through in order to reach the destination as quickly as possible. The packets will travel until we reach an AS that contains the destination IP address.

You should note that when being sent, packets are sent out in a random order. However the packet header contains information on how to re-order the packets so that the data is read correctly. As well as this, packets won't travel the same route every time, there could be some variation.

One last thing is that different layers of the "OSI model" will assign different headers. So just know that at the **Application level**, we have our actual data being sent. However we'll have different headers like the IP header (contains IP data), and TCP/UDP header (contains transport related data). Just a heads up.

#### What do you mean "datagrams"?
A datagram is a segment of data that's sent over a packet-switched network. It has enough info to be routed from source to destination. The takeaway is that there are multiple types of packets. A datagram is the type of packets we're talking about here, IP and UDP. Later we may talk about other types of packets.

**Note:** When someone says "packet-switched network" they just mean a network that transmits data using packets. Which should be the normal. Also when we say that IP is a "connectionless protocol", we mean that IP sends each packet from source to destination, but it doesn't handle error checks (acknowledgements) or anything. That's where TCP has to come in.

### TCP/IP (Transmission Control Protocol)
A transport protocol that dictates how data is sent and received. TCP is here to make sure that the packets are ordered correctly so that we can read the data correctly. And it adds some error checking to make sure none of our packets were lost in transit, and if they were a request to resend those packets is sent. The typical flow that happens with TCP:
- TCP opens and starts listening for connections. When packets are sent, TCP makes sure they're ordered.
- The recipient acknowledges each packet that arrives. If some packets aren't acknowledged, then we know they're missing so the client will resend them.

TCP is designed for reliability, rather than speed. It has this check that all packets arrive in order, and things can take longer since you're waiting for all packets to arrive. 

#### TCP Handshake (Important)
You send an email to someone. When an email is sent over TCP, a connection is established and a 3-way handshake is made:
1. Client sends an SYN (initial request) packet to the server.
2. Server sends a SYN-ACK packet, agreeing to the connection.
3. Client sends an ACK packet to the server to confirm. Now the connection is established and the data is sent over. The email is split into packets, traversing until it finds the server, and then is reassembled by TCP into the original contents of an email.

Here's how they terminate the connection:
1. Client sends finished packet. Server sends acknowledgement packet.
2. Now the server sends a finished packet, then the client sends the acknowledgment.
3. TCP connection is now terminated, so now more data transfer will happen. This is a four-way handshake.

**Note:** The reason you see TCP/IP together is because originally they were designed to be used together, so it's called the TCP/IP suite. However, other transport protocols were made. 

### UDP/IP (User Datagram Protocol)
Another widely used transport protocol. It's faster than TCP, but less reliable as it doesn't make sure all packets are delivered and in order. It doesn't even establish a connection for sending or receiving data.

TCP is more used when reliability is critical. Such as in HTTP/HTTPS, in email protocols, file transfer, SSH, and database connections. UDP is used when we want speed and can tolerate occasional data loss. So you'd see this in video streaming voice calls, online gaming, etc. Notice how in those examples we can have dropped frames, lag, etc. Most people don't work with TCP or UDP at the byte level, but you're working with higher level abstractions that build on them.

#### Explaining ports and sockets
When packets are routed to a computer, it doesn't just go to the entire computer. It goes to a specific program/process on the computer. So a specific program on the sender sends the data, and a specific program on the recipient receives the data. If we send data to port 443, the program that's exposed or running on port 443 of the host machine gets that data. 

**Sockets** represent the connection betwee ntwo machines over a network. On the server side, a socket is created by binding an IP address and a port number together (`0.0.0.0:443`). Then this socket listens for incoming connections. On the client side, a socket is created to connect to the server's IP and port. 

Think of ports as doorways whilst the sockets are the entire tunnel, a combination of IP, port, and protocol that lets data travel between programs on different machines.

## Credits
1. [Internet Protocol - Cloudflare](https://www.cloudflare.com/learning/network-layer/internet-protocol/)
2. [TCP/IP - Cloudflare](https://www.cloudflare.com/learning/ddos/glossary/tcp-ip/)

