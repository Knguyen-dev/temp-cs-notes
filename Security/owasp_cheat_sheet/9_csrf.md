# OWASP: Cross SIte Request Forgery (CSRF)

### What is CSRF?
CSRF, AKA session hijacking, is when a malicious actor steals a user's session cookie. Imagine you're on a web app. By logging in, the web app sends the login request to the server. The server verifies that your credentials are correct, and generates a session for you and sends the session ID in a cookie back to you (client, your browser essentially). These cookies are then stored in your browser, and whenever you makes a request to the server, those cookies are sent in the request to verify that it's you, a logged in user.

The session cookie is proof that you're authenticated and have certain levels of permission to do stuff on your account. Now then, let's say you click a link on evil.com. It's a fun website, but it silently steals your session cookie. Now it can use that cookie to impersonate, and the servers behind vulnerable.com will fall for it. Because with a session cookie, while we can verify whether it valid, we can't really verify whether it's actually you or a malicious actor that's using it. The malicious actor can now change your profile info such as your username, change your password, and even delete your account. In summary, evil.com just 'forged' a request to vulnerable.com using your cookie, and this is called Cross-Site-Request-Forgery.

### Defense Guidelines 
- If you're vulnerable to XSS, any defenses to CSRF can be bypassed. 
- If your framework has built-in CSRF protection, use it.
- If it doesn't, add your own CSRF tokens. If you're using stateful auth, use the synchronizer token pattern. Else use double submit cookies, OWASP details them.
- If you have a frontend and backend (API driven) setup, and you aren't using native form submissions, then use custom request headers (`X-CSRF-Token`).

## Token-Based Mitigation

### Synchronizer Token Pattern
A CSRF token is a random, unique string tied to a user's session. It's used to verify that form submissions or other actions are coming from the actual site, not from an outside attacker. Here's the flow:
1. User logs into your site. We create a session and generate a CSRF token. Then do two things with the token:
  1. Store the token on the server, in the user's session or in a map.
  2. Send the token to the client in a way that JavaScript can read it. Could do it with a cookie that has `HttpOnly: false`
2. The client browser now has some work. It will include this CSRF token:
  1. In a hidden form field (for HTML forms)
  2. Or in a custom header (for XHR or Fetch api)
3. When the form is submitted, the CSRF token is sent alongside the request. The server checks whether CSRF token is present, and if it matches the CSRF token stored for that particular session. 
4. If it matches, accept the request, else reject it.

In any case we should follow these rules about CSRF tokens: Unique per session, secret, and random long values.

#### Transmission of CSRF Token
Typically we just put it in the response payload, so like an HTML or a JSON response. Then it's going to be sent back to the server as a hidden field on form submissions, a custom header (like `X-CSRF-Token`) in a fetch request, or even in your JSON payload. Do not transmit your CSRF token as a cookie. DO not have it in browser storage. Do not leak it in the server logs. Requests with custom headers are subject to SOP, meaning by default you can only make the request to the same origin. As a result, the request will work IFF it's made from your same origin. Attackers can't use a custom header, it would be blocked by SOP. Prefer using custom headers as its more secure since with hidden form fields, attackers can still try to a cross-origin POST request.


### Disallow simpmle form submissions
When using native form submissions, the form sends a simple request to the server. This request can be sent to any origin, which introduces the risk of CSRF. A request that has `application/x-www-form-urlencoded, multipart/form-data or text/plain` is simple. So one defense is removing these.

### Custom headers and cors 
You need to set `Access-Control-Allow-Credentials` so that your API can work with cookies. Then you'd set `Access-Control-Allow-Origin=<your_origin>` to the websites that you want cookies from. YOu'd set this to yourfrontend and so your frontend can only transmit cookies ot your server.

### Control SameSite cookie attribute
Setting SameSite will help you control when the cookie is sent to other sites, if at all.

## Credits
- [OWASP CSRF](https://cheatsheetseries.owasp.org/cheatsheets/Cross-Site_Request_Forgery_Prevention_Cheat_Sheet.html)