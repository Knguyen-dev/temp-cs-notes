# Networking tools

### What do you mean?
Networking tools are really useful. Stuff for connecting to a remote desktop with SSH, wireshark for packet analysis, or just ping for testing basic connectivity. 

### 1. Ping
Quickly check if your computer can reach another computer.

```bash
ping <hostname or IP address>

# Ping Google's open DNS server, and this is commonly used to check if your 
# computer can connect to the internet.
ping 8.8.8.8
```

### 2. SSH: Secure Shell
Used for connecting into a remote desktop

```bash

ssh <your_username>@<remote hostname or ip address>

# Or just 
ssh <host_name or ip_address.>
```
Sometimes the host will authorize user, so that connecting via SSH won't require password input.

### 3. IP:

```bash

# Shows information for the network devices on the machine.
ip address

# This means your machine has 2 network devices essentially. So one for ethernet
# and the loopback thing just allows your computer to talk to itself without needing the other network interfaces to be up.
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet 10.255.255.254/32 brd 10.255.255.254 scope global lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host
       valid_lft forever preferred_lft forever
2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc mq state UP group default qlen 1000
    link/ether 00:15:5d:a9:ee:53 brd ff:ff:ff:ff:ff:ff
    inet 172.31.74.52/20 brd 172.31.79.255 scope global eth0
       valid_lft forever preferred_lft forever
    inet6 fe80::215:5dff:fea9:ee53/64 scope link
       valid_lft forever preferred_lft forever

# Displays routing info; mainly unnecessary if you're not going to configure your IP
# addresses or manually configure routign.
ip route
```


### 4. Domain Information Groper (DIG)
Grabs info about a domain name. This is mainly sued for checking whether a domain name is resolving to the right IP address. Let's use dig to find the IP address of the domain name `google.com`

```bash
dig google.com

; <<>> DiG 9.18.30-0ubuntu0.22.04.2-Ubuntu <<>> google.com
;; global options: +cmd
;; Got answer:
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 59671
;; flags: qr rd ra; QUERY: 1, ANSWER: 1, AUTHORITY: 0, ADDITIONAL: 1

;; OPT PSEUDOSECTION:
; EDNS: version: 0, flags:; udp: 512
;; QUESTION SECTION:
;google.com.                    IN      A

;; ANSWER SECTION:
google.com.             282     IN      A       142.250.190.110

;; Query time: 30 msec
;; SERVER: 10.255.255.254#53(10.255.255.254) (UDP)
;; WHEN: Sun Apr 13 14:51:36 EDT 2025
;; MSG SIZE  rcvd: 55
```
In the "answer" section you'll find the IP address of google.com, and "A" indicates that it's an IPv4 address. Well now that you can that "google.com" has answers, you at least know the domain name is actually mapped to an IP address. Since our main goal is just figuring out the IP, we could just do:
```bash
dig +short google.com

# See if a mail server is at the domain name
dig google.com MX
```

### 4. SCP: Secure Copy Protocol?
For copying files between servers in a secure way. It's only used for transferring files, and it's non interactive meaning you can only use it as commands. SFTP is more interactive, and allows you to do thing ssuch as create or delete directories and files.

```Bash
# General form
scp [option] [user_name@source_host:path/to/source/file] [user_name@target_host:target/path]

# Copy a file from local to remote
scp desktop/sample.txt root@136.183.142.28:/home/remote_dir

# Copy file from remote to local; so just the same idea but switched
scp 147.182.143.27:/home/remote_dir/sample_example.txt home/Desktop
```

### 5. SFTP: Secure File Transfer Protocol
Basically an interactive version of SCP. You're using SSH encryption to transfer files between systems. You may get this confused with FTPS (File Transfer Protocol Secure). They achieve the same mission but in different ways. So SFTP just creates one single port connection that it's going to send files through, whilst FTPS is going to create a new port for each file transfer request. This can cause issues with firewalls, and the solution just means extra work, so many choose SFTP to save headache times.

```bash
sftp [username]@[remote hostname or IP address]

# Downloads a file from the remote directory to my current local directory
get fileName

# You could download the remote file and name it something else on the local
get /path_to_remote_file.txt local_copy.txt

# Uploading files from local to remote
put local_file.txt
put ./local_file.txt /remote/target/path
```





## Credits
- [Top 5 Linux Netowkring Commands - Bit Goblin](https://www.youtube.com/watch?v=MT85eMsMN6o)
- [SCP Command in Linux](https://phoenixnap.com/kb/linux-scp-command)
- [60 Linux Commands that you should know - NetworkChuck](https://www.youtube.com/watch?v=gd7BXuUQ91w)