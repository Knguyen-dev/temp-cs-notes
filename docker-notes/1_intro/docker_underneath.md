# Docker: Under the Hood

The Docker daemon socket is a Unix socket that's typically located at ***/var/run/docker.sock***. It's the main way that Docker clients (like the docker CLI) communicate with the Docker daemon `dockerd`. Then `dockerd` is the background process that actually builds, runs, and manages containers. 

- Think of it as a communicate pipe between your terminal and the Docker engine.
- The socket exposes a REST API, which the `docker` CLI uses behind the scenes.

It's important to note that anyone who can access `docker.sock` can control Docker and gain "root access" to the host machine running Docker. They have highest level of privilege to the host machine and if someone bad gets ahold of this, your host machine is compromised. Now that someone has access to the docker socket, they can interact with the docker daemon and run a bunch of commands:
```bash

# Creates a bind mount for the entire host file system and mirrors that 
# in the container in the /host directory. We use chroot to switch the root directory
# to /host. We do privileged to run the container with higher privileges. But basically the attacker can run this container often to see what's happening:
# 1. Can see all files and directories on the host 
# 2. Can modify or delete critical files for the system.
# 3. Read environment variables, config files, credentials
# 4. Install malware
# 5. Watch for changes, like they can see logs, user activity, and file edits in real-time.
docker run -it --rm \
  -v /:/host \
  --privileged \
  alpine chroot /host
```
The Docker API is a RESTful HTTP API that allows us to programmatically interact with the Docker daemon. It has endpoints for commands like managing containers, images, showing logs, etc. Again, this is the core thing that powers the docker CLI, Docker Compose, and many other docker tools.