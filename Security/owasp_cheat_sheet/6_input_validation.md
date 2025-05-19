# OWASP: Input Validation

### General Info
- Syntactic and Semantic Validation: Make sure correct syntax of fields (e.g. SSN, phone number). Make sure the values are correct in the specific business context (start date comes before end date)
- Using an allow list: A technique where you define what is allowed. So you'll accept the input if it's in the allow list, else reject it. You could also use a DenyList, but this should only be used as a complement to an allow list, rather than a replacement. 
- Regular Expressions: Regex is really useful for input validation. You apply it to all data, define the set of characters to be accepted, and define a min and max length for the data. 
- Client vs Server-side validation: Server side validation is the real check because the client can be modified, they can use postman to hit the endpoints, etc. Client side validation helps reduce bad validation from reaching the server.

### Validating Different Types Info 
- rich content is hard. Use a library!
- To prevent XSS, please html encode any data before rendering it showing it. This is common when the user enters an html script and you render their data as html. As well as this 
- do file upload: This is talked about 
- Email address validation

### Email Address Validation and Verification
The format of emai laddresses is defined by a standard called RFC 5321, and parsing these emails is very difficult. There are so many regexes. In real applications, emails validation is much more strict so an app may reject an email that still technically conforms with RFC 5321, and that's realistic. The process should be that you do some basic validation, and then give the email to a mail server and see if it can send it or not. Here's teh basic validation stage:
- Email has two parts separated by `@` symbol.
- Doesn't contain singles "dangerous characters" like backticks, single quotes, double quotes, or null bytes. "Dangerous" can be flexible depending on what you want.
- Domain part contains only letters, numbers, hyphens, and periods.
- Email address is of reasonable length. 

Then in the semantic validation (email verification), we try to email the user. It lets us know whether the email is correct, and whether things send correctly. The idea is to create a 32 character random string, associate with the user as a "email verification token" that has some expiration date. Then construct a link to your `/api/email/verify/:token` endpoint.



## Credits:
- [Input Validation](https://cheatsheetseries.owasp.org/cheatsheets/Input_Validation_Cheat_Sheet.html)