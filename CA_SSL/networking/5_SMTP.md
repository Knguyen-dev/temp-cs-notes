# SMTP (Simple Mail Transfer Protocol)

### What is SMTP
A standard for sending emails. It let's an email client (e.g. our computer, web app) exchange data with a mail server. It's a mail delivery protocol, not a mail retrieval protocol. Also, it will sometimes send mail to an email provider's mail server, rather than to the recipient directly.

### How does it work? 
- SMTP Connection Opened: SMTP uses TCP to transport stuff. After the TCP handshake, the email client sends a Hello packet.
- Email data sent: Client sends some commands and then the email's contents, which include the header, body, destination, and any other info.
- Mail Transfer Agent (MTA): The server should be running a MTA, which checks the domain of the recipient's email address, checking if it differs from the sender. It then queries the DNS to find the recipient's IP address, akin to a Post office looking up a person's address.
- Close the TCP connection.

Akin to network routing, the first email server is not our destination. Now the email server will repeat this SMTP process with another mail server, and this chain keeps going until the email reaches the destination mail server that's controlled by the recipient's email provider.

### Reviewing SMTP Terminology
**SMTP envelope:** The info that the email client sends the mail server pertaining to the source and destination of the email. It's separate from the email header and body.

**SMTP Commands** are prefinded text instructions that tell a client or server what to do.
  1. `HELO/EHLO`: The Hello command that initiates the SMTP process.
  2. `MAIL FROM`: Tells the server who is sending the email. For example if Alice we're trying to send an email to Bob, the client could send `MAIL FROM: <alice@example.com>`.
  3. `RCPT TO`: This command is for listing the email's recipients. If there are multiple clients, then teh client would send this multiple times. An example is `RCPT TO: <bob@example.com>`.
  4. `Data`: The contents of the email such as who it was from, subject line, to email, and body of the email, alongside other data.
  5. `RSET`: Resets the connection, clearing all previously transferred info without closing the SMTP connection. The client would use this if they sent incorrect information and wanted to redo.
  6. `QUIT`: Ends the connection. 

An **SMTP** server is a mail server that can send and receive emails using the SMTP protocol. You can haev several programs running on a SMTP server:
  1. **Mail submission agent (MSA):** Receives emails from the email client.
  2. **Mail transfer agent (MTA):** Transfers emails to the next server in the delivery chain. It may query the DNS to find the recipient's mail exchange (MX) DNS record if needed.
  3. **Mail delivery agent (MDA):** Receives emails from the MDA and stores them in the recipient's email box.

Remember that whitelisting creates a list of users that you trust and are able to send emails or access your app. **Greylisting** is the idea that if we receive an email from an unknown user, we'll temporarily reject it with a "try again later response". Typically if it's a spammer, they don't follow up on the email, however if it's a real person, they do retry. This takes advantage of the tendency of spammers to not follow up on things.

### What ports does SMTP use? 
- **Port 25:** Mostly for connections between SMTP servers. Firewalls on enduser networks typically block this port to protect against spammers.
- **Port 465:** Previously used by SMTP with SSL. But SSL has been replaced with TLS, so modern email systems don't use this port. You may see it in legacy sysetms.
- **Port 587:** Default port for email submissions. SMTP communications via this port use TLS.
- **Port 2525:** Not officially associated with SMTP, but some email services offer SMTP delivery over this port.

### Comparing various email protocols
**IMAP (Internet Message Access Protocol)** and ***Post Office Protocol (POP)** are used to deliver the email to its final destination. Remember that SMTP was for delivering between SMTP servers, but we never talked about the part about receiving the emails.  The email client retrieves the email from that last mail server in the chain using IMAP/POP.

### Using SMTP to send emails vs a service?


To send emails programmatically:
1. Connect to some SMTP server on some hostname and port. For example, if you're sending emails from a Gmail, it's ideal that you use a SMTP server owned by gmail. However it's not a strict requirement.
2. Provide the username and password to your email account. Now things should work.

**SMTP** hosting refers to the server infrastructure that handles the sending and managing of emails via the SMTP protocol. Most businesses have a custom domain, and they typically get an SMTP host. You'd usually have to mess with prootcols like SPF, DKIM, and DMARC to ensure your emails are delivered and aren't marked as spam.

Alternatively, you can use third-party email services like SendGrid, Mailgun, Amazon SES as these services proivde reliable SMTP servers and APIs for sending emails. This reduces the complexity of managing your own SMTP host, and can give you more features.

## Credits
- [What is SMTP Cloudflare](https://www.cloudflare.com/learning/email-security/what-is-smtp)