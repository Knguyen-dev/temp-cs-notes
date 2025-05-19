# Performance Monitoring

### What is it?
The idea of seeing how efficient our processes are,. We'll use toosl to trakc metrics, such as throughput, latency, and hopefully find bottlencks.

### Virtual Memory with vmstat
This tool reports things like memory, IO, CPU, and other stuff. The first time you run this command, it gives you an average of the info it got before the reboot. Then following commands try to get the most recent stuff.

```
vmstat[options][delay [count]]
```
- options that you can give to display different information:
  - `-a`: Gives you the active and inactive memory of the system
  - `-s`: Displays various memory statistics as well as CPU and IO event counter.

- delay: Time interval between updates. If no delay is specified, it gets the average data since the last reboot.
- Count: Defines the number of updates printed after the given delay.

```bash
vmstat

# You get information about processes, memory, swap, IO, system, and CPU.
procs -----------memory---------- ---swap-- -----io---- -system-- ------cpu-----
 r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st
 1  0      0 6012852   2120 817048    0    0  2805   289  797  657 21  7 71  1  0
```
- **Procs:**
  - r: The number of runnable processes (running or waiting for runtimes)
  - b: The number of processes in an uninterruptible sleep.
- **Memory:**
  - swpd: The amount of virtual memory used.
  - free: The amount of idle memory.
  - buff: Amount of memory used as buffers.
  - cache: Amount of memory used as cache.
  - inact: Amount of inactive memory (-a option).
  - active: Amount of active memory (-a option).
- **Swap:**
  - si: Amount of memory swapped in from disk (/s).
  - so: Amount of memory swapped to a block device (/s).
- **IO:**
  - bi: Blcoks received fro ma blcok device (blocks/s).
  - bo: Blocks sent to a block device (blocks/s).
- **System:**
  - in: The number of interrupts per second, including the clock.
  - cs: The number of context switcher per second
- **CPU:**
  - These are just percentages of total CPU time.
  - us: The time spent running non-kernel code. (user time, including nice time)
  - sy: Time spent running kernel code. (system time)
  - id: Time spent idle. 
  - wa: Time spent waiting for IO.
  - st: Time stolen from a virtual machine.

#### Examples!
```bash

# If si and so are non-zeor the sytem is using swap. If so is frequently > 0, then
# your system is actively swapping out memory, and that's really slow
vmstat 1 5

# If your program is slow at doing computations look at:
# us: The user cpu time, the time spent on the app
# sy: The system cpu time, stuff that the kernel needs to do. 
# wa: This is IO wait. If this value is high (30%-50% or more), then your CPU is 
# often idel and waiting IO stuff. This could be due to a slow hard-drive, bad network, etc.

# If the 'cs', number of context switches per second, it could be inefficient threading or that you're running too many processes. You should als ocheck 'r' which is the run queue that indicates the enumber of processes that are waiting to run.
```


## Credits
- [vmstat - Red Hat](https://www.redhat.com/en/blog/linux-commands-vmstat)
- [More vmstat - Phoenixmap](https://phoenixnap.com/kb/vmstat-command)