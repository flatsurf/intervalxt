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

import re
import cppyy

from cppyythonizations.pickling.cereal import enable_cereal
from cppyythonizations.util import filtered

# Importing cysignals after cppyy gives us proper stack traces on segfaults
# whereas cppyy otherwise only reports "segmentation violation" (which is
# probably what cling provides.)
import os
if os.environ.get('PYINTERVALXT_CYSIGNALS', True):
    try:
        import cysignals
    except ModuleNotFoundError:
        pass

def pretty_print(proxy, name):
    proxy.__repr__ = proxy.__str__

cppyy.py.add_pythonization(pretty_print, "intervalxt")

# Singleton object that makes sure that all Lengths that we have seen during
# this cppyy session are known to cereal. This might break when unrelated
# lengths types had been loaded at pickle time that are not available at
# unpickle time (even though they are not needed) but there is nothing we can
# do about this unfortunately.
class LengthRegistrar:
    REGISTERED_LENGTHS = set()

    def __init__(self):
        self.known_lengths = set()

    def track(self, name, headers):
        self.known_lengths.add((name, tuple(headers)))

    def __call__(self):
        for length, headers in self.known_lengths:
            if length not in LengthRegistrar.REGISTERED_LENGTHS:
                for header in headers:
                    cppyy.include(header)

                cppyy.cppdef('LIBINTERVALXT_ERASED_REGISTER((::intervalxt::Lengths), (%s));'%(length,))
            LengthRegistrar.REGISTERED_LENGTHS.add(length)

lengthsRegistrar = LengthRegistrar()

def enable_cereal_(proxy, name):
    headers = ["intervalxt/cereal.hpp", "intervalxt/sample/cereal.hpp", lengthsRegistrar]

    enable_cereal(proxy, name, headers)

cppyy.py.add_pythonization(enable_cereal_, "intervalxt")
cppyy.py.add_pythonization(enable_cereal_, "intervalxt::sample")
cppyy.py.add_pythonization(enable_cereal_, "intervalxt::cppyy")

def enable_label_printing(proxy, name):
    r"""
    Make labels print as "a", "b", "c" and not only as their memory address.

    Labels do not have an intrinsic name. They only get a name in libintervalxt
    once they are associated with a Lengths object.

    To make them print nicely, we try to make sure that any code path tha
    creates a label attaches the correct Lengths object to it and use it here
    to print them nicely.
    """
    printer = None
    def pretty_print(self):
        if hasattr(self, "lengths"):
            return self.lengths.render(self)
        return printer(self)
    printer = proxy.__repr__
    proxy.__repr__ = pretty_print
    proxy.__str__ = pretty_print

def register_lengths(labels, lengths):
    r"""
    Helper function for enable_label_printing to actually register Lengths with
    each label.
    """
    labels = list(labels)
    for l in labels:
        l.lengths = lengths
    return labels

def enable_register_lengths_iet(proxy, name):
    r"""
    Make sure that Label knows which Lengths it belongs to so it can print
    nicely in Python.
    """
    top = proxy.top
    proxy.top = lambda self: register_lengths(top(self), self.lengths)

    bottom = proxy.bottom
    proxy.bottom = lambda self: register_lengths(bottom(self), self.lengths)

def enable_register_lengths(proxy, name):
    r"""
    Make sure that Label knows which Lengths it belongs to so it can print
    nicely in Python.
    """
    labels = proxy.labels
    proxy.labels = lambda self: register_lengths(labels(self), self)

cppyy.py.add_pythonization(filtered("Label")(enable_label_printing), "intervalxt")
cppyy.py.add_pythonization(filtered("IntervalExchangeTransformation")(enable_register_lengths_iet), "intervalxt")
cppyy.py.add_pythonization(filtered(re.compile("Lengths<.*>"))(enable_register_lengths), "intervalxt::cppyy")

# Set EXTRA_CLING_ARGS="-I /usr/include" or wherever intervalxt/cppyy.hpp can
# be resolved if the following line fails to find the header file.
cppyy.include("intervalxt/cppyy.hpp")

from cppyy.gbl import intervalxt

def Lengths(lengths, headers=None):
    if len(lengths) == 0:
        raise ValueError("lengths must be non-empty")

    lengths = cppyy.gbl.std.vector(lengths)

    # We cannot ask for lengths.value_type, due to
    # https://bitbucket.org/wlav/cppyy/issues/269/typedef-lookup-fails-for-array-type
    if headers is None:
        if type(lengths) == cppyy.gbl.std.vector["int"]:
            headers = ["intervalxt/sample/integer_coefficients.hpp", "intervalxt/sample/integer_floor_division.hpp"]
        elif type(lengths) == cppyy.gbl.std.vector["mpz_class"]:
            headers = ["intervalxt/sample/mpz_coefficients.hpp", "intervalxt/sample/mpz_floor_division.hpp"]
        elif type(lengths) == cppyy.gbl.std.vector["mpq_class"]:
            headers = ["intervalxt/sample/mpq_coefficients.hpp", "intervalxt/sample/mpq_floor_division.hpp"]
        elif type(lengths) == cppyy.gbl.std.vector["eantic::renf_elem_class"]:
            headers = ["intervalxt/sample/renf_elem_coefficients.hpp", "intervalxt/sample/renf_elem_floor_division.hpp"]
        elif type(lengths) in [cppyy.gbl.std.vector["exactreal::Element<exactreal::IntegerRing>"], cppyy.gbl.std.vector["exactreal::Element<exactreal::IntegerRing>"], cppyy.gbl.std.vector["exactreal::Element<exactreal::IntegerRing>"]]:
            headers = ["intervalxt/sample/element_coefficients.hpp", "intervalxt/sample/element_floor_division.hpp"]
        else:
            raise TypeError("unknown length type %s; you need to specify the headers that are needed to unpickle such lengths, i.e., the headers containing floor division and coefficients for that type"%(lengths))

    for header in headers:
        cppyy.include(header)

    lengths = cppyy.gbl.intervalxt.cppyy.Lengths[type(lengths).__cpp_name__](lengths)

    lengthsRegistrar.track(type(lengths).__cpp_name__, headers)

    return lengths


def IntervalExchangeTransformation(lengths, permutation):
    if isinstance(lengths, (tuple, list)):
        lengths = Lengths(lengths)

    permutation = cppyy.gbl.std.vector[int](permutation)

    iet = intervalxt.cppyy.IntervalExchangeTransformation(lengths, permutation)

    iet.lengths = lengths

    return iet

# Hide the original sample::Lengths as instantiating them leads to segfaults in
# cppyy, see https://bitbucket.org/wlav/cppyy/issues/268/segfault-with-types-in-unnamed-namespaces
intervalxt.sample.Lengths = Lengths
