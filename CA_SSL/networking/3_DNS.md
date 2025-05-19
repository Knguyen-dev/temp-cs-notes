# DNS (Domain Name System)

### What is DNS
We access websites through domain names like `nytimes.com`, however web browsers wokr with IP addresses. DNS translates domain names into IP addresses, allowing us to type in website names to travel to those sites. Remember a **domain name** is a unique easy to remember address to remember a website e.g. `google.com` or `facebook.com`. Each device has a unique IP address that others can use to find them. DNS servers remove the need for us to remember IP addresses, but now we can just type in the domain name that maps to that IP address.

### How does DNS resolution work?
First we have four types of DNS servers involved in loading a webpage:
- **DNS recursor:** Designed to receive quieries from client machines, then makes additional requests to find the domains. The librarian that looks for a book. This is like step 0, it initiates the process of the seasrch.
- **Root nameserver (Root server)**: The index that points to different racks of books in the library. Itt points to different locations. Step 1 in the search proces.
- **TLD nameserver:** A specific rack of books in a library. Step 2 in the search.
- **Authoritative nameserver:** Dictionary on a rack of books. If it founds the requested "record" it'll return the IP address of that requested hostname back to the DNS recursor (librarian.) Last step in the search. 

**Note:** If you're querying for a subdomain, there will be another nameserver after teh authoritative nameserver, which is responsible for storing the subdomain's record and therefore IP address.

#### Steps to how DNS works:
1. User types `example.com`, which is received by recursive resolver.
2. Resolver queries DNS root nameserver.
3. Root server responds with a TLD DNS server, which stores info about the domains it has.
4. Resolver queries the TLD, and TLD responds with the IP address of the domain name they wanted.
5. Resolver quieries the domain's nameserver to get the IP. Then the resolver responds to the browser with the IP address.

#### What is a DNS record?
Whilst learning about DNS, you may hear about **DNS records**. These are instructions that live in authoritative DNS servers and provide info about a domain including the IP address associated with that domain. They consist of a series of text files written in "DNS syntax", which just tell the DNS server what to do. And yes all records have a TTL to indicate how often a DNS server should refresh that record, because maybe the info of that record has changed.


### DNS Caching 
The idea is that after we get an IP address associated with a domain name, let's cache it so that on future requests, we can deliver those IP addresses to other clients. This reduces the amount of times we need to go down the chain, and gives faster response times. The main form of this is browser-level DNS caching, as browsers cache the result for a set period of time.

### DHCP 
DHCP is a networking protocol for dynamically assigning IP addresses to each host on your organization network. So for a given network, DHCP makes sure all of those devices have an IP address. It also assigns DNS address.


## Credits
- [What is DNS and how it works - Cloudflare](https://www.cloudflare.com/learning/dns/glossary/what-is-a-domain-name/)
- [What are DNS records](https://www.cloudflare.com/learning/dns/dns-records/)
- [DHCP and Dynamic DNS](https://www.cloudflare.com/learning/dns/glossary/dynamic-dns/)