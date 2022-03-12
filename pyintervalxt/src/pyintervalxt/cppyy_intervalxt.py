######################################################################
#  This file is part of intervalxt.
#
#        Copyright (C) 2019-2020 Vincent Delecroix
#        Copyright (C) 2019-2022 Julian Rüth
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
from cppyythonizations.printing import enable_pretty_printing
from cppyythonizations.util import filtered, wrap_method
from cppyythonizations.boost.type_erasure import expose

# Importing cysignals after cppyy gives us proper stack traces on segfaults
# whereas cppyy otherwise only reports "segmentation violation" (which is
# probably what cling provides.)
import os
if os.environ.get('PYINTERVALXT_CYSIGNALS', True):
    try:
        import cysignals
    except ModuleNotFoundError:
        pass


cppyy.py.add_pythonization(enable_pretty_printing, "intervalxt")


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
                if length == "intervalxt::cppyy::Lengths<std::vector<__gmp_expr<__mpz_struct[1],__mpz_struct[1]> > >":
                    import logging
                    logging.warning("GMP does not provide a cereal interface yet. Therefore serialization of mpz lengths is not supported yet.")
                    continue

                if length == "intervalxt::cppyy::Lengths<std::vector<__gmp_expr<__mpq_struct[1],__mpq_struct[1]> > >":
                    import logging
                    logging.warning("GMP does not provide a cereal interface yet. Therefore serialization of mpq lengths is not supported yet.")
                    continue

                for header in headers:
                    cppyy.include(header)

                cppyy.cppdef('LIBINTERVALXT_ERASED_REGISTER((::intervalxt::Lengths), (%s));' % (length,))

            LengthRegistrar.REGISTERED_LENGTHS.add(length)


lengthsRegistrar = LengthRegistrar()


def enable_cereal_(proxy, name):
    headers = ["intervalxt/cereal.hpp", "intervalxt/sample/cereal.hpp", "intervalxt/sample/cppyy.hpp", lengthsRegistrar]

    enable_cereal(proxy, name, headers)


cppyy.py.add_pythonization(enable_cereal_, "intervalxt")
cppyy.py.add_pythonization(enable_cereal_, "intervalxt::sample")
cppyy.py.add_pythonization(enable_cereal_, "intervalxt::cppyy")


def name_labels(labels, name):
    r"""
    Helper function for Label.__str__ to attach a labels name upon creation.
    """
    labels = list(labels)
    for label in labels:
        label._name = name(label)
    return labels


# Print label as "a", "b", "c" and not only as their memory address.
# Labels do not have an intrinsic name. They only get a name in libintervalxt
# once they are associated with a Lengths object.
# To make them print nicely, we try to make sure that any code path that creates
# a label attaches its name to it.
cppyy.py.add_pythonization(filtered("Label")(wrap_method("__str__")(lambda self, __str__: str(self._name) if hasattr(self, "_name") else __str__())), "intervalxt")

cppyy.py.add_pythonization(filtered("IntervalExchangeTransformation")(wrap_method("top")(lambda self, top: name_labels(top(), self.lengths().render if hasattr(self, "lengths") else str))), "intervalxt")
cppyy.py.add_pythonization(filtered("IntervalExchangeTransformation")(wrap_method("bottom")(lambda self, bottom: name_labels(bottom(), self.lengths().render if hasattr(self, "lengths") else str))), "intervalxt")
cppyy.py.add_pythonization(filtered(re.compile("Lengths<.*>"))(wrap_method("labels")(lambda self, labels: name_labels(labels(), self.render))), "intervalxt::cppyy")
cppyy.py.add_pythonization(filtered(re.compile("Lengths<.*>"))(wrap_method("render")(lambda self, render, label: str(render(label)))), "intervalxt::cppyy")

# Expose methods on type-erased intervalxt::Lengths.
cppyy.py.add_pythonization(filtered("any<intervalxt::LengthsInterface,boost::type_erasure::_self>")(expose("push")), "boost::type_erasure")
cppyy.py.add_pythonization(filtered("any<intervalxt::LengthsInterface,boost::type_erasure::_self>")(expose("pop")), "boost::type_erasure")
cppyy.py.add_pythonization(filtered("any<intervalxt::LengthsInterface,boost::type_erasure::_self>")(expose("subtract")), "boost::type_erasure")
cppyy.py.add_pythonization(filtered("any<intervalxt::LengthsInterface,boost::type_erasure::_self>")(expose("subtractRepeated")), "boost::type_erasure")
cppyy.py.add_pythonization(filtered("any<intervalxt::LengthsInterface,boost::type_erasure::_self>")(expose("coefficients")), "boost::type_erasure")
cppyy.py.add_pythonization(filtered("any<intervalxt::LengthsInterface,boost::type_erasure::_self>")(expose("cmp")), "boost::type_erasure")
cppyy.py.add_pythonization(filtered("any<intervalxt::LengthsInterface,boost::type_erasure::_self>")(expose("similar")), "boost::type_erasure")
cppyy.py.add_pythonization(filtered("any<intervalxt::LengthsInterface,boost::type_erasure::_self>")(expose("get")), "boost::type_erasure")
cppyy.py.add_pythonization(filtered("any<intervalxt::LengthsInterface,boost::type_erasure::_self>")(expose("render")), "boost::type_erasure")
cppyy.py.add_pythonization(filtered("any<intervalxt::LengthsInterface,boost::type_erasure::_self>")(expose("only")), "boost::type_erasure")
cppyy.py.add_pythonization(filtered("any<intervalxt::LengthsInterface,boost::type_erasure::_self>")(expose("forget")), "boost::type_erasure")

# Expose printing on type-erased intervalxt::Length.
cppyy.py.add_pythonization(filtered("any<intervalxt::LengthInterface,boost::type_erasure::_self>")(enable_pretty_printing), "boost::type_erasure")


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
            headers = ["intervalxt/sample/renf_elem_coefficients.hpp", "intervalxt/sample/renf_elem_floor_division.hpp", "e-antic/cereal.hpp"]
        elif type(lengths) in [cppyy.gbl.std.vector["exactreal::Element<exactreal::IntegerRing>"], cppyy.gbl.std.vector["exactreal::Element<exactreal::RationalField>"], cppyy.gbl.std.vector["exactreal::Element<exactreal::NumberField>"]]:
            headers = ["intervalxt/sample/element_coefficients.hpp", "intervalxt/sample/element_floor_division.hpp", "exact-real/cereal.hpp"]
        else:
            raise TypeError("unknown length type %s; you need to specify the headers that are needed to unpickle such lengths, i.e., the headers containing floor division and coefficients for that type" % (lengths,))

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

    iet.lengths = lambda: lengths
    # In a previous iteration of pyintervalxt, iet.lengths was a property and
    # not a method (as it is in the C++ interface.) To keep backwards
    # compatibility, we expose all of lengths() on lengths itself, see #158.
    for attr in dir(lengths):
        if not attr.startswith('__'):
            setattr(iet.lengths, attr, getattr(lengths, attr))

    return iet


# Hide the original sample::Lengths as instantiating them leads to segfaults in
# cppyy, see https://bitbucket.org/wlav/cppyy/issues/268/segfault-with-types-in-unnamed-namespaces
intervalxt.sample.Lengths = Lengths
