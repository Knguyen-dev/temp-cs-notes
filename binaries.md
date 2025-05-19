# Binaries and Executables

### What are they? What's the difference?
- **Binaries:** Name for a file that just contains machine code. It's a broad term that includes executables (`.exe`), libraries (`.so`, `.dll`), object files (`.o`, `.obj`), etc. Basically anything compiled from source code into a format the computer understands is a binary.
- **Executables:** A type of binary that you can run directly. It's a binary that contains entry points (like a `main()` function). Examples are `myapp.exe` on Windows or `program.out` and `program` on Unix-like systems.

### How to tell them apart?
```bash

# Run this command on the binary to get information about it. There are two scenarios:
# - If it says something like "ELF 64-bit LSB executable" then it's an executable.
# - If it says "relocatable" or "shared object", then it's a binary, but not an executable. Meaning we can't run it directly, but rather it's used to support other software.
file <yourFile>
```