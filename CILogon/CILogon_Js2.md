# JS2-Gateway Authentication and User Management


## Explaining CILogon

### What is CILogon
OIDC (OpenID Connect) allows users to log in through third-party identity providers (like Google), but most major providers require you to register and sometimes pay. CILogon is a free identity platform designed for researchers, universities, and federally recognized institutions. What makes CILogon special is that it supports over 5,000 identity providers, including:
- Google
- Microsoft
- University and research institution logins

This allows users to log in with either common accounts (e.g. Google) or their institutional credentials.

**Note:** To use CILogon, your app must be affiliated with an approved institution (like a university or research org). 

### Scopes and Claims
When you authenticate with CILogon, you typically request the openid scope.Minimal claims guaranteed under openid:
- `sub`: Unique user ID across the provider
- `iss`: The issuer (CILogon)

Optional scopes:
- `email`: May provide user’s email (if available)
- `org.cilogon.userinfo`: Adds CILogon-specific claims:
- `idp`: Identity provider’s unique ID (e.g., Google, IU)
- `idp_name`: Human-readable name of the provider

These optional claims may not always be present — always handle missing fields gracefully. Our app does this, so that's fine. 

### Demonstrating CILogon's OIDC Process

##### 1. Register User and Set up environment
Let's assume you've already [registered as an OIDC client on CILogon](https://cilogon.org/oauth2/register). Now let's setup the environment to start testing with curl. First set the environment variables that the server know that you're a valid OIDC client (authorization).
```bash
export CILOGON_CLIENT_ID=cilogon:/client_id/6e8fdae3459dac6c685c6b6de37c188c
export CILOGON_CLIENT_SECRET=euWajTysidMofassawoigDiweoj1olwa
export CILOGON_REDIRECT_URI=https://localhost/callback
```

##### 2. Redirect the user to authorization server with scopes (CILogon)
Then hit the authorization endpoint. We'd typically redirect the user to this endpoint:
```bash

https://cilogon.org/authorize?
  response_type=code&
  client_id=cilogon:/client_id/6e8fdae3459dac6c685c6b6de37c188c&
  redirect_uri=https://localhost/callback&scope=openid+profile+email+org.cilogon.userinfo
```
This is an OAuth2.0 request, when the user logs in correctly and accepts scopes, the server will return an authorization code at our `redirect_uri`. We have our `client_id` to let the server recognize us and whatnot. What's important is that `redirect_uri`, as it lists our scopes:
- **openid:** Lets CILogon know that this is an OpenID Connect request, so we're doing authentication rather than a base form OAuth2.0 authorization request.
- **profile:** If possible we want stuff like the user's name, family name, their name. I say "if possible" because remember this may not be defined or given, even if the user gives permission for it.
- **email:** If possible, we want their email.
- **org.cilogon.userinfo:** If possible we want the identity provider, and institution or organization related info. 

##### 3. Receive Authorization Code and exchange it for access token
Now let's say the user accepts all the scopes and authenticates successfully. CILogon will now send an authorization code to our `redirect_uri`. So that redirect request would look like this:
```
https://localhost/callback?code=NB2HI4DTHIXS6Y3JNRXWO33OFZXXEZZPN5QXK5DIGIXTMYZTMU4TQMLDGVRGMY3EGA4TSNLDGFQTSNRXGVSTSMBZGIZDKNJ7OR4XAZJ5MF2XI2D2I5ZGC3TUEZ2HGPJRGYZTOMZVGM3TANRVHE3SM5TFOJZWS33OHV3DELRQEZWGSZTFORUW2ZJ5HEYDAMBQGA
```
Now that we have this authorization code, we can then make a request for an access token:
```bash
https://cilogon.org/oauth2/token?
  grant_type=authorization_code&
  code=<authorization_code>&
  client_id=<client_id>&
  client_secret=<client_secret>&
  redirect_uri=<redirect_uri>
```
We should get some JWT tokens (access tokens):
```json
{
  "access_token": <access_token>,
  "refresh_token": <refresh_token>,
  "token_type": "Bearer",
  "expires_in": 900
}
```
Note that `refresh_token` is only return if refresh tokens are enabled for your client. That's done when you register your client, or you can just ask CILogon to enable them for your client. 

##### 4. Use Access token to get authenticated user information
Again, here they gave us this access token that we can use on a CILogon endpoint:
```bash
# Endpoint you're hitting to get the authenticated user's info
https://cilogon.org/oauth2/userinfo

# Request body of your request
{
  access_token: <access_token>
}

# Output you'll get; of course this is the ideal case when all fields are given back.
{
  "subject_id": "tfleury@ncsa.illinois.edu",
  "sub": "http://cilogon.org/serverD/users/35110",
  "idp_name": "National Center for Supercomputing Applications",
  "eppn": "tfleury@ncsa.illinois.edu",
  "cert_subject_dn": "/DC=org/DC=cilogon/C=US/O=National Center for Supercomputing Applications/CN=Terrence Fleury D35110",
  "eptid": "https://idp.ncsa.illinois.edu/idp/shibboleth!https://dev.cilogon.org/shibboleth!85ENhMSxWWw+m0L8Wx9LDiH+jDE=",
  "iss": "https://dev.cilogon.org",
  "given_name": "Terrence",
  "acr": "https://refeds.org/profile/mfa",
  "aud": "cilogon:/client_id/6e8fdae3459dac6c685c6b6de37c188c",
  "idp": "https://idp.ncsa.illinois.edu/idp/shibboleth",
  "token_id": "https://cilogon.org/oauth2/idToken/4c10db2c8dc2e02d0795e6c3a340763d/1637353708383",
  "affiliation": "member@ncsa.illinois.edu;employee@ncsa.illinois.edu;staff@ncsa.illinois.edu",
  "name": "Terrence Fleury",
  "family_name": "Fleury",
  "email": "tfleury@illinois.edu"
}
```
If refresh tokens are enabled for the client, then here's how to use them to refresh your access token:
```bash
# Hit CILogon's oauth2.0 endpoint for tokens
https://cilogon.org/oauth2/token

# Request body:
{
  client_id,
  client_secret,
  refresh_token
  # The scopes you used in the original OIDC request to get that access and refresh token.
  scope=scope=openid+profile+email+org.cilogon.userinfo
}
```
Okay that should be it for introducing how CILogon is used and setup, now let's move onto JS2 and how it manages users and authentication.


## JS2-Gateway Authentication and User Infrastructure 

### Database Design and Infrastructure
While JS2 uses containers, it doesn't have a postgres container. The application actually connects to an external database that's hosted on IU's servers (Big Red 200). Here's the flow:

In `database.py` the we connect to our external database:
  1. Test if a raw database connection works. If this doesn't work it's probably something wrong with the environment variables or your setup.
  2. Test if we can use SQLAlchemy as an ORM.
  3. Create a database session, which is just SQLAlchemy's way of allowing us to interact wtih the database.

This is run automatically on container start since we do a little trick by importing the code, which runs it.


In `models.py` we define our database model. Let me explain the the context behind the database setup:
```Python
class User(SQLModel, table=True):
    """Database model for user information and authorization. The table is named "user_details".
    
    Fields:
    - id: ID of the user.
    - email: Email of the user.
    - name: Name of the user.
    - institution: The name of the institution or organization that the user is associated with. This is a big thing in the application as we want to know what organization a user is from to see if they're legit, and also have a way to verify them. Ultimately pretty as the researchers at Zhu Lab are going to decide whether or not to approve these people.

    - approved_user: Effectively acts as a boolean field to record whether a user has been approved by Zhu Lab or not. The idea is that a user can only login and use the calculators 
    IFF they are approved.

    - admin_rights: Whether a user is an admin or not. Admins can see stuff like the admin dashboard. They are admins with higher privileges.

    - approved_at: The time at which a user was approved.
    - archived: In this application, we use soft deletion. Meaning if you delete a user, instead of deleting the user record from the table, we set a boolean. As a result, the user shouldn't be able to login, with these account credentials. They just wanted soft deletion and the idea of being careful with people's accounts.

    - onboarded: So when the user is registered into our system, we want them to 
    confirm their user information with us. So here they'll confirm the name, 
    email, and institution that they're registering with us with. They can't 
    change the email, but they can change the name and institution. This is helpful because sometimes CILogon isn't going to be able to provide 

    NOTE: Sometimes CILogon isn't going to be able to provide the name of the user (profile claim) or their institution name (idp_name), so we want the user to be 
    able to fill those out.
    """
    __tablename__ = "user_details"

    id: Optional[int] = Field(default=None, primary_key=True)
    email: Optional[str] = Field(default=None, max_length=45)
    name: Optional[str] = Field(default=None, max_length=45)
    institution: Optional[str] = Field(default=None, max_length=45)
    approved_user: Optional[int] = Field(default=0)
    admin_rights: Optional[int] = Field(default=0)
    approved_at: Optional[datetime] = Field(default=None)
    archived: Optional[int] = Field(default=0)
    onboarded: int = Field(default=0)    
```

### Auth. Infrastructure

#### Authentication Flow and User Journey:
Let's map out the user journey. The user goes to the application, they hit the sign-in button. This redirects them to our first API route in the process `/auth`.

**auth_me (`/auth`):**
1. Start the OIDC process by redirecting the user to the `https://cilogon.org/authorize` endpoint where the user can authenticate using whatever IDP they want.
  1. Note: We want scopes like `profile` to get the user's name. We want `email` since that's pretty important for communication. And we want the `org.cilogon.userinfo` because that can give us the institution that they authenticated with.

**auth_callback flow (`/callback`):**
- Authentication process completed, but we don't know if it worked or not. In any case we may or may not have gotten a token.
1. Attempt to make request to to exchange authorization code for an access token. If this fails, redirect the user back to the home page for "bad token response".
2. Use access token to get make a request for the user's identity. If this fails, redirect the user to the home page for "bad userinfo response".
3. Parse the user info JSON we got. If we didn't get an email claim, then that's no good, redirect the user back to the home page saying that their email is missing.
4. At this point the login is successful, so we'll set a cookie containing the access token, which will be used later to prove their authentication state. Then redirect the user to the homepage for a successful authentication. However it is not done yet.

Now that the user has been redirected, the `AuthProvider` component will run an effect that hits the `/auth/me` endpoint. This starts the last chain in our OIDC authentication process.

**auth_me (`/auth/me`) flow:**
1. Obtain the access token from the cookie we issued the user. If it doesn't exist, fail the request.
2. Do a request to obtain the user's information again. This worked the first time and on the first time we guaranteed that the user has an email. If the request fails to run, or we get a non-200 code, just delete the current access token cookie and say that we couldn't get the userinfo.
3. Parse the user info. Here check if the email exists (which it should). Check the database session (it should exist too). Query the MySQL database for a user our authenticated user's email.
4. If a user already exists, this means the user has already registered with the js2 service in the past, they're in our records already. So we'll return a `user_auth` object containing all the info we have recorded about them. So return a response with this `user_auth` object.
5. Else, a user doesn't already exist, this means it's their first time authenticating and registering with the js2 application. Create a record for the new user in the database containing their name (if it exists), and institution. Set status like approved, admin, archived, and onboarded to false. If transaction fails, log the errors.
6. After adding them to the database, they're officially registered, so return a user object with that saved information and also indicate that a new user was created. Then return the response saying a new user was created.

The client will receive the user's information, update the states, and then the user should officially be logged in. If the user ever wants to logout, then we just hit the **auth_logout (`/auth/logout`)** route, delete their cookie, redirect them to the home page saying they logged out. 

#### Onboarding Flow
After registering for the first time, a user will need to get onboarded. This just means that they have to confirm their information with us, and after that they're all set.

1. After authentication, the page reloads and `OnboardingCheck` will see that they're not onboarded yet and redirect them to the `/onboarding` route.
2. The user now needs to confirm their info with us. At worst, we may not have a name and name of the institution the user authenticated with, so we'll require them to enter that information here! The email should exist though, we won't let them change that. On form submission the user hits the `/api/user/onboard` endpoint.

**complete_onboarding (`/api/user/onboard`)**
1. Get access token from the user, if it doesn't exist, return a 401
2. Verify the acess token is valid by hitting the CILogon userinfo endpoint. If it isn't then return a 401. 
3. Parse the user info, if the email isn't available, we return a 400 since all users should have an email.
4. If the email from CILogon matches the email from the form data, then things look good so far, else reject the request.
5. Update the user in our database to be onboarded, update their name and institution as well. Fail the request if the user isn't found, or if the we failed to update the user.
6. After doing these updates, if the user is still unapproved, we should send email notifications to administrations and to the user that an unapproved user has no been onboarded. This lets admins decide to approve the user later.

#### Admin Dashboard and Approval Flow
The admin dashboard is useful for whether we want to update the user. Essentially an admin would hit a button to update the user, toggling their admin rights, approval status, etc. This hits the **update_user (`/api/admin/users/:userId`)** route:

1. Make sure the user making a request to this endpoint is an administrator

  - **Note:** This makes use of `auth_utils.py`, and it makes sure that the user making the request is an admin. It goes through getting the user from the access token cookie and making sure the user is an admin in our app.

2. Make sure the user being updated actually exists.
3. Update the fields and update the user in the database. In our application, if a field isn't sent, such as `admin_rights` then we don't update it.
4. If the user wasn't already approved, and this request approved them, we'll send notifications to the appropriate places.

#### Email Services
The last thing I want to talk about is the email services. Emails are mainly sent when a user is registered and needs to be approved. Here are the various emails we send out:
- **User Registration Email (To Admins)**: When a user completes their onboarding, we'll send an email to all admins to notify this happening.
- **User Pending Email:** When a user completes their onboarding, we'll also send an email to the user who completed their onboarding that we'll approve their account soon.
- **Account Approval Notification:** When a user is approved, we'll send an email to the user that was approved to notify them.