# OAuth2.0

### What is OAuth2.0?
In the early days of the internet, if you wanted a website A to access your account/information on website B, let's just say to import your photos or something, you'd actually give your login credentials to site A. This was really dangerous, as you're giving them access to your entire account. There's no guarantee that they won't delete your account, or access more than just your photos. With OAuth2.0, you can authorize one application to access your data within another application, without giving the former your login credentials. As well as this, you can set permissions to guarantee that the application wanting to use your data won't be able to do things you don't want it to do. An example is letting a photo editing app access your photos stored on Google Photos.

### High Level Overview
1. User uses Pinterest and wants Pinterest to have access to their Google photos.
2. User clicks the continue with google or link with google.
3. User's browser is redirected to google's domain. Google tells the user to log into their own google account.
4. They are shown the list of things that Pinterest wants. E.g. "Pinterest wants to access your Google photos". User grants consent here.
5. Assuming they grant consent, the user is redirected back to the original app, Pinterest, via a callback/redirect_uri.
6. Now Pinterest is able to access the Google Photos API and get the user's photos since the User granted permission.

### OAuth 2.0 terminology:
- **Resource Owner:** The user who owns the data (e.g., you) and allows the application to access the data.
- **Client:** The application requesting access, Pinterest.
- **Authorization server:** The server in which the user will talk to in order to authorize permission and give consent. Here it'd be accounts.google.com, or something like that.
- **Resource server:** API or system that holds the user's resources, so here it's the Google Photos API.
- **Authorization Grant:** This code or thing (depending on type of auth. grant) ultimately proves that the user has consented to give the client the resources.
- **Callback (Redirect URI):** This lets the authorization server know where to redirect the user back to on the client application. "After the user clicks yes, where on the client application should we send them?". Users are redirected to the redirect uri whenever they consent or not.
- **Access token:** The client uses this to get the resources.
- **Scopes:** The authorization server will have scopes such as "photo-read", "photo-add", etc. and those represent permissions in the system. The client will then include the scopes that they want in regards to the user's data. That list of scopes is used to generate that consent screen that shows the user the list of things that the client wants to access.

### More detailed example: Code flow
1. Direct to authorization server:
  - `redirect_uri`: "pinterest.com/callback", redirect the user here after they indicate their consent.
  - `response_type`: "code". There are many types of authorization of authorization grant we can use, but here we'll use code.
  - `scopes`: "photo-read", "photo-write", or something like this. In reality, Google uses long strings to represent scopes, so here we're just making things easier to read.
2. Tells the user to login, and the user does so. The user then gives permission:
  - Redirect to the url indicated by the redirect uri. 
  - Authorization server sends an authorization code to the client, specifically to that `redirect_uri` since that's what we indicated in `response_type`. 
3. The client then makes a request to the authorization server, and exchanges this authorization code with the access token, which is what the client really wants.
  - The authorization server obviously has to make sure that this authorization code is valid, not expired or forged. But if everything's good, the authorization server sends the client the access token.
4. The client can now make requests to resource server, but they need to attach this access token to their requests:
  - Normally the resources server wouldn't allow the client or other 3rd parties to access the resource owner's data. However the access token proves that the user has given the application permission to access data, their photos in this case.
  - However the client can't just do anything. If the user has only granted read only permissions, then that's all the resource server will let the client do. So the client can't delete or add new photos since they were only given permission to read photos.

### Purpose of the authorization code:
A common question is "Why not just use the authorization code to access resources? Why do we have to use it to get an access token? Why is this extra step here". In network security there are two types of channels:
- **Back channel:** A highly secure channel/connection/environment. A request made from your web server, which only you have access to, to a Google authorization server is very secure. Things are private, we have SSL/TLS encryption, etc. 
- **Front channel:** A secure, but less secure environment. An example is your browser or client-side application. Anyone can inspect source or use the chrome developer tools to see the javascript and html you're running. So if you have an API key there, anyone could see it if they wanted to. So we trust it, but not completely, not as trustworthy as our web server.

When the client is redirected to the authorization server, this is happening in the front channel. These are browser webpage redirects, and stuff like the `redirect_uri`, `scopes`, and `response_type` are fully visible. These pieces of information aren't secret, it doesn't matter if someone knows the scopes we requested or where we're going to send the user back on our app. The authorization code coming back to the client is also transmitted through the front channel as a query parameter. However the next step is on the back-channel. 

What if someone was listening in on your network requests. They could steal the authorization code, and try to get that access token before you. Actually that wouldn't work. Using just the authorization code, you don't have enough pieces of the puzzle to get the access token via OAuth 2.0. When the client requests for an access token, alongside that authorization code, they send a **secret key** that only the client and the authorization server know about. With this secret key, client proves to the authorization server that they are the registered client that's allowed to redeem the authorization code, and ultimately receive an access token. 

This kind of mixes the best of both worlds of the front and back channel. The front channel is being used for this user friendly consent and interaction, and then the backchannel holding up this security aspect.

### Setup and the raw flow:
1. **Step 1:** To setup OAuth2.0, you need to register yourself as a client. You go into Google and into the authorization server, and register as a client. As a result you'll get:
  - **client_id:** This is an identifier that allows the authorization server to identify you as a OAuth2.0 client.
  - **client_secret:** You keep this secret, you'll use it when exchanging the authorization code for that access token.
  - Now you're ready to start making OAuth requests.

2. This is what happens when you click that continue with google, or link with google button. You will be redirected to this domain:
```bash
https://accounts.google.com/o/oauth2/v2/auth?
  client_id=abc123&
  redirect_uri=https://pinterest.com/callback&
  scope=profile&
  response_type=code&
  state=foobar
```
3. Then the user has to login (sometimes) and confirm their consent:
```bash
<!-- When the user denies consent, the authorization server redirects back to you with an error -->
https://pinterest.com/callback?
  error=access_denied
  error_description="The user did not consent"

<!-- Here's what happens when consent is granted. -->
https://pinterest.com/callback?
  code=39jz09RQO0sMB3
  state=foobar
```
3. Now exchange the authorization code for an access token:
```bash
POST www.googleapis.com/oauth2/v4/token
Content-Type: application/x-www-form-urlencoded

code=39jz09RQO0sMB3&

# client_id is public, but client_secret is secret. The former is used to identify you, now the authorization server and now the authorization
# server knows you. It expect a certain client_secret, so we must match it to prove that we are the registered client we say we are, and that we can 
# redeem this authorization code for an access token. Think of it as your usernmae and password to authenticate with the authorization server. Your username 
# can be publish, but your password stays private.
client_id=abc123&
client_secret=secret123&
grant_type=authorization_code
```
4. Authorization server hands you back an access token (OAuth token), and extra info about it like how long it's valid for:
```bash
{
  # A common access token that's used is a jwt. 
  "access_token": "39jfs09djfsejrBlibgoi",
  "expires_in", 3920,
  "token_type": "Bearer",
}
```
5. The client can now use that access token to make a request to the resource server:
```bash
GET api.google.com/some/endpoint
Authorization: Bearer 39jfs09djfsejrBlibgoi
```
  - Authorization server needs to validate the token (make sure it's not fake or expired) and they need to make sure the request the client is making is within scope.



- **Client ID:** Used so that the authorization server can identify the client.
- **Client Secret:** A secret shared between the client and authorization server. This allows them to securely send information behind the scenes. 
  - **Note:** When the client signs up to use OAuth2.0 for a certain provider, they'll receive the client ID and secret. This will later be used to identify and verify the client app's identity.
- **Resource Server:** The API or service holding the user's data (e.g., Google Photos API).
- **Authorization Server:** Issues access tokens after verifying permissions (e.g., Google's OAuth server). The authorization server and resource server could be the same server. There's also times where the authorization server is from a different organization that the primary organization trusts.
- **Authorization Code:** A short lived code that the authorization server sends back to the client. The client will send this and the client secret to the authorization server in order to get an access token.
- **Redirect URI:** The URL that the authorization server will redirect the resource owner to, after permissions has been granted to the client. This is also known as the **callback url**
- **Scope:** The permissions you give the client when they're accessing your account info. It usually appears as check boxes and has things like "Allow access to photos", "allow access to files", "create contacts", etc.
- **Consent:** After you set the permissions, you'll be prompted with a final confirmation with choices like "Yes" or "No". This is here to verify your consent of whether you'll allow the client to access your data in the manner you specified.
- **Access Token:** A short-lived token used by the client to access protected resources.
- **Refresh Token:** A long-lived token used to obtain a new access token without the need user intervention. Without this the user would have to enter their login info and permissions to the  authorization server after the access token expires. 
- **OAuth flow:** The standard and steps used in the security process. For OAuth2.0 there are like about 3-4 different OAuth2.0 flows that cater to different situations. Here we'll talk about the "Authorization Code" flow. 



---
### How it works

### Other OAuth 2.0 flows 
- **Authorization Code:** The one we talked about above. It uses front and back channel communication
- **Implicit flow:** Uses front channel communication only. With this, they step the authorization code sending and exchange step. After the user consents permission, the authorization server sends the application the access token. This happens when you only have client side app that a JavaScript, React, Angular app, where there's no server side code being run.

These latter two aren't as common:

- client credentials flow: Back channel only. Commonly used for machine-to-machine communication, or communication between services.
- Resource owner password flow: Backend channel only, it's commonly used to make older machines work properly, and it's not recommended for newer apps.

#### Implicit Flow Explained
Ideally, you'd have a backend, a private server that acts as your backchannel, however if you don't, then you'd use the implicit flow:
1. User clicks on "connect with google". They are redirected to the authorization server with some info:
  - redirect uri
  - response type
  - scope
2. The user logs in and ideally consents. Then the authorization server redirects the user to the redirect uri, and sends an access token to that redirect uri endpoint. So no authorization code this time, we skipped that step and sent the access token. Of course this is a little less secure, but sometimes that's just how things are sometimes.

### History and why OAuth2.0 is for Authorization Not Authentication
Originally, stuff like "Continue with google" or "continue with facebook" was implemented using OAuth 2.0. However, OAuth 2.0 was designed for authorization (giving access to resources), rather than authentication. It's bad practice. This is because in OAuth2.0, there's no standard way to get the user's information. If you're a web app and you want user info like emails, usernames, etc. it's going to be a tricky to get that because OAuth doesn't by default let you do that. As a result, when companies like LinkedIn, Facebook, etc. implemented OAuth2.0 for authentication, they had their own custom tools and implementation to get that user info. As a result, this is why you'll hear a lot of conflicting information about how OAuth2.0 works online. As a result, the scientists wanted to standardize this and improve/extend OAuth 2.0 so that it can support authentication cleanly. Enter OpenID Connect or OIDC

### OpenID Connect (OIDC)
An extension on top of OAuth 2.0. Here's what it adds:
- ID token: Represents the ID and has information about the user.
- UserInfo endpoint: If the ID token doesn't have the information you want, you can request more user information from this endpoint that all OIDC compliant servers implement.
- Standard set of scopes 

So if you're talking to an authorization token that has implemented OIDC, you can not only request for an access token, but also an ID token. That's literally all OIDC adds.

#### OpenID Connect flow
A lot of this is going to look the same, but we have a little more stuff.

1. User clicks "Login with Google", and they're redirected to Google's domain/authorization server. 
  - redirect uri: `pinterest.com/callback`
  - response type: `code`, still using code flow.
  - scope: openid profile, so here we pass in a new scope called "openid" and this identifies it as an OIDC request (authentication) rather than a standard OAuth (authorization) request. Technically we're using OAuth to faciliate OIDC, but I digress.
2. User logs in, and they give their consent. Authorization server redirects the user and sends an authorization code to the client's redirect uri endpoint.
3. The client requests the authorization server for an access token and an ID token:
  - client_id 
  - client_secret
  - authorization code.
4. The application can now decode the ID token to get information about the user, and who has just logged in. If the client needs more information about the user, they can attach their access token to a request to the UserInfo endpoint. The UserInfo endpoint will see the user.

**Note:** I just know that in different places, what the client gets in return after giving the authorization code may vary a little. It could be an access token that you give to another endpoint that gives user info. It could be just an id token that you deocde, probably with some shared key, and that's how you get the authenticated user's info. Or it's a mix of both, an id token that you decode to get user info and then an access token you use if you want to get extra user info.

#### Example Flow: OpenID Connect Implicit
1. User clicks continue with 
```bash
https://google.com/oauth/oidcendpoint?
  client_id=<client_id>&
  redirect_uri=<redirect_uri>&

  # Identifies the request as an OIDC request (authentication) instead of standard OAuth2.0 where we'd be requesting for 
  # authorization to use user resources.
  scope=openid&

  # Here we're saying we don't want to get an authorization code, we just want the 
  # access token directly. This is the implicit flow.
  response_type=id_token&
  response_mode=fragment&
  state=foobar
```
2. Then we get an ID token back (which is a typically a JWT), where we'll validate and decode to get the information of the user that was authenticated.

Here an example on how to call the userinfo endpoint when we have an access token. This is needed when the info that you decoded from the 
```
GET www.googleapis.com/oauth2/v4/userinfo
Authorization Bearer <access-token>

200 OK
Content-Type: application/json

{
  "sub": "you@gmail.com",
  "name": "Nate Barbettini",
  "profile_picture": "http://plus.g.co/123"
}
```

### Use cases and closing 
OpenID Connect is used for simple login, single sign on across sites, and mobile app login. However delegated authorization, where we give a 3rd party limited access to our resources, is done with the base form OAuth 2.0.
- Use OAuth 2.0 for granting access to your API and getting access to user data in other systems. 
- Use OpenID Connect for logging the user in and making your accounts available in other systems.

WHich grant type (flow) do I use? 
1. Basic web app with a backend that stores users in the database.
  - Use OIDC (code flow) for handling login. So you exchange code for id token and access toekn. The authorization server will handle login, security, and will help establish a session for the user. 
2. Native Mobile App: Use OpenID Connect (code flow + PKCE). Basically exchange code grant for id and access token. You'd store tokens in protected device storage, whilst using ID token to know who the user is. You'd attach the access token to outgoing api requests.
3. JavaScript (SPA) (no backend): Implicit flow
4. Microservices and machine-to-machine: Use client credentials flow.

## Credits
- [OAuth 2.0 and OpenID Connect - OktaDev](https://youtu.be/996OiexHze0?si=VXACWHCDgKJ0tEms)