
## OSI Model
### What is the OSI Model?
An academic and conceptual framework that shows the various layers that are involved when sending data over a network. It's good in the academic sense and when working with others since it abstracts most of the complexities away.
- **Application Layer:** Concerned with the app itself and how it communicates. For example, browsers and websites communicate with HTTPS and HTTP. Or maybe a mail server communicates with SMTP and mail protocols. In any case, your typical software applications will directly rely on this.
- **Presentation Layer:** Preparing data to be used by the application layer. Concerned with the syntax of data and how it's sent and received. So like data could be sent as HTML, JSON, and CSV. This is where encoding occurs, compressing data to be sent, then decoding that data so that the client can use that data.
- **Session Layer:** Maintains connections and is responsible for controlling ports and sessions. 
- **Transport Layer:** Transmits data using transmission protocols including TCP and UDP. 
- **Network Layer:** Related to routing, forwarding, and addressing across a network or multiple networks. Transmits data using transmission protocols including TCP and UDP. The idea of segmenting data (split into packets), routing those packets to the destination, and then reassembling those packets back into their original content.
- **Data link Layer:** Defines the format of the data on the network. Technologies used to connect two machines across a network, where the physical layer already exists. We're data frames, digital signals encapsulated into packets. It's often split into two layers though:
- **Physical Layer:** Refers to the physical wires and communication medium that the data goes through. This includes fiber-optic cables, copper cabling, and air (wireless). 

### Practical Model: TCP/IP Model
The OSI model is more educational due to its abstractions, but the TCP/IP model is more commonly used in practice. This model has five layers physical, data link, network, transport, and application. It's more detailed, which is good. TCP breaks data into segments, and hands it off to IP. IP sends those segments through the networks. Then the TCP module in the receiver, reassembles those segments in order to get the original message. TCP/IP contains more protocols at different layers. It starts with the application layer, which creates the message, and then sends it to lower layers which encapsulate the message sent by an upper layer. The piece of data has different names depending on what layer we're on:
- Application: Application message
- Transport: TCP Segment or UDP Datagram
- Network: IP Packet
- Data Link: Ethernet frame

When the message reaches the destination each corresponding layer is responsible for unwrapping that message. When a client create sand sends a message, we go in this order:
1. Application
2. Transport
3. Network
4. Data-Link
5. Physical 

With application being the top level, most abstracted, and physical being literally digital logic and electrical signals. Now let's assume our client has sent an email, and we'll go through the layers from the bottom to the top!

#### Physical Layer
Let's start at the bottom-most layer. Data is represented as binary. The physical layer converts this binary sequence to different physical signals. Such as electrical, light or radio depending on the physical medium used. The most common medium is Ethernet, and that decides what type of cable you use.

#### Data-Link Layer
The data here is called an ethernet frame. We can divide this into two layers:

##### Medium Access Control (MAC) Sub Layer
On the sender's side, the MAC layer would wrap the IP packet given from the network layer with a header and trailer:
- Header: Contains a MAC address of the sender and receiver. This is a 6 byte UID embedded in the network interface card of a computer. 
- Trailer: Error checking data for ensuring the ethernet frame is fine.

Think about CSMA/CD. When computers send data on a network line, it's ideal that we can send the data without blocking anyone else. However sometimes we'll have conflicts, with two pieces of data colliding. The computers will wait to resend the messages if this happens.

##### Logical Link Control (LLC) Sub Layer
- **Flow control:** Restricts amount of frames the sender can send so that the receiver isn't overwhelmed. It makes sure to tell the sender to slow down in order to avoid losing data.
- **Error control and retransmission:** Error detection protects against lost or damaged frames. If a frame is good, we'll send an acknowledgement (ACK) to the sender. So if there's no acknowledgement, then the sender knows to resend that frame!

Though, the transport layer provides all of the functionality here, so this layer is usually not used. However the core concepts of what it's doing transfers over.

#### Network Layer
On the sender side, the network layer passes TCP segments (or UDP datagram) to the network layer. The network layer does these:
1. Assigns each TCP segment with a header containing the source and destination IP addresses. This officially makes it an IP packet.
2. Routes those packets through the network, using the best path it can find.

It should be noted that routing is only needed when sending packets across different networks. If the source is destination is within the same network, we don't need to route. We'll just add on the MAC addresses of the source and destination, which is enough information to get the data to the destination.

To get more technical with routing, we're given an ethernet frame with IP addresses, data, and the source MAC. We aren't able to get the destination MAC since they're in different networks, so we'll just update the frame's MAC to the route's MAC. When the ethernet frame reaches the MAC, we see their MACs match, we unwrap the packet, check the IPs. IPs don't match, re-wrap the ethernet frame, but now update it's MAC address to the destination's MAC. This is possible because the router is connected to the destination network. Now the packet can be sent to its destination.In summary, the MAC addresses will change after every hop, but the IP addresses stay constant, and are used for comparisons.

#### Transport layer
For the sender, the transport layer can choose either TCP or UDP:
1. **TCP:** Supports segmentation, so can split up large data into TCP segments. It's reliable, so use it when you need error checking, ordered-data transfer, re-transmission, etc. However it's slower. To establish a connection, it does a 3-way TCP connection handshake. 
  1. **Error checking with packets:** The sender calculates a check-sum or string with the given data. The receiver is then supposed to re-calculate this. If it matches, well it's what the sender got, so our data should be good accept it and acknowledge. If our re-calculated checksum is different, our packet is wrong, don't send acknowledgement, and then the sender will resend that packet. 
  2. **Packet ordering:** TCP adds numbers to each packet to indicate their order, which helps the receiver reassemble the packets into the original IP packet. 
2. **UDP:** Doesn't support segmentation, so you need to send data that can fit in a UDP datagram. It lacks error checking and the ability to re-order packets. However it's fast, making it a good use case for stuff like video streaming, DNS, DHCP, SNMP, and RIP.

#### Application Layer
The top-most layer. User applications use the below protocols protocols to communicate and send data to each other. Chrome uses HTTP and HTTPS for people to surf. Outlook uses SMTP or POP to send mail, etc. Here are some protocols provided by the application layer: DNS, DHCP, FTP, HTTP, IMAP, IRC, POP3 SMTP

## Credits
- [TCP/IP Layer](https://youtu.be/2QGgEk20RXM?si=6fCc_2fNvVtqzaRJ)
- [What is the OSI Model - Cloudflare](https://www.cloudflare.com/learning/ddos/glossary/open-systems-interconnection-model-osi/)