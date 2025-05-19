


# Content Security Policy 

A feature that that restricts the code and scripts that the browser is allowed to run. A common use is controlling what JavaScript code the document can load, and this helps against XSS attacks. You can also use it to defend against clickjacking, and ensure that your pages are loaded over HTTTPS.

A CSP is sent to the browser as the Content-Security-Policy response header. It looks like:
```bash

# First part tells the browser to only load resources from the same origin as the document 
# Second part says that you can load images that are from the smae origin OR `example.com` 
Content-Security-Policy: default-src 'self'; img-src 'self' example.com
```
So again we're controlling the resources that the document is allowed to load. Of course for XSS, a website should sanitize the input before including it in a page, however CSP is that extra layer that protects a site when sanitization fails. So you set the directives like this:
- `script-src` sets allowed sources for JavaScript.
- `style-src` sets allowed sources for CSS stylesheets.
- `img-src` sets allowed sources for images.
- `default-src` is a fallback for all resources whose directives weren't explicitly listed in the CSP header.

### Best Practice and takeaway?
Your policy needs to prevent the execution of untrusted scripts. In any case, all you need to know is that you should just create a "Strict" CSP. The idea is that your script elements will have a nonce. 

Modern frameworks escape user content by default, so XSS is less of a daily concern — unless you're rendering raw HTML (e.g., via dangerouslySetInnerHTML in React). In those cases, XSS becomes relevant again, and sanitization + CSP is critical.



## Credits 
[CSP ](https://csp.withgoogle.com/docs/strict-csp.html)