
# Defining variables: Variables allow us to not repeat ourselves. As a result if you wanted to change the value, the compiler in this case, you just need to change 
# things in one place, rather than in many places.
# Variables and semantics:
# It's probably a little helpful to understand what the project is setting up.
# CC: Compiler we're using
# INCDIRS:
# OPT: Optimization flag, don't worry about it, it doesn't matter for the purpose of learning Make.
# CFLAGS: Variable containing all the flags we're going to use to run the project. Now if we ever have to modify the flags, we 
# can just do it in one place. 
# CFILES: A variable listing the source files that we want to compile
# OBJECTS: The name of the object files that we want to generate from those .c files
# BINARY: The name of the executable that's generated from making these object files work together.
CC = gcc
INCDIRS = -I.
OPT= -OO
CFLAGS= -Wall -Wextra -g $(INCDIRS) $(OPT)
CFILES = x.c y.c
OBJECTS = x.o y.o
BINARY = bin

# Each block is called a build rule. In every build rule, we have the target to be built (left), dependencies that it requires in order to build.
# Then below it will have commands, which we will see later. It's typical for "all" to be a build rule that runs everything or the main thing. 
# "To build everything, the binary must exist, and it must be different from the previous time we built it". One last thing is that when 
# you call make, without a specific build target, it will run the first rule it sees in the Makefile. By convention, we place our all rule
# as the first thing in the Makefile, and so that "all" target is built.
all: $(BINARY)

# There's a binary, and it depends on the object files to exist. So for this rule to execute, all object files need to be properly created
# "To build the binary, all objects must have been created". For this to work, we created a rule to create those object files. So after those 
# object files have been created, we can create the binary. This command could look like "gcc -o bin x.o y.o".
$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

# We can reason "To build an object file with a certain name, a corresponding C file must exist". We can generalize this using a wildcard (%), which is kind of like 
# a regular expression in make. This is the generic rule, "To build some_file.o, you need to have some_file.c, for any given file". Then we're using gcc more gcc special variables.
# The @ symbol is replaced with whatever is on the left side when this rule is being built, and the ^ is being replaced with whatever's on the right side when this rule is being built.
# So this rule is triggered by $(BINARY): $(OBJECTS) rule, and this rule is ran multiple times. Ran for every object file that we listed in OBJECTS. 
# Remember Make says, "I will only run this rule for a given C file, if the C file has changed from when we last ran this rule". So if the C file didn't change, it's not going to rebuild 
# the object file, and that's great.
%.o:%.c:
	$(CC) $(CFLAGS) -c -o $@ $^

# All make files typically have a "clean" rule which removes all of the generated project artifacts. You can think of this as 
# "To build 'clean', we don't need anything" so it just runs the commands for the rule.
clean:
	rm -rf $(BINARY) $(OBJECT)

# Insight: If you type make for the first time:
# 1. All rule wants binary
# 2. Binary needs object files 
# 3. For each object file, we need the respective C file. We have the respective C file, so generate the object file.
# 4. All object files are generated, the binary can be built and so we run the command to build the binary.
# 5. The binary now exists, so all can be built and run. All has no commands to run so we stop there.

# Motivation: If a given C file doesn't change, why would I need to compile it to get its object file? We don't need to. 
# If you run make after building the binary, and you didn't change anything, then the all rule won't run because the binary didn't change.
# If you delete the binary, then doing make will rebuild it. This is because, to build "all" we require "bin" and since we don't have bin, we need
# to create it. To build "binary" we need object files, and we already have those object files. The object files didn't change and they exist, so just 
# create that binary. Notice how we didn't go through the entire process of compiling the C files to object files, because the object files ALREADY EXISTED.
# Also it's not going to run the compile rule for an object file, if the coresponding C file didn't change.

# Motivation 2: Here's the idea lsituation. If you were to modify y.c, changing the yfn(), you can reason that the code for this has changed, and so the object files has to be compiled. 
# Then since an object file has changed, we hope that we re-run the command to create the binary, since one of its object files has changed. And yes, this works as required.


# Big Problem: What if you change the logic within z.h, changing the constant. You know that y.c must also change because it imports z.h. 
# I mean if you look closely we have no rule or thing for z.h. While this Makefile is great, it'll only detect when the contents of the .C files
# have changed, but it won't take into account that the .h files affect some .c files.