

## Credits

OWASP Security 
- [OWASP Top Ten](https://owasp.org/www-project-top-ten/)
- [OWASP Cheat Sheet Series](https://cheatsheetseries.owasp.org/)
- [OWASP ASVS](https://github.com/OWASP/ASVS?tab=readme-ov-file)
- [OWASP API Top Ten API Security](https://owasp.org/www-project-api-security/)

Network security. You should probably understand networks first. But after ou should know:
  - TLS/SSL, why it matters and how to configure it.
  - Read about man-in-the-middle attacks. 
  - Firewalls and ports 
  - Secure networking protocols (SSH, VPNs)
  - Zero Trust Architecture 
  - DNS security (DNS spoofing )

- [Cyber roadmap](https://roadmap.sh/cyber-security)

The cyberoadmap also has good stuff for networks and security. I think the devops roadmap is pretty good for that as well. You should probably read both and compare data.

Authentication and identity management:
- Understand OAuth2.0, OpenID Connect, JWT best practices.
- Learn multi-factor authentication. I think this is the one we haven't covered yet.

CVE Triage. There are thousands of CVEs, so we typically just focus on popular libraries and tools that we can use to detect them:
- Node: npm audit for Snyk
- Python: pip audit or safety?
- Docker/containers: Docker Scout or "Trivy"

- In terms of CVEs, watch out for ones with remote code execution.
- Privilege escalation CVEs

Threat modeling. Learn how attackers think:
- STRIDE and PASTA.
- OSWAP Threat Modeling Tool

In terms of DevOps, you should use pre-commit hooks to scan for secrets. Use CI tools to integrate security like SonarQube.

This one's a really large overview of it. You can only do so much of it, so you should really focus on stuff like API, authentication, and probably the beginner or foundation stuff.


### Fun projects
- A secure notetaking or pastebin app. Kind of like bitwarden secure notes or IU secure notes. Where you're learn client-side and server-sie encryption, access control, input sanitization (avoid xss), and expiring links and tokens. A bonus could be use passoword-protected notes and share one time links. You could also encrypt secrets client side before upload (zero knowledge style). Encryption at rest.

- Secure file uploader: Create an application that we can use and reuse to uplaod files. We'd avlidate MIME types and extesnions. Prevent directory traversal. Store and serve files securely.

- Vulnerable App Clone and Secure Rewrite: Pick an app liike OWASP Juice or DVWA and rebuild it securely.

- API Gateway or Proxy: Write a simple proxy for routing traffic. Learn TLS termination, rate limiting, IP filtering, and secure headers. 

- Secure by default web app starter kit. Sensible defaults like helmet, csrf protection and secure cookies, etc.


These are the ones that are still under development. I'll get to these againsoon
TODO: 8_CSP, SSRF.

## Credits:

https://www.cloudflare.com/learning/security/api/owasp-api-security-top-10/

