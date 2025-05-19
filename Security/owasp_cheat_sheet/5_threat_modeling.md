# Threat Modeling
Threat modeling is basically us going through and trying to find security holes in it. Throughout the software development process, developers and security engineers use this idea of threat modeling. There are four main steps to this threat modeling process.

### Threat Modeling Explained:
#### Step 1: Assess and Diagram the Application
Get a description the application, system, or network we're modeling. We're trying to identify:
- Components such as database servers, web servers, libraries, etc. that are apart of the application. 
- How do these components exchange data? Do they use encryption? What protocols are they using?

Then you diagram this high level overview out:
![](https://cf-assets.www.cloudflare.com/slt3lc6tev37/5bjpgWGls2OeL00T3Vnqw1/157fb99b19200278245afa18d7f10bd8/threat_modeling_simple_example-1.png)


#### Step 2: Identify Security Flaws 
Now we have to identify security issues. This is the main part and obviously the tough part. You could be looking at potential injection vulnerabilities, lack of input sanitization, lack of cryptographic methods and security, bad authorization, bad authentication, etc. We can use that component/data diagram to help us out, but you're probably going to also need the source code sometimes as well. 

In this example we can see that the connection between the web server and the database server isn't secure. Without HTTPS traffic isn't encrypted, and also without TLS (certificate of auth), people can do MITM to impersonate as the database server or web server. An example is that the attacker impersonates the web server, and can now send really bad SQL queries to our database.

#### Step 3: Fix Flaws
Threat modeling takes place throughout the development process, the entire SDLC. So fixing the flaws could change the application architecture, or deploying a quick fix. 

#### Step 4: Verifying Changes Have Applied
Now you should run the app in a dev environment to see if the TLS is configured, if the connections are HTTPS, etc. You'd do checks for other things that you've applied.

### Comparing Threat Modeling With Other Methodologies
There are other methodologies to identify security issues within an app. Here is **STRIDE**, a methodology developed by Microsoft. It's an ancronym:

- **Spoofing:** IP spoofing.
- **Tampering:** The idea of maliciously changing persistent data (like in a database), or changing data that's being transmitted over the network. So like how users can try to tamper with a JWT.
- **Repudiation:** The idea that an app has no logging or auditing. Or hte logs can be altered by attackers. We aren't able to "repudiate" or deny these actions.
- **Information disclosure:** Unauthorized access to data.
- **Denial of service:** A threat that tries to deny access to valid users. So like making the site tepmorarily unavailable.
- **Elevation of privilege:** When users or attackers gain privilege access in an unintended way and to try to compromise the system.

The idea is to look for these specific threats across your application architecture.


### IP Spoofing

The idea of faking the IP of your computer to hide yourself and impersonate another computer system. You'd usually use this to DDoS against a target device or an entire network. Blocking the IP of the device making the request is worthless, it's fake, and the attacker can easily change it. 

The solution is packet filtering, meaning we can stop spoofed packets from at least entering the network. A common approach against psoofing is ingress filtering, a tyep of packet filtering that's implemented on **edge devices**, nodes near the edge of the network. 

On the technical level, look at the source headers on teh apckets, if those headers don't have their origin (they look fishy) then reject those packets. Some networks have **egress filtering**, which looks at packets exiting a network and verifies the source headers to make sure those packets have been sent by a legitimate node. This prevents people from wtihin the netwrok fro mlaunching an outbound malicious attack using IP spoofing.



## Credits
- [Threat Modeling - CloudFlare](https://www.cloudflare.com/learning/security/glossary/what-is-threat-modeling/)
- [Threat Modeling Process (STRIPE) - OWASP](https://owasp.org/www-community/Threat_Modeling_Process)
- [IP Spoofing](https://www.cloudflare.com/learning/ddos/glossary/ip-spoofing/)