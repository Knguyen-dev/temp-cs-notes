# Session Management

### Introduction
A session is a series of requests and responses that come from the same user. Web apps create sessions to keep track of users. After anonymous users make their first reqeust, we can create a session to track them. We also track authenticated users. After logging in, a user is assigned a unique session ID, which is stored on the server (in-memory or persistent storage) and sent to the client as a cookie. For subsequent requests, the client includes this cookie, allowing the server to identify the user:
1. **Login Request:** The client sends credentials for validation.
2. **Session Creation:** If valid, a session object is created server-side. This object will contain the user information that's enough to uniquely identify the user and facilate access control. The session is stored either in memory, or you could store it in a persistent database. 
3. **Client Cookie:** The session object we created has unique ID. The session ID is sent to the client as a cookie. This session cookie will then be sent with their request everytime they make one to our server.
4. **Session Verification:** For subsequent requests, the server checks if the session cookie with that session ID exists in our storage mechanism. If it does, then we can say that the user is authenticated, else we can't. Of course there's also the idea of sessions expiring so take that into account.
5. **Logout:** On logout, we need to unauthenticate the user. The server deletes the session, and the client cookie is removed. As a result the user isn't authenticated anymore.

### Session ID
The session ID can be thought of as the user's credentials. It's the only proof the client has to prove that it's an authenticated user. If this cookie is ever stolen or copied, then a malicious actor can use this cookie to interact with the server, and the server will think that it's the user. Then the malicious actor can make requests on the user's behalf. I mean if they even guess the random string representing the session ID, then it's game over. This is the idea of session hijacking.
- **Session ID Fingerprinting:** Most web frameworks set the name of the session cookie as `PHPSESSID`, `JSESSIONID`, `ASP.NET_SessionId`. It's very identifable what underlying technologies the backend is using, so it's recommended to change that to something generic like `id`.
- **Session ID's value:** The value of the session ID, the long string itself shouldn't be encoded with sensitive data. It should be meaningless so even if an attacker gets their hands on it, it shouldn't have any data inside. On the server, you'll store information like IP address, UID, roles/perms, etc inside the session object.

### Session Management: Cookies 
Sessions should be transferred as cookies. You should not use other mechanisms. 
1. `Secure` to ensure the session cookie can only be exchanged through an encrypted channel (HTTPS). Good for production, but leave it out for development since you're typically working on localhost. 
2. Use `HttpOnly` which means scripts such as JavaScript aren't able to access the cookie. 
3. Use the `SameSite` attribute to control how the cookie is shared.
  1. `Strict` will make it so the cookie given to the client isn't sent on any cross origin requests. 
  2. `lax` will make it so if it's the same domain, or a GET request to a different one, the cookies are sent. For anything else they aren't. 
  3. `None` means they're sent to any domain.  Using SameSite is just an extra layer of security, it shouldn't replace CSRF tokens in general.
4. `Expire` and `Max-Age`: Cookies with one of these attributes are considered persistent. They will be stored by the web browser until they expire. Session cookies are should be handled as non-persistent, so the cookie will be cleared from the client if they close their browser. You don't want your session ID to remain on the client cache, less someone steals it.



If for some reason you do persist sensitive data into your cookie, encrypt it.

### Session Fixation Attack
* **What it is:** A type of attack where the attacker sets or "fixes" a known session ID for the victim **before** they log in.
* **How it works:**
  1. The attacker somehow obtains a valid session ID **before** the user logs in (e.g., by visiting the login page and getting a session cookie).
  2. The attacker then **tricks the user into using that session ID**, perhaps by:
     * Sending a link like `https://example.com/login?sessionid=attackerSessionId`
     * Using cross-site scripting (XSS) to set the session ID
  3. The user logs in **without the application generating a new session ID**.
  4. Now both the user and attacker share the same session.
  5. The attacker can use the session ID to impersonate the user.

* **Why it's dangerous:** If the server **doesn't regenerate the session ID upon login**, the attacker can gain full access without needing the user's credentials.

### Session ID Life Cycle: General Guidelines
- Don't trust session ID, validate it before processing the request to see if the user making teh request is authenticated or not.
- Create new session if the user's privilege level changes, of course invalidate previous session. Big changes like password change, if their permissions change, or role changes. This is critical in helping prevent session fixation attack.
- ***Session Cookie Expiration:*** Set an expiration for a session. Invalidate it client side (deleting session cookie), and also server side (deleting session object from storage).
- **Timeouts:** An idle timeout is the amount of time a session will remain active in-between requests. So if there's no HTTP activity after a certain amount of time, we'll invalidate their session. Then an absolute timeout is the idea that "after this certain amount of time we will invalidate their session regardless".
- **Prevent Browser Caching:** Prevent the browser from caching the session cookie by setting `Cache-Control: no-store` in responses containing session IDs.
- **Logging Session Events:** Log creation, renewal, destruction, privilege changes, and requests made with the sessio . Details may include timestamp, source IP address, and target endpoint.
- **Bind session ID to other properties:** Store it with client IP, user agent, and other info. This is for helping detect session hijacking.


## Credits:
- [Session Management](https://cheatsheetseries.owasp.org/cheatsheets/Session_Management_Cheat_Sheet.html)