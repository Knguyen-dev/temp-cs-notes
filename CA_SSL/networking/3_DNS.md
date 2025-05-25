# DNS (Domain Name System)

### What is DNS and how does it work?
We access websites through domain names like `nytimes.com`, however web browsers wokr with IP addresses. DNS translates domain names into IP addresses, allowing us to type in website names to travel to those sites. Remember a **domain name** is a unique easy to remember address to remember a website e.g. `google.com` or `facebook.com`. Each device has a unique IP address that others can use to find them. DNS servers remove the need for us to remember IP addresses, but now we can just type in the domain name that maps to that IP address.

1. Client types in `example.com` in a web browser and it travels through the internet to be received by a DNS resolver.
2. Resolver recursively queries a DNS root nameserver. Root server responds the the address of a Top-Level-Domain (TLD).
3. Resolver makes a request to the `.com` TLD. TLD server responds IP address of the domain's name server `example.com`.
4. Resolver sends a query to the domain's nameserver, and receives the IP address for `example.com`.
5. DNS resolver responds to the web browser with the IP address of the domain. The client should now be able to see the webpage.

---
### Server Types
First we have four types of DNS servers involved in loading a webpage:
- **DNS Resolver (recursive resolver):** This is step one, it's the middleman between the client and the DNS nameserver. After receiving a DNS query it either uses cached data or makes requests. It can request the root nameserver, TLD nameserver, and then the authoritative nameserver. After it receives a request from the authoritative nameserver, the resolver sends a response to the client.
- **Root nameserver (Root server)**: Accepts resolver's query and responds by directing them to a TLD nameserver. It does this based on the extension of the domain `.com`, `.net`, etc.
- **TLD nameserver:** Maintains info for all domain names that share a common extension. The resolver gives a domain name, and the nameserver responds with an IP address.
- **Authoritative nameserver:** The last step in the journey for an IP address. It contains specific info about the domain it serves. It'll respond with the IP address when the domain has a DNS A record. Or it can respond with a CNAME, causing the resolver to do a whole new DNS lookup.

---
### Query Types 
- Recursive: DNS client needs a DNS server (typically the DNS recursive resolver) to get the information.
- Iterative: DNS client gives the hostname and the DNS resolver returns the best answer it has.
- Non-recursive: When DNS resolver already has the answer, due to a cache or it's sure of the DNS name server.

---
### What DNS Record Are
DNS records (zone files) are instructions that live in the authoritative DNS servers, and provide info about a domain including its IP. Records are just a series of text files written in DNS syntax.

---
### Explaining subdomains
A subdomain is a additional part of our main domain name. You'd use this to logically separate your website into sections . For example, `blog.example.com`:
- `blog` is the subdomain
- `example` is the primary domain
- `.com` is the top level domain.

## Credits
- [What is DNS and how it works - Cloudflare](https://www.cloudflare.com/learning/dns/glossary/what-is-a-domain-name/)
- [What are DNS records](https://www.cloudflare.com/learning/dns/dns-records/)
- [DHCP and Dynamic DNS](https://www.cloudflare.com/learning/dns/glossary/dynamic-dns/)