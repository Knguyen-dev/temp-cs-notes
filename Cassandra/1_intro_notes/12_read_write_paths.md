# Cassandra Read and Write Paths



## Write Paths 
![](https://www.scylladb.com/wp-content/uploads/cassandra-memtable-diagram.png)

So far, we've learned how Cassandra decides what node to store data on. Let's dive into how it stores data on the node. Every instance of Cassandra is running on a Java VM. It has access to RAM and disk storage (HDD or SSD). Let's look at the workflow of what happens when we add a new record:
1. **Commit-log:** Record is written to this node's commit-log stored on disk. It's an append only log that's mainly a mechanism to ensure data durability in case of a node failure or crash. Every write operation in Cassandra is written into the log before being written to the memtable. The main idea is that on startup any mutations in the commit log will be applied to the memtable, reconstructing the memtable.
    - USA 3
    - USA 4
    - USA 2
    - USA 4

2. **Memtable:** In memory the record is also written to this node's MemTable. A memtable isn't durable, but it's quicker and it keeps the records sorted. Read operations go to the memtables and the SSTables. When you query recent data, Cassandra checks the memtables first since they're fastest to access. As well as this, there's the common scenario where we get the record from the memtable and one from the SSTable. In this case, it's always true that the Memtable will have a more recent timestamp (more recent version of the data), so the data from the memtable is returned instead. 
    - USA 1
    - USA 2
    - USA 3
    - USA 4
3. After the data has been written to both the memtable and the commit-log, the query is returned as successful. 
4. **SSTable:** This is your long term storage files on disk. They're immutable/durable, sorted, and optimized for reads. When your data isn't in the memtables, Cassandra will read from the SSTables.
    - Eventually the memtables grow to a limit, and so it'll flush out all of its data to disk, and create an **SSTable**. Then the memtable and commit logs are destroyed, and new ones are created, it's a full reset to indicate a blank slate.
    - In general, Cassandra has a lot of indexes and caches that will have our data, in order to minimize the event of doing a full scan on an SSTable.
    - Through **compaction**, SSTables are merged/compacted to save space.

### How HDD and SSD affect your setup 

With HDD, aka "spinning disk", it's actually manually moving mechanical parts in order to access your data. In particular, the head of the hard disk points to exact memory locations physically. In this type of setup, you'd want your commit logs and SSTables on two different hard disks. This is to avoid contests for control, as the commit log would be appending data, but it may need to relinquish control of the disk head to the SSTable for some read query, and this happens back and forth, making both of their operations slower. This physical aspect of HDD is probably it's pretty slow. You probably don't need to worry about this since most computers run HDD now, but if you're ever working on a legacy system, then maybe this advice could help you. With SSD, you can place both on the same SSD, due to the non-mechanical nature of SSD, there's no contest for control

## TLDR and Takeaway

I kind of glazed over a lot of the information regarding the internals of the read paths and the amount of caches and indexes it has to optimize reads. I also glazed over the process of compaction. Though in general I don't think most people are going to need to know that.