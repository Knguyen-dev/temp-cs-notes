# Docker Security

Certain misconfigurations can mess up the security of your docker app. So I'll run through some rules:


### Rules 
Keep your Host (your computer) and Docker up to date to start off.

#### 1. Don't expose Docker Daemon Socket 
- Exposing or giving someone access to the Docker socket is equivalent to giving someone unrestricted access to the host machine running that docker daemon. 
- Don't enable TCP on your docker daemon socket. You do this by running `-H tcp://0.0.0.0:XXX` or something similar on your docker daemon. Essentially, you're allowing unencrypted and unauthenticated access to your Docker Daemon. If your host machine is connected to the internet, you're making it so that anyone in the internet can use your docker daemon.
- Don't expose the docker daemon socket to other containers. This happens when you run a docker image this bind mount. Again exposing the docker daemon socket to a container gives the container full control over the host system.
```Dockerfile
volumes:
  - "/var/run/docker.sock:/var/run/docker.sock"
```

#### 2. Always set a user
Ideally you want to configure the container to use an unprivileged user to prevent escalation attacks.

#### 3. Use Least Privilege Principle for Containers
Limit the permissions that the container has to what it needs. So the Linux kernel capabilities are all the privileges the container can use. By default, Docker uses a subset of these. The main takeaway from this is that you don't run containers in privileged mode.

#### 4. Prevent in-container privilege escalation
Always run your docker images iwth `--security-opt=no-new-privileges` to prevent privilege escalation. It just prevents the container from gaining new privileges using `setuid` and `setgid`.

#### 5. Becareful if inter-container connectivity
All docker containers will run within the default docker bridge. As a result the containers can communicate with each other. So you can ping another container with its IP address. If you want them to be on different networks

#### 8. Read-only bind mounts
```Dockerfile
version: "3"
services:
  alpine:
    image: alpine
    read_only: true
```
If you're running a bindmount and you do read only, it prevents the container from making changes to the host's filesystem. So the changes on the host will be reflected in the container, but not the other way around. 

#### 9. Integrate container scanning toools into the CI/CD Pipeline

CI/CD pipelines are really good for including various security checks such as linting checks, static code analysis, and container scanning. 


#### 10. Keep Docker Daemon Logging at info



## Credits: 
- [Docker Security - OWASP](https://cheatsheetseries.owasp.org/cheatsheets/Docker_Security_Cheat_Sheet.html)