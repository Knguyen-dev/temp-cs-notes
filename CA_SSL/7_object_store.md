# Object Stores

### What is an object store?

- **Object storage:** A data storage architecture for storing unstructured data, which sections data into units—objects—and stores them in a structurally flat data environment. Each object includes the data, metadata, and a unique identifier that applications can use for easy access and retrieval. 

---
### How does it work?
With object storage, the data blocks of a file are kept together as an object, together with its relevant metadata and a custom identifier, and placed in a flat data environment known as a storage pool. 

When you want to access data, object storage systems will use the unique identifier and the metadata to find the object you need, such as an image or audio file. You can also customize metadata, allowing you to add more context that is useful for other purposes, such as retrieval for data analytics.  

### Object compared to file and block storage 

Traditional file and block storage isn't designed to handle large volumes of data, especially unstructured data. But let's see how they'd do:

- **File storage:** Organizes data into folders, and to find it, you need to know the exact location/path the data was stored in. The exact room, cabinet, drawer, and folder that contains the document. This is a hierarchial storage structure, where files are named, tagged with metadata, and placed in folders. Searching and retrieving data files can become more time-consuming as the number of files grow, but this is a simple (and non-scalable) way of doing things.
- **Block storage:** Breaks files into separate blocks and stores each block separately. So we assign an ID to each chunk of data, and when you need to access a file, we'll use these IDs to reconstruct the desired file. We don't need to use a path to the data, so the location doesn't matter. This type of storage is good for orgs that store a lot of transactional data or when working with mission-critical apps.
- **Object storage:** Saves files in a flat environment (bucket) as separate objects. each object containing the ID, metadata, and actual data. Ideally, you're using this in a case where you're reading files many times, but only writing to objects a few times. 

---
### Why though?
Good for businesses, since many of them collect a lot of unstructured data and need a way to store it all.

It's unstructured so there are multiple different formats, shapes of data, and ultimately it's not going to be cleanly stored in a central repo.

Many any object storage solutions, such as AWS S3, Google Cloud Storage, and Azure Blob Storage, allow direct HTTP GET requests for retrieving stored files, effectively making them auto-hosted.


---
### MinIO Object Storage 
An object storage solution that provides an AWS S3-compatible API, and it supports all core S3 features. It can be deployed anywhere. In public or private, cloud, baremetal infrastructure, etc. Here's what this means:

1. **Same API Endpoints & Methods:** MinIO provides RESTful endpoints that match AWS S3's API structure. This allows you to use the same HTTP methods (`PUT`, `GET`, `DELETE`, etc.) to interact with MinIO as you would with S3.
2. **Compatible SDKs & Tools:** Since the API is S3-compatible, you can use AWS SDKs, CLI, or tools like `s3cmd` and `boto3` (Python's S3 library) to interact with MinIO without modification.
3. **Supports Core S3 Features:** MinIO implements key S3 features such as:
  - Bucket and object management (`PUT Bucket`, `GET Object`, etc.).
  - Access control (policies, IAM-like users, presigned URLs).
  - Lifecycle management, versioning, and replication.
4. **S3 Version Compatibility:** MinIO aims to be fully compatible with all core S3 API functionalities, up to a certain version of the S3 API. However, it might not support all proprietary AWS S3 features, such as AWS-specific analytics, Glacier storage, or AWS-specific event notifications.
5. **Alternative Backend:** MinIO essentially acts as a replacement for AWS S3, but you can deploy it on-premises, on your own cloud provider, or at the edge, rather than relying on AWS infrastructure.

---
### S3FS (AWS S3 File System)
Allows us to mount an S3 bucket via FUSE (filesystem in userspace). We can interact with files and directories in an S3 bucket like a local file system.

So essentially, you can interact with AWS S3 buckets as if they were folders or files on your local file system. You'll see this when using since MinIO allows you to self-host S3 buckets, and it uses S3FS to allow you to see and interact with those buckets if needed.

In this context, **mounting** just means attaching a storage system (in this case, an S3 bucket) to your local file system so that it appears like a normal folder or drive. So after you mount the bucket it'll appear with `ls` you can `cd` into it.

#### How does this work?
Tools like s3fs (for AWS S3) or MinIO's s3fs allow our operating system to communicate with a remote S3 storage via the **FUSE layer**. This makes remote cloud storage behave like a local file system. 

When you mount an AWS S3 bucket using s3fs, it establishes a persistent connection to the S3 service. As long as the connection is active, the bucket appears like a local folder, and you can interact with the files. If it drops, then you won't be able to interact with those files. Remember no data is actually stored on your system (unless explicitly cached), the idea is that your system is getting the data from the S3 API.

#### Relating back to MinIO
With Minio the object data is stored on our machine because we are hosting it, but it's still its own S3 service. So to see those folders and files, we still mount the S3 bucket to our machine to interact with the bucket as if it's a file or folder on our local machine.

#### Credentials file
With s3fs, you probably see a AWS credential file or a custom password file in order to get authorized to do things. The motivation for this is that your S3 service is going to be secured, and you'll need to pass credentials for s3fs to have permission to get your object data in the remote.

You'll enter your credentials in `${HOME}/.passwd-s3fs` for a specific user, or `/etc/passwd-s3fs` to have a system wide credential.


#### Commands and examples
```
s3fs mybucket /path/to/mountpoint -o passwd_file=${HOME}/.passwd-s3fs
```
Mount the bucket named `mybucket` to a local directory with path `/path/to/mountpoint`. We pass our password file to make sure our request is authenticated and that it works.


## Credits
- [what is Object Storage? - Google Cloud](https://cloud.google.com/learn/what-is-object-storage)
https://aws.amazon.com/what-is/object-storage/
- [Object Storage Explained - AWS](https://docs.aws.amazon.com/AmazonS3/latest/userguide/Welcome.html)
- [MinIO Linux docs](https://min.io/docs/minio/linux/index.html)

- [S3FS Repo](https://github.com/s3fs-fuse/s3fs-fuse)
