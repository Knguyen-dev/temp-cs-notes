# SAML (Security Sssertion Markup Language)

### What is it?
An XML based open standard for exchanging ID information between services. A similar alternative is OIDC, which uses JWT instead of XML. Here's an example with SAML:
1. User visits the gmail app (service provider), it needs you to login.
2. gmail sends a SAML Auth. Request to the browser and then the browser forwards the user to an Identity provider. This could be Okta, Auth0, or even LinkedIn or GitHub.
3. IDP shows a login page, user sends those login credentials. After a successful, the IDP sends the client a signed SAML Assertion. This contains info about a user and what the user can access, in relation to the service provider. The service provider independently verifies  the SAML assertion is correct, using public key cryptography.

OIDC flow is similar to SAML, but instead of passing signed SAML assertions around, we just pass JWTs.