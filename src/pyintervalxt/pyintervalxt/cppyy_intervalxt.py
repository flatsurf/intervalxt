######################################################################
#  This file is part of intervalxt.
#
#        Copyright (C) 2019 Vincent Delecroix
#        Copyright (C) 2019 Julian Rüth
#
#  intervalxt is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  intervalxt is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with intervalxt. If not, see <https://www.gnu.org/licenses/>.
#####################################################################

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

def IET(lengths, permutation):
    from cppyy.gbl import std
    Label = intervalxt.Label[int]
    return intervalxt.IntervalExchangeTransformation[Label]([Label(l) for l in lengths], permutation);

intervalxt.IET = IET
