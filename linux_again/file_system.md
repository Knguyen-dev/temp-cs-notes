# Linux File System/Structure

### File Structure 

![](https://linuxhandbook.com/content/images/2020/06/linux-directory-structure.png)
- Linux allows case sensitive names. So you could have files "File" and "file" in the same directory, and since they have different casing in their names, their names don't conflict.

Your Linux file system may look something like above. This layout is outlined in the FHS standard, which defines the structure and file layout. However, not all distributions of Linux follow the same format, and over the years things have changed. However there are some main ideas that stay the same.


- `bin`: Stands for binaries, programs, applications. Functions and code for running linux commands like Cat or ls are stored here. 
- `sbin`: System binaries that a system administrator would use. A standard user wouldn't have access to these. So you'd have to run Linux in single user mode, allowing you to have permissions to do repairs
- `boot`: Contains everything your OS needs to boot up and start. Do not modify this folder.
- `dev`: Where your devices live. With Unix and Linux, everything is represented as a file. You should rarely be here.
-  `/etc`: Where all your system wide configurations are stored. If you're looking for the settings of a system wide application, it's probably here.
- `lib`, `lib32`, and `lib64`: Thi is where the libraries are stored. So libraries have extra code that is needed by your application, and the sofware in `bin` and `sbin` make use of these libraries.
- `mnt`: The mount folder is where you'll find any mounted drives such as floppy disks, usb sticks, etc. Nowadays most distributions mount your stuff in the `media` directory. In general, if you're mounting things manually, use the `mnt` directory, and let the OS manage the `media` directory
- `opt`: Optional folder where manually installed software would reside. 
- `proc`: You'll find sudo files, files that contain information about system processes. So if you know the process id is 2344, you'd navigate to `/proc/2344` to see the files and information for that process. Of course these files in this directory aren't actually files, but the OS translates that information into file form. Another 
- `root`: Root user's home folder. 
- `run`: Fairly new. Everything in this file exists in ram.
- `snap`: Where snap packages are stored. These are self-contained applications that run differently from regular packages and applications.
- `srv`: Service directory where service data is stored. For most it's empty, but if you run a web server, you'll store the files that'll be accessed by external users here. 
- `sys`: An old directory. A way to interact with the kernel. It's created everytime the system starts up, so you wouldn't store anything here and nothing would be installed here.
- `tmp`: A temporary directory where files are temporarily stored by applications during a session. For example if a word processor is saving sessions of your work, if you crash, it may look in this directory to see if there are any copies that can be used to restore your work.
- `usr`: This is the "user application space", where applications will be installed that are used by the user. As a opposed to the stuff in the `bin` directory that's used by the system and system admin. 
- `var`: Variable directory, contains data that's expected to grow in size as you use the system. An example is like a log

- `home`: Where you store your personal files and documents. When you enter this folder, you'll see that there are separate directories for each user, allowing each user to have their own home directory. Each user cna only access their won, unless they're using admin permissiosn. It can also store application settings via hidden files (files with . prepended). 


## Credits
- [Linux File System/Structure Explained - DorianDotSlash](https://www.youtube.com/watch?v=HbgzrKJvDRw)