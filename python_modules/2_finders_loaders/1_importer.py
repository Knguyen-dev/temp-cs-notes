import sys
'''
[
  <_distutils_hack.DistutilsMetaFinder object at 0x000001f1cfc50190>, 
  <class '_frozen_importlib.BuiltinImporter'>, 
  <class '_frozen_importlib.FrozenImporter'>, 
  <class '_frozen_importlib_external.PathFinder'>
]
'''
print(sys.meta_path)

built_in_importer = sys.meta_path[1]
module_spec_object = built_in_importer.find_spec("itertools")
itertools_module = module_spec_object.loader.load_module("itertools")

path_finder_object = sys.meta_path[-1]
module_spec_object = path_finder_object.find_spec("socket")
socket_module = module_spec_object.loader.load_module("socket")

