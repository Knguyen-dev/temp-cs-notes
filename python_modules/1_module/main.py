import module1

# You can use aliases as well
# import module1 as myMod

print(dir(module1))

print(f"Using dict: {module1.__dict__['x']}")
print(f"Using dot notation: {module1.x}")

# You can just call things like this
module1.my_func()