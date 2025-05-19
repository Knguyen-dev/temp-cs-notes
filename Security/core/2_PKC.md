# Public Key Infrastructure

## Review on asymmetric and symmetric encryption

### Asymmetric Encryption
- **Asymmetric (public key) encryption:** A cryptographic system that uses two keys for encryption and decryption. Both keys are mathematically related and are generated in pairs.
  - **Public Key:** Freely distributed and is used to encrypt data. Anyone can use the public key to encrypt a message, but only the holder of the related private key can decrypt the message. Public keys are often used for encryption because they can be shared openly and used by anyone without compromising the security. It doesn't harm us if anyone can encrypt a message and send it to us.
  - **Private Key:** Kept secret by the owner and is used to decrypt data that has been encrypted with the corresponding public key. Private keys should be kept secret as they're used to decrypt sensitive information. It does harm us if the private key is leaked, as now anyone could decrypt these private messages that were meant for us only!

Example: If Alice wants to send an secure message to Bob, she gets Bob's public key and uses it to encrypt her message. Now the message can only be decrypted using Bob's private key!

A big drawback of public-key cryptography is that it's computationally expensive. The other drawback is if you accidentally reveal your private key or lose it. In the former, just assume its compromised and shut down your services temporarily. Then generate a new pair and update things.

In the latter, it really just depends on what you're doing. If it's related to SSL/TLS, then yeah generate a new cert. If it's a JWT signing key, invalidate all tokens signed with that key. If it's SSH, then you'll need to find another way to get access.

---
### Symmetric Key Encryption
Here we use the same cryptographic keys for encryption and decryption. They could be the same exact key, or there can be a simple transformation/relationship between the two keys. The keys are a **shared secret** between the two parties. This just means both parties need to have access to the secret keys for them to be able to encrypt the messages they send and decrypt messages that they receive.

For symmetric key encryption, the main drawback is that all parties that are involved need to have access to the secret key. However it's very fast, which is why it's still used. If the key is exposed, we'll need to use another key for our future communications.

## PKI Explained

### What is PKI?
Referring to the software, hardware, policies,  and other components that are used to create, distribute, store, revoke, and work with digital certificates and public keys.

It's also used to refer to when we bind/link public keys to people or devices. The main idea is to maintain an 

---
### What is key escrow
The idea that someone else, hold your private keys. This could be an admin or someone else within your organization. This is typically employed because when people leave your organization, you want to do two main things:
  1. Prevent any chance of the person taking the private key with them and somehow leaking it.
  2. Be able to decrypt and access any encrypted data or work that they left behind for the company.


## Credits:
1. [Public Key Infrastructure - Professor Messeur](https://youtu.be/xHAMEF7-inQ?si=vfc7ZxxtBGJShPIB)