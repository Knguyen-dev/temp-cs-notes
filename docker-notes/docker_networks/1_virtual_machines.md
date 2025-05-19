### What is a virtual machine?

An application that behaves like a computer, running on your local machine. Our real physical computer has an OS, CPu, RAM, storage, GPU, etc. Let's say you wanted to leanr how to use the Linux operating system. Instead of buying a new computer, we can just simulate one. This is all made possible by the hypervisor, called virtual box. Its job is to create fake/simulated computers inside your local machine. This fake computer has simulated CPU, RAM, etc. More formally, the local machine's OS is referred to as the "Host OS". Then the OSes of your simulated computers are called "Guest OS". These are referred to as type 2 hypervisors, and they're built on top of windows. A type 1 hypervisor is what you see in enterprise apps or companies. 

### Why to use virtual machines?
- It's a safe way to learn hacking. 
- It's a safe way to learn other operating systems
- It's a safe way to try and experiment with things. Want to see if changing some configurations is going to break your computer? Do it on a virtual machine. Want to see if a script is malware? A virtual machine. You can do anything to a virtual machine because at the end of the day, it's not going to affect your real computer.

### How to setup a virtual machine

#### Download and computer setup
1. Go to bios (restart and kee pressing f12.), cpu config, enable virtualization technology (intel) or amd V (for amd). Then save and exit from the bios
2. Download the OS that you want to use. Here we'll use kali linux 64-bit installer. Also download the latest version of ubuntu (ubuntu desktop). We're downloading `.iso` files, which is the norm for downloading operating systems.
3. Download the virtual box hypervisor from oracle. Downlaod fro the OS that your local machine is on.
4. Download VirtualBox and the extension pack. The reason for the latter is that it gives virtualbox more functionality, to do things with usbs and other things.

#### Create new VM

## Credits
[Virtual Machine - NetworkChuck](https://youtu.be/wX75Z-4MEoM?si=uVOnevurl0jvEDD9)
