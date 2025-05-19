

### File permissions in linux
Every file and directory has 3 permission groups:
- Owner: Permissions for the person who owns the file or folder.
- Group: Permissions for the group assigned to this file or folder. 
- Others: File permissions for other users; take care of this one.

Then each permission group will get 3 permissions:
- Read: Read the contents of a file
- Write: Modify the contents
- Execute: Execute a file. An example would be running the `ls` command on a directory.

#### Example 1: Reading file permissions
```bash
drwxrwxr-x 2 ubuntu ubuntu 4096 Oct 2 11:55 files
```
- `d`: Indicates that this is a directory, the name of this directory is `files`.
- `rwx`: This first set of 3 characters represents the owner's permissions on this directory. The owner, whose name is ubuntu, is able to can read, write, and execute commands on this directory.
- `rwx`: The group, named ubuntu, has the ability to also read, write, and execute on the directory.
- `r-x`: All other users can only read or execute commands on this directory, meaning they can't write like the others. This group is also named ubuntu.

#### Example 2: Reading permissions again
```bash
-rw-rw-r-- 1 ubuntu ubuntu 21 Oct 2 11:56 main.py
```
- `-`: This first dash indicates that this is a file. If it was a directory, this dash would be the letter d. The file we're looking at is `main.py`
- `rw-`: The first set of 3 characters after that inital dash are our own permissions. So the owner of this file can read and write ,but they cannot execute any commands on the python file.
- `rw-`: The group assigned to this file can read and write, but they can't execute commands on the file. They have the same permissions as the user.
- `r--`: All other users only have permission to read the contents of the file.

### Changing file permissions or ownership
You can either change ownership or change the permissions on a file 

#### Using Chown to change the owner of the file 
```bash
-rw-rw-r-- 1 ubuntu ubuntu 21 Oct 2 11:56 main.py
```
To change the user to Kevin and the group to KbizzzyyCorp you run these commands:
```bash

# Updates owner only
chown kevin <filename>

# Updates owner and gorup 
chown kevin:KbizzzyyCorp <filename>
```

#### Using Chmod (Change mode)
First let's understand the linuxing number system:
- r = 4
- w = 2
- x = 1

So a read gets 4, write gets 2, and a execute gets one. Now looking at an exmaple:

```
-rw-rw-r--
```
- Owner: Using the numbering system, this sums up to 6 since we only have read and write.
- Group: Also sums to 6.
- Others: Sums up to 4.

Overall these file permissions is a 664. If we want the owner and group to have execute permissions, we need to add x to the user and group sections. Then owner and group would be `rwx` which is 7 each, making this hypothetical situation 774. So we would do `chmod 774 main.py` to update the file permissions for `main.py`. As a result:
```
-rwxrwxr-- ubuntu ubuntu main.py
```


#### Common file permissions
- File baseline: 644
  - owner: Read and Write
  - Group and others: Read only
- Directory Baseline: 755
  - Owner: Read, write, and execute.
  - Group and others: Read and execute only; to clarify you want people to be able to execute so they can run linux commands still.
- Key Pair:
  - Owner: Owner can read the keys.
  - Group and others: No permissions at all, you don't want others being able to see the public or private keys, or any sort of secrets.



## Credits
- [Linux file permissions in 5 minutes - Travis Media](https://youtu.be/ngJG6Ix5FR4?si=x_gU1e4w-Lkyds4A)
