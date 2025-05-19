# Caddy File 


### What is a `Caddyfile`
The file where you place your configurations for caddy. It's intended to be easy to interpret and modify. Here's an example:
![](https://caddyserver.com/old/resources/images/caddyfile-visual.png)

### Blocks, site blocks, and global options block
A Caddyfile consists of at least one or more "site blocks", which will represent the sites that the Caddy web server is interacting with. These will indicate the address of the site and how we want Caddy to integrate wtih that site. Here we have site blocks for `www.example.com` and `example.com`. There's also the global options block, which sets configs that apply to all sites. Most common one is listing "debug" to get logging. Above we listed `example.com`, which will make sure to establish HTTPS connections for the site. And yeah there's a certificate.

### Directives
Directives are keyords that customize how a site is served. they'll appear within site blocks. So look at above, we're making Caddy act as a reverse proxy for `example.com`. Here the keyword `reverse_proxy` is the directive.
- lb_policy: A subdirective within the directive block. Here it sets the load balancing to be used between backends. 
- Global options block: A block for setting global configs, it should be at top. For the rest, we 
- Snippest/Named Routes: Appear next.

#### Directives: Tokens and Quotes
A Caddyfile is reprsented as tokens before parsing. The whitespace matters a lot since because are separated by whitespace. Directives often need arguments.
```
<!-- Two args -->
directive abc def

<!-- One arg -->
directive "abc def"
```

### Addresses with Caddy
There are different ways to specify an address in a Caddyfile. How this address is interpreted will determine the type of connection (HTTP or HTTPS) that the Caddy web server will establish with the client.

- `example.com`:
  - Caddy listens for requests to the domain `example.com`
  - Caddy enables HTTPS for this address and automatically manages the TLS certificates.
  - Client connects to Caddy over HTTPS
- `http://example.com`:
  - Caddy explicitly serves the site over HTTP, rather than HTTPS since it was explicitly stated.
  - Useful if you don't want to use HTTPS for some reason (e.g. local testing).
- `localhost`: Caddy sets up an HTTPS connection for `localhost` using a self-signed certificate. This is useful for local development.

#### What happens when you specify an address
1. **Caddy listens for requests:** Caddy listens for incoming requests to the specified address (e.g. `example.com` or `localhost`)
2. **Connection Type:** Caddy determines whether to use HTTP or HTTPS based on the address format that you specified.
3. **TLS Management:** If HTTPS is enabled, Caddy automatically handles TLS certificates for the address. As a result, you don't directly have to manage it.

#### Caddy's role: A web server, reverse proxy, or load balancer?
Caddy is a web server, and it can perform multiple roles depending on the configuration. However by default:
  - Listens for HTTP/HTTPS request to the specified addresses.
  - Serves static files if configured to do so.
  - Handles HTTPS for domain names specified.

You can then configure Caddy to be a **reverse proxy** for specific sites, using the `reverse_proxy` directive. Remember that a reverse proxy is a specific role where a server:
- **Intercepts client requests** and forwards them to one or more application servers (e.g. Express, Django, etc.s)
- **Handles responses** from the backend servers and sends them back to the client.

```
example.com {
  reverse_proxy localhost:5000
}
```
Here Caddy listens for requests to `example.com`, and then forwards those requests to a backend server running on `localhost:5000`. The backend server processes the request and Caddy sends the response back to the client. To expand on this, a **load balancer** is a type of reverse proxy that:
- Distributes incoming requests to multiple backend servers, typically multiple instances of the application server,  to balance the load.
- Ensures high availability and fault tolerance by routing traffic to healthy servers. 
```
example.com {
  reverse_proxy {
    to backend1:5000 backend2:5000
    lb_policy round_robin
  }
}
```
So here nested in the `reverse_proxy`


A static file server (e.g., serving HTML, CSS, JS files directly).
A reverse proxy (forwarding client requests to another server).
A load balancer (distributing traffic across multiple backends).
A TLS terminator (handling HTTPS connections).






### Matchers 

### Placeholders

### Snippets

### NamedRoutes

### Environemnt Variables

## Credits:
- [Caddyfile Docs](https://caddyserver.com/docs/caddyfile)
- [Caddyfile Concepts](https://caddyserver.com/docs/caddyfile/concepts)
- [Caddyfile Tutorial](https://caddyserver.com/docs/caddyfile-tutorial)