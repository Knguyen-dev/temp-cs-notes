# API Security Core

### Authentication
- Basic Auth sends their plain text credentials (http) over the network, no security at all
- Use standard authentication mechnaisms for generating tokens, storing credentials etc. For example, use OpenID Connect or SAML. When hashing passwords, use something well established like bcrypt or scrypt standard. It's recommend that you have 2FA for your users. This requires users to provide two forms of ID to access their accounts. Many services and apps offer that.
- **Max Retry:** Limits the number of login attempts the user can make within a specific period of time. If they exceed this amount, then the user is locked out of thier account for a specified period of time. - **Jail:** Block IP addresses or accounts from making any more login attempts after exceeding the max retry. This helps prevent brute fore and bad actors from repeatedly attempting to access the system. 
- Encrypt all sensitive data. Of course you're going to encrypt passwords and use HTTPS to encrypt your traffic, but also try to think about other things. If you're a developer for an API that hands out secret keys to the user maybe you want to encrypt that key, and hten let the user decrypt it. Stuff like that, as sometimes you're not just encrypting someone's password


### Json Web Tokens (JWTs)
- Create long complex secret keys
- The backend should enforce a specific algorithm when signing jwts. The wrong of going about things would be decoding the header of the jwt to get the algorithm for hashing. This can allow users to break in because they can set the value of the jwt header to none, and likely gain control. It sounds obvious, but don't trust the client, verify the jwt with the same one you use to sign them. If it isn't valid, reject the token.
- TTL (time to live) and RTTL (refresh time to live) should be kind of short. So balance security, but also user experience.
- Avoid storing sensitive info in the payload. The payload is literally base64encoded, so anyone with a brain could decode it. Also use a small payload to reduce network overhead.

### Access Control
- Rate limit and API throttling: Set a fixed limit on how mnay requests a client can make within a defined window (e.g. 100 requests per hour). However api throttling controls the pace of requests over time to ensure stability. It's more dynamic, as we're literally reducing or controlling the speed of a request based on our server load. This helps mitigate DDoS and Brute force attacks, preventing the system from being overwhelmed.
- Use HTTPS: It encrypts network traffic. You'll need to obtain a SSL/TLS certificate and configure your server to use HTTPS. Also for the TLS handshake you need to support some ciphers as these will be used to encrypt data in transfer. Choose a common, secure cipher like AES, and avoid outdated ciphers like RC4 and TLS 1.0/1.1. The certificate will help prevent MITM attacks.
- Disable Directory Listings: Most webservers have a feature where users can view the contents of a directory on the server. Turn this off because you don't want your user being able to see this type of information. 
- Restrict Private APIs: Private APIs should only be accessible from a whitelist of IPs to ensure only authorized users or systems can access the IP. Helps hide and protect your API from malicious actors.

### OAuth2.0
- Validate `redirect_uri`: This is relevant when you're creating an OAuth2.0 service yourself. An open redirect attack is when an attacker intercepts a request from your client, and then changes the `redirect_uri`, in hopes of stealing the authorization code or access token. So just make sure things are validated. This is more relevant for the implicit flow, where there isn't a client secret though.
```bash
https://auth.example.com/oauth/authorize?
  client_id=trusted-app&
  redirect_uri=https://trusted.com/callback

https://auth.example.com/oauth/authorize?
  client_id=trusted-app&
  redirect_uri=https://malicious.com/steal-code
```
- Avoid using implicit grant flow, as it's a little less secure since someone can steal your access token and use it for themselves.
- Use `state` param to avoid CSRF. State stores a random value before initiating an authorization request. We send this value with the request, and in our callback we verify that the state parameter matches what we originally generated. If it does, we can confirm that the whole authorization was initiated by us.

### API and Input Validation
- Use proper http methods. GET(read), POST(create), PUT/PATCH(replace/update) and DELETE (delete).
- Rely on server-side validation. Validate, sanitize, and escape user input. You don't want the user to be able to execute malicious code. Like imagine on a social media app, the user makes a post with the contents being a maliciious script. Now anyone that loads thier their post is going to have their browsers affected. Also if you have file content, make sure to decode it into utf-8, as it could be different and a lot of issues can be prevented by just decoding things into utf-8. Never trust the user input.
- API Gateway: Use this for caching, setting rate limits and other security features. In this situation your services are private facing, in an internal network and the only way traffic will reach them is through an API Gateway. Use it to handle authorization and authentication, reducing the burden on individual services. Use it to rate limit, protect against DDoS, filter traffic. It could also handle encryption and decryption of traffic, and also logging. Finally you can integrate it with other security tools to give yourself more protection.

### Data processing
- Avoid personal ID in urls. Like `/users/242/orders` should be `/me/orders`. It just helps avoid the risk of exposing the user's personal ID, and it's just a better way of setting up your endpoint.
- Requests where you're processing a huge amount of data should be perofrmed as background jobs (asynchronous tasks). If you try to perform this sequentially, you're probably going to time out the HTTP request since it could take a bit to actually process things.
- **SQL Injection:** Use prepared or parameterized statements to fight against SQL injection.
  
### API Responses 
- XSS and `X-Content-Type-Options: nosniff`: Tells your browser to not sniff/guess the content type of the response by looking at hte file sent. An attacker can upload an HTML file containing malicious scripts, and disguise it as a `.jpg`. Now it's on your server, if someone requests it, your server will send it but have headers saying its an html file. The issue comes when browsers ignore the header and try to analyze the contents of the file, executing the malicious code in the file, leading to a potential XSS attack. So stop the browser from sniffing the file contents to reduce this.
- Clickjacking and `X-Frame-Options`: Set this header to deny to prevent your webpage from being embedded within an IFrame. IFrame abuse is common with fake digital engagemnt as a malicious actor can embed a live Twitch stream (Youtube Video, TikTok page) inside a hidden IFrame on a high traffic site. Then visitors are counted as views. IFrame abuse is can be combined with clickjacking 

- Content Security Policy: Setting this to `default-src 'none'` is the best practice to help prevent XSS. It tells the browser to not loda any scripts, stylesheets or images that aren't explicitly whitelisted in the CSP header. 
- Remove fingerprinting headers: These are headers that can be used to identify the web server and version that your app is running on. This info can be used by attackers to identify vulnerabilities so remove it.


### CI/CD
- CI/CD pipeline should have tests, scanning for vulnerabilities with your packages and code.
- A rollback solution is ideal as well.  You want ot be able to rollback your code to a previous vreison that works.


### Monitoring
- **Centralized Logging:** Setup agents to monitor all requests and responses. A separate monitoring service should be deployed alongside your API, tracking anomalies and performance. You should get real time alerts if anything weird happens. You'll have tools like ELK stack, Splunk, or Graylog.
- **Setup Alerts:** Your app needs to be able to sends alerts to the people in the devleopment process. Regular SMS, Slack, or Email alerts are good. Keep it simple.
- **IDS/IPS System:** Intrusion detection/prevention systems should allow us to detect suspicious activity and then block the network traffic. Again you'd deploy these alongside the API.


## Credits
- [API Security](https://roadmap.sh/best-practices/api-security)