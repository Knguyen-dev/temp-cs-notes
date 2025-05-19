# Local Host 


### Intro 

`127.0.0.1` is normally the IP address assigned to the local-only interface. This is a network adpater that only communicates within the same host, and it's used when you want to serve clients on the same host. So a process listening on `127.0.0.1` will only receive local connections.

**localhost** is typically the hostname for `127.0.0.1`. However `0.0.0.0` has other meanings, but mainly it's the idea that a server can listen on every available network interface