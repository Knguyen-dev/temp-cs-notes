# Docker Basics

### What is docker
Docker is a platform that helps us create and run 'containers', which are isolated environments that 'contain' our code and everything it needs to run perfectly. An isolated process. So you can have a container for a node app, react app, etc. Docker is a tool for managing these containers. 

#### How is Docker beneficial?
Imagine you have a node app that runs on a specific Node.js version and you want your teammates to be able to run it. Teammates would need to download dependencies, configure environment variables, etc. Docker simplifies the process of sharing and running applications by packaging them in containers. These containers include all the necessary dependencies and configurations, ensuring consistent behavior across different environments. This eliminates the need for teammates to set up complex development environments, saving time and reducing potential issues.

---
### Containers

#### Containers: Container vs Virtual Machines
- Virtual Machines: An entire operating system with its own kernel, hardware drivers, programs, and applications.
- Container: Just an isolated application, a single process that has all the files it needs to run. If you run multiple containers, they all share the same kernel. To run the process that is your app, you run the container. 

#### Port Forwarding With Containers
When we create or run a container, they aren't exposing ports to the outside world. E.g. in example 1, our api is running on port 4000 inside the container. If we access port 4000 on our machine we won't see it. We do port mapping to ensure that the traffic from port 4000 on our local machine reaches port 4000 on that container. As a result, it is only here that we're able access our container.

---
### Images:
A blueprint used to create a container. It stores stuff like the runtime environment, code, dependencies, and environment variables. The idea is that first you make an image (blueprint) for your application. Here you specified the source code, dependencies, OS, etc. You then use the image to create the container, and run that container. You can then share this image with other people to their own containers and now everyone is running the same container, interacting and testing the same app.

Images are composed of layers, with each building upon the previous. The order of the layers matter:
- **Parent image:** Includes the OS, runtime environment, for the container we want to create. 
- **Source code:** The code for our program/process.
- **Dependencies:** Dependencies for our program.
- **Run commands:** Commands to run the program.

You would create an image by making a `Dockerfile`.

#### Versioning images with tags
This is definitely going to be useful since a lot of software is going to have many iterations and versions. New features are going to be added and so you'll definitely keep track of different images.

---
### DockerHub
You can store images on your computer, but if you want to share it to others, you'll publish it on a registry. Docker Hub is a public registry that anyone can use to share their images, and it's the default one. However there are other container registries such as Amazon Elastic Container Registry, Google Container Registry, etc.

#### Registry vs Repository
- Registry: Centralized location that stores and manages container images.
- Repository: A collection of related container images within a registry. 

So a registry is the platform, whilst a repository is just one of the things on that platform. For example, GitHub is kind of like a registry, and the GitHub repositories are just the many things on there.

---


### Docker Compose
A tool that allows us to create a `docker-compose.yml` file. Here we can list the configurations of any amount of containers that we want. Because before you would have to have multiple dockerfiles, which you would call the build command on to build the containers. Then you'll do the commands to run each container, and then the commands to stop the containers.

#### Workflow Before: `docker-compose.yml`
```
<!-- Build image -->
docker build -t <image-name> <path-to-docker-file>

<!-- Use image to run a container -->
docker run -d --name <container-name> -p <host-port>:<container-port> <image-name>:<tag>

<!-- Then stop a container -->
docker stop <container-id>
```

#### Workflow After: 
Define services for each image you want to build/container you want to run. That's actually the main or hard part, and then you can just run a couple of commands:
```
docker compose up

<!-- Stops and deletes containers, but our images and volumes are still good-->
docker compose down

<!-- Deletes all containers, images, and volumes -->
docker compose down --rmi all -v
```


With docker-compose, it aims to reduce the number of commands you'll run in order to activate and deactivate containers. Also when using Docker bare bones, we typically have to list various flags and whatnot which is annoying, so this helps reduce of that typing.







---

### Examples:
Here we're running our docker-compose file to run our services. We named our project "soiltool-dev". We did `--build`. With docker-compose, if you change a service's odckerfile or the contents of its build directory, you need to use `docker compose build` ot rebuild the service. We also ran it in detached mode
```bash
docker compose -f docker-compose.dev.yml -p soiltool-dev up --build -d
```

Here we stop all services, we actually wait 1 second until we forcefully shut down all containers. Also we remove orphans

Remember orphans are created in multiple ways:
- When you defined a service in your `docker-compose.yml` and then remove it from your file, Docker doesn't automatically remove the container for that service.
- When you rename a service in `docker-compose.yml`, the old service container remains running while the new container is created for the renamed service.
```bash
docker compose -f docker-compose.dev.yml down -t 1 --remove-oprhans --volumes 
```


### Other topics
- Run through the docker workshop example, as that's a lot tougher to do