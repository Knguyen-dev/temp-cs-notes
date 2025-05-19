# Remote Code Execution (RCE)

### What is RCE?
An RCE attack is one where an attacker can run malicious code on an organization's computers or network. A a result, tehy can steal data, install additional malware, and just overall mess things up.

RCE is also a part of a larger group of vulnerabilities called acrbitrary code execution (ACE). In any case, RCE is the most dangerous since the attacker doesn't need to have prior access to the system or devvice.

### How RCE works
It takes advantage of vulnerabilities in applications and network infrastructure. Here are several types of RCE:
- **Injection vulnerabilities:** An example is SQL injection or command injection. This happens because we have bad input sanitization. If we aren't careful, the input that the user.
- **Insecure deserialization:** Serialization simplifies the transmission of data by packing it into a single string of bits that are going to be unpacked by the receiving system. But if the structure of this data isn't well defined, the attack can create an input that's misinterpreted when unpacked. You can imagine that depending on the situation, this can lead to code execution.
- **Out-of-bounds write:** A buffer is a fixed-size piece of memory that's allocated to store data. Insecurely reading or data data could be dangerous as an attacker could have placed data that can be interpreted as code, and control the flow of the application. I mean they could overwrite parts of the memory, crash the program (DOS), etc. You can probably see this happening more easily in C, but it's possible everywhere.
- **File management:** Some apps let users upload files. A user could upload a file containing malicious code and may want to trick the application into executing it.

### How to prevent RCE
- **Vulnerability scanning:** 
  - ***Static Code Analysis:*** Examines code without runnning it. We parse code or compiled code to find issues like syntax errors, bugs, and in this case vulnerabilities. So this would be used ot identify well known vulnerabilities.
  - ***Dynamic Code Analysis:*** Analyzes code while it runs. So we'd observe the behavior during execution to find issues like memory leaks, exceptions, and vulnerabilities.
- **Updating and patching:** If an RCE is found in the app or a third party library, then apply updates and patches to fix things.
- **Input validation:** Most RCE vulnerabilities involve poor input validation and sanitization, so just improve it.
- **Secure memory management:** Attackers can use exploits like buffer overflows  
- **Network monitoring:** Attempted RCE exploits can be detected and blocked by a web app and api protection. However a network security solution can help las well.

## Credits
- [Remote Code Execution - CloudFlare](https://www.cloudflare.com/learning/security/what-is-remote-code-execution/)
- [Remote Code Execution - Imperva](https://www.imperva.com/learn/application-security/remote-code-execution/#:~:text=RCE%20is%20considered%20part%20of,to%20the%20system%20or%20device.)