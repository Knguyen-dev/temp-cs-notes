# Secure Shell (SSH) Protocol

### What is it?
A method for securely sending commands to a computer over an unsecure network. 

- It authenticates and encrypts connections between devices.
- Allows for tunneling/port forwarding. You know how tunneling is a way to cross terrain or boundaries that you can't normally cross right? In networking, tunnels are just a way to transport data across a network, using protocols that aren't supported by that network. 

SSH is often used for controlling servers remoting, managing infrastructure, and transferring files. An analogy could be how a store owner may email and instruct their employees from afar to ensure the store runs well when they're gone. In the same vein, SSH allows admins to manage servers and devices remotely.

**Telnet** is an older remote management protocol, but it's not used anymore because it's not secure. The commands the admins send can be seen by other people.

---
### What SSH does
- **Remote encrypted connection:** SSH can be used to setup a connection between you and faraway machine, often a server. It's secure in the sense that transmitted data is encrypted. Even if someone does gets the data, it's literally encrypted, so they're not going to be able to do anything useful.
- **Tunneling:** We're moving packets across a network using a protocol or path that we wouldn't be able to normally use. To get technical, we wrap our data packets with additional info called "headers" (adding headers to packets?), which influences the destination they're routed to. Then we use a technique called _port forwarding_ to send those packets from one machine to the other.

---
### How it works?
Built on top of the TCP/IP protocol suite. So that's how it's sending packets, as opposed to using UDP. 
Assume you want to remote SSH into a server:

1. Give the server your public key. This public key is now stored in `~/.ssh/authorized_keys` on the server.
2. The server now creates a long string/number, which is typically called a "random challenge" or "nonce". They encrypt this challenge with your public key. Then they send to back to you. 
3. The only way someone can decrypt this nonce is if they have your private key, and you're the only person that has that. You use your private key to decrypt the nonce, and send it back to the server.
4. This proves to the server that you own the private key that matches the public key you gave to the server. It gets a little more technical, but they will have the information to generate a shared session key for encrypting and decrypting each other's data. 

---
### Security
Let's talk about Public Key Cryptography (PKC) and authentication. SSH uses PKC to encrypt data, and also for authentication. 

In an SSH connection, both sides will have their own public/private key pair, and they're going to authenticate each other by using these keys.

This is probably different to how you're familiar with PKC, as with HTTPS, you'd use PKC to verify the identity of the web server. 

PKC authenticates connected devices, meaning your client is going to verify the server's public key to see if you're connecting to the correct machine. And the server may also check your public key (if using SSH key-based auth) to confirm that your device is allowed to connect.
 
However, even though the computers trust each other, the server still needs to know who's trying to login. So you'll typically need to input a username nad password. 

On successful login, the user would be able to execute commands on the remote machine as if they were using their local machine.

---
### SSH port forwarding (tunneling) explained
For Bob to send a message to Dave, he first sends the message to Alice, and then Alice sends it to Dave.

You're using in-between computers to send data. Imagine you want to remotely make changes to a server B, which is in a private network.

However server B only receives network packets from other computer within the private network. You're not going to be able to directly remote SSH connect to server B. However, you notice that server A is in the private network, and you're able to remotely connect to A. So if you remote SSH to A, using A you can then do remote SSH again to connect to server B. Now you have access to server B since you did it through server A.

## Credits: 
- [Learn SSH - CloudFlare](https://www.cloudflare.com/learning/access-management/what-is-ssh/)