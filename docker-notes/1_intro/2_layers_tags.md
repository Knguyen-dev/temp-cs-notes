# Layers 

## Dockerignore
We don't have node_modules, is alright since we're not going to run this app directly on your computer. We're going to run an image to create a container that runs our app for us in an isolated environment. It'll create a node_modules with all dependendencies inside the image, ready to be used when we start a container.

Sometimes a 'node_modules' folder will exist in our project, and so all of those dependencies are there. We'll we don't want to include those as a part of our source files when creating the image as we're downloading those when building the image (Defined in Dockerfile). Some stuff in 'node_modules' may be out of date, and also copying the entire node_modules will make it so our build process takes more time.

So we'll use a dockerignore file, which will ignore copying over all directories or files included there during the image build phase. So when we build this image, it won't copy over anything inside node_modules to the image.

### Dockerfile commands
**Dockerfile Keywords and Their Uses**

A **Dockerfile** is a script containing a series of instructions to automate the creation of a Docker image. Below are commonly used keywords in a Dockerfile and their functions:

### 1. `FROM`
- Specifies the base image for the container.
- Example: `FROM ubuntu:latest`

### 2. `RUN`
- Executes commands inside the image during build time.
- Example: `RUN apt-get update && apt-get install -y curl`

### 3. `COPY`
- Copies files from the host machine into the image.
- Example: `COPY myfile.txt /app/`

### 4. `ADD`
- Similar to `COPY`, but also supports extracting tar files and downloading URLs.
- Example: `ADD myarchive.tar.gz /app/`

### 5. `WORKDIR`
- Sets the working directory for subsequent commands. This is extremely useful as one of the use cases is to make sure your application files are downloaded in a specific directory on the container. 
- Example: `WORKDIR /app`

### 6. `CMD`
- Specifies the default command to run when a container starts.
- Example: `CMD ["node", "server.js"]`

### 7. `ENTRYPOINT`
- Similar to `CMD`, but used to define an executable. This is basically defining the default executable or command that will run when the container starts. It's essentially defining the main process of the container. This is different from CMD and they often work together. CMD sets default parameters that can be override nwith the docker CLI when a container is running. On the other hand, entrypoint defines default parameters that can't be overridden you run a docker container and use different CLI commands.
- Example: `ENTRYPOINT ["python", "app.py"]`

### 8. `ENV`
- Defines environment variables.
- Example: `ENV NODE_ENV=production`

### 9. `EXPOSE`
- Declares the port the container listens on.
- Example: `EXPOSE 8080`

### 10. `VOLUME`
- Defines a mount point for persistent storage.
- Example: `VOLUME /data`

### 11. `ARG`
- Declares build-time variables.
- Example: `ARG APP_VERSION=1.0`

### 12. `LABEL`
- Adds metadata to an image.
- Example: `LABEL maintainer="Your Name"`

### 13. `USER`
- Specifies the user to run commands as.
- Example: `USER appuser`

### 14. `HEALTHCHECK`
- Defines a command to check the container’s health.
- Example: `HEALTHCHECK CMD curl --fail http://localhost || exit 1`

### 15. `ONBUILD`
- Specifies a trigger instruction to be executed when the image is used as a base for another build.
- Example: `ONBUILD COPY . /app`

### 16. `STOPSIGNAL`
- Defines the system call signal to stop the container.
- Example: `STOPSIGNAL SIGTERM`

### 17. `SHELL`
- Specifies the shell used to run commands inside the container.
- Example: `SHELL ["/bin/bash", "-c"]`

These are the core keywords used in Dockerfiles to define and configure containerized applications efficiently.



### Layer Caching
In our image, every line in our docker file represents a layer. We got the parent layer, the layer for setting the working directory of the container, layer for source code/dependencies, etc. 
When building the image, adding each of those layers take time. When we simply changed the parent layer, when we ran the command to create the image, it downloaded the new parent iamge, but it also redid the work for all of those unchanged layers as well.

Now let's say we changed our code in app.js. If you eventually want those changes to be available in a created container, you'll create a new image that has the new changes in your code. When changing the parent layer it took around 8 seconds to build a new image. Now when we changed our code it only took 3 seconds? When docker builds our image, it stores a cache of the image at each stage for every layer. So when before it builds images, it looks for the highest cached layer it can use

For example, in our second build, we changed the code layer, which is the third layer up. So 'COPY' and everything would be changed/rebuilt, but we can use the "WORKDIR" layer since it is before the change. So here docker only has to re-build 5 layers, which means less work and less build time. However, in our first example, we changed the parent image, our foundational layer. There are no layers before that we can use for a cache, so docker builds all of the layers again, which is why it took longer. 

- ISSUE: So we want to be able to cache layers such as 'dependencies', so that even if our source code layer is the only layer that changes, we don't have to rebuild the dependencies layer.

- SOLUTION: Simply move 'dependencies' layer lower/before the code layer in dockerfile. By doing this, if source code is the only thing tha changes, we can cache up to 3 layers.

- ISSUE 2: However, we are doing 'npm install', but the package.json is not there yet, in the image at least. package.json is only copied over in 'COPY' layer. So we need to have package.json there before we do npm install.

- SOLUTION 2: Copy over the package.json as a layer, before our 'npm install' (dependencies layer). Now our npm install layer should work perfectly. As a result, if our code layer changes, we may be able to cache up to 4 layers.

- Takeaway: Now creating the image we see we cached up to the 'WORKDIR' layer, which makes sense since the layers after it underwent some changes. Now if you make a change in your code and rebuild the image, you should see we cached up to the 'npm install' (dependencies) layer.



## How to version our images using tags:
On dockerhub, the tags for our node image indicate the node version nand linux distribution. For example '17-alpine' and '16-alpine' were some tags that indicated Node.js versions 17 and 16, with the linux 'alpine' distrubtion. Another example is '21-alpine3.18', which is a parent image for Node.js version 21 with alpine version 3.18. These tags are just different versions/variations of the same parent image for node. 

### How to create a tag
Create a tag by adding a colon ':' after the image name, and then after that colon you specify some version and linux distribution. As a result you can create multiple versions of your images, that have slight variations.
```
<!-- First let's delete all containers, images, and volumes. Don't worry about volumes, it's covered in a later section -->
docker system prune -a

<!-- Build an image with a version; so the tag named here is 'v1', which indicates its the 'v1' version. We put '.' at the end to point to the relative path to the docker file that we're using to build this docker image. -->
docker build -t myapp:v1 .

<!-- Creates and run docker container 'myapp_c' based on the image 'myapp' with tag/version 'v1'. The port mapping indicates that the process runs on port 4000 in the container, and we're linking that to port 4000 on the localhost.-->
docker run --name myapp_c -p 4000:4000 myapp:v1
```

---
### Dangling and unused images:
First let's review terminology about images and image layers.
- image layers: Docker images are built with layers. Each layer represents some instructions that build the software for that image. The docker layers are stacked, top layer using the software/tools from the layer below it, and we've created the final image.
- images: A collection of layers of software stacked together. This then creates a template, called an 'image' that's used to create containers.
- Dangling Images: Images (complete images not just individual layers) that aren't tagged and aren't referenced by any container. They often result from intermediate build steps when creating a new image.
- Unused images: Images that aren't referenced by any container, so there aren't any running or stopped containers that are using these images.

When Docker builds an image, it creates several layers. Some of these layers may become untagged and unreferenced (dangling) if the build process updates/replaces them with new versions. 
```
<!-- Here the repository name and tag are none, indicating that these images aren't tagged. So these are dangling-->
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
<none>              <none>              4e7c8db2e3f0        2 days ago          1.23GB
<none>              <none>              b6d7e2d9f12b        2 days ago          1.23GB
```
