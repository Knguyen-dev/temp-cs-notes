# OWASP Top Ten
The OWASP Top ten is jsut a popular list that consists of the 10 most crtiical application security risks. It was compiled by the Open Web Application Security Project (OWASP). It's just a 


## 1. Broken Access Control
This is just the general mistake that user permissions in the application aren't what htey're supposed to be. As a result we have data being leaked, modified and even destroyed by unauthorized users. So here are some common vulnerabilities:
- Violations principles of least privilege, so bad privileges off the bat.
- Users can somehow modify the url, html page, localhost, or something like that to bypass checks.
- Users can edit another person's account or stuff by merely having the other's ID. 
- Acting as a user without even being logged in, or a user having admin privileges.
- User can tamper with metadata scuh as a JWT, or cookie to get privileges.
- Cors is bad so untrusted origins can acces the API.

Just stupid stuff like this. How to prevent this:
- Have private routes, think in least privilege, rate limit your API, disable directory listing, pay attention to rbac and make sure a user can do a request.

## 2. Cryptographic Failures
The lack or failure of cryptography. Sometimes data is transferred as clear text using insecure protocols. Or they use outdated cryptographic algorithms, the secret keys are weak or are in github repos. The CA may not be valid. Prevent this by using HTTPS, use established encryption algorithms. Use secure protocols, store passwords in their hashed form. 

## 3. Injection Attacks
This mainly happens when:
- User data isn't validated.
- We aren't using prepared statements are parameterized ones. 
- Or in ORMS if we're building dynamic query strings, and the typical thing is that that malicious input is sent there.

Just use a safe API that does parameterized queries or prepared statements. For ORMs, don't try to build query strings dynamically, just use built in methods that help you query instead.

## 4. Secure Design in the SDLC (Threat Modeling)
Just the idea that when building your application we should build with security in mind. This is more so in align with the software development cycle:
- Write unit and integration tests
- Integrate security language in user stories.
- Developers and Cybersecurity professionals should do threat modeling, which is the idea of planning out the kinds of security holes that we could find in the application. Threat modeling should be standard.

## 5. Security Misconfigurations
This is just the idea of dumb decisions. Unenecessary features, suc has port, accounts, privileges. Maybe the error handling shows really confidental errors to the user. Very obvious security recommendations aren't used, or even the software is out of date. Or if the server isn't setting up security headers like `X-Frame-Options: Deny`, `Content-Type`, and `Content-Security-Policy`.

## 6. Vulnerable or Outdated Components
You're vulnerable when you don't know the versions of frontend and backend components or packages, or other pieces of software. If things are out of date, there's more risk. If you don't scan for vulnerabilities you increase the risk, if you don't chekc the compabiltiies, you're at risk, etc. Here's how to fix it:
- Remove unnecessary or outdated things
- Use frameworks and libraries such as OSAWP Dependency Check, reitr.ejs, etc. to continuously check for Common Vulnerability and Exposures (CVEs) and National Vulnerability Database (NVD).

## 7. ID and Auth failures (Broken Authentication)
When you allow stuff like weak passwords, bad credential recovery methods, passwords are stored plaintext, missing MFA, if the session ID is in the url, or reusing a session after login. Or bad session invalidation on logout or similar. Here's how you fix it:
- Use MFA if possible, use longer passwords, limit login attempts, create new session IDs after logins. Also session ID shouldn't be in the url because people can now easily steal it and do easy CSRF.

## 8. Software and Data integrity failure .
Related to code and infrastructure. An example is when we use modules or components from unverified sources. Or when a bad CI/CD pipeline introduces malicious code, or just breaks the system. Or even how we may have an auto-update feature that downloads the lastest versions without checking if they even work. This is bad, but it's worse when you think attackers can uplaod their own code to destroy us. The solution:
- There's gpg keys or checksums that ensure that you're downloading something from the right source. 
- Make sure your libraries are using trusted repos.
- Use OWASP Dependency Check to verify components that contain known vulnerabiltiies.
- Make sure your CI/CD pipeline has proper access control.
- Don't send unencrypted data to untrusted clients. Please verify them before sending.

## 9. Logging and Monitoring
Logging and monitoring is esential to the security process. We need to keep track of real time breaches and suspicious activity. Here are the things we need:
- Log all login, access control, and server-side input validation errors. Also log high value transactions, such as bank transfers, the user buys a new game, etc.
- Generate logs in a consumable format. Logs shouldn't be local, save them to the cloud or something or multiple systems. 
- Encode log data to prevent injections or attacks on your logging sytsem.
- Your team should have monitoring and alerts working together so that your team can respond fast. You should have some kind of incident response or recovery plan whne things go wrong.


## 10. Server side request forgery
An attack that makes the server side app make requests to an unintended location. They can make you make a request to a service within your infrastructure or maybe some external service. One common way this happens is when our app mishandles urls:
```bash
# Vulnerable service fetches image
GET /fetch-image?url=http://example.com/cat.jpg

# Attacker programmatically changes image into a request for admin creds.
const imageUrl = req.query.url;
fetch(imageUrl).then(res => res.pipe(response));

# It works. As the service holding the admin credentials trusted the vulneralbe service, and so it gave it the ifnormation.
GET /fetch-image?url=http://localhost:8000/admin
```
There are two main scenarios where this happens:
- Applications can only send requests to sites on an allowlist.
- Application can send request to any external website, no allowlist.

### Scenario 1: Whitelist
An example is that an HR service can resopnd with names and personal information of employees. Then there's a vulnerable service that accesses service. Whilst the user can directly make requests to the HR Service, they may be able to use SSRF to trick the vulnerable service into making a request to the HR service. As a result, the HR service thinks this is a good request since it's coming from the vulnerable service, rather than some random user. Alright let's lay some protections:
- Application Layer: Do input validation. For the HR service, our internal application, you should have a whitelist of what ip addresses can actually access it. 



As a result we have unauthorized access or actions done within our software. Typically a SSRF exploits trust relationships, using a vulnerable application to do a weird action in other apps. The idea that, you can't access the `/admin` route directly, but you cna trick an internal service into making a request to `/admin`.


