# Multithreading and multiprocessing

### Reviewing what multithreading is
Python is multithreaded (context-switching is possible), but not simultaneously multithreaded, meaning two threads from the same process can execute at the same time. 

- **Process:** An instance of a program, such as an instance of firefox. It contains code and data contained as memory. Then it has at least one thread.
- **Thread:** Contains registers and stack, has access to data in the process. The OS will return when and how a thread will run.

A process can have multiple threads if it wants. However two processes will have separate code, data, and threads. 

---
### IO-Bound Tasks
For IO bound tasks, they spend most of their time waiting for external resource like disk I/O, network requests, or database queries to finish. These tasks aren't calculations, they don't need the CPU, so the CPU is often idle waiting for data to arrive.

- **Why is multithreading good for IO-bound tasks?** Since IO-bound tasks spend a lot of their time waiting, you can spend that time doing something else. Spin up other threads to perform different IO operations concurrently. While one thread waits for a response, another thread can be worked on. This at least keeps your CPU busy and improves your overall throughput.

---
### CPU-Bound Tasks
CPU-bound tasks require intensive computation, such as math calculations, video processing, or simulations. The bottleneck here is the **processing power of the CPU**, rather than waiting on IO.
- **Why doesn't multithreading help here?** Your CPU is already busy, by adding more threads, you're introducing more tasks that it has to switch to and work on. Not to mean the overhead of having to switch between the CPU-bound task which could be on one thread, and your other tasks on other threads, it could actually slow things down. For example, imagine you're solving a complex math problem, but every few minutes, someone interrupts you to do something else. Even know you're eventually going to be both tasks done, the constant interruptions will make it harder to stay focused on your original task, **reducing your efficiency**.
- **Using CPU parallelism on IO-bound tasks?** I mean sure you can now finish multiple IO tasks in parallel, but now you're using multiple CPU cores on tasks where they're not going to be fully utilized. You're essentially wasting multiple cores on your computer, as they're just going to be sitting idle waiting for a network/disk response.
- **How do you optimize a CPU-bound task?** Focus on adding more CPU cores and the idea of those cores working on that task in parallel. 

---
### Global Interpreter Lock and more
In Python, only a single thread can run at a time period. Doesn't matter if they're from different processes or whatnot because remember your Python program is like a process, and so only that one thread in the Python process can run. A thread needs to have a protected GIL to run. Again, this ensures only that single thread can run and ensures **thread safety**. Thread safety is just the idea that there won't be any **deadlocks**.

  - **Deadlock:** Happens when two or more threads or processes (threads in this context) get stuck waiting on each other to release a resource, preventing further execution. Normally one thread is executing, it releases so that the core can work on another thread. 

### Takeaway 
- When multiple processes are being used in parallel. So multiprocessing is key for executing multiple cpu intensive tasks that have little IO at the same time.
- While multithreading is used for managing multiple IO heavy tasks.
- Python is multithreaded. It's concurrent but not parallel meaning it can handle multiple threads at the same time, but it does switches which threads it's doing the work on.


## Creditss
- [Multiprocessing vs Threading - Dave's Space](https://youtu.be/AZnGRKFUU0c?si=ejOCe10J08PEDXWN)
