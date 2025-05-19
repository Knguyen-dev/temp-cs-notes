
### 1. Module Object
You often refer to the python file. However it's a little deeper than that. In the file `main.py`, note that we import `module1`. With the `.py` that's the file name, but without the `.py` that's the module name. When you import a module, a module object is created in memory, just like a variable. So running `main.py` will prove that previous sentence. So a **module** is just an object in python that contains the classes, variables, functions, and first class things. All those things that you declare in the global scope are available as module object attributes after the import. So `my_func` is going to be an attribute on the module object representing module1. You can import with aliases as well:
```python
import module1
import module1 as myMod
```
Not all modules that you import are written in python. Some are written in compiled languages like C.

### 2. Finder, loaders, sys module
Python exposes its import system through the `meta_path` attribute of the `sys` module. `meta_path`: A list of objects. There are two main things. A **finder**, which locates a module, and a **loader** is a thing that loads that module. They're both classes basically, and the objects in meta_path are all finders. Those implement find. But some objects do finding and loading, and that guy is called an **importer**.

Built-in modules are modules that are compiled into the Python interpreter. The expression `sys.builtin_module_names` will return a tuple of all built-in module names. 

Since an importer is a finder, we can call the `find_spec` method on it and pass a string `itertools` to it. It should return a `ModuleSpec` object, which has info on which loader to use if you want to load the Itertools module. A PathFinder looks for python modules under different file system paths. 

Let's see the process again:
```python
import sys, socket, module1
```
Check global cache of already loaded modules, which is just a dictionary of module names as keys and module objects as values. If in cache, return it.
Else python uses finders and loaders to get the module. The loader creates the module object in memory and then it's added to the global cache. And the in memory module name is what you imported it as, so this is the namespace. Now `module1` is a reference to the module object. If you delete the module name, it deletes the reference from the namespace, but the object still exists. Here you can see that we can implement our own importing logic.


### 3. Importing Variants 

```Python
import socket
```
Again the 'socket' module name with the reference to the in-memory module object is now in the global cache. As well as this, functions and other stuff that socket defined are also loaded in memory, since you imported them. Though don't worry it's not like you imported the entire library, but everything was loaded.
If you import it as `my_socket` the namespace will be `my_socket`, so you can still access those imports, but only under the `my_socket` namespace. 
```Python
import socket as my_socket 

# Works
my_socket.cool_socket_function()

# No
socket.cool_socket_function
```
Again regardless of if you're importing socket, or just a single function from socket, don't be fooled in thinking that you're only partially loading the module. The reason that people don't use wild card is because it 'masks' the module name that the import comes from. Here's a picture:

```python
# This happens
import socket
socket.my_cool_socket_function()

# Now let's use a different import
# This works, but in big projects, you don't want imports from libraries to share the names as your own functions.
# And also it makes it harder to know where the function even came from.
from socket import *
my_cool_socket_function()
```
Again using regular imports or using from keyword doesn't give you a performance boost or save memory. In either case, python compiles and executes the entire module that's being imported.

Now let's talk about `__name__ == "main"`. If you run `my_module` directly, then the function prints out "__main__ says hello". This indicates that the module or python file was run directly, as opposed to being run due to being imported.

If you run `another_module.py`, it imports `my_module`, which causes all the code in `my_module` runs. So the function will output "my_module says hello", which indicates that `my_module` wasn't run directly. 

### 4. Packages 
A way for structuring python's namespaces. Akin to how modules are like code in a file, packages are just modules but as a directory. When you import a package, it still becomes a module object, with the only difference being it has an attribute `__path__`, which is the location of where its code is. 

We have a `__init__.py` in our `pkg1` directory. This is called a regular package, it's just a directory with one `__init__` file in it. The directory's name is the package's name.

### 5. Nested Packages
```python

import pkg1.pkg2.mod2
pkg1.pkg2.mod2.say_hi()
# 1. Imports pkg1
# 2. Imports pkg2
# 3. Then imports mod2, and runs it
# However you probably don't want to call it this way
import pkg1.pkg2.mod2 as mod 

# This is a lot shorter and better
mod.say_hi()
```