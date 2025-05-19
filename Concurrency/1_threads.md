# Threads 

### Premise and Intro
Let's say we have a CPU with 4 cores. The amount of cores you have equate to the exact number of operations you can do at the exact same time.e We have 4 cores, so at most we can do 4 operations at the same time. The idea of doing things at the same time is called **parallelism**. E.g. we have 4 cars driving in their 4 respective lanes (they're not blocking each other). If you had a bunch of cars behind each other, they can only go as far as the car in front.

Then when we hear about clock speed e.g. 2.6ghz, which just means each core can do 2.6 billion cycles per second. And remember we can do 1 to 4 instructions per cycle depending on the computer architecture.

However, in a computer, there are more than 4 operations. So how do we use our 4 cores to take care of these various operations, processes, or tasks that are happening on the computer?

### What are threads? Switching between (Concurrency)?
A thread is just a set of operations that we need to get done. Every thread is going to be assigned to 1 core, so that the core can execute and finish the work indicated by that thread. However, a CPU core can only execute one thing at a time. What if there are two threads on it? To be efficient, the CPU core will do some work on thread A, and then switch to do some work on thread B. It switches very quickly, but as a result we're able to finish these 2 threads "at the same time". This switching of threads is called **context switching**.

  - **Note about threads:** Technically it's a data structure that stores instructions that a process/program wants to execute.

The motivation for this relates to the idea of **hanging threads**. A hanging thread is one that's not really doing anything at the moment. E.g. waiting for an API response to get the data. The thread had some work with sending an API call, but at the moment we're not doing much work since we're waiting for data. Let's get some other work done while we wait for that data. The CPU core will switch and work on other threads whilst we're waiting (no work or instructions). This idea of our CPU cores handling multiple threads at the  time by quickly switching between them and doing work is called **multithreading**, a subset of **concurrency**.


#### Example 1
```Python
def fetchData():
  time.sleep(10)
  print(1)

def doCalc():
  print(2)

fetchData()
doCalc()
```
If we executed this entire program on one thread (which is how it normally is), we would do `fetchData()` 10 seconds pass and we get 1. Then a millisecond after 2 is printed. It's easy to see that `fetchData()` makes us do a lot of waiting (a hanging thread), and in that time we could be doing `doCalc` which is a quicker operation. Now with multithreading, here's the flow:
1. Execute `fetchData()`, after a couple moments we realize there's no more work here, so switch to the thread containing `doCalc()`'s instructions.
2. Do work and finish `doCalc()`.
3. Switch back to `fetchData()`, wait until we have the data, that means we have work, and print the data.

This is a lot more efficient.

#### Example 2
```Python
import threading 
import time 

def fetchData():
  print("ran")
  time.sleep(1)
  print("done")
  time.sleep(0.85)
  print("Now done")
x = threading.Thread(target=func)
x.start()
print(threading.activeCount())
time.sleep(1.2)
print("finally")
```
1. Calls `fetchData()` outputs `ran`. It sleeps so we switch back to the main thread.
2. Print the active number of threads and then sleep. No work on main thread so switch to thread x.
3. We print done and sleep for 0.85 seconds. Now the main thread is done with its waiting of 1.2 seconds and outputs "finally". Main thread is done so the core does work on thread x.

#### Example 3: Synchronizing threads

```Python
def validateInventory():
  time.sleep(1)
  print("Inventory Validated")

def validatePayment():
  time.sleep(1.5)
  print("Payment was good")

def handleCheckout():
  time.sleep(1.2)
  print("Checkout was handled")

x = threading.Thread(target=validateInventory)
x.start()
y = threading.Thread(target=validatePayment)
y.start()
z = threading.Thread(target=handleCheckout)
z.start()

# Wait until all these threads finish before moving on
x.join()
y.join()
z.join()

print("Thank you for ordering!")
```

### Takeaway (Parallelism vs Concurrency)
- **Concurrency:** The idea that we're managing many things at the same time. It doesn't always mean that we're doing them all simultaneously, it could be the idea of switching between tasks. 
- **Parallelism:** This is the idea of doing multiple things at the same time. 
- **Multithreading:** This is concurrent but not parallel. We can only do one thread/task at a time, but we can switch in between them very fast. 


## Credits
- [Concurrency, Threading, and Parallelism Explained Pt.1 - Tech with Tim](https://youtu.be/olYdb0DdGtM?si=zIPcCPbQ4Y_1D7t2)
- [Threading explained in Python Pt. 2- TechWithTim](https://youtu.be/cdPZ1pJACMI?si=Ks7wldv0vSvCoIAM)
- [Concurrency vs Parallelism - ByteByteGo](https://youtu.be/RlM9AfWf1WU?si=O07Z1u1_Q3fnoXVV)
