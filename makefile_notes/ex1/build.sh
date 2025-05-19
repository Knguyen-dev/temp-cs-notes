#!/bin/sh

# Example 1:
# When being asked to build a project, the first thing that people probably do is 
# creating a shell script that runs their program. So here we're executing gcc
# and the hashbang at the top just tells the computer to run this thing through the shell.

# Premise: 
# So z.h defines a constant and exposes a function yfn(), a prototype, that we hope to implement later.
# We import z.h to x.c and y.c. For y.c we implement that yfn() function and use the constant defined in z.h
# In x.c, we use the API that was exposed, making use of yfn() but not knowing how it's implemented.'

# - If x.c or y.c are changed, you will have to rebuild the respective .c file and create the new executable. You not only need to create their new .o files 
# and then use those .o files to create the binary, compiling together the .o files into an executable.
# - If you change z.h, you'll recompile x.c and y.c since it imports that header file. 

# Motivation:
# We can reason that this has no real intelligence, it doesn't try to prevent compiling things that haven't changed. 
# Here's our ideal behavior: If we modify y.c, we'll need to recompile it, however we shouldn't need to recompile x.c because x.c does not import  y.c. x.c 
# is accessing yfn() through the header function, it shouldn't care about its implementation. Our x.o file should be exactly identical to what it was before.
# What if you need to add optimization flags? You'll need to manually add them to both lines manually. Now what if you had to change things in 10 different places manually?
# That's a high risk of error, and of course no one does that because we have build systems.
gcc -Wall -Wextra -I. -g -c x.c
gcc -Wall -Wextra -I. -g -c y.c 
gcc y.o x.o -o bin