# OWASP: Logging

### Motivation
You may want to debug things, save data for business reasons, monitoring performance, etc. Or for security reasons like identifying security incidents, auditing user actions, and having real time monitoring in security.

## How to Implement Logging

### Data Sources 
- Application code itself
- Closely related applications 
- The database
- The network firewall and network and host intrusion detection systems. 
- Any other place related to your application where you want to record information.

### Where to store logs?
You typically log data into a file syste mro a database. For desktop or mobile apps, they may send it to their local storage and then also a remote data store. But there are some guidelines:
- File-based logs: Set permissions on who can access log directories and files.
- Database Logs: 
- Use a common logging format. This will allow you to integrate with other systems. Also for other event information like error stack traces, create separate files for those.

### Which events to log
- **Input validation errors:** Such as protocol violations, non-accepted encodings, bad syntax, etc.
- **Output errors:**
- **Authentication events:** Login success or failure. Such as session management failure e.g. session ID invalid, jwt bad, etc.
- **Authorization events:** Permission denied, user permissions changed.
-- **Application Errors:** Runtime error, connection issues, performance issues, third party service errors. Etc.
- **App startup, shutdown:** Log stuff related to the start up and shut down of the app. Like what port is starting up, what services are runnning, etc.
- **Log higher risk events:** Admin user deletes other users, changes privileges, etc. Also log suspcious activty and whatnot.

### Log Attributes
Ideally you want to set it up so that your log makes sense looking at it in isolation, but you should also be able to aggregate them. 
- Log date and time
- Log the name and version of your app. Log the service, protocol, the endpoint.
- Log who made the request, who was involved in the event. So like the user or machine identifier, IP address, etc.
- Log what happened. The type of event "Login" severity of the event, and description.

### Data to exclude
- App source code
- Session ID values. If you need to track specific session events, just hash it.
- Access tokens
- Sensitive personal or account info.
- Secrets such as db connection strings, api keys, and secrets used for encryptions.

## Credits
- [OWASP Logging](https://cheatsheetseries.owasp.org/cheatsheets/Logging_Cheat_Sheet.html)