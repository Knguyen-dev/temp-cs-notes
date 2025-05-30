# Storage
We're talking about different mechanisms a system can use to store data either temporarily or permanently (persistent). This is often skipped over, so let's just provide a decent foundation for the different storage techniques/types.

### RAID (Redundant Array of Independent Disks)
The idea of storing the same data on multiple hard disks or SSDs to protect our data in case of a drive failure. There are different levels of RAID, and not all of them provide data redundancy. Here are teh common ones:
- **RAID 0 (Striping):** Data is split evenly across all drievs in the array. 
- **RAID 1 (Mirroring):** At least two drives contain the same copy of data, so if one fails, we can use the other one. Then of course create another backup with the good one.

---
### Volumes
We're talking about a fixed amount of storage on disk. In Docker or Kubernetes we mount volumes to persist logs or database files even if the container restarts. In cloud VMs (AWS EC2, GCP Compute), you attach a  volume (EBS/PD) to persist app data.
---
### File Storage
- Organizes data in a hierarchical folder system (directories and subdirectories)
- Stores data as files, which can be retrieved using their path
- The benefit is to create a user-friendly solution to storing or retrieving files. To locate a file in file storage, enter the complete path of the file you want. 

You may see these in download sites or any place where users are browsing online files. But anyways, solutions for file storage would be Amazon EFS, Azure files, Google Cloud Filestore, etc. 

**Note:** Don't misconstrue this with Object storage. Here we're storing all data as files. Like inputting information for a book. Whilst object storage is for uploading and storing files/objects.

---
### Block Storage
Dividing data into blocks (chunks) and stores them as separate pieces. Each block is given a UID, allowing teh storage system to place the smaller pieces of data wherever it is most convenient.

Block storage decouples data from user environmnets, allowing the data to be spread across multiple environments. This creates multiple paths ot the data and allows the user to retrieve it quickly. When a user or application requests data form a block storage system, the underlying storage system reassembles the data into blocks and presents the data to the client.

You're really going to see this in databases, but it's like a raw hard-drivev. Fast IO, low latency.

---
### Object Storage
A data storage architecture for storing unstructured data, which sections data into units—objects—and stores them in a structurally flat data environment. Each object includes the data, metadata, and a unique identifier that applications can use for easy access and retrieval. 

In any case you see it on websites that need to host user upload images. Stuff like Pinterest, Facebook, Instagram either use distributed object storage, or have created their own proprietary solution.

#### Comparing object storage vs file and block storage
Traditional file and block storage isn't designed to handle large volumes of data, especially unstructured data. But let's see how they'd do:
- **File storage:** Organizes data into folders, and to find it, you need to know the exact location/path the data was stored in. The exact room, cabinet, drawer, and folder that contains the document. This is a hierarchial storage structure, where files are named, tagged with metadata, and placed in folders. Searching and retrieving data files can become more time-consuming as the number of files grow, but this is a simple (and non-scalable) way of doing things.
- **Block storage:** Breaks files into separate blocks and stores each block separately. So we assign an ID to each chunk of data, and when you need to access a file, we'll use these IDs to reconstruct the desired file. We don't need to use a path to the data, so the location doesn't matter. This type of storage is good for orgs that store a lot of transactional data or when working with mission-critical apps.
- **Object storage:** Saves files in a flat environment (bucket) as separate objects. each object containing the ID, metadata, and actual data. Ideally, you're using this in a case where you're reading files many times, but only writing to objects a few times.

---
### Network Attached Storage
A storage device connected to a network that allows the storage and retrieval of data from a central location for authorized network users. It's flexible, you can add more if you need.

A use case should be having a shared file system within homes or businesses. So the NAS device at home stores family phoots and vidos whilst the office NAS is used to access business related files.

---
### Hadoop Distributed File System (HDFS)
A distributed file system designed to run on consumer hardware. It's highly fault tolerant, has high throughput, and is suitable for apps that have large datasets. You can store very large files across machines in a large cluster. 

You'd see this a lot in big data processing, analytics and ML pipelines. Another example could be storing log files for a telecom company across hundreds of machines for processing. Or maybe storing anonymized medical data for large scale research.

We use it since we want sequential, large scale/write. It's good for apps needing horizontal scalability and fault tolerance.

## Credits
