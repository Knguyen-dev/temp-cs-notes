# Watchtower

### What is it?
It's an app that monitors your running Docker container sand watches for changes in the images that they were built from. If it sees that an image has changed, the corresponding odcker container will be restared using the new image.

The idea is that using watchtower, you can update the running version of your application by pushing a new image to the container registry that you set things up with. It'll pull down the new image, shutting down the existing container and restarting it with the new image, also starting it with the same options that were used to deployment initiallly.

```
$ docker ps
CONTAINER ID   IMAGE                   STATUS          PORTS                    NAMES
967848166a45   centurylink/wetty-cli   Up 10 minutes   0.0.0.0:8080->3000/tcp   wetty
6cc4d2a9d1a5   containrrr/watchtower   Up 15 minutes                            watchtower
```
So let's say we are running watchtower and it's configured at some location. 