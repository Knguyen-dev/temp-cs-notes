# OWASP: Multi-factor Authentication
MFA or 2FA is when a user presents more than one type of evidence to authenticate in a system. There are 5 different types of evidence (factors). Here are the 3 common ones:
- **Something You Know:**	Passwords and PINs, Security Questions
- **Something You Have:**	OTP Tokens, U2F Tokens, Certificates,Smart Cards, Email, SMS and Phone Calls
- **Something You Are:**	Fingerprints, Facial Recognition, Iris Scans

Note that using multipl instances (type of auth) from the same factor doesn't offer much for securityy, and doesn't make it MFA. But then again, any MFA is better than none. 


### Why and when to use MFA?
MFA is highly recommended, Microsoft even suggests that 99.9% of all account compromises could have been solved with it. Typically, you aren't going to have implementing your own MFA, you're going to use an easy to use library, which is good! Security is easy to mess up, so leave it to professionals. Here are some suggestions for most real apps:
- Require some form of MFA for all users.
- Give users the option to enable MFA using TOTP.
- Require MFA for higher privileged users like admins and such.
- Use MFA as a service. Though make sure your service is secure, like any other dependency, but a little more important.

Now assume you are using MFA, here are the locations where the user should do an MFA:
- For changing passwords or security question.
- Changing the meail address associated with the account.
- Disabling MFA
- Elevating a user session to an admin session. So like it seems any privilege change really.

However this can get really annoying if you do this everytime. That's fine if you're in a high risk app. But if you want ot make things less annoying, just prompt MFA when they're doing a high-risk action:
- Require MFA when the user logs in from a new device or location. Or from just a high-risk location.
- Allow corporate IP ranges


### Implementing MFA

#### Scenario: Failed Login 
When the user's password has been compromiseed or htey lost their second factor. They should be able to do this:
- Prompt user for another form of MFA
- Allow users to reset their MFA.
- Nofiy user of failed login, tell them to change their password if things persist. It could be a real time notification with the timestamp of when the login happened.

#### Scenario: Resetting MFA
There are times when a user lose their additional factors. To prevent a user from being locked out, we want them to regain access, even without the factors. However we still want to keep out attackers who don't have the factors either. There's no best way to do this, but they all have their pros and cons. I'll give the most common one I see:
- Provide the user with a numbero f single-use recovery codes hwen they first setup MFA.
