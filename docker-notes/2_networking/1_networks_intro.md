
# Docker Networking 


### Bridges and the Default Bridge
In Docker, a bridge is a type of network that allows containers to communicate with each other while isolating htem from external networks. You'll also hear the word driver, which also means "network type". The default bridge is the name of a default network in Docker. When you run a container without specifying a network, Docker assigns it to the default bridge network. There are some key points you should remember:
- **Internal Communication:** Containers can communication with other containers on the same network. You can see this in action if you exec into one of those containers and try to ping another container. Note that on the default bridge network, container-to-container communication only works with IP addresses, not hostnames. So if you're going to ping or reference another container, you'll need the other's IP address. However if you're on your own user-defined bridge, then Docker provides built-in automatic DNS resoltuion, which basically means you can reference other containers via their container names, and Docker will handle converting/interpreting those as IP addresses. 
- **Isolation:** Containers in a bridge network are isolated from the host system and outside world by default. So network traffic from the outside world won't reach a container, unless you do port mapping connects the traffic from a port on your machine to a port on the container. 
- **Custom Bridge Networks:** You can create your own custom networks. This is useful when you have multiple containers that need to communicate ineranlly, or you only want to expose specific servers to the outside world. You'd typically do port mapping on your web server (or API gateway) which is the only ting that needs to be accessible to the outside owrld. Then all other containers (e.g. databases or backend services) stay within the interanl bridge network without exposed ports. So basically those won't be accessible outside the Docker network as long as you don't explicitly expose their ports.






```bash
docker run -itd --rm --named thor busybox
docker run -itd --rm --named mjolnir busybox
docker run -itd --rm --named stormbreaker nginx
```

When we deploy our containers docker automatically places them in the bridge network. Docker creates 3 virtual ethernet interfaces and links those to the docker bridge. That bridge also handed out IP addresses, which means that the bridge is running dhcp. 

docker inspect bridge

Here you'll see the containers and ip addresses of those containers on your bridge network. And the containers can talk to each other.

docker exec it thor sh

Then you can ping the other containers in the network, using their respective ip addresses. 

Nginx will be used on port 80. Can you local host acccess the webserver in the container? If you check `http://10.7.1232:80` it's not going to work. You have to do port mapping and connect port 80 on the local machine and port 80 on the container. 

### User Defined Bridge
You can create custom networks and connect multiple containers on that network. 

```bash
docker network create -d bridge my-first-network
docker run --network=my-net -itd -name=container3 busybox
```
1. Created a `bridge` type network called "my-first-network"
2. Ran a container called "container3" based on the image "busybox". The container is run on the network "my-net" instead of the default `bridge` network. 

And the main benefit is that your containers are isolated, secure, and organized like this.


### Drivers
We have a bunch of different drivers
- **bridge:** THe default netowkr driver. We just used this to create our own custom network.
- **host:** Has an application use the local machine's network. You don't even need to do port mapping as traffic on your computer will reach that container without it. Sometimes you don't need to run stuff in isolation.
- **none:** The container has no network connectivity. This is useful for security sensitive workloads that don't need to be on the network at all.

There are multiple others, but for development, you'll probably be using user defined bridges and that's it. However, if you're deplying at scale, take a look at overlay networks.


### Connecting to multiple networks
A container can be connected to multiple networks. E.g. a frontend container is connected toa bridge network with external access, and then an internal network to communicate with backend services that don't need external network access.


### Reviewing commands we learned
```

docker network create -d bridge my-first-network
docker run --network=my-net -itd -name=container3 busybox
sudo docker exec -it thor sh
sudo docker exec -it loki sh
```


### MISC Notes:
```
docker run -p 80:80 -d httpd
```
Anyone who tries to access port 80 on our machine, then they are forwarded to the container inside. Well run this in detached to be able to use our terminal and we'll use `httpd` (official apache image). Now this works and traffic from the outside world can access your apache web server running inside hte container. 

Well this only works because no one else is running port 80 on the local host. Let's run `docker inspect <container-id>`, and this allows us to see more detailed information about the container. Let's break down the network info:
```JSON
"Networks": {
  "bridge": {
      "IPAMConfig": null,
      "Links": null,
      "Aliases": null,
      "MacAddress": "02:42:ac:11:00:02",
      "DriverOpts": null,
      "NetworkID": "38a31a350a9a609b1db0c8c5d0f8f591be6ee43d3aac63a0577c71310eef301f",
      "EndpointID": "7d81f7777915d563f3582103a91e13f008c2300659dd9f43090bf21bc21befd7",
      "Gateway": "172.17.0.1",
      "IPAddress": "172.17.0.2",
      "IPPrefixLen": 16,
      "IPv6Gateway": "",
      "GlobalIPv6Address": "",
      "GlobalIPv6PrefixLen": 0,
      "DNSNames": null
  }
}
```
Our container is only apart of one networked named `Bridge`. This network has a gateawy of `172.17.0.1`, which is the IP address of the container.

```
<!-- This works -->
curl http://localhost:80

<!-- This doesn't work -->
curl http://172.17.0.2
```
- The reason is because there isn't a "bridge" that connects your mac or windows machine to the container. Docker spins up a virtual machine and does the bridging. 
- Our gateway connects to your local host, connecting your running ocntainers to the outside world.

- The container can't go out? 



#### Example 1: 
When you run a container, traffic from outside world won't be able to reach it. You can inspect it ot see the netwokrs its own nad also other metadata. However you see they are all in put in that default bridge network. This can be bad if we go int oproduction and the hacker know what our passwords are. You can create your own custom networks, but let's leave that for later.

- In your container, inside the docker network, I am still able to send out traffic to `Google.com`. The container sends traffic out, the bridge/gateway allows that traffic to go to the outside owrld.
- Every network has simple logic. I want to go to `142.250.217.142`. It does a subnet mask on itself. It asks if this guy is in my subnet. Does a slash 16 on the IP, which means something. If they don't match, then it keeps going. 

- When you do an ns lookup, `ns lookup google.com` you're probably going to get back the IP address of docker's DNS server. This is the bridge network ,or at least teh bridge network lives here and all dns queries go here. If you do a `nslookup `
- Asking someone outside of your network for your host is not possible. This relates to how you can't ask things for host names outside hte bridge network?
- You can piick yourself, the gateway, and other containers. 


Do this:
```
nslookup google.com
``` 
You'll get a IP address of the virtual machien that docker spins up to bridge traffic from each world. This is the dns server for docker. If you want 


- the idea is that sometimes you want to have different netowrks for different parst o your application. A database network, other netowrks talk ot each ote=her whilst sometome don't even work together whith other netwokrsssssssssssssssssssssssssssssssseeeeeeeeeeeeeeeeeeeeeeeeee
- These are contaienrs inside the network that can only talk to each other. They're limited by IP address and not DNS names. 
- You need some options to make it come back?


## Credits
- [Docker Networking Docs](https://docs.docker.com/engine/network/)
- [Docker networking - NetworkChuck](https://youtu.be/bKFMS5C4CG0?si=4jJAWPAK2SJeKHuk)


Next? Maybe firewalls or how we manage CA certificates?