# OWASP: Cross Site Scripting
Protect all variables by making them go through validation and are then escaped or sanitized. Escaping input means converting characters like <, >, ', and " into their safe equivalents (e.g., &lt;, &gt;) so that the browser treats them as plain text, not code. This helps prevent user input from being interpreted as HTML or JavaScript—protecting against XSS.

**Sanitizing** is different as we just strip away and remove the dangerous input entirely.

### Defense 1: Output Encoding
When the user is typing stuff out and you want to render that content. If you're using a framewokr, then the framework will have default encoding protection, which will encode those special characters into safer representations. However if you're not using a framewokr, you'll just use an established library.

There are many scenarios. If inserting in HTML, use HTML entity encoding, converting those html special characters into a safegroup of characters. The OWASP cheat sheet focuses on this in more detail.

### Defense 2: HTML Sanitization
Sometimes you need your users to generate HTML via a WYSIWYG editor, so the user needs to be able to see the stylings and contents of their HTML. Using output encoding here will prevent XSS, but it'll leave their HTML output unusable and not styled. You should use HMTL sanitization in this case, which means we'll strip dangerous html from content. The ideal library for this is DOMPurify. Of course if your app has to store HTML content, have server-side sanitization.

### Defense 3: CSP
You would need to implement a strict Content Security Policy that will only allows scripst with the right nonce or hash to be executed. You'd set all of your scripts with this hash, and then this should prevent all other scripts from running. But don't solely Don't solely rely on CSP headers. CSP isn't a one size fits all thing, buts best when its complementing another defense strategy. For example, CSP can have different scopes/effects across browser versions, and at worse can break legacy apps.

### Main Types of XSS
- **Stored XSS** Occurs when you store the user input to be rendered later into a page. The risk here is when you forgot to escape or sanitize the input. An example is how a user could store HTML in a comment or user profile. An attacker can embed malicious JavaScript in the HTML. If the input isn't properly sanitized and we are rendering their input as a script, then anyone who visits the site and happens to bring up that user's comment will have that malicious script ran on their browser.

- **Reflected XSS** is when a site takes user input and embeds it in the page, but without storing it on the server. An example would be a multistep form that uses your answer to the first question to determine the next question, eg "what's your favourite food?" If the user answers "pizza", the form then asks "what's your favourite pizza?" But the user answers with <insert malicious JavaScript>, the form will ask "what's your favourite <insert malicious JavaScript>" if it doesn't correctly filter/validate the user input. It's "reflected" because the user's input is reflected back to them in the HTTP response from the server. But since this isn't stored on the server, the attacker would need to trick the user into opening a crafted URL with the malicious answer embedded in it. 

- **DOM XSS:**  Happens entirely on the client side. Again, you would need to trick a user into opening a specially crafted URL to execute this attack. 

### General Rules Review
The second OWASP has full details, but I'll list the general rules:
1. Sanitize data before inserting it into HTML
2. Sanitize data before inserting it as a HTML attribute 
3. Careful about inserting untrusted data into an event handler.
4. Sanitize untrusted data before inserting it into CSS atttribute. 
5. Sanitize before making that untrusted data as a url.

- Treat untrusted data as displayable text. Avoid treating it as code or markup within JavaScript code.  I think that sums it up in general. Sanitize stuff that could be scripts before storing or rendering them out. If possible you don't want to render it out as html because that heightens the risk of XSS, render it out as text. That sums up most everything.


## Credits
- [XSS Prevention](https://cheatsheetseries.owasp.org/cheatsheets/Cross_Site_Scripting_Prevention_Cheat_Sheet.html)
- [XSS Prevention on the DOM](https://cheatsheetseries.owasp.org/cheatsheets/DOM_based_XSS_Prevention_Cheat_Sheet.html)