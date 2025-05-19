# Cors (Cross Origin Resource Sharing)

### Premise
On the web we have the SOP (Same-Origin-Policy), a browser security measure that ensures that websites can only receive requests from the same "origin". 

For two websites to be from the same origin all of these components must match:
- Protocol e.g. "http"
- Host e.g. "store.company.com"; remember that .com, .edu, etc. indicates the end of the host name.s
- Port number, e.g. `http://store.company.com:80` means port 80 is used.

#### Example 1 
- URL 1: https://example.com/page
- URL 2: https://example.com/api/data

Same origin because they have the same protocol (https), host (example.com), and port (default 443 for HTTPS).

#### Example 2
- URL 1: https://example.com
- URL 2: https://api.example.com

Different origins because example.com and api.example.com are different host names. 

#### Example 3: 
- URL 1: http://example.com
- URL 2: https://example.com
Different origins because the protocols (http vs. https) are different. 

---
### Securely bypassing SOP with CORS
Sometimes two websites with different need to talk to each other. For example, maybe two websites want to communicate, how do you think "Sign in with Google" works? To do this they need to do it securely, and that's why the CORS standard was created. It's a standard that allows websites from any origins to talk to each other. CORS achieves this using a **preflight request**. 

---
### CORS Preflight Request
Before the client sends their real request to get or send data to the server, they send this preflight request to the server in order to check whether the server will permit the actual request. It's called "preflight" because the sending of the actual request is like the actual flight, the actual plane taking off. In the preflight request's headers, there's info indicating what HTTP methods and headers that will be in the actual request. So before the actual request is sent, the server gets like this warning indicating what info will be in the actual request. And then the server can choose whether to accept or reject the actual request. 

---
#### CORS Request-Response Process
When you make a cross-origin request, this is the request-response process:

1. **Client sends a Preflight Request (OPTIONS request):** The browser sends an `HTTP OPTIONS` request, this is our preflight request. This preflight request will have information in its headers indicating what type of HTTP method and headers the actual request will have. The main thing is that this preflight will contain info about the current origin's protocol, host and port. 

```http
<!-- Server that's being requested -->
OPTIONS /api/data HTTP/1.1
Host: api.example.com
Origin: https://myfrontend.com
<!-- Indicates that the actual method is a POST request  -->
Access-Control-Request-Method: POST
<!-- Indicates the headers that the actual request will use-->
Access-Control-Request-Headers: Content-Type
```
2. **Server responds with CORS headers:** The server the preflight. If it accepts, it'll respond with headers that specify which origins, methods, and headers are permitted.
```
HTTP/1.1 200 OK
<!-- Allowed origins -->
Access-Control-Allow-Origin: https://myfrontend.com
<!-- List of allowed HTTP methods for the client -->
Access-Control-Allow-Methods: GET, POST, OPTIONS
<!-- List of allowed custom headers -->
Access-Control-Allow-Headers: Content-Type
<!-- Tells the browser how long it can cache the preflight -->
Access-Control-Max-Age: 3600
```
3. **Client sends actual request (if permitted):** If everything works out the browser will send the actual post request.

---
#### Simple Request
Some requests don't trigger CORS preflight request, these are called simple requests. A cross-origin request is complex, and it has these properties:
  - Methods other than GET, POST, HEAD
  - Headers other than Accept-Language, Accept, or Content-Language
  - Content-Type header other than multipart/form-data, application/x-www-form-urlencoded, or text/plain.

So like a request to delete or modify existing data is considered complex. Which makes sense, those types of operations are dangerous, so it makes sense that there needs to be a preflight request.

---
### Best Practices 
Make good access lists. Actually grant individual access using a comma-separated list rather than using a wildcard. Only use a wildcard when you want to make your API public, otherwise wildcards and regexes can introduce vulnerabilities. Avoid using null origin in your access list. Some browsers send null in the request header for certain scenarios. So don't include a null value in your access list, as it introduces security risks.

### SameSite Cookies Explained
Websites often have external services such as content recommendations, advertising, etc. which can set cookies in the user's browser to track the user and get more information about them. A website itself can also set cookies on the user to do the same thing.

However a company can own multiple websites with different domains, but we want to the same cookies across those domains. For example, a company couold have several websites for different hotels, and a common reservation booking website. If the user goes to one hotel website and then traverses to the reservations websiste, you'd probably want to let the cookies transfer. Especially if they're something important like authenticated related cookies. So since these sites are on different domains this is a cross-site request. 

To handle these situations we use the SameSite cookie attribute. This attribute controls hwen cookies are included in requests. One of the things it helps with is preventing csrf attacks. If your cookies are transferred anywhere, and you go on a malicious site, it's going to take your cookies and in the worse case make requests on your behalf. By using SameSite, you can control when sites would get your cookies. So if you did SameSite='strict' no other domains other than the one that issued your cookie would be able to access it. That makes it a lot harder to do a CSRF. This attribute has three possible values: `Strict`, `Lax`, and `None`.

- `SameSite=Strict`: The cookie is only sent if the request originates from the same site. So basically for all requests that are directed to the same domain, your cookies will transfer, else they will not transfer. In english if you travel off the original website, to some other websites, your cookies won't transfer and so those other websites won't see that cookie. This is really good for security and preventing CSRF, and it's commonly used in financial services, banks, and other institutions that are at high risk of CSRF attacks. However, if you need cross-site communication, this will limit your abilities. Your hotel example would have a harder time working.
- `SameSite=Lax`: Sends cookie on same saite navigations and GET requests to external sites. This is the default and pretty safe.

**Note:** Top Level navigation is when the URL in the browser changes. So if you go to a different site, that's top level. But if you load 

- `SameSite=None`: Cookies are shared between sites, on all requests. So requests from the same domain and different domains will send the cookie. However you need to have the 'Secure' attribute on the cookie for this to work, which means it can only be transmitted over HTTPS. Ideally this should never be set unless necessary. 





## Credits
- [What is CORS - Amazon](https://aws.amazon.com/what-is/cross-origin-resource-sharing/)
- [Cross-Origin Resource Sharing (CORS) - MDN](https://developer.mozilla.org/en-US/docs/Web/HTTP/CORS)

- [SameSite Cookies](https://cookie-script.com/documentation/samesite-cookie-attribute-explained)