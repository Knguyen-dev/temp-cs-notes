# Replication, Data Centers, and Racks 

## Data centers and "racks"
Cassandra can be used across multiple datacenters, allowing us to store and replicate our data in multiple regions around the world. Let's say you had two data centers, one in the EU and the other in the USA. If you write a record in the EU data center, it is first stored in the EU datacenter, but a copy of that record will be eventually replicated into the US data center. That means even if you somehow lose that record in the EU data center, you'd still be able to access that record via the USA data center. Even worse, if the entire EU data center goes down, you'd atleast be able to access those records in the USA data center. A **rack** in a data center is a cluster of connected machines. It's the cluster of nodes. The EU data center could have racks 1 through 4, and the USA may have the same setup. When we create a record, we specify the data center and then the rack. Then we look at the value of the record's partition key, which is hashed, and decides what node in the cluster the record is going to be stored in. 

For example, let's say you're writing a record to the EU data center on Rack 2. However, after it's stored, we can replicate this record other racks, meaning I can replicate to Rack 1 and 4. The motivation is that even if Rack 2 goes down, you can still access this data through Racks 1 and 4.

```bash
 
# Exec into the Cassandra Container
docker exec -it 9a43277e64f0 bash

# This command shows you the information of all nodes in the cluster, and 
# also what rack that node belongs to.
nodetool status

Datacenter: datacenter1
=======================
Status=Up/Down
|/ State=Normal/Leaving/Joining/Moving
--  Address     Load        Tokens  Owns (effective)  Host ID                               Rack 
UN  172.23.0.2  136.31 KiB  16      100.0%            79791dca-b454-4f59-8178-17fa8d213236  rack1

# Rack: The rack that the node belongs to, it's running on rack1.
# Tokens: The number of virtual nodes that the node contains
# Owns: How much of the token range that this node owns. Since it's the only node in the cluster, it owns the entire token range.
```

Let's say you're adding a new node and it is going to be set at a different data center and rack.
```bash

# Go to this folder and file. Here you can modify the rack and datacenter that this node is associated with
# However if you're adding a new machine, you'd update these values before adding the node? I think?
cd /etc/cassandra
vim cassandra-rackdc.properties
```

## How Replication Works?
Let's talk about how Cassandra determine what nodes our data is replicated to. There are two strategies: Simple and Network Topology Strategy. We'll talk about the simple strategy since it's a lot simpler honestly.

For every "keyspace" in the cluster, we set a **replication factor**, which is just an itneger. This number determines the amount of nodes the record is replicated to. For example, if the replication factor is 3, the data is going to be copied into 3 nodes. This is quite easy to follow:
1. Calculate the token, which is the hash based on the partition key's value, for the record you're trying to create.
2. Add the record to the token range it falls in. Replication count is 1 at this point.
3. Then based on the replication factor, move around the ring and add it to the corresponding nodes until the replication count is equal to the replication factor.

In English, if the replication factor is 3, you'll store that record in 3 different nodes. Then the record is stored in the respective partitions of those 3 different nodes.

**Note:** You can't set a replication factor higher than the number of nodes. Because at that point you'd be going full circle and replicating the record to nodes that already have the record, it'd be dumb.