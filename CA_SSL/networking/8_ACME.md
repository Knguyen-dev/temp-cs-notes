# LetsEncrypt and ACME Protocol

### Self-signed vs Signed certificates
**Self-signed:** Certificates that have been signed by the owner. Meaning the signature (encrypted server public key) was encrypted by the the site itself, rather than a trusted third party. That's bad practice because any attacker or person can generate their own certificate and claim to be the real `goodwebsite.com`. If you blindly trust self-signed certs, you'll likely get a victim of a man-in-the-middle attack.

You usually only use self-signed in a controlled environment like for development or testing, and then use a CA issued cert for prod.

**Signed:** Certs that have been signed by the CA, which is a trusted third party.

### Traditional/Old Cert Management
We have a webserver hosting `example.org`. When we get an HTTPS request, we need to return a signed cert. Usually a server admin needs to get this cert from a CA, and configure the server to use that cert. Here are the steps involved:

1. Create Certificate Signing Request (CSR)
2. Submit CSR to a trusted certificate authority (CA)
3. Admin needs to prove that they own `example.org` before the CA can actually issue a cert.
4. Obtain signed cert from the CA.
5. Update server config to use this signed cert.
6. Now when the webserver receives HTTPS requests, they can present this signed cert in the SSL/TLS handshake, therefore proving their identity and allowing for the setup of encrypted communication. 
7. We'll continue using this signed cert until it expires. After it expires, the server admin has to manually do this process over again.

This is a time-consuming and cumbersome task, and it's easy for a server admin to forget to renew a cert before it expires.


### Introducing ACME (Automatic Certificate Management)
ACME is a protocol that simplifies the process of obtaining and managing signed certificates. Essentially, it automates the process, so that a person no longer needs to manually renew or manage signed certs:
- Eeveloped and used by LetsEncrypt and became a standard in 2019.
- There are two key parts to the protocol:
  - **ACME Server:** The CA that's capability of issuing signed certs automatically. This is responsible for sending ACME "challenges" to the client, which will test if the client actually owns the domain they want to certify. If they successfully complete the challenge, then the server will issue the cert.
  - **ACME Client:** Typically runs on a webserver, but it's responsibility is to prove to the ACME server that the webserver really does control the domain name that it's requesting a certificate for. Also the ACME client and server communicate using JSON and HTTPS.

### Account Creation and Certification Process
The ACME client needs to create an account with the ACME server. It'll then generate a public/private key pair and send an account creation request (ACR). The ACME client includes its public key, which will be signed by the ACME server's private key. The ACR can also include contact info to notify an admin if there's anything wrong with the account. This process is done only once. Now let's talk about the process to get the certification:
1. Client sends request for a certificate to the server for a given URL.
2. Server sends over a challenge to the client, and successful completion of said challenge will prove that the client owns the domain. Here are the possible challenges:
  - Provision an HTTP resource with this specific content at this location. So just host a file with some kind of random content on your site to prove that you own the domain.
  - Provision a DNS record with specific content under `example.org`.
  - They just need to complete one of these challenges to be verified
3. The client will tell the server that the file or http resource is ready.
4. Server does a get request that will look like:  
  - `GET http://example.org/.well-known/acme-challenge/...`. 
  - If the server sees the content is present, then they know that the client owns the domain. Because who else is hosting a file at this specific location with the exact matching hash/content? No one except for the client wanting to certify that domain.
5. Client has now proven ownership over domain. Now the client does these steps:
  - Generates public/private key pair.
  - Generates Certificate Signing Request (CSR) using the key pair
  - Send CSR to the ACME server.
  - CSR is signed with the client's private key.
6. Server verifies the signature and then sends back a signed certificate to the client to use.
7. The good news is that the process to renew the certificate is exactly the same as obtaining one. So this can me automated, and that's what the ACME protocol suggests. 

---
### ACME Implementations
LetsEncrypt is a popular server-side implementation of ACME, which isn't surprising because they literally created the standard and developed an implementation.

There are also many client implementations for various different languages. The goal is ot have ACME clientes integrated directly into your web servers. As a result an admin doesn't need to install an ACME client on their own.

I'm guessing they mean install it as a library so that the developers deal with it?

### Handling automation
A LetsEncrypt issued certificate expires 90 days after it has been issued, and LetsEncrypt recommends that you renew it after 60 days have passed. To automate this, just create a script and have a CRON job that runs once a day.

In the script:
  1. Check if the server certificate is due to expire in the next 30 days.
  2. If it does, use the obtain certificate operation you have, and reinitialize your key-manager, which basically means that you're configuring your server to use the certificate.


## Credits
1. [ACME Protocol](https://www.youtube.com/watch?v=rIwszDULXvc)
2. [How it works - LetEncrypt](https://letsencrypt.org/how-it-works/)
