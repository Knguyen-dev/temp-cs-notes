# Caching


### What is caching
The idea of saving the results of an expensive operation in memory. As a result, if we want to do the same operation again, we can used the saved result and save a lot of time and resources. This is similar to the idea of memoization. Caching improves response times and can save server or database resources. For example, having a CDN to serve images instead of forcing your server to do it will save a lot of resources such as bandwidth. 

Another common case is saving database resources. An example could be how popular items on Amazon can cause a lot more reads and writes specifically for that item. You can use a cache to save the info of the item, and reduce the amount of times we need to access the database. 

One last thing we should know is that caches will usually have a TTL (time-to-live), which indicates how long a cache will store that information. Imagine caching someone's user profile for 1 day, and then within that day the changes pieces of their profile. If you're relying on a cache to serve their profile, we'll only see their old profile that we cached/copied, and not the new profile. This introduces the common decision of how long we should cache data, based on how time-sensitive that data is and how much our server should be able to handle. 

---
### What is a cache hit, cache miss?
- **Cache hit:** We found the data in the data, so we don't have to query our database/find it on our server. Good case. 
- **Cache miss:** Data wasn't found in the cache, so you have to query the database, make a request, etc. After you do send you not only send the response back to the client, but you'll refresh the cache with that new data. 
- **Cache invalidation:** Declaring a cache entry as invalid, and then removing or replacing that entry with new data. This is really helpful since sometimes based on some events, you want to update the cache to maintain freshness.

---
### Types of caches
- **CDN caching:** CDNs are a type of cache. You'd usually use a CDN to serve static content like images, videos, logos, etc.
- **Web server caching:** Reverse proxies can act as caches. It allows you to cache the HTTP response to a given http request. As a result, sometimes the request won't even reach your application/origin servers.
- **Database caching:** Databases typically have some level of caching. You can tweak these to help you out. Such as allowing you to cache the results of frequently executed queries.
- **Application caching:** In-memory caches such as Memcached and Redis fit the bill. They are separated from your application and database. Essentially after you get the results from the database, you can cache the results in one of these. Then the next time you're processing this same request, you can ask the cache to return its result instead. It's a lot faster since it's RAM, but also you'll need to balance how fresh your data is.

### Caching strategies

#### Write-through cache
![](https://github.com/donnemartin/system-design-primer/raw/master/images/0vBc0hN.png)

Our cache acts as the middle-man between our service and database. Here's the flow for a write operation: 
1. Application adds/updates entry in the cache.
2. Cache synchronously writes entry to the database.
3. Then send response to client.

Then for a read, you simply just read from the cache and respond to the client. Fast for reads and it's a clever as this also synchronizes the cache with the database, so you still have persistent storage. But slow for writes.

#### Cache-aside (reading data)
![](https://github.com/donnemartin/system-design-primer/raw/master/images/ONjORqk.png)

The client communicates with the cache and database. We'll get a request, and if the data is in the cache, then we'll return that data instead of querying the database. Else we'll do a different flow:
1. Look for entry in cache, but it's a cache miss.
2. Query data from the database.
3. Update cache to include this data.
4. Return data as a response.

- **Disadvantages:** 
  1. There could be a slight delay when there's a cache miss since we dedicate time to checking and writing to the task. 
  2. Also if data is updated in the database, then the cached data is old/stale. If you application prioritizes fresh data, then you'll need to cleverly handle the cache invalidation process.

Overall I think this is the main technique you'd use.


#### Write-behind (write-back, asynchronous writes)
![](https://github.com/donnemartin/system-design-primer/raw/master/images/rgSrvjG.png)

So this setup is for caching and asynchronous writes. Here's how the flow works:
1. API writes to cache.
2. Cache adds an event to a queue.
3. Some event processor will process the event asynchronously. Eventually the data from the event will be persisted into the database. Finalizing the synchronization process

You could have data loss if the cache goes down before it's able to enter the queue and eventually be persisted into the store. Also this approach is a lot more complex to implement than others.

#### Write-around (Write to db, read to cache)
![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-I/caching/write-around-cache.png)

- Write operations go to the database
- Read operations go to the cache.
- Could be slower if you read the data after it has been written.

---
### Cache Eviction
The process of removing items from a cache to make kroom for new ones. It happens when the cache is full or when items are no longer useful or valid. Caches have limited space and:
1. We can't store things forever
2. Some data becomes stale, unused, and irrelevant
3. We need to prioritize what stays in the cache for optimal speed and hit rate.

Here are some cmommon and usefulalgorithms:
- **Least Recently Used (LRU):** Evicts the item that hasn't been used in the longest amount of time.
- **Least Frequently Used (LFU):** Counts how often an item is needed, and the least used ones are discarded first.
- **Random Replacement (RR):** Randomly selects a candidate item and discards it to make space when needed. Sometimes fair.

Of course, if you're not implementing a cache yourself, it's not necessary to know about these eviction strategies.

---
### Distributed and Global Caches
![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-I/caching/distributed-cache.png)
Most caches are just a single physical server or device that's used alongside an application. However in a **distributed cache**, you're going to pool together the RAM of multiple computers and that acts as a single in-memory data store. Kind of niche.

![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-I/caching/global-cache.png)
In a **global cache**, you have a single cache that all of your nodes will use.

### Examples of Caches
Stuff like Redis, Memcached, AWS Elasticache.