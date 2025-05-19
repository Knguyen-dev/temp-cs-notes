This has implications on how you call python modules from the cmd, how you structure your imports, how you can package it for distribution You have to do a lot of navigating on the cmd, to call python from the right place or else your imports won't work. Being able to call a python util project from anywhere can get hard, as there's many ways to go about this. By using a `pyproject.toml` and an editable install makes developing bigger.


## Review on Python Packaging
- Clicking on `main.py` on the bottom right we see the python interpreter that's being used. This is our global/system Python, rather than the Python version used for the project.
- `pip list`: Shows all packages on the syste m, rather than the packages for a given project.

Click on `main.py` click the bottom in the bottom right and create a venv virtual environment and clone the global python that you were using, or whatever python you want.

A module is just a python file. A package is a directory that contains python modules/files.
```Python
from utils.add import add_numbers
```

### Learning `__init__.py`
Originally you needed `__init__.py` in order to have a directory be a package. Now that's not necessary but it still has important uses.

#### 1. Giving you cleaner imports

```Python

# Without `__init__.py` you'd have 
from mypackage.module1 import function1
from mypackage.module2 import function2

# With __init__.py you can shorten the imports; assuming function1 and function2 are in the __init__.py module
from mypackage import function1, function2
```

#### 2. Running initialization code
Remember when a module is imported in Python, the code from that module is automatically run. You don't typically see this if the module you're importing is just function definitions. However if your module has a print statement, then importing that module will run it and you'll see that print statement.

Anyways, sometimes a package needs to setup some things when imported, such as loading config files, initializing database connection, etc. So we'd create an `__init__.py` file to handle take care of those steps for us. If we're talking about how this is typically used in APIs and web frameworks, `__init__.py` is often used to create instances of the application/server.

```bash
myapp/
│── __init__.py  # Initializes the other 4 packages, keeping things modular and organized.
│── config.py    # Config settings
│── routes.py    # API routes
│── models.py    # Database models
│── utils.py     # Utility functions
```


#### 3. Controlling `import *` Behavior
By doing `import *`, we're importing everything from a package. All modules, any classes, functions, and variables! This can be dangerous and kind of messy as a lot of people prefer you just import what you need so that hte code is more readable. Anyways, `__init__.py` allows us to control what gets imported when a developer uses wildcard on your package.

Here we make sure that when using the wildcard on `mypackage`, they will get `function1` and `function2`, rather than giving another file access to everything:
```Python
# mypackage/__init__.py
__all__ = ["function1", "function2"]
from .module1 import function1
from .module2 import function2
```


---
### Learning `pyproject.toml`

#### Before Era: Using `requirements.txt` and `setup.py`
A `requirements.txt` is a simple list containing the packages that your project needs. Then you can run an installation command to install all packages listed in the file. 
```txt
flask
requests>=2.25.1
numpy==1.21.0
```
Then run it with `pip install -r requirements.txt`. It doesn't package the project or define how it's structured, you'll use `setup.py` for that. This file defines:
  - Project name, version, author, and description
  - Dependencies: An alternative to requirements.txt 
  - Metadata: Project homepaeg, license, etc.
  - Entry points for command line tools if needed.
```Python
from setuptools import setup, find_packages

setup(
    name="mypackage",
    version="0.1.0",
    author="Your Name",
    description="A sample Python package",
    packages=find_packages(),
    install_requires=[
        "requests>=2.25.1",
        "numpy==1.21.0"
    ],
)
```
#### Modern Era: Using `pyproject.toml`

Modern Python projects use `pyproject.toml`. Now we think that our `utils` package contains some pretty useful software and we want to distribute it so that it can be used in other places be it locally or through the pypi repository. You'd first create a `pyproject.toml` file. 

- `[build-system]`: Tells Python packaging tools the things needed to build the project. So `requires` will list all dependencies for building the project (not for running it). Then `build-backend` specifies the tool that handles the packaging process.
  a. setuptools: Main library for packaging and distributing Python projects. Helps define package structure (files, metadaa, dependencies)
  b. Generates `.whl`files. These are pre-built distributions of a package that install faster than raw source distributions. So instead of compiling source code in every install, hweels make installation faster.
  c. A **build backend** is what actually handles the "packaging" process. This does some magic that makes it possible for our p1 project to be a python package.

So your `pyproject.toml` file is kind of like your `package.json` that you have in JavaScript. So in JavaScript we'd use the `npm` package manager and your `package.json` side by side. Or maybe you use yarn. However what's different is that you have several package managers that you can use. 

And one thing to know, while you can use a `pyproject.toml` with pip, the amount of functionalithy is limited. With pip you're not going to be able to replace that `requirements.txt` as the dependencies array in the .toml file won't work.

However with something like `poetry` package manager, you're able to get the full functionality of toml.

## UV Package Manager

## Credits
- [Anatomy of python packages](https://youtu.be/m2EAQk4Qlew?si=33v_ckHdQR0mj0rh)
- [Pyproject.toml: The modern Python Project definition file](https://youtu.be/QMY-OkckDwo?si=oYgiecGk3gOPAh5v)
- [Packaging your python code with pyproject.toml](https://www.youtube.com/watch?v=v6tALyc4C10)
- [uv - python package and project management](https://www.youtube.com/watch?v=igWlYl3asKw)