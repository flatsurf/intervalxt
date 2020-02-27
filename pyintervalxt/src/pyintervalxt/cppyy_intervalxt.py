######################################################################
#  This file is part of intervalxt.
#
#        Copyright (C) 2019-2020 Vincent Delecroix
#        Copyright (C) 2019-2020 Julian Rüth
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

from cppyythonizations.pickling.cereal import enable_cereal

# Importing cysignals after cppyy gives us proper stack traces on segfaults
# whereas cppyy otherwise only reports "segmentation violation" (which is
# probably what cling provides.)
import os
if os.environ.get('PYINTERVALXT_CYSIGNALS', True):
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

# Singleton object that makes sure that all Lengths that we have seen during
# this cppyy session are known to cereal. This might break when lengths had
# been loaded that are not available at unpickle time (even though they are
# not needed) but there is nothing we can do about this unfortunately.
class LengthRegistrar:
    REGISTERED_LENGTHS = set()

    def __init__(self):
        self.known_lengths = set()

    def track(self, name):
        self.known_lengths.add(name)

    def __call__(self):
        for length in self.known_lengths:
            if length not in LengthRegistrar.REGISTERED_LENGTHS:
                cppyy.cppdef('LIBINTERVALXT_ERASED_REGISTER((::intervalxt::LengthsSerialization), (%s));'%(length,))
            LengthRegistrar.REGISTERED_LENGTHS.add(length)

lengthsRegistrar = LengthRegistrar()

def enable_cereal_(proxy, name):
    # Additional registration code is necessary for erased types such as
    # Lengths. There seems to be no sane way to check this at runtime with
    # cppyy. std::is_assignable_v lets too many types through that are
    # actually not Lengths and performing an actual assignment lets cppyy
    # crash in many cases.
    if proxy.__cpp_name__.startswith('intervalxt::sample::Lengths<'):
        lengthsRegistrar.track(proxy.__cpp_name__)

    headers = ["intervalxt/cereal.hpp", "intervalxt/sample/cereal.hpp", lengthsRegistrar]

    enable_cereal(proxy, name, headers)

cppyy.py.add_pythonization(enable_cereal_, "intervalxt")
cppyy.py.add_pythonization(enable_cereal_, "intervalxt::sample")

# Set EXTRA_CLING_ARGS="-I /usr/include" or wherever intervalxt/cppyy.hpp can
# be resolved if the following line fails to find the header file.
cppyy.include("intervalxt/cppyy.hpp")

from cppyy.gbl import intervalxt

def IET(lengths, permutation):
    from cppyy.gbl import std
    lengths = intervalxt.sample.Lengths[int](lengths)
    return intervalxt.makeIET(lengths, permutation)

intervalxt.IET = IET
