
# CILogon

### Review on OIDC and Identity Providers
Basically OIDC is typically provided as a service. For example, you can't just add "Continue with google" onto your website for free. Stuff like OAuth2.0 and OIDC require you to connect with those authorization servers, and that requires you to get an API key or register with those applications. So let's review what you would need:
1. **OAuth2.0/OIDC-complain identity provider:** Such as Google GitHub, Microsoft, Facebook, etc. These providers act as the place where users log in (and where tokens are issued).
2. **An app:** A backend application to handle the OAuth2.0 flow.
  - Redirect the user to the provider's authorization url
  - The user logs in and is approved.
  - The provider redirects them back to your app at endpoint with a authorization code. Your backend exchanges the code for an access token.
  - The access otken cna be used for keeping track of user's session and getting the user's information.
3. This typically costs money as you need to register with an identity provider. You could try simplify things with Auth0, which handles the complexities of it, but still it starts to cost money after more than 100 users. 

### How CILogon helps
It's a free idnetity provider for researchers, academics, national labs, etc. So this is ideal for research apps needing institutional login such as your university etc. You just need to be affiliated with a participating organization. 

In short, it provides OIDC for federal or federally approved institutions. So like schools, research labs, the government would like you to use CILogon. 


- Recommended to use cerified OIDC implementationsf found [here](https://openid.net/developers/certified-openid-connect-implementations/).
- You have to register your client first and it needs to be approved. Your client is probably your authorization provider, so like Indiana University in this case.
- They list their API endpoints for OIDC (Authentication), and also OAuth2.0.

#### Scopes
Our client cna request at most the scopes that it's registered with. So the set intersection of requested and registered scopes are taken. Here are the scopes it supports:
- **openid:** required, but after you'll get these claims:
  - sub: UID for the user.
  - iss: Issuser of the `id_token`, e.g. `https://cilogon.org`; makes sense since they sent the token.
  - aud: Audience of the client, the client_id of the OIDC client. Represents the university or organization, the app we're associated with, that wants the tokens to store the user's authentication state.
- **email:** optional, but this would be the email address of the user being authenticated.
- **profile:** optional, but this gives personal info suhc as `given_name`, `family_name`, and `name`.
- **org.cilogon.userinfo:** CILogon-specific scope, optional
  - idp: Identity provider's ID. Such as `https://idp.example.edu/idp/AltasUniva`. The identity provider is the service or organization that the user already has an account with, and they're going to be the guys that verify that their account is good.
  - idp_name: Name of the identity provider, e.g. "Indiana Unviersity".
  - eppn: The "eduPersonPrincipleName", e.g. "jsmith@example.edu". This is not intended to be used as an email address. The idea is that it's in teh form `user@scope`, where it's a name identifier and the "administrative domain of the identity system where the identifier was created and assigned". Basically they hope that your organization identifier is ther, so iu.edu, .nsa.gov, etc.
  - eptid: `eduPersonTargetedID`, a simpler version of SAML 2.0's name identifier format. In SAML, we had an XML string inside `<saml:NameID>` tag alongside other important XML elements which identify the source and intended audience. Here they just made this a tuple. Basically you're gonig to see some long URIs.
  - The rest is pretty much in the docs and specific use cases, but `oidc` is the sub reported by the identity provider I think. 


### Confidential vs Public clients. 
Like what type of application or organization needs this OIDC. When you register your client with an identity provider, you (the client) and the IDP will share a secret key. This is used to authenticate the identity of the client. We need to make sure it's you, before even attempting to send over someone else's user information to you.

- Confidential: These apps can securely authenticate with the authorization server. These are typically server side web apps, and they can keep their registered client secret safe from other users.
- Public clients: These are apps that aren't able to keep a client secret private, due to the nature of the app, so a different flow is used. These are applications that are running in a browser or on a mobile device. 

### Optional Authorization Parameters
- `idphint:` You can curate and refine the list of identity providers the people on your site can choose.
- `initialidp`: You can set a default identity provider if that they start off with.
- `skin:` You can have custom login pages I think


### User ID and Claims 
The standard claims are openid, email, and profile scopes. The custom ones are eppn, eptid, etc. To do simple auth., we only need the openid scope.

THe idea is that the sub and iss (used together) is going to be the thing used to uniquely identify a user. You can reason that the "sub" is locally unique for a given issuer. Then an issuser id is also unique. So if you had a database of all issuers and their recorded users, you'd use this combination to store it in one table.

Other claims aren't guaranteed to be unique. I mean a given issuer may have users with the same email. And also email can change over time. Essentially, don't use email, phone_number, or preferred_username as UIDs. Rely on the 'sub claim' as a UID, as that will always be accepted. 

## Credits
https://youtu.be/I4MeXh84tnA?si=ClMi8zWViBgCs4QK
https://www.cilogon.org/oidc