
BINARY = bin
CODEDIRS = . lib
INCDIRS = . ./include/
CC = gcc
OPT = -O0

# "When you build the program, generate dependencies in a way that Make will understand. So now when I modify the .h, we will also rebuild the .c file"
DEPFLAGS = -MP -MD

# Create a variable D for each iteration, iterate through all include directories.
# For a given directory generate a -I<include_directory_name>, so it's prepending -I to each include directory.
CFLAGS = -Wall -Wextra -g $(foreach D, $(INCDIRS), -I$(D)) $(OPT) $(DEPFLAGS)

# Iterate through all directories in our CODEDIRS, e.g. "." and "lib". So we're looking at the current directory and "lib" directory for C files.
# For a given directory D, within it look for any C source code files, and that's what we're outputting. So we should get a list of paths to C files.
CFILES = $(foreach D, $(CODEDIRS), $(wildcard $(D)/*.c))

# For every c file, take that c file name, and convert it to an object file name. That's the gist of things, and we're doing the same 
# idea when creating the dependency files (.d)
OBJECTS = $(patsubst %.c,%.o,$(CFILES))
DEPFILES = $(patsubst %.c,%.d,$(CFILES))

# Other than that, the rules are the same.
all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

%.o:%.c:
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -rf $(BINARY) $(OBJECT)