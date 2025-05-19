# FastAPI Notes

### What is it?
A web framework for creating APIs. Though in order to serve APIs, FastAPI uses a package called uvicorn as its webserver.

uvicorn is a webserver that uses ASGI, as opposed to WSGI. These are just protocols for communication between web servers and Python web apps. The former basically just supports async/await.

In terms of technical stuff, with WSGI, a worker thread gets blocked when waiting for a db call or API call. Though other threads are unaffected. With ASGI, a given thread can multitask better and start handling another request whilst waiting for the slow database or API call to finish. 

So technical switching to ASGI can increase the max throughput on the server, but not the speed of each individual request.

## Credits
https://fastapi.tiangolo.com/