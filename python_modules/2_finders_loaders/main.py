import sys
import types

module_name = "my_custom_module"
module_file = "/workspaces/module1.py"
mod = types.ModuleType(module_name)
mod.__file__ = module_file

# Set reference in global cache
sys.modules[module_name] = mod

# Read the source code from the file, compile and execute it
with open(module_file, "r") as code_file:
  source_code = code_file.read()
  code = compile(
    source_code,
    filename=module_file,
    mode="exec" 
  )

  exec(code, mod.__dict__)
  mod.say_hello()