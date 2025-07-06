# Snitch

A snitch is a component that understands the topology of our cluster. For each node, the snitch will know what data center and rack it's located in. It's main job is knowing which is the most responsive node that can be used for a given request. For example, if our request is received at data center 1, then it's best to read/write data from data center 1 than from other data centers.

There are several snitches in Cassandra:
- Simple Snitch: Assumes all nodes are in teh same data center and rack. Good for testing, but not good for production. If your production environment is actually distributed, the simple snitch won't know that, and as a result the choices it makes aren't optimized for serving our reads and write.
- Property File Snitch: This snitch actually uses a Cassandra config file to know the location of our nodes. Though the only downside is that we have to fill out this config file. Nowadays this snitch has gone out of style due to that manual nature. If you had 5 Cassandra nodes, you'd need to setup this file 5 times. If you add a sixth node, you need to update that sixth node's file, and the file of the 5 old nodes. Imagine if you're at a scale like Apple with 25,000 nodes. It's going to be a pain to add one more node knowing that you'll need to change the files of 25000 other nodes. 
- Gossiping Property File Snitch: For the node that we're currently on, you setup the data center and rack. Once that's done, the node tells all other nodes in the cluster it's information, making the whole cluster aware. As a result, this makes adding more nodes easier as you just need to update one node and through the "gossiping protocol" this information is sent to all other nodes. This is recommended for production.

The last component is the **dynamic snitch**, which is activated by default, so any snitch also uses the dynamic snitch. Using the gossip protocol, it monitors the performance of all the nodes in our cluster. It uses this information to determine what nodes would be best to query for a request, in hopes of optimizing efficiency. Let's say you have a coordinator node, and for a read operation, our coordinator can also choose from 3 other nodes. Node A is in the same datacenter as the coordinator, whilst B and C aren't. Due to A being in the same datacenter, the dynamic snitch would like prioritize its data because it sees better performance. Then for the second best node, it'll query that, and so on.

To takeaway, Cassandra is only querying what it thinks is quickest responding node for the full representation for the data. All other nodes that it queries, it only asks for a small part of data called the "checksum", which is here to verify whether the data from the quickest responding node matches the data from other nodes.


## Example: Walking through Cassandra Config Files 
```bash
# In Cassandra Container

cd /etc/cassandra

# This contains our current snitch being used in Cassandra. It'll show you the the Gossiping Property File snitch, property file
# and AWS web service/cloud snitches. You'll probably see endpoint_snitch: SimpleSnitch as the default
vim cassandra.yaml

# Remember the current datacenter and rack are stored here. The Gossiping property file snitch will use these files and propagate
# them throughout the cluster.
cassandra-rackdc.properties
```