# Content Security Policy

It's a way to help prevent XSS, clickjacking, and cross-site leak vulnerabilities.


### Defending against XSS with CSP
1. Restricts inline javscripts from running:
```JavaScript
// This won't work anymore
<script>document.body.innerHTML='defaced'</script>
```
2. Restricts the page from loading remote scripts:
```
<script src="https://evil.com/hacked.js"></script>
```
3. Prevents the execution established unsafe JavaScriptlike `eval()`

4. You can also restrict where forms are submitted. So doing 
```bash
# If the app tries to submit forms to any other domain, it'll be blocked.
Content-Security-Policy: form-action 'self' https://trusted.example.com;
```
5. It restricts objects, so you can't use the html `object` tag to put executables on the page.

### Defending Against Framing Attacks 
To do clickjacking or something similar, the malicious website needs to load the target website in an IFrame. You used to use `X-Frame-Options` header for this, but now you use the `frame-ancestors` CSP directive.

### CSP: Choosing the policy delivery
Before we indicate the "type" or even setup the policy itself, we have to talk about how we can "deliver" or apply the CSP. You'd set the `Content-Security-Policy` in the header of the response from your web server. This is the preferred way of doing it, and you'd do this for all HTTP responses. 

### CSP Types 
Originally, you just created allow-lists which would define the content and sources that were allowed to be loaded in your HTML page. Now we can just create a "Strict". You create a strict CSP by setting some "fetch directives" and then using either a hash or nonce based mechanism.



#### TODO
Still 



### Demonstration: Tying everything together

## Credits
https://cheatsheetseries.owasp.org/cheatsheets/Content_Security_Policy_Cheat_Sheet.html
