# Pipes and Processes

### General Introduction
- **Process:** A program. It could be something as complex as Microsoft word, or your friendly neighborhood python script.
- **Stdin, stdout, and stderr:** Standard input is when you read input from the keyboard. Standard output and standard error are what happens when you print to the screen, or when you see an error get printed on the screen. They're common streams in computer programming.

- **Stream:** An abstraction for a series of bytes moving from one place to another. It hides the complexity of how data physically moves. Internally streams are often built on top of lower level mechanisms like sockets (for network streams), pipes (interprocess communication), and files (for File I/O). When I say it's built on top of, I mean that the stream itself doesn't move the bytes, but calls lower level operations that do the real work (e.g. sending data through a socket, reading from a file, etc). Think of it as an interface or API that lets you do higher level operations, without needing to know the underlying implementation.

```Java
InputStream in = new FileInputStream("file.txt")
```
- `InputStream` is the stream, the abstract interface that we are talking about. `FileInputStream` is used for this specific situation since we're dealing with files, but if you were dealing sockets and networks, you may use a different `InputStream` implementation. The idea is that `FileInputStream` is going to call lower level functions to read data from that file. You as a developer don't need to know or understand the low level implementation, you just need to call `.read()` and the abstraction reads data into your program. 

### What are Pipes; Interprocess Communication
Sometimes we need our program to communicate with another program. This commonly happens with webapps, when you need to communicate with an API, but in this context we mean a process sending and or receiving data from another process. An example of this is that let's say you had a python program `visualize.py` that can visualize data. This python program accepts data from a binary called `calculate_fluids.exe`, which could be some scientific program that calculates fluid information really quickly and effectively. We can run the binary from our python script, putting in some data, and receive that binary's output data. Then our python script we plug in that output data, and create some graphs for us. That is the core of **interprocess communication**, allowing processes to send and receive data inbetween each other. 

- **Subprocesses:** In the above siutation, we can say that the main process was the `visualize.py` as it calls/executes the `calculate_fluids.exe` binary. So this binary acts as like a helper program, and as a result we'd call it a **subprocess**. 

A **pipe (pipeline)** is a mechanism for interprocess communication using message passing. So basically, the sdout of one process can be passed directly as the input (stdin) to the next process. Or another example is the idea of sending or receiving data from a program being run as a subprocess. So when you hear someone saying that they're "piping data to a subprocess", it just means they're programmatically sending data to another program via that other program's stdin.







## Credits:
- [Stdin, stdout, and stderr](https://learn.microsoft.com/en-us/cpp/c-runtime-library/stdin-stdout-stderr?view=msvc-170)
- [Pipelines - Wikipedia](https://en.wikipedia.org/wiki/Pipeline_(Unix))
- [Piping to a subprocess](https://www.gnu.org/software/libc/manual/html_node/Pipe-to-a-Subprocess.html#:~:text=15.2%20Pipe%20to%20a%20Subprocess,being%20run%20as%20a%20subprocess.)