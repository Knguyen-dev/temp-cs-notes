# Bind Mount and volumes

## What are they?
They allow containers to persist and share data, but in different ways.

### Bind Mount
When you use a bind mount, a file or folder from the host machine is made available to the container. It's directly mapping a file or folder from the host machine to the container. So changes on the host machine are reflected in the container, and also vice versa. There are a couple common use cases for this:
- When we want to create files in a container and persist them on the host machine.
- When we want to share source code between a development environment on the host machine, and a container. 

Because before, everytime you change your source code, you need to rebuild the image and run a new container. Now changes in our source code are immediately reflected in the container.

```
services:
  web:
    image: nginx
    volumes:
      - ./data:/app/data # Bind mount (Host ./data -> Container /app/data)
    ports:
      - "8080:80"

```


#### Considerations with bind mounts
- When you bind mount a file or folder into a container, whatever exists at that path inside the container is hidden and replaced by the mounted content from the host. The original container files aren't deleted, but they become inaccessible while the mount is active.
- Bind mounts will have write access to files on host. Things will happen inside your container, and modify your file system, so make sure things are setup correctly. Or you could use the readonly option to prevent the container from writing to the mount, and as a result only your changes on host will be reflected inside the container and not vice versa.

### Volumes 
Rather than mirroring, volumes are more about persistent storage. When you create a docker volume, docker manages the path/location your mounting, usually at `/var/lib/docker/volumes/` on the host. If you mount a non-empty volume to a directory in the container that has files, then the volume ignores these files. However if the volume is empty, it copies these pre-existing files into it. But yeah other than this, volumes usually start off as empty. One thing to know is that volumes persist after the container is destroyed, whilst bind mounts do not. Here are some uses for volumes:
- Data persistence: Volumes are commonly used to persist and store data that the application generates or uses between container activations.

```yml
services:
  web: 
    image: nginx
  volumes: 
    - my_docker_volume:/app/data # Docker managed volume
  ports: 
    - "8080:80"
volumes:
  my_docker_volume: # defines the volume; without this, it'd be a bind mount
```
---
### Anonymous vs Named Volumes
- **Named volumes:** Created explicitly and given a name. Can we reused across multiple containers, and persists even if no containers are using it.
- **Anonymous volumes:** Not given a name, literally assigned a random string. Used when you need a volume but don't care about managing it. They are persisted until manually removed.