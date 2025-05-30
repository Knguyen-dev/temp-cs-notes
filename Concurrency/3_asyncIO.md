# AsyncIO Notes

### Review on synchronous and asynchronous 
- Synchronous: You do tasks one after another. Do A, then do B, then do C. You can only do a task when all the tasks before it are done.
- Asynchronous: You can have a task run in the background while your main code still runs. As a result you program can handle multiple tasks at the same time, without needing to wait for each other.

You're going to probably use these packages:
- AsyncIO: For dealing with IO tasks that involve a lot of waiting, rather than calculations.
- Threads: Good for parallel tasks that share data with minimal cpu use.
- Processes: For maximizing performance on cpu intensive tasks.

### Event Loop
A central hub that manages and executes tasks. Allowing multiple tasks to be executed, seemingly in parallel. A task goes into the event loop and it's executed immediately or hangs. If it hangs, then it still stays within the event loop, but another task will enter the loop to be worked on. Then once it stop hanging, then the event loop will work on it. Once all tasks are completed, the event loop will close.

Everything runs in the main thread, no thread switching involved. Apparently here's how it works:
1. Main code runs utnil it yields (returns or awaits something)
2. Then the event loop takes control:
  1. Checks if any IO is ready 
  2. Runs the callbacks/coroutines on that data.
3. These tasks run until they await something, and then the control goes back to the main main code.

This is called cooperative multitasking, where tasks voluntarily give up control.

### Synchronous vs Asynchronous web servers
- **WSGI:** It handles requests in a blocking manner, so one request per worker thread/process at a time. To handle multiple requests concurrently, the WSGI server spawns multiple worker threads, each capable of handling one request at a time. So if there were 15 requests made at the same time, it'd spawn 15 threads to handle the situation. Since each request blocks a wokrer, WSGI works best when handling CPU-bound tasks (e.g. database queries or calculations). However it struggles with long-lived connections (e.g. WebSockets, background tasks).
- **ASGI:** It uses an event loop so that a single worker thread/process could handle multiple requests concurrently. If a request involves an async operation (e.g. database IO, external API call), where we'd be waiting, the server will work on another request whilst the former is still idle. This type supports WebSockets, background tasks, and the streaming of data.

#### Relating to threads 
For CPU-heavy workloads, having dedicated worker processes (not just threads) allows each core to focus on one computation-intensive task at a time, making WSGI a natural fit. You use that one worker to focus on the CPU intensive task, and I mean as a result it won't take as long. I mean it's better than the alternative of trying to context switch wen you have a cpu bound task because well you're probably not going to get that task done in a reasonable amount of time.

Problem with WSGI: Since each worker blocks while waiting, it cannot handle another request during that time. This leads to inefficiency when dealing with many I/O-heavy operations. Because we're waiting ALOT, when we could be using our resources to work on something else whilst that waiting is happening.


#### When do I use one over another?
- Use WSGI if your app is a traditional CRUD app with mostly synchronous database queries.
- Use ASGI if you expect t obe dealing with these:
  - WebSockets (e.g. ral time updates)
  - Background async tasks 
  - Streaming responses (e.g. live video feeds, streaming data)


### Co-Routines
- Coroutine Function: A function with async  
- Coroutine Object: An object returned from a co-routine object. Then you'd use this to run our async function. This co-routine object needs to be awaited in order for us to actually get the result of its execution.
- Await keyword: Can only be used inside a co-routine function.

```Python
import asyncio
# Co-routine function

async def main():
  print("Start of main co-routine")
# main -> Co-routine object 

asyncio.run(main())
```

#### Example 1: Scheduling a task 
A task is a way to schedule a co-routine to run as soon as possible, allowing us to run multiple co-routines to run simultaneously. Let's show an inefficient implementation first:

```Python

async def fetch_data(delay, id):
  print(f"Fetching data: {id}")
  await asyncio.sleep(delay)
  print(f"Data fetched: {id}")
  return {"id": id}

async def main():
  task1 = fetch_data(2, 1)
  task2 = fetch_data(2, 2)
  result1 = await task1
  print(f"Received result1: {result1}")

  result2 = await task2
  print(f"Received result2: {result2}")
```

Before, you'll await a co-routine, and then once that's finished you can then await the next co-routine. That takes a total of 4 seconds, and things are essentially synchronous with how we set it up.

A better implementation would be as a co-routine is sleeping, waiting, or hanging on something that's not in control of our program, then we can switch to another task. As a result, we're telling our program to work on something else while a task is waiting on something. So this is like the idea of multithreading that we talked about in previous sections.
```Python

async def fetch_data(delay, id):
  print(f"Fetching data: {id}")
  await asyncio.sleep(delay)
  print(f"Data fetched: {id}")
  return {"id": id}
  
async def main():
  task1 = create_task(1, 1)
  task2 = create_task(2, 3)
  task3 = create_task(3, 1)
  
  # Start task 1; if this isn't doing anything, another task will take over. 
  result1 = await task1

  # Start task 2; 
  result2 = await task2
  result3 = await task3
  
  print(f"Received result1: {result1}")
  print(f"Received result2: {result2}")
  print(f"Received result3: {result3}")

  # Wait for all tasks to finish before printing
  print(result1, result2, result3)
```

#### Example 2: Gather Function
Just a quick way of creating tasks, and then awaiting to get their values.

```Python
async def fetch_data(delay, id):
  print(f"Fetching data: {id}")
  await asyncio.sleep(delay)
  print(f"Data fetched: {id}")
  return {"id": id}

async def main():  
  # 3 tasks or threads
  results = await asyncio.gather(
    fetch_data(1,2),
    fetch_data(2,1),
    fetch_data(3,3)
  )

  # Print the results
  for result in results:
    print(f"Received result: {result}")

asyncio.run(main())
```
This has bad error handling though, if one fails, it doesn't stop anything else. You should look at TaskGroup if you want to be able to execute multiple tasks concurrently

---
#### Example 3: Locking 
What if two co-routines are writing to a file concurrently. We can have a lock to prevent that from happening, ensuring that only one thread is affecting that resource. As a result, only one thread is writing to the file.


## Credits
- [AsyncIO in Python - TechWithTim](https://youtu.be/Qb9s3UiMSTA?si=HqAklpoZ1iPWA4qA)
- [AsyncIO and the Event Loop Explained](https://youtu.be/RIVcqT2OGPA?si=EhaLWgGrq56iA2xu)
