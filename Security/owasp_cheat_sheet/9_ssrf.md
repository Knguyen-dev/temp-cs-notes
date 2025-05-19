# Server Side Request Forgery

### What is SSRF? Why is it dangerous?
SSRF (Server-Side Request Forgery) is a vulnerability where an attacker tricks a server into making HTTP requests to arbitrary URLs, including internal or protected endpoints. The attacker can't access these resources directly, but by manipulating a feature that fetches URLs (e.g. image previews or import-from-URL functionality), they can make the server access internal services, open ports, or sensitive files on their behalf.

You → tell the server → "Hey, fetch this URL"
The server → obeys and accesses internal stuff → which you normally can't reach

In this type of exploit, the websites that are vulnerable are ones where they have an endpoint or process that directly works with URLs that hit the backend. Common situations are:
- Image fetchers: Paste an image URL and we'll fetch it and display it.
- PDF generators: Provide a url, and we'll turn the resulting page into a PDF.
- Import-from-URL features: import data from this external service.

If the server doesn't restrict or check what URLs it can fetch, an attacker could enter: 
- `http://www.example.com/admin`: They could hit an admin panel. 
- `file:///etc/passwd`: To read files on the server.

---
### Types of SSRF

#### Server SSRF Attacks
![](https://www.imperva.com/learn/wp-content/uploads/sites/13/2021/12/How-Server-SSRF-works.png)

So on the frontend, we have a an interface that lets the user enter in a url. The attacker replaces that with the admin page for one of the related services in the internal network. For example, we have a weather app that queries its server for the weather forecasts:
```bash
POST /meteorology/forecasts HTTP/1.0
Content-Type: application/x-www-form-urlencoded
Content-Length: 113
weatherApi=http://data.weatherapp.com:8080/meterology/forecasts/check%3FcurrentDateTime%3D6%26cityId%3D1


# The attacker can change this to the following:
weatherApi=http://localhost/admin
```
So we have something on the frontend that does a POST request to the backend. So the backend receives a form POST request with a parameter `weatherApi`. This parameter is a url and traverses the file system in this case. Instead of getting that weather forecast data, the attacker goes to access an important file like `admin`. So that's kind of similar to directory traversal attacks. 

#### Backend SSRF attacks (Leveraging Trust)
This is moreso the idea where the server is authorized and is trusted with a given backend component. So whe nthe server makes requests to that backcend component, it has full access rights. Again the attacker can do something similar to the first scenario
```bash
weatherApi=http://192.168.12.5/admin
```
As a result, at the server's route handler it makes the request to another service. The other service gives the backend the admin info, and sends that info back to us. This is more in line with the introduction, wherer I talked about the attacker tricking the server to access data from another service.


---
### Mitigating SSRF?
Validate user input with regex and use allow lists on user inputs to indicate what services or resources that the the client is allowed to query. So whitelist the IP addresses or DNS names that your app requires access to. Also disable unused schemas. If the app only needs HTTP or HTTPS, then don't let them use `file:///`.

TLDR: The core issue is that the client can submit a URL and our server is blindly trusting it. The server isn't validating or restricting what types of URLs that can be accepted. 

### Example: Image Fetcher
```Java
// Herewe have an endpoint that accepts a url (for a get request) and returns 
// an image of that request. It's vulnerable because it accepts any URL.
// The attacker can make a request and do imageUrl=http://localhost:8080/admin
// which would allow them to get the admin page
@PostMapping("/fetch-image")
public ResponseEntity<byte[]> fetchImage(@RequestParam String imageUrl) {
    try {
        URL url = new URL(imageUrl);
        HttpURLConnection conn = (HttpURLConnection) url.openConnection();
        conn.setRequestMethod("GET");

        byte[] imageBytes = conn.getInputStream().readAllBytes();
        return ResponseEntity.ok().body(imageBytes);

    } catch (IOException e) {
        return ResponseEntity.status(HttpStatus.BAD_REQUEST).build();
    }
}
```
Here's the fixed version that uses an allow list for the list of allowed hosts they can download urls from:
```Java
private static final Set<String> ALLOWED_HOSTS = Set.of("images.example.com", "cdn.trustedsource.org");

@PostMapping("/fetch-image")
public ResponseEntity<byte[]> fetchImage(@RequestParam String imageUrl) {
    try {
        URL url = new URL(imageUrl);
        String host = url.getHost();

        if (!ALLOWED_HOSTS.contains(host)) {
            return ResponseEntity.status(HttpStatus.FORBIDDEN).body("Host not allowed".getBytes());
        }

        HttpURLConnection conn = (HttpURLConnection) url.openConnection();
        conn.setRequestMethod("GET");

        byte[] imageBytes = conn.getInputStream().readAllBytes();
        return ResponseEntity.ok().body(imageBytes);

    } catch (IOException e) {
        return ResponseEntity.status(HttpStatus.BAD_REQUEST).build();
    }
}
```

## SSRF Indepth:
![](https://cheatsheetseries.owasp.org/assets/Server_Side_Request_Forgery_Prevention_Cheat_Sheet_SSRF_Common_Flow.png)

In general, the attacker sends a request to vulnerable application, such as the image fetcher. By default the image fetcher uses any URL, but in this case it's making a request to the "TargetApplication", which we'll assume is a service that the attacker is normally blocked from. It gets the data since the vulnerable application is authorized, and returns the data to teh attacker.

There are two main cases in which SSRF can happen:
- Application can send requests to identified trusted apps. Use an allow list since you know the options of sites they can choose, that's input validation (application layer). You could also setup a firewall to prevent the vulnerable application from accessing other services that it's not supposed to talk to.
- Application can send requests to any external IP address or domain name. Here you can't really use an allow-list since there are like literally an uncountable amount of urls. Use a deny-list. 



## Credits 
- [SSRF Explained - ACI Learning](https://www.youtube.com/watch?v=-pNYmgK_dWo)
- [SSRF InDepth Examples - Imperva](https://www.imperva.com/learn/application-security/server-side-request-forgery-ssrf/#:~:text=A%20Server%2DSide%20Request%20Forgery,to%20read%20data%20from%20URLs.)
- [SSRF Prevention In Depth - OWASP](https://cheatsheetseries.owasp.org/cheatsheets/Server_Side_Request_Forgery_Prevention_Cheat_Sheet.html)