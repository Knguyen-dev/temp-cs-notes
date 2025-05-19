# OWASP: Json Web Token


## Introduction To JWT 
JWT (JSON Web Token) is a token-based authentication mechanism used for secure data exchange between parties. It allows the server to verify the identity and permissions of a client, without needing to store session data on the server (stateless). I mean ideally it doesn't store state as that's the main idea, but in real implementation you may store state to increase security and to do additional features.

### Anatomy of a JWT Token
A JWT is composed of three parts separated by periods (.):
- **Header:** Specifies the algorithm used for 'signing' the JWT (e.g., HS256) and the type of token (JWT). Then it's Base64Url encoded.
```Json
{
  "alg": "HS256",
  "typ": "JWT"
}
```
- **Payload:** Contains the claims or data you want to encode and embed into the JWT. Such as user ID, role, or expiration time (exp). You'd put a user ID because your server will want to know the user that's making the request. This is also Base64Url encoded.
```Json
{
  "sub": "1234567890",
  "name": "John Doe",
  "admin": true
}
```
- **Signature:** A string created by hashing the header, payload, and secret key using the specified signing algorithm that was specified in the header. This is the jwt signing process.
```Python
# This is how you'd generate the signature.
jwt_signature = HMACSHA256(
  base64UrlEncode(header) + "." +
  base64UrlEncode(payload),
  secret)
```

So a jwt is in the following form:`xxxxx.yyyyy.zzzzz`. We should note that since the header and payload are simply Base64Url encoded, anyone could take those parts of the token and decode them to see what's inside. Due to this, it's important that we don't put any sensitive information about the user in the payload. If we change anything about the payload of the JWT, maybe saying that a user is an admin when they aren't, the the JWT will produce a different encoded payload, and therefore a different signature when the server goes to calculate the signature of the JWT. This is very useful when validating a JWT.

---
### JWT Validation/Usage Process
1. **Client Request**: The client includes the JWT in the `Authorization` header when making a request.
2. **Server Verification**:
   - Extract the JWT and split it into header, payload, and signature.
   - Recreate the signature using the header and payload with the secret key. Run them through the same hash function to replicate our JWT signing process. 
   - If the recreated signature matches the one in the JWT, it's valid; otherwise, it is rejected. In the most likely sense, it means that the JWT has been tampered with, because if it wasn't, then the hash produced should have matched the existing signature.
3. **Decoding**: If valid, the server decodes the payload (reversing the base64 encoding) to access user data (e.g., ID, role) and authorize access to resources. 

### Token-Based Authentication overview
Your going to be hearing about **access tokens**. These are the tokens that the client uses to authenticate themselves. In the case of JWTs, these tokens have a short TTL for security purposes in case the token is stolen. However we don't want the user to login everytime this short-lived token expires. We'd also give the client a **refresh token** (which is also stored on the server) which will be used in exchange for another access token. A refresh token will be long-lived. In general here are your best practices:
- Use HTTPS.
- Have short lived access tokens and long lived refresh tokens.
- Use secure mechanisms like HTTP-only for storing tokens on the client.
- Don't share tokens: These are literally the thing you use to identify yourself, so if someone has yours, then they can login to your account. It's like a password, don't share it.


## Part 2: Jwt And Security

### Vulnerability: None Hashing Algorithm
This is an attack where the user alters the token and changes the hashing algorithm in the header to none, to indicate that the token has already been verified and that we don't need to do that server-side hashing process. As a result the user could take their current JWT, make themselves an admin, remove that hashing algorithm, and it all works since the exploit prevents any server side validation of the JWT.

This is a basic and simple exploit, and the solution is to use a moden JWT library that doesn't have this vulnerability.

### Vulnerability: Token Sidejacking 
When a token has been stolen by an attacker. Now an attacker has the token and could make requests on the user's behalf. Let's prevent this by adding 'user context' to a token, which is composed of:

- **Random Fingerprint:** A random string generated during the authentication phase, which we'll store server-side. We'll also send this to the client as a hardened cookie (HttpOnly, Secure, SameSite, MaxAge flags). Don't set expired header to ensure the cookie is cleared when the browser closes. Typically set the Max-Age of the cookie to be the same as the token expiry.
- **Fingerprint Hash:** A hash of the random string which we'll store in the token. From OWASP, this is just to prevent an XSS where the user is able to read the fingerprint and modify it.

Now even if an attacker steals your JWT, it is useless without that user-bound fingerprint that only the client has. Remember the client has a cookie for the JWT and this SHA256 hash of the fingerprint. Let's put it all together:
1. User authenticates:
  1. Generate random fingerprint; this identifies the client, and will persist across different access tokens.
  2. Store the plain-text version as a secure cookie.
  3. Store a SHA256 version inside the JWT that you're going to generate.
2. One every request:
  1. Client should have sent the fingerprint cookie containing the raw fingerprint.
  2. Re-compute the hash and check if it matches the hash stored in the JWT. If it doesn't then reject the request since the JWT may have been stolen. If it wasn't stolen, the client should have sent over the fingerprint as well since we gave them that when they logged in. 

The hashing of this random string also makes sense. Without the hashing, the user takes the jwt, they decode it, they see the fingerprint and set it as a cookie in their browser. Now they have jwt and fingerprint, which is all they need, so then they send it to the server. The server again does no hashing, so they pass the fingerprint check, and the JWT check. Now with hashing, the attacker steals the JWT, they decode it to see a SHA256 version of the fingerprint. They can send that to the server, however remember the server expects the non-hashed version, it will fail the fingerprint check since you're hashing a hash. The only way you pass hte fingerprint check is if you have that non-hashed version of the fingerprint, which the server will hash and compare it to the hashed version inside the jwt. The only person who has that will be the client, which is in a HttpOnly cookie, making it immune to XSS. 

### Vulnerability: Lack of Token Revocation
Ideally, JWTs are supposed to be stateless, so we don't have any state or session on the server-side to keep track of or revoke the token. So if an attacker takes their token, we can't invalidate the token to block the attacker. Using the token sidejacking solution above should alleviate most worries, as the the fingerprint is hardened, making it as secure as how we manage session IDs. In the worse case both the jwt and the fingerprint are stolen. To simulate a logout, clear the JWT cookie from their session storage. If they close the browser, both the jwt and the fingerprint will be cleared automatically. 

Another solution is to create a token denylist. It would keep a SHA256 hash of the jwt that was revoked, and its revocation date. Again this list is a list of tokens that have been invalidated earlier than expected, so an entry in the deny-list with persist until the token becomes expired. As a result removing it from the deny list is fine since if the attacker tries to use the token, it'll fail in the token
verification process.

### Vulnerabilit: Token Information Disclosure 
When the attacker steals a token and decodes it to learn more about the user and the system. For example the attacker could learn the security roles, how we identify a user or authenticate them, etc. While this isn't a direct attack on the system, you don't want your attackers learning more about your system. One solution to protect against this is to use symmetric encryption so that the whole token, or at least just the payload, is encrypted, and you can only decrypt it if you have the corresponding decryption key. In any case, your server will encrypt the token if sending it out, and then decrypt the token when receiving it.

### Vulnerability: Client Side Token Storage
- Sending tokens automatically to other sites can increase the risk of CSRF.
- Don't store cookies in localStorage or sessionStorage if possible. 
- If you must store JWTs in the browser (e.g. localStorage) you should take extra precautions. 

### Vulnerability: Weak Token Secret
If you're signing the token using an HMAC based algorithm, the security that you're receiving is dependent on the strength of the secret used to sign the token. Remember that attackers can try to crack the secret using already established tools. If they're successful, your JWT system is compromised as they can now forge their own valid JWTs. In any case make sure the secret used to sign the JWT is random and at least 64 characters in length. Use a cryptographically secure random generator.

## Credits
- [Introduction to JWTs - jwt.io](https://jwt.io/introduction)