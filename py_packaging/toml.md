# Toml Files 

### What are toml files?
TOML (Tom's Obvious Minimal Language) is a data serialization language designed to be a minimalistic/readable config file format.

It's an iterative to other config file formats like YAML or JSON as it's designed to be more human-friendly and simpler. 

**NOTE:** For VsCode you need to download an extension that gives your IDE toml file support.

#### TOML maps unambiguously to a hash table
This means the structure of a TOML file directly translates int oa key-value format, which is also the core idea behind hash tables.
- **Keys** in TOML (e.g., title = "Hello") directly correspond to keys in a hash table.
- **Values** (e.g., numbers, strings, booleans) map cleanly as values in the hash table.
- **Tables (sections)** in TOML ([database]) become nested hash tables (dictionaries, objects, etc.).

For example, this TOML file:
```toml
[database]
host = "localhost"
port = 5432
```
Can be represented by this json object:
```json
{
  "database": {
    "host": "localhost",
    "port": 5432
  }
}
```
No ambiguity or complex parsing, but just simple mapping. There are a couple of reason why this is important includes:
- Easy parsing: Many programming languages should be able to parse TOML files with minimal effort.
- Readable/predictable: Very easy to visualize and understand the structure of a TOML and what you're actually writing in JSON object form.

However a disadvantage is that you may see this file as less concise.

### When do I use them?

TOML is great for defining app settings, environment variables, and dependencies. 

A popular Rust package manager called "Cargo" uses a toml file for containing project dependencies and build settings.

#### For Python: What is `pyproject.toml`
This is a config file used by python package managers, so you'd see this if you've worked on Python projects. In this toml file, you're limited to 3 possible tables:
- `[build-system]`: Essentially required. Allows us to declare which **build backend** to use and which other dependencies are needed to build the project.
- `[project]`: Most build backends use this table to specify your project's basic metadeta, such as dependencies, your name, etc.
- `[tool]`: Here you can specify 

## Credits 
- [Toml Files](https://learnxinyminutes.com/toml/#:~:text=TOML%20stands%20for%20Tom)
- [What is pyproject.toml? ](https://packaging.python.org/en/latest/guides/writing-pyproject-toml/)