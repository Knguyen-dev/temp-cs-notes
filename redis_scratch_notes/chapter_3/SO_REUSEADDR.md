# Explaining Socket Reuse Address

### Simple Explanation
If you don't set it then when you restart your server you'll get an error saying that the socket is already in use.

The key thing in TCP is being able to restart a closed process on the same address. The flag is needed because the port goes into a `TIME_WAIT` state to ensure all data is transfered. If two sockets are bound to the same interface and port, data will be delivered to both sockets.

Thi sis 

### Situation
TCP's selling point is reliability (protection against packet loss), packet reordering, but also protecting against packet duplication.

What happens when a packet sent right at the end of the conversation is duplicated and delayed. So the 4 way shut down happens packet is received right before the delayed-duplicated packet? 

The TCP stack should discard/drop that packet instead of using it when we re-open our connection. To handle this case we use a state called `TIME_WAIT`. After closing, the side that sends the final `ACK` enters `TIME_WAIT`, which often 30 seconds. It prevents old packets from being misinterpreterd was part of the new connection.

So if a late packet arrives the OS will recognize it's for a recently closed connection, drops the packet silently. It won't reopen the connection or send back data to the client.



### Example
Let’s say:
- Client sends: Hello
- Server sends: Bye
- Both sides close connection via 4-way FIN
- Then a delayed, duplicate Hello from earlier shows up at the server

If the server is in TIME-WAIT, it drops the packet. If it's not in TIME-WAIT anymore (e.g., due to misconfiguration), then this could cause confusion or data corruption — which is why TIME-WAIT exists.

### Takeaway
We want to sleep or wait on that socket for a certain amount of time, leaving it in a `TIME_WAIT` period. In this period, this IP/port combo can be reused. 


## Credits:
- [Simple Explanation](https://stackoverflow.com/questions/577885/what-are-the-use-cases-of-so-reuseaddr)
- [Complex and Detailed Explanation](https://stackoverflow.com/a/3233022)