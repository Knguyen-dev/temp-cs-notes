# Docker Commands Cheat Sheet
A quick little guide and cheatsheet for docker commands

### Images commands:
```
<!-- List all images -->
docker images

<!-- Pull a parent image from dockerhub -->
docker pull <image-name>:<tag>

<!-- Build image from a docker file -->
docker build -t <image-name> <path-to-docker-file>

<!-- Remove/delete an image -->
docker image rm <image-name>

<!-- Example of deleting multiple images with one command -->
docker image rm <image_name_1> <image_name_2>

<!-- Deleting an image, even if it's in use -->
docker image rm <image_name> -f
docker image rm <image_name> --force

<!-- Inspect an image; just displays detailed information about the image -->
docker inspect <image-id>

<!-- List all 'dangling' images; so images that aren't tagged and don't have any containers associated with them -->
docker images -f dangling=true

<!-- Deletes all dangling images; if you add '-a' -->
docker image prune

<!-- Build a docker image -->
docker build -t <image_name> <path_to_dockerfile>

<!-- Running image to create new container; remember that 'docker run' will always create a new container before starting/running it, whilst later 'docker start' simply runs/starts an existing container. -->
docker run -p <host_port>:<container_port> --name <container_name> <image_name>


```

### Container commands:
```
<!-- List all containers (running and stopped) -->
docker ps -a

<!-- List all running containers -->
docker ps 

<!-- Run a container -->
docker run -d --name <container-name> -p <host-port>:<container-port> <image-name>:<tag>

<!-- Start a stopped container; you could also use the container's name instead of the ID; as well as this you can add the '-i' flag to get an interactive terminal based on the process running  -->
docker start <container-id>

<!-- Stop a running container -->
docker stop <container-id>

<!-- Restart a container -->
docker restart <container-id>

<!-- Removes/delete a container -->
docker rm <container-id>

<!-- Inspects a container; this gives you metadata about the container -->
docker inspect <container-id>

<!-- Remove all stopped containers -->
docker container prune

```

### Flags