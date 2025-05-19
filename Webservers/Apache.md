
- ProxyPass: Used to change the headers sent from a application server to Apache. SO if


Your app is on localhost:80080 and wants the user to go to `http://localhost:8080/new_path`, so it'll send a reidrect with the location header to that new path. Apache takes this and sends it to the browser, but the browser will only go to `http://localhost:8080`, receiving an error


- ProxyPassReverse: Intercepts those headers and rewrites them to match the Apache prox yserver.
- ProxyPass: Creates a reverse proxy that appears to teh client like an ordinary web server. The client makes request to the domain, and the reverse prox ydecides where to send those requests, and returns the content as if it were the origin.


### Virtual Hosts
A virtual host allows you to host multiple web apps on a single physical server. Each site can have its own domain name, configuration, content, as if they were on separate physical servers.

- VirtualHost:
- ServerName: Specifies the domain (e.g. `js2test.ear.180013.projects.jetstream-cloud.org`)
- ServerAlias: Additional aliases are sometimes used for domain redirects or handling multiple domain names
- ServerPath: Specifies the directory where the files for that domain are located.

You usually look inside the `/etc/apache2/sites-enabled` or `/etc/httpd/sites-enabled` directory to find the config files for the virtual hosts that you are using.


## Credits 
[Apache Virtual Host Documentation](https://httpd.apache.org/docs/2.4/vhosts/)