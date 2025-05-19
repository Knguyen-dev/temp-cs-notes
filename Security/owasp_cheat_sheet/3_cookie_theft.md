# OWASP: Cookie Theft Mitigation

If an attacker steals a valid session cookie, they're able to hijack the user's session for the duration of the session's lifetime. There's no counter measure once they have the a valid cookie, because now they are treated as if they are teh authenticated user. The only thing we can do is detect when a cookie is stolen, and invalidate it as fast as possible.

### Cookie Theft Mitigation
If a cookie is stolen, certain environment info could change. If stolen by an attacker from another country, the IP address is going to be significantly differnet. Here are the many vectors or angles we can use for analysis:
- **IP Address:** Access from a different region.
- **User-Agent:** Access from different device.
- **Accept-Language:** Access from different language setting.
- **Date:** Access at a different time of day.

However this isn't full proof. The ip address can change if they connect to a different wifi network. The user agent could change if the yupdate their browser. So if you are making comparisons, think about how significant they are before you invalidate a cookie. In any case you'd store this type of information inside the session object when the session is first created. On following requests, compare the info on the current request to the information stored in the session object. If it's a significant change, then it's possible that the session has been hijacked. 

### Session Validation
If session could have been hijacked, then temporarily invalidate the user's session and ask them to re-authenticate. On success, you'll should generate a new session on the server, and hand them a new session cookie. Make sure to invalidate the old session, to ensure that if it was hijacked, then the attacker won't be able to use that old cookie.

## Credits
- [Cookie Theft Mitigation](https://cheatsheetseries.owasp.org/cheatsheets/Cookie_Theft_Mitigation_Cheat_Sheet.html)