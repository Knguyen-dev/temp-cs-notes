# Caddy Docker Proxy

### The 

We use a specific image that integraets caddy with Docker. The plugin looks for `labels`. So if a service has a label, this indicates that hte service should be served by Caddy. 

Then an in-memory Caddyfile. In this Caddyfile, there will be site entry blocks and proxies that point to each Docker service using its DNS name or container IP. 

## Credits
https://github.com/lucaslorentz/caddy-docker-proxy