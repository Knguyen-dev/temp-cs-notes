# OWASP: Password Storage
When we store passwords, we don't store them plaintext in the database. Instead we store their hashes, so that in case the app or database is compromised, at least the attackers don't have the passwords of the users. 

### Hashing vs Encryption
Almost always, passwords should be hashed, not encrypted:
- **Hashing:** One-way operation, you can't decrypt a hash to get the original plaintext. Even if an attacker obtains the hash, they can't reverse engineer it to get the original password. Attackers can only guess.
- **Encryption:** A two way operation, you can decrypt an encrypted code, and get the plaintext. You definitely don't want the attackers to have a chance at reverse engineering things and getting your users' passwords.

#### How attackers crack password hashes 
The best practice is using a strong password and a modern hashing algorithm. Attackers still "crack" these password hashes, and guess the original plaintext. Let's say we have access to some user's password hash:
1. Select a password you think the victim has chosen (e.g. `password1!`)
2. Hash that password.
3. Compare the hash you calculated with the hash of the victim. If the hashes match, that means they were the result of the same plaintext. As a result the password has been cracked as we now know the original plaintext password.

They programmatically do this process with a large number of passwords likely gained from a list of passwords from compromised sites, a dictionary of common passwords, etc.

### Enhancing Password Storage 
A **salt** is a unique randomly generated string that you use in the hashing process. The salt should be unique for each user. As a result, even if two users have the same plaintext password, since their salts will be different, the resulting password hashes will be different. As a result, you'd need to crack both hashes to determine whether two users have the same password, as opposed to before they would have just had the same hash. This makes it a lot harder to crack passwords, and modern algorithms do the salting automatically. To clarify before it wasn't uncommon to store the password salt with the user in the database, but now the salt is embedded in the hash.

The **work factor** refers to the number of iterations the hahsing algorithm goes through for a given password. This is typicallly stored in the hash. The motivation behind this is that since we're going through more iterations, the password hash is going to be more complex and harder to crack. Though the downside is that this is more computationally expensive.


### Password Hashing Algorithms
- **Argon2id:** THe most modern hashing algorithm and the one you should pick.
- **scrypt:** Use scrypt when you don't have Argon2id.
- **bcrypt:** A really popular one, and it's still considered generally secure, but you should just use this for older systems.

#### Upgrading Legacy Hashes:
When the user enters their password (typically through authentication), that input should be re-hashed using the new algorithm. Expire the current user's password and require them to enter in a new one. The only issue now is that the old (less secure) password hashes will still be stored in the DB, until the user logs in. This is an issue because we want to secure their accounts as quickly as possible and there's a decent amount of risk of their passwords being cracked. Here are two scenarios to handle this:
- **Method 1:** Expire and delete the password hashes of users who have been inactive for an extended amount of time. Next them they login, you'd prompt them to reset their password (e.g. via email link) and then update their password to be the new password hash.
- **Method 2:** You may use existing password hashes as inputs to more secure algorithms. So just do `bcrypt(md5($password))` which is a temporary fix since these are easier to crack. Now the next time they authenticate, hash the plaintext and update the password hash in the database.


## Credits
- [OWASP Password Hash](https://cheatsheetseries.owasp.org/cheatsheets/Password_Storage_Cheat_Sheet.html)