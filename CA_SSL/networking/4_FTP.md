
# FTP and SFTP

### What is FTP (File transfer protocol)
Network protocol used for transferring files betwee na client and server. Using this, you can download, upload, and manage files on a remote system. 

FTP is insecure, it transmits data in plain-text. Instead always opt to use **SFTP (Secure File Transfer Protocol)**. This does the same thing, but is backed by SSH, allowing for traffic to be fully encrypted. This encrypts both the commands you're running and the data that's being transmitted, protecting against eavesdropping and MITM.