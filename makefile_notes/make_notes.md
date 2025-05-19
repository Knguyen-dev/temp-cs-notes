# Makefile

### What are build systems
Organize the steps to build your code. Automate the linking and compilation of your source code in order to get those final executables. You shouldn't really be typing out the commands every time. In a team of many developers, everyone needs to be able to build the code in the same way. 
- **Organize/Automate:** Automate the compilation and linking of source files into executables
- **Rebuild necessary files:** Recompile only the changed portion of the source code, and the portions dependent on the changed code.
- **Make maintenance better:** The build sysetm should be a programming language that allows us to avoid redundant code.



### Additional stuff 
- You can run any shell script in your Makefile. Docker commands, git commands, etc.
- When running commands e.g. "git status", in the terminal it'll print out the command you submitted, and the results of that command. That's pretty standard. However if you don't like seeing the command being run getting outputted, then add the @ symbol in front of it when running it in the Makefile. This can be really useful as it reduces clutter in your terminal.

### Types of Assignment 
#### Simple Assigment
The `:=` (colon-equals) notation is called simple variable assignment. It is different from `=` (recursive assignment). When we use `:=`, the right hand side is evaluated immediately and assigned to the variable. As a result the value won't change if the environment or files change later. 

```
BACKEND_SRC := $(shell find backend -type f -name '*.go')
```
- We run the find command at the moment the line is executed. Then the result (list of `.go` files in `backend/`) is assigned once to `BACKEND_SRC`.
- Even if new files are added later, `BACKEND_SRC` won't update.

#### When to use each `:=` vs. `=`
- Use `:=` when you want to evaluate once and store the result.
- Use `=` when you want it to be recomputed each time it's used.

## Credits
- [Learn make in 60 seconds - Jacob Sorber](https://www.youtube.com/watch?v=a8mPKBxQ9No)
- [Makefiles: 95% of what you need to know - Gabriel Parmer](https://youtu.be/DtGrdB8wQ_8?si=Pf2l8HR-h-knd8RU)