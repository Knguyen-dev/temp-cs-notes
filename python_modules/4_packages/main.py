
'''
We import the package. It will use the 
PathFinder to locate 'pkg1' by looking through all
paths in 'sys.path'. The first entry in 'sys.path' will be 
the current directory, allowing python to locate the package

We import a package using the directory name, and the code inside the __init__ file
is created automatically. This turns into a compiled code object of the module object 
that will be created when you import a package. See that __pycache__, that's the compiled
code for the pkg1 module object.

However now we have mod1, a module inside our pkg1 package. We can't just access the module 
like pkg1.mod1. This would throw an error saying that the module object 'pkg1' has no attribute mod1.
You need to do import pkg1.mod1. This will import the package, but also import that module. You can verify 
that 'pkg1' and 'pkg1.mod1' are both in sys.modules as well.
'''

# Imports just pkg1
import pkg1

# Imports that mod1 directory as well
import pkg1.mod1
 
pkg1.say_hi()
pkg1.mod1.say_hi_fancy() 