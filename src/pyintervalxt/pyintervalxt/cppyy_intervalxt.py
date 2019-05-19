import cppyy

# Importing cysignals after cppyy gives us proper stack traces on segfaults
# whereas cppyy otherwise only reports "segmentation violation" (which is
# probably what cling provides.)
import os
if os.environ.get('PYINTERVALXTL_CYSIGNALS', True):
    try:
        import cysignals
    except ModuleNotFoundError:
        pass

def make_iterable(proxy, name):
    if hasattr(proxy, 'begin') and hasattr(proxy, 'end'):
        if not hasattr(proxy, '__iter__'):
            def iter(self):
                i = self.begin()
                while i != self.end():
                    yield i.__deref__()
                    i.__preinc__()

            proxy.__iter__ = iter

cppyy.py.add_pythonization(make_iterable, "intervalxt")

def pretty_print(proxy, name):
    proxy.__repr__ = proxy.__str__

cppyy.py.add_pythonization(pretty_print, "intervalxt")

for path in os.environ.get('PYINTERVALXT_INCLUDE','').split(':'):
    if path: cppyy.add_include_path(path)

cppyy.include("intervalxt/cppyy.hpp")

from cppyy.gbl import intervalxt
