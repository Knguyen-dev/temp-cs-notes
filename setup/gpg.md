# GPG Key Setup

### Install GPG
On windows and linux they're different. So on window's you'd have to go online, but for linux you just do:
```
sudo apt-get install gnupg
```

### Step 1: Generate GPG Key
1. `gpg --full-generate-key`; How to generate a gpg key. Use 'RSA and RSA'. Choose '4096 bits'. Higher the bits means higher security. Choose '1y'; it's always good practice to have the gpg key; You can always renew the key so that even if it does expire, you don't have to go through the motions of creating an entire new key again. Continue going through the creation process
2. Now we should have a key pair. We have 'pub' which is our public key, and then 'sub' which are our potential additional keys we could have associated with our primary key.

### Editing Key
Let's say it's been a year later, and you want to update that gpg key pair 

gpg --edit-key <email-for-your-key>


### Revocation Certificates
These digital certificates allow you to invalidate a given gpg pair. This is created 
```
gpg --output revoke-mycert.asc --gen-revoke <email-for-your-key>
```
Then to actually use the revocation certificate, you will import it:
```
gpg --import revoke-mycert.asc
```

### Step 2: Exporting that GPG Key Pair
First locate the key you want. do:
```
gpg --list-keys
gpg --list-secret-keys
```
You know that a key-id matches for both public and private keys, the unique identifier for a key pair.

1. `gpg --export --armor <key id or your email>`: 
2. Copy the public key block, and then post that stuff on GitHub.
3. `gpg --export-secret-keys --armor <private-key-id> > private-key.asc`: this gets the private key and writes stuff to a file. 
4. Let's pretend you've moved your private, and optionally your public keys to a new computer.
5. You have .asc files, you literally just do `gpg --import /path/to/private-key.asc`. You can also do the same for the public key.

**Note:** If transferring key files from Windows to Linux, be careful! Convert the line endings for the file from CRLF to LF. You can do this in VScode. It's only after this, that you can send it over and do the import command on those .asc files.


## Configuring Git and Signing stuff

### Review on git tags
```
git tag v1 ; light-weight tag, a pointer to acommit 
git tag -am "Annotated tag" v2
git tag -sm v4 ; Annotated  
```

### Setting up Git config

Do `vim ~/.gitconfig`, and then make your file like this:

```
[user]
        name = <your-name>
        email = <your-email>
        signingkey = <private-key-id>
[commit]
        <!-- Auto -->
        gpgsign = true
[program]
  program = /usr/bin/gpg
```
Use the email for that specific signing key.

One more thing. If working on WSL2 or linux. In our `~/.bashrc`, have this line:
```
export GPG_TTY=$(tty)
```
1. `gpg --export --armor <your-email>`: You can pass the output flag to write this to disk. But yeah you'd copy this public key block to your GitHub account.


### Migrating keys
So I have a private key I want to migrate from one machine to another:

1. `gpg --list-secret-keys --keyid-format=long`: 
```
/home/knguyensky/.gnupg/pubring.kbx
------------------------------------
sec   rsa3072/<key-id> 2024-04-03 [SC]
      ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890ABCDEF
```
2. Copy the key ID and do this, which exports a private or public key:
```bash
gpg --export-secret-keys --armor <key-id> > my_private_key.asc
gpg --export --armor <key-id> > my_public_key.asc
```

3. Transport the key to windows using some method such as the `/mnt` folder, a shared folder between windows and linux

4. Assuming you have powershell or something open and GPG installed:
```bash
gpg --import \path\to\my_public_key.asc
gpg --import \path\to\my_private_key.asc
```
5. Then trust the key:
```
gpg --edit-key <key-id>
```
6. Check if the key was imported successfully:
```
gpg --list-secret-keys --keyid-format=long
```


### Download stuff
```
sudo apt update
sudo apt install gnupg -y
```
Then setup git signing

```
sudo apt install pinentry-curses -y
git config --global user.signingkey <your-private-key>

```

### Make
```
sudo apt install make
```