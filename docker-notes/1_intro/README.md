# Docker notes 

## What is Docker:
Docker is a platform that helps us create and run 'containers', which are isolated environments that 'contain' our code and everything it needs to run perfectly. So you can have a container for a node app, react app, etc. Docker is a tool for managing these containers. In these notes we'll talk about the basics and foundations of what you need to know for docker. We'll of course work through basic code examples as well.

## How is Docker beneficial?
Imagine you have a node app that runs on a specific Node.js version and you want your teammates to be able to run it. Teammates would need to download dependencies, configure environemnt variables, etc. This is a lot of work just to run one app. Docker simplifies the process of sharing and running applications by packaging them in containers. These containers include all the necessary dependencies and configurations, ensuring consistent behavior across different environments. This eliminates the need for teammates to set up complex development environments, saving time and reducing potential issues.

## Containers vs Virtual Machines:
Virtual machines can solve the same problems that containers solve, but sometimes containers can be better. However, there's no hard-fast rule because there will be times where you'll use a VM over a container, so there are uses for both.

1. Virtual Machines: Runs its own operating system, which can make your system performance slower.
2. Containers: Shares the resources from the host's operating system, which makes them typically quicker and more light-weight. Now they do have a slimmed-down version of an operating system, but they still use the host machine's kernel under the hood. 

## How to download Docker:
On windows we download 'Docker Desktop', whilst on Linux you can use the docker desktop app or the engine directly.
1. First download docker desktop: [Docker Download Site](https://www.docker.com/get-started/)
2. Now we need WSL (Windows Subsystem for Linux). And set it to WSL2. For WSL2 installation/setup refer to this [link](https://replit.com/@knguyensky/redis-tutorial#README.md)
3. At this point you should have probably Ubuntu and WSL2 on your computer. Now open docker desktop and register an account. You're all done at this point, but now whenever you're using Docker, you need to make sure the docker desktop app is running.


## Images and Containers simplified:
- An image is a blueprint for a 'container', and it stores anything the application needs to run. It stores the following things, but doesn't run them:
1. runtime environment
2. The app's code
3. Any dependencies that the app needs
4. Extra configs such as environment variables
5. Commands
Images are read-only/immutable, so once created you can't change it. If you want to change things, you'd have to create another image with your change. 

- A container is the runnable instance of an image. So when you run the image, it will create a container which is a process that runs our application exactly how it's outlined in our image (blueprint).

A container is an isolated process, so it works independently of any other process on your computer. It doesn't need other processes to run in order for it to work because all it needs is inside the container itself.

- An example: I can make an image (blueprint) for my application. I got the node version, source code and dependencies. Now that I have my blueprint of what the program needs to work, I can use this image and create a container, which is a process that runs my application. So my 'image/blueprint' can create a working instance of my program. So I can share this image with my teammates. They'll use this image to create their own containers, which are just processes that are running my program, but this time on their machines. And there we go, we got everyone to run my program, and they didn't need to do any setup or install things. All they needed was the image and docker installed.

## How are images made & Parent Images:
An image is composed of layers, with each layer building upon and using the tools in the previous layer. So the order of the layers matter. Here are the typical layers explained starting from the lowest layer, the 'parent image':

1. Parent Image: Includes the operating system and runtime environment for the container we want to make/run. For example, having a particular linux distribution (OS), and a particular node version (runtime environment). This layer is an image itself, so we're basically creating a new image on top of it.
2. Source code: Our program's code.
3. Dependencies: Dependencies for our program.
4. Run commands: Commands to run the program.

## Docker Hub: 
An online site that contains a bunch of pre-made images that we can use as the parent layer for our own images. So instead of creating a parent image ourselves, we can find and download popular/widely used parent images here.

For example, let's say you wanted to create an image for a node app. Well the first thing we need is a parent image, so we look up 'node' on docker hub. Here we can find the official image. It shows a code block, which indicates how we can download the image, which is called 'pulling' in docker. So we do that by typing this command in the terminal:
```
docker pull node
```
We'll explain this more later, so hold off on downloading this right now Scrolling down you'll see 'supported tags', which are different variations of the node image that we can use. These specify different versions of node, and also different linux distributions. For example, "21-alpine3.18" indicates node version 21, with a linux dist. called 'alphine' in its version of 3.18. Also if this isn't already obvious, when making images, we should always specify the version of node, or the runtime, that we want to avoid letting docker just use the latest verison. Because the latest version could potentially have conflicts with what we had previously, etc.


### Downloading an Image and running it
- Download the node image by running this in the terminal. It doesn't matter the location, docker will put it in a specialized place. In this example since we didn't specify a version of the NodeJS runtime, docker creates the image with the latest version of node, indicated by 'Using default tag: latest'
```
docker pull node; 
```
Now going to our docker desktop, we see our newly downloaded image in our 'images' tab. The tag should say 'latest', meaning it includes the latest version of NodeJS. Now this is an image, a parent image, but still an image meaning we can 'run' it to create a container. In this case, the container will have a linux environment and node installed, and it would run that process. 

```
docker run node
```
This probably exited immediately. Well that's because we didn't add any extra commands, our node image likely defaulted and just ran node. This command runs opens an interactive shell (bash) inside the container. Allowing us to mess with a node shell. Then when we're done we can pause it using docker desktop. Congrats, we now know how to run images, and mess with containers.
```
docker run -it node bash
echo "hello world" // prints hello world
```

## Dockerfiles and how to create our own docker images:
So far we've only downloaded images, our parent images. To make an image, we use a dockerfile. The dockerfile will contain set of instructions to create the image. It will list out all of these different layers, and instructions to create those layers. Let's look in our example-api to get more familiar with this


## Volumes 

### Review on images
I create an application. I build the image with my docker file, and then I create a container. Okay great the container works, but now let me change the code for my app. Now I know if I run my container, those changes haven't taken place. Images are immutable, if I wanted to see my changes, I'd have to rebuild and update the image. Then create and run a new container based on the updated image to see my changes

This is a long winded way of doing things. However volumes can give us a workaround for this.

### How do volumes work
Essentially you can specify folders on your host computer that can be made available to a running container. You'd map folders on your host computer to folders in your container, and as a result when a specific folder on your host computer changes, the mapped folder on the container changes. By doing this we can map things such as our source code folder from our host computer to container, and so everytime we change our code on our host compute, those changes are replicated in the container and we see those changes on the container. 

Now the benefit to this is that we don't have to be frequently creating new images and then containers to see our changes. However it should be noted, that by using volumes in this way won't change your image. Changing the folder on your host machine, won't change the image, it'll only change the mapped folders. Remember for an image to be changed you mess with the docker file and build the image, which is a completely separate thing. So if you want to share your image with others or create new containers that have our chnages, we'll have to build our image. 

### Nodemon Implementation and setup
First let's install nodemon for the container. Now when the container runs, it starts our server with nodemon. So when javascript or json files INSIDE the container change, then the express server restarts. Without nodemon, when we setup our volume, the host and container files will change, but we'd need to manually restart the node instance inside the container. Now with nodemon, when files change inside our container, it auto-restarts. Last thing is that we're going to do 'npm run dev' when container starts, and '-L' flag is just needed for nodemon when working with docker on windows.

```
<!-- Create new image since we updated docker file with nodemon -->
docker build -t myapp:nodemon .

<!-- Create and run new container based off of that new image. We put 'rm' flag so that after we stop the container, it is deleted. THis is so that we don't store unnecessary containers everytime.-->
docker run --name myapp_c_nodemon -p 4000:4000 --rm myapp:nodemon
```
Now running the command, we've essentially started a nodemon process. However we aren't done yet. If we change our files on our computer, those changes aren't reflected in the container, so nodemon isn't going to restart the server. We need to map our code files to container files.

### Volume setup
To use volumes you'd do --v <host-file-path>:<container-path>. Where the host file path is the path to the folder we want to map in the container itself. So this folder will be copied AND mirrored inside the container. So any changes made to the files in this folder will be mirrored in the container. So we want our 'example-api' folder in our machine, our project/app folder, to be copied and mirrored inside the container's '/app' directory.
```
docker run --name myapp_c_nodemon -p 4000:4000 --rm -v /workspaces/CS-Programming-Notes/docker-notes/example-api:/app myapp:nodemon
```
However there is one issue and one last thing we want to consider. If we delete node_modules locally, which could happen since we don't really need it, then it's also deleted in the container. And this would cause the container to not work as it needs those dependencies like 'Express' to work. 

 By default, Docker creates a separate file system for each container. Here we map the local node_modules directory in the container to a local directory that Docker creates and manages for us. So changes made to node_modules on our local machine doesn't affect the node_modules inside the container because the node_modules inside the container is managed by docker now.
 `-v /app/node_modules`


So our final script would be this. So now changes made to node_modules won't be replicated 
```
docker run --name myapp_c_nodemon -p 4000:4000 --rm -v /workspaces/CS-Programming-Notes/docker-notes/example-api:/app -v /app/node_modules myapp:nodemon
```
 
 - NOTE: You'd want this primarily for performance reasons and consistency. 'node_modules' is big, so excluding it from volume mapping allows us ot avoid copying a large directory from local to container everytime we make a change. Allow 'Docker' to manage it, which is more effective. Also we ensure the dependencies in our app are consistent. If we were to change and add packages to our loca lmachine, and could be introducing conflicts with dependencies in the container. If we wanted to add new packages, re-build your images to have good separation and delineation.

## Sharing images on docker
1. Go onto your Dockerhub and login (create a free account if not already done).
2. Create new repository. Our namespace is 'knguyendocker44' and the name we gave the repo is 'my-first-docker'. So the actual name of the image, is the combination, so 'kngueyndocker44/my-first-docker'.


```
<!-- Let's build the api image for the api service -->
docker build -t knguyendocker44/myapi ./api

<!-- Before we can push something up to docker, we must login. Docker will prompt us for credentials. -->
docker login

<!-- Now push the image up to the repository you created for said image -->
docker push knguyendocker44/myapi

<!-- Pull a docker image from your repository -->
docker pull knguyendocker44/myapi
```
And that's the benefit of docker. A user would be able to download an image, and run the container built by the image. And it would work since that image has all of the dependencies and code inside of it. Regardless of what kind of machine a person is on, this would work.



# Credits:
1. [Docker Crash Courses (Net Ninja)](https://youtu.be/31ieHmcTUOk?si=HuiGOuP-odYcf5ZA)
2. [Docker Beginner to Pro](https://youtu.be/RqTEHSBrYFw?si=u3DAIlbRJULxxlSw)
- The second one is optional, if you ever want to get more knowledgeable.