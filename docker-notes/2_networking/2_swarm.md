# Docker Swarm: Swarm Mode

### Motivation

**Docker Swarm** is a group of machines all running docker, allowing for a scalable and reliable platform to run many containers.

Each machine has a **node (Docker Daemon)**, a central process running Docker, and all of these daemons will interact with each other using the Docker API. Your services can be deployed and accessed by other nodes within the same cluster, so not just your own.

You just need a bunch of machines running docker that are all on the same subnet, meaning they can directly communicate with each other without a router. Your machines should have ports 2377, 4789, and 7946 open.

### Setting up Swarm
```Bash

# Creates the swarm. Every swarm must have one manager and the node that 
# first initializes the swarm becomes the first manager. You can run 
# admin commands here
docker swarm init --listen-addr 10.0.0.4:2377

# On VM1: Let the current docker node join our swarm as a worker node
# There's service discovery built in, so any containers you run should be able to reach other containers 
docker swarm join 10.0.0.4:2377

# On VM2: Now you should have one manager and 2 workers.
docker swarm join 10.0.0.4:2377

# The swarm manages individual containers on the different docker nodes for us. Now we'll deal with higher level abstractions called services, which basically mean "I want to run this container, but I'll let the swarm create and manage those instances for me". So a service is a job or declaration to create and manage one of more instances (replicas) of a container, option to define settings like environment variables, networks, volumes, and other things.

# Our service "cool_app" spins up 10 tasks (containers) on our manager node.
# If you don't specify the swarm that they will run on then the tasks/containers are distributed across hte swarms in the network.
docker service create --name cool_app --replicas 10 ping00 alpine ping docker-docker-swarm-00

# Lists all services that are running 
docker service ls

# You'll see a single task that's running on swarm-02
# You can then ssh or connect to the machine running that node, and 
# you'll definitely see that you have a container running that pings.
docker service tasks ping00
```
In Swarm mode a container can reach other hosts via domain names. A node cna reach containers running on other nodes. You have load balancing and service discovery built in, mkaing this very attractive for microservice and general scalable architecture. 

```Bash

# Our web service spins up 3 containers of nginx and blaances traffic between them. Let's say that the actual docker containers themselves are running on swarm-02, so the 3 (zero-index) node in our docker swarm. We 
# can browse to swarm-01 and we can still access our app running on 02. 
# When node-01 gets a request, the swarm knows that the node isn't running a task that can handle the request. So the request to node-01 is silently forwarded to swarm-02, the node that has the containers running our app. 
docker service create --name web --replicas 3 -p 80:80 nginx

# In a production environment, you'd have your domain name pointing at your public facing load balancer. THne this load balancer would be able to distribute those requests across your swarm nodes, knowing that the swarm itself will take care of re-routing the request to the correct node.
# However let's say that a node goes down. All swarm nodes are sending heart-beats, so the manager will know when they go down. As a result, the swarm manager will create and allocate new replicas to offset the ones that went down with the node.

```

### Reviewing and reinforcing core ideas
- A service is basically a group of containers. A task is an individual container within a service
- Within Docker Swarm, a given node can access and manage the containers for another node. There's typically only one manager node, with the rest being workers. If there are multiple managers, one must be the primary. The manager should have all permissions and knows the statuses for the other nodes.
- Docker swarm has built in load balancing, and service discovery. The former is evident when you know that when you make your service spin up multiple replicas, those replicas are distributed across nodes.

  - **Ingress Mode:** When you publis ha port, every node in the swarm starts listening on port 80, even if a given service's tasks aren't setup to listen for stuff on port 80. Then regardless of which 

  - **Service Discovery:** Services can communicate with each other using their service names, and Docker will resolve that name into an IP address.
  
### Why use Docker Swarm? What problem is it trying to solve?
Docker (alone) is really good at running a single or multiple containers with docker compose to stream line things. However as our infrastructure grows we have some issues:
- Our application has a single point of failure, the machine running the docker node. 




### Extra Stuff: 
#### Breaking Down a Docker Environment:
There are five main components to a Docker environment:
- docker client
- Daemon: interacts with host operating system to manage with the docker containers. In a regular setup, you may have multiple computer with each running a docker container started by its own daemon. However if one service fails, well that's it for that service, we'd have to manually start it again (separate environments). However with Docker Swarm, each container is managed by its Docker daemon in the same environment. If one container goes down, another docker daemon can step in and restart that container.
- Container
- Images
- Docker registry


#### Features of Docker Swarm
- Decentralized
- Secure
- Auto load balancing and therefore highly scalable
- Rollback an environment back to a safe environment

In Swarm, containers are launched using services. A service is a group of containers that are created from the same image. This goes hand in hand with load balancing, say one of your services is under heavy load, you can create more instances of that service to balance things out. 

However you must have at least one node deployed before you deploy a service in docker swarm. There are two types of nodes:
- Manager node: Manages the tasks executed by the worker nodes.
- Worker node: Listens to the manager nodes.

Essentially the manager is aware of the status of the swarm environment at all times due to a bi-direction communication. Workers are listening for instructions from the manager and are sending their statuses to the manager. It should be noted that the communicate between these services is facilitated by a REST API, so you could do interesting stuff such as making an endpoint to manage a docker swarm or containers within one.

#### Benefits and innerworkings
- Services can be accessed and deployer by other nodes within the same cluster.
- While creating a service a user has to specify which container image to use?
- You can setup commands and services to be either global or replicated. The former means your functionality is given to all worker nodes, whilst the latter means only a select few worker nodes are going to get those services and tasks.


#### Tasks and Services
- A **service** is a description of a task or state. The **task** does the actual work.
- Users create servicesl, and those services start tasks.
- When a task is assigned to a node, it can't be assigned to another node.


You can have multiple managers within a docker swarm environment, however there still needs to be a primary manager node.

#### Recap of the steps 
- Manager node:
  - Command line interface allows us to interact with Docker via an API.
  - API then allows us to orchestrate task sand services
  - Task allocation allows us to allocate work for tasks based on IP address. 
  - Then a manager schedules for a task to be done.
- Worker node:
  - Checks for a task
  - Executes the task


#### Demonstration:
You'll need a virtual machine for it. Remember it's the idea that there are multiple separate computers and docker nodes running.

```bash
# Initialize Docker Swarm with IP address of the network we're connecting to
sudo docker swarm init --advertise-addr 192.168.2.151

# You should have a token identifying the docker swarm environment, which will be used for your worker enviornment. This will allow it to connect to the manager and docker swarm enviornment as a worker.

# Creating a new service using the alpine image from docker.com. When displaying all services, you'll probably see if you're on the manager node, however a worker node isn't going to see it becasue by default a service isn't global.
sudo docker service create --name helloworld alpine ping docker.com

# make a global service, now all nodes are able to access this servicee.
# Now if you go back to your worker node, which is assumed to be on a separate machine, the docker service helloworld1 should be 
sudo docker service create --name helloworld1 --node global alpine ping docker.com

# Force the docker node to leave the swarm. 
sudo docker swarm leave --force 
```
So you can tell that we aren't reall



## Credit
- [Docker Swarm Tutorial](https://www.youtube.com/watch?v=KC4Ad1DS8xU)
- [Docker Swarm: A more technical introduction](https://www.youtube.com/watch?v=Tm0Q5zr3FL4)