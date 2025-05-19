
# Docker Networks 




### What is a docker networking
The ability to let containers communicate with each other or non-docker things. Containers have networking by default, which is how they're able to make connections to the outside owrld, however the container doesn't know what network it's connected to or who's on it. It literally only sees info like an ip address, a gateway, a routing table, etc.

```bash
# Create apache image and run container on port 80 local host
docker run -p 80:80 -d httpd

# Should show container with that container ID
docker ps

# Get the HTML for the endpoint
curl http://localhost:80
```

Inspecting our container, we see the json data. 



You can create custom networks and connect multiple containers to be on the same network. After your containers will be able t ocommunicate with each other using their respective IP addresses or container names. Here they create the `bridge` network driver and run a container in that network.




```bash
docker network create -d bridge my-net
docker run --network=my-net -itd --name=container3 busy box
```
There are many network drivers that docker lets you use to create a custom network, just look at the documentation for it. A

### Connected to multiple networks
It's also normal for a frontend container to be connected to a bridge network. Here it allows frontend to have access to the outside world for http requests, but also give it the privilege to communicate to backend service containers on the internal network.

When data is sent to an IP address within a connected network, the info is actually set to the 

## Credits
- [Docker Networking Crash Course](https://www.youtube.com/watch?v=OU6xOM0SE4o)
- [Docker Networking - Docker Docs](https://docs.docker.com/engine/network/)
- [Docker networking is CRAZY - NetworkChuck](https://www.youtube.com/watch?v=bKFMS5C4CG0&t=184s)