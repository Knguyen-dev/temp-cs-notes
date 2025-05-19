# WSGI and ASGI

### What is a WSGI?
You have a webserver on one side (Apache, or Nginx) and then a python web application on the other side. The web server gets HTTP requests, and then magically those requests get to the python web app. The middle man that facilitates this communication is an application server that implements the (WSGI Protocol), allowing these two to communicate.

### Why do we have it in the first place (history lesson)
HTTP protocol is static, meaning you can only be returned a static file or data. If you type in `www.example.com` you're going to be returned an html file that renders the home page. In 1989 you couldn't return an HTML form, they didn't have the ability to process user details in html at the time, so something like

```HTML
<form>
  ...
</form>
```
Didn't exist yet. The solution was instead of sending back an html file, for some requests we would make that request execute a file like `form.py` (they used Perl). This file would process their info just like an html form. The script file (`form.py`) would inherit the web server's (parent process) environment variables. So things like the query string, path queried, and other things describing the request. This info went into our script file, and then the script file would output info that would be our http response. 

One thing that happened a lot was that the names of the environment variables that we passed from the web server to the script file were randomly named, as developers just came up with different names. As well as this, they were many different web servers that also enforced different naming conventions. This made using a script file from one project to another pretty tough as we always had to refactor due to the differing names. Developers wanted to standardize this, and so common gateway interface (CGI) was created.

#### What is CGI?
- A standard for the environment variable names and purpose when being passed from web server to some external script. So the environment variable for the path was `PATH_INFO`.
- They then standardized the way the script must be called, saying that the script file must have a function.

#### Purpose 
To standardize the way external scripts were executed on web servers. It was designed to be easily integrated and used. So now the script file had to be like this:
```Python

def application(env, foo):
  # get first_name, last_name, and email from env
  http_headers_list = [('Content-type', 'text/plan')]
  foo("200 OK", http_headers_list)
  return "Thank you for your request"
```

#### Tying it back together (Workers)
One important issue is that loading external scripts takes a lot of time. The time it took the webserver to parse the request and for the script file to send a response, was a fraction of how long it took to load script file's binary from the hard-drive. By the way the process of fetching that binary is called "forking".

The solution to this was using in-memory workers. When the web server was idle, we would create web server workers. In this context, they're just instances of python interpreters and the dependencies to run them. These workers would work together to load the Python binary (pre-working). While web servers like Nginx can't prefork, the middleman web servers like uvicorn or uWSGI can pre-fork and load your WSGI app (your python web app).


### WSGI 
Let's explain with an example. I want to make breakfast with eggs and coffee.

I will first make tea and then omelette. I'm doing these tasks in synchronous/sequential order. This is how WSGI works, it takes a request, gets your data, then it returns the response. Then it the request that came after. For a WSGI-based application to process your request, it has to finish processing all requests before yours. However this doesn't mean that a WSGI-based app can only process one request at a time. 

A single worker (process or thread depending on how it's implemented) can only handle on request at a time. However to handle more requests, the WSGI application can just spawn in more workers. The maximum number of workers being 2 times the number of cores plus 1. So you can only have so many workers.

### ASGI

It starts making the coffee, and it heats up the coffee. We're not really doing anything whilst heating up the coffee, so rather than sitting around and waiting, we'll start on our second task of making an omelette. We do the steps for making the omelette, not much waiting is done here so we keep making the omelette, and now we're done with it. The coffee has heated up so we do the final step for this task and add the milk.

Essentially for ASGI, a worker can handle multiple requests at once. This is concurrency, as if the worker sees it's going to be waiting around a lot, it's going to focus processing another request, and then once we're done waiting, it goes to process that former request. It doesn't process the two at the same time, so it isn't parallelism.
```Python
# Request 1
async def home(request):
  # task 1
  # task 2
  # long task 3 - proceed to process Request 2
  # task 4
  return response

# Request 2
async def dashboard(request):
  # task 1
  # task 2
  # task 3
  # task 4
  return response
```

### Which is better?
You may think that ASGI is automatically better, but in order to get the best performance you need to analyze the context.

ASGI can handle a higher number of requests per unit of time. However in terms of latency, there may not be much of a difference. It also depends on the way the developer wrote the asynchronous code. In the worse case scenario, ASGI may take longer than WSGI.

You can reason that ASGI is going to be better at handling IO bound tasks. For CPU bound tasks, you may prefer WSGI, but the likely thing is that there's not going to be a noticeable difference. Maybe if you scale up the number of requests, there could be a difference.


## Credits
- [All you need to know about WSGI](https://www.youtube.com/watch?v=UklcIDgHtwQ)
- [WSGI and ASGI explained](https://www.youtube.com/watch?v=UklcIDgHtwQ)