# Parent image for node applications; we're going to use the 17-alpine tag
# A tag is a way people can version their images. This means we're running Node.js 
# version 17 running on the Alpine Linux distribution.
FROM node:16-alpine

# Inside the container, we're installing nodemon globally
RUN npm install -g nodemon

# Settings the working directory for commands liek RUN, CMD, ENTRYPOINT, COPY, and ADD that follow this command in the Dockerfile. This creates 
# an '/app' directory inside the container (if it doesn't already exist) and sets it as the current working directory. 
WORKDIR /app

# The package.json from our local machine is copied into the container "/app", which equals . because we set the working directory.
COPY package.json .

# Inside the container, and inside the "/app" directory, we can do "npm install", and this will work because a packaeg.json is in the /app directory.
RUN npm install

# Copy files from our current working directory (on local machine, typically where the dockerfile is) to the container's working directory (/app). 
COPY . .

# Specify a command when the container runs. So when the container runs, we're going to be running this script in our package.json. Of course for this 
# to work, make sure that your package.json in your local machine, you should have this script available.
CMD ["npm", "run", "dev"]

# This indicates that our API, this process, will run on port 4000 inside the container. It doesn't publish the port on our local machine, meaning that
# you're not going to be able to do requests on port 4000 on your local machine to access the API. To do this, you'll have to do port forwarding, 
# bridging traffic from port 4000 on the container to a local host port on your own machine. That will have to be done as a command when building 
# the image (via commmand) rather than in the Dockerfile.
EXPOSE 4000