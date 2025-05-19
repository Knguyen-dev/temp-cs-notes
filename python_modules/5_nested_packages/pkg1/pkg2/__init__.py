

'''
You can also use relative imports. It's relative 
to the module's position in file structure. As well as this 
you can only use relative imports with the from syntax. Finally 
you can only use relative imports within packages.

For example, while pkg1 and main.py are in the same directory, you can't 
use relative import to import pkg1 into main because your root isn't a package.
'''

# import pkg1.pkg2.mod2
from . import mod2

# import pkg1.mod1
from .. import mod1


## Credits

[Pythons import](https://www.youtube.com/watch?v=QCSz0j8tGmI&t=143s)