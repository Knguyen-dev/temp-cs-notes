# OWASP: SQL Injection

SQL injection is basically when the input of a user is able to directly affect the SQL statements that our application uses in unexpected ways:
```php
$username = $_GET['username'];
$password = $_GET['password'];

// Imagine username = "MyAdminUsername" and password = " OR '1
$sql = "SELECT * FROM users WHERE username = '$username' AND password = '$password'";

// Resulting query: SELECT * FROM users WHERE username = 'admin' AND password = '' OR '1'='1'
// Allowing the user to login.
```
This typically happens if when we're doing some kind of dynamic string concatenation. The main solution should be using prepared statements with variable bindings. This is also referred to as parameterized queries. This technique basically makes it so, regardless of what input is supplied, the attackers won't be able to change the intent of the SQL query. So for us, it would look for a passwor d that matches the string "" OR '1". 

Escaping user input can work, but it's discouraged since it's pretty weak compared to just using parameterized queries.

You'll may hear this being referred to as **RCE (Remote Code Execution)**, as that term is more general for any type of injection attack. but yes is a type of RCE that's targeted towards our databases.


## Credits
- [SQL Injection](https://cheatsheetseries.owasp.org/cheatsheets/SQL_Injection_Prevention_Cheat_Sheet.html