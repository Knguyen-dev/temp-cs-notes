

### Setup UV
```Bash
# Using powershell install UV globally; that's what our project uses; 
powershell -ExecutionPolicy ByPass -c "irm https://astral.sh/uv/install.ps1 | iex"

# Verify your install
uv 

# Initialize project using UV, for python version 3.13
uv init --python 3.13 my-first-uv


```


### Coding in FastAPI
After you get up some routes in FastAPI, run the project on local host port 8000:
```
uv run uvicorn main:app --port 8000 --reload
```

### Setup required dependencies
```bash
uv add fastapi sqlalchemy alembic uvicorn
```
Download fastapi for the api, sqlalchemy for interacting with a database server and the database, and then alembic for handling sql migrations. Uvicorn is the typical ASGI for FastAPI, so we download it. Note that when we do this, uv will automatically create a virtual environment and install these things in the virtual environment. This is really good to keep our project dependencies isolated from the dependencies on our system. As a result, the only thing that's global should be UV itself, and that's normal. Basically like Npm. This will also generate a `uv.lock` which will show the dependencies, transitive dependencies, and metadata. The `pyproject.toml` file makes it more concise by showing direct project dependencies. Then doing `uv run <filepath>` will activate that virtual environment as well.

#### What if we git cloned this project?
Assuming we just git cloned this project, we will have the `uv.lock` file but not the virtual environment folder or the packages that are in that folder. To solve this, just do:
```
uv sync
```
This creates that venv folder and sets your dependencies to be the same as those described in the `uv.lock` file.

#### Installing optional dependencies
These are dependencies that are nice to have, but not required for the package/project to work.:

```

uv add <dependency-name> --optional <group-name for the optional dependency.>
```

#### Installing dev dependencies
```
uv add <package-name> --dev
```

#### Removing depend
```bash
uv remove <package-name>
```

#### Dependency tree
```
uv tree
```

---
### Running script files
Create a script in the parent directory of the project.

```bash
uv run --with requests api_request.py
```
Here we're running a python file and include the request library in that file. This iUV looks in the CWD and it will look for the `venv` folder to find the libraries. So 

### Other stuff
But yeah apparently you can just install ruff, black, and pytest. The first two are a surprise as I didn't think that's the formatters and linters and whatnot.

### Creating a virtual env

```bash
# Create the virtual environmnet
uv venv

# And activate it somehow
source .venv/bin/activate
```

### Using Tools
Notable python libraries like ruff, black, etc. are referred to as tools. UV gives these special interfaces and commands to use these.

From observations, uvx installs tools temporarily, uses them, and then uninstalls them. So if you want to use something but not have it installed. While uv installs it on your venv for real. uv is probably used a lot more.

```
uvx ruff check main.py
uv run ruff check main.py
uvx ruff format 
```
So it's a linter and formatter. 

Then you have HTTPx and pytest which you use to test your endpoints

uv run pytest


## Credits

[UV Python package manager tutorial - BugBytes](https://www.youtube.com/watch?v=igWlYl3asKw)

[Tools in UV - BugBytes](https://www.youtube.com/watch?v=ph_XLky5pRs)