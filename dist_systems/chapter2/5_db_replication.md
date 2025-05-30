# Database Replication
Replication is where we share information across redundant resources like multiple databases or servers to improve reliability, fault tolerance, or accessibility. So like instead of having one database, you have multiple databases that are basically clones of each other. You may also hear this with servers and deployment, as having multiple replicas of an application, means you have multiple instances of it, distributing load across the cluster.

### Master-Slave Replication
![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-II/database-replication/master-slave-replication.png)

A replication strategy where:
- **Master Database**: Handles **write (insert, update, and delete)** requests.
- **Slave Databases**: Handle **read** requests.

When the master is updated, changes are **replicated** to the slaves. You usually have multiple slave databases since most of the time you're going to have a lot more reads than writes. Then a single master handles the writes (which are less in nature most of the time).

#### **Advantages**:
1. **Backup Data**: If one slave database fails, replicas provide redundancy. Slaves an go offline for a while, and get up to date by synching with master.
2. **Load Balancing**: Queries are distributed:
   - Writes go to the master.
   - Reads go to slaves (heavier load).
   - Modern databases handle synchronization automatically.

#### **Disadvantages**:
1. **Single Point of Failure**: If the master fails:
   - Promote a slave to master (manual or automated).
   - Operate in read-only mode temporarily.
   - Use a **multi-master** setup to avoid this risk.
2. **Complex setup**: Replication means more hardware and complexity. 
3. **Replication lag**: More read slaves means more replication lag, so it takes more time to get everyone to have consistent data.


### Master-Master Replication
![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-II/database-replication/master-master-replication.png)

An approach for systems with high write demands or strict regional requirements. Honestly this approach is only necessary for high-scale apps:
- **Multiple Masters**: Write requests are distributed across multiple masters.
- **Replication**: Changes are replicated asynchronously between masters and their respective slaves.

**Note:** Like illustrated above, you can have just two master databases with no read slaves.

#### **Challenges**:
- **Conflict Resolution**: Complex mechanisms are needed to handle simultaneous writes.
- **Use Case**: Best suited for high-scale apps where a single master cannot handle the write load.

