import subprocess 


# Dir command is built into shell, so on windows
# you'll have to do shell=True. Now you could also 
# add more arguments to your command. Without the shell argument, you would need to run the command
# by passing an array of arguments. 
# With capture_output, instead of stuff being outputted to consol, it's being captured to the 
# p1.stdout. By default this is captured as bytes.
subprocess.run("dir")
p1 = subprocess.run(['ls', '-la'], capture_output=True, text=True)

# The standard out is where our output data usually goes


# The arguments passed: p1.args
# Return code: p1.returncode, 0 means successful since zero errors.
# p1.stdout: 

# You could decode things if you didn't already set things for text
# If you set text=True
p1.stdout.decode()