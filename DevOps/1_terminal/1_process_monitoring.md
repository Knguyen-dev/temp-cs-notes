# Process Monitoring

### What is it?
The monitoring of processes and systems ot make sure they're working properly. We can do this to identify anomalies, bottlenecks, and see real time activity. Process monitoring toools should do:
- CPU usage
- Memory consumption
- I/O operations and thread activity.


### LSOF (List of Open Files)
A common one that you're going to use is `lsof` (list of open files), a linux command that finds out which files are open by a given process. In Linux, stuff related to processes are represented as files, even if they aren't files, the OS translates their information into "virtual" whose contents will change depending on when you open it. So like pipes, sockets, directories, and devices are considered as files.

#### Example 1: Intro

```bash
# List the processes that are using the /run directory?
lsof /run

COMMAND    PID      USER   FD      TYPE     DEVICE  SIZE/OFF       NODE NAME
init         1      root  cwd      DIR      253,0      4096          2 /
init         1      root  rtd      DIR      253,0      4096          2 /
init         1      root  txt      REG      253,0    145180     147164 /sbin/init
init         1      root  mem      REG      253,0   1889704     190149 /lib/libc-2.12.so
init         1      root   0u      CHR        1,3       0t0       3764 /dev/null
init         1      root   1u      CHR        1,3       0t0       3764 /dev/null
init         1      root   2u      CHR        1,3       0t0       3764 /dev/null
init         1      root   3r     FIFO        0,8       0t0       8449 pipe
init         1      root   4w     FIFO       0,8       0t0       8449 pipe
init         1      root   5r      DIR       0,10         0          1 inotify
init         1      root   6r      DIR       0,10         0          1 inotify
init         1      root   7u     unix 0xc1513880       0t0       8450 socket

# What files does the the process with ID 890 use?
lsof -p 890
```
- **Command:** The name of the command or process.
- **PID:** Process ID (PID) of the process.
- **USER:** THe owner of the process.
- **DEVICE:** The device number of the file system.
- **SIZE/OFF:** Size of the file or the offset in teh file.
- **NODE:** Node description of the local file; this could be teh number of the local file. Basically it's used to idneityf the file.
- **NAME:** Path to the file.
- **FD:** File descriptor definition. You'll see stuf like "cwd" for current working directory. Tells you how the file is being used.
- **TYPE:** Type of file. It could be REG for regular file, could be a directory, or buffer or something else.

```bash

# List all network connections "LISTENING & ESTABLISHED"
lsof -i

# List the process that 's using port 8080
lsof -i :8080

# Narrow things down to specific states. To clarify, listen beings it's listening for connections whilst established means it's currently handling a request. It's in an active two way connection doing a data exchange.
lsof -i -sTCP:LISTEN

# Query all processes with a process ID of 1
lsof -p 1

# Lists all files that are opened by processes belonging to user knguyensky
lsof -u knguyensky

# Find out what files a user is looking at whawt the last command they ran
lsof -i -u knguyensky

# Find processes running ona specific port, network port 22 in this case
lsof -i TCP:22

# List only IPv4 and IPv5 opened files 
lsof -i 4
lsof -i 6

# Top is a live task manager in the terminal.
top 

# PS gives you a snapshot of currently running processes. 
ps

# Here's a command to get the memory for a single process.
ps -p <PID> -o %mem,rss,cmd
```

## Credits: 
- [LSOF Tutorial](https://www.redhat.com/en/blog/analyze-processes-lsof)
- [LSOF Manual](https://man7.org/linux/man-pages/man8/lsof.8.html)