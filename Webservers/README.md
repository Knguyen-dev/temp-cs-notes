# Webservers 

### What is a web server:
Refers to software or hardware systems that handle requests from clients and seve web content such as HTML pages, images, and other resources. They process HTTP request, interact with application servers or databases if needed, and send the apppropriate response back to the client. Popular web severs incldue Apache HTTP Server, Nginx, and Microsoft Internet Information Services (ISS). 

#### Hardware and Software Aspect:
Anything with a network connection can be a webserver such as a phone, small IoT device, etc.

- **Hardware Side:** The computer needs to store website files like html docs, images, files, etc.
- **Software Side:** Software that lets clients request data. Has several components, but at minimum we have an HTTP server, which is the software that understands the HTTP protocol and can handle HTTP requests.

#### Review of HTTP and Terminology:
- **HTTP Server**: Parses HTTP requests and abstracts low-level handling.  
- **Web Server**: Includes an HTTP server, file storage, and supports other protocols.  
- **Static Web Server**: A computer running HTTP server software.  
- **Dynamic Web Server**: A computer with HTTP software and a persistent data store.  
- **Application Server:**  Handles dynamic content, often interacting with a database. This is often interchangeable with the dynamic web server.
- **Origin Server:** The server that's the origin of the content being served. It could be static or dynamic.
- **HTTP** (application layer) = Language of communication.  
- **TCP** (transport layer) = Handles packet transmission and reconstruction.  
- **Static Routing**: Retrieves files directly from the web server's filesystem. So this is images, static files, etc.
- **Dynamic Routing**: Requests resources based on specific identifiers or traits. So like looking for posts with a certain ID.


### Web Servers and Their Role in Development

#### Development Focused web servers
Frameworks like Express and Django allow us to run development-focused web servers. These are typically built into the framework to simplify development and testing. You could use it as your web server in production, but it isn't designed for that and won't scale well.

#### Production-Grade Web Servers
Web servers like Nginx and Caddy are designed to handle high traffic, serve static files efficiently and act as reverse proxies. I mean Nginx is a popular high perofrmance web server that can act as a reverse proxy, it can load balance traffic, and forwards requests to application servers like Express or Django. I'll explain this aspect later. Web servers like Caddy are modern, simple, and have automatic HTTPS and security a lot easier.

#### How They Work Together 
In a production environment, development-focused servers (like Express or Django) are typically used as **application servers**, while production grade servers (like Nginx or Caddy) act as **reverse proxies**. Here's how it works:

1. **Reverse Proxy Role:** A production grade web server sits in front of the application server. It handles incoming client requests. It directly handles serving static content (e.g. html, css, js), but when it gets a dynamic request, the reverse proxy will forward it to an application server.
2. **Load Balancing:** If there are multiple instances of the application server, the production grade server can distribute traffic among them.
3. **Security and Performance:** Production grade web servers handle SSL/TLS (HTTPS), caching, and compression, offloading these tasks from the application server .

#### Example Workflow 
1. Client sends a request to teh domain
2. The request hits the production-grade web server (e.g. Caddy or Nginx)
  - If it's a static file request, such as getting the site's logo, the production grade web server will send a response and handle the request directly.
  - If it's a dynamic request, then the server forwards it to an application server (e.g. Django or Express). 
3. The application server processes the request and sends the repsonse back to the proudction grade web server.
4. The production grade server forwards the response to the client.



#### Webservers vs Application Servers
### What will this consist of?
- The theoretical stuff.
- And also practical stuff like projects, how to setup files, going through the motions
- The main ones are Caddy, Apache, and Nginx. I think we use the former 2.

## Credits

### Intro to web servers
- [What Is a Web Server - Hostinger](https://www.hostinger.co.uk/tutorials/what-is-a-web-server)
- [Intro to Webservers - MDN](https://developer.mozilla.org/en-US/docs/Learn_web_development/Howto/Web_mechanics/What_is_a_web_server)

#### Caddy 
- [Making a basic static website with Caddy](https://www.youtube.com/watch?v=WgUV_BlHvj0)
- [Caddy Offical](https://caddyserver.com/)
- [Caddy Docs](https://caddyserver.com/docs/getting-started)
- [Caddy Github](https://github.com/caddyserver/caddy)





[Simple 7 minute Caddy tutorial](https://youtu.be/WgUV_BlHvj0?si=UAk-3lmGBM6llq1f)
[Apache vs Nginx](https://youtu.be/9nyiY-psbMs?si=7YU7d-rCs60fn20q)