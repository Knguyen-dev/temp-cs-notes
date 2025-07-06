# Cassandra Drivers

Let's focus on a python cassandra driver
```bash
pip install cassandra-driver
```

```python 

from cassandra.cluster import Cluster

cluster = Cluster()
session = cluster.connect("test_keyspace")

session.execute("INSERT INTO python_test (id, first_name, last_name) VALUES (uuid(), 'Arthur', 'Morgan')")

rows = session.execute("SELECT * FROM python_test")
for r in rows:
  print(row.first_name, row.last_name)

print("finished")
```