# OWASP: Authentication Cheat Sheet

### General info
- User ID (UID): Uniquely identiifes a user. Ideally it should be kind of random to prevent it from being inferred by other people.
- Username: Human-readable identifiers that are chosen by users. This could act as the UID. Allow users to use their email address as their username (verify the email too). Additionally they should have the option to have a custom username, other than their email address.

### Authentication Solutions and Sensitive Accounts
- Don't let accounts that are meant for internal use, such as admin accounts, be able to login through the regular frontend interface that a normal user would use. Exposing powerful accounts to such high traffic and public areas increases the risk of them being compromised. Internal accounts are authenticated at a separate internal dashboard, a private route behind a firewall or something.
- Don't use the your internal company login system (IDP) to also log users in on your more public system. So have separate login systems or IDPS for public uses and internal staff/systems. I mean if a hacker breaks the login for public users, they will also have access for your internal system, so to prevent this, create a 

### Passwords

#### Password Strength:
- Length less than 8 is weak. The maximum length should be at least 64. Include a password strength meter on the frontend.
- Don't truncate passwords, leave them as is. Allow usage of all characters including unicode and whitespace. Also there shouldn't be requirements for upper or lowercase, number, or special characters.
- Block common and previously breached passwords from being used.
- Do credential rotation when a password is leaked. Notify the user and require a password reset from them. Or when your auth infrastructure changes such as migrating to a new IDP or algorithm.
- If passwords are leaked, then Avoid requiring periodic password changes for your users, just tell them to pick a strong password and enable MFA.

#### Password Management:
- **Implement password recovery:** If the user wants to change their password then make them enter their current password as well. Imagine the user is using a public computer, we don't want some random changing their password.
- **Updating sensitive info:** Prompt for credentials before updating sensitive account info like user password or email address. Or before purchases and whatnot. It's a countermeasure to CSRF or XSS, making it harder for the attacker to od those actions.

  - **Note:** In the OWASP cheatsheet, they list how we should also go about changing the user's email address depending on whether or not they have MFA on. It's a complex process, but with MFA, we use it to authenticate the user and store their new email as a pending change. Then we'll send two emails to the user, the first warning them about the email change, and the second that will confirm the change and set that new email address. A similar idea is in the other algorithm. 

- **Vague error messages:** In terms of error messages, you shouldn't give the exact mechanism that caused a login to fail e.g. "passowrd invalid", "account disabled". Instead jsut say the credentials were incorrect. Same for password recovery, don't mention whether or not we had a user, just say "If that email exists, we'll send you an email). Any status message, you want to reveal as little info as possible. 
- **Multi-factor authenticated:** Implement this whenever possible.
- **Login Throttling:** Prevent an attacker from making too man login attempts. Set a maximum number of attempts. Account lockout is a common technique to prevent any more attempts. You can ban the IP, but sometimes there's also the idea of multiple IP addresses that the attacker is using. Try to avoid a DoS, so give the user a way to log back in by recovering credentials.
- **CAPTCHA:** Helps prevent automated login attempts. Captcha has weaknesses so view it as something that delays attacks rather than preventing them. It could be more user friendly to require a captcha after a small number of failed login attempts.
- **Logging and Monitoring:** Log failures, account lockouts, password failures, etc.



## Credits:
- [Authentication Cheat Sheet - OWASP](https://cheatsheetseries.owasp.org/cheatsheets/Authentication_Cheat_Sheet.html#authentication-general-guidelines)