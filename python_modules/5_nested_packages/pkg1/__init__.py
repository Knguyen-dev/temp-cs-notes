
'''
Using absolute import here, our outermost package. The execution entrypoint 
is the main.py file. Then pkg1 is at the same level at main.py, which is why the absolute 
imports start here.

Now you should be able to do import pkg1 to be able to 
access pkg1.pkg1.mod2.say_hi(). This is because:
  1. We import pkg1. This runs __init__, and imports pkg2, 
  which run's pkg2's __init__ file. We're able to access it's stuff too.

'''

import pkg1.pkg2.mod2