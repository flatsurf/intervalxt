# -*- coding: utf-8 -*-
r"""
A Python Interface to libintervalxt

EXAMPLES::

    >>> from pyintervalxt import IntervalExchangeTransformation
    >>> iet = IntervalExchangeTransformation((18, 3), (1, 0))
    >>> iet
    [a: 18] [b: 3] / [b] [a]

    >>> top = iet.top()
    >>> top
    [a, b]

    >>> iet.lengths.labels()
    [a, b]
    >>> iet.lengths.get(top[0])
    18

Pickling works::

    >>> from pickle import loads, dumps
    >>> loads(dumps(iet))
    [a: 18] [b: 3] / [b] [a]

The lengths in the above example are limited to small (32bit) integers, if you
need larger entries, you might want to use mpz integers::

    >>> from gmpxxyy import mpz
    >>> iet = IntervalExchangeTransformation((mpz(str(2**128)), mpz(1)), (1, 0))

Note however, that such IETs cannot be serialized yet.

(Currently, the following crashes with "terminate called after throwing an
instance of 'cereal::Exception'" see https://github.com/wlav/cppyy/issues/13)::

    # >>> loads(dumps(iet))
    # Traceback (most recent call last):
    # ...
    # cppyy.gbl.cereal.Exception: ...

Rational numbers can be modeled with mpq fractions::

    >>> from gmpxxyy import mpq
    >>> IntervalExchangeTransformation((mpq(13, 37), mpq(23, 32)), (1, 0))
    [a: 13/37] [b: 23/32] / [b] [a]

Again, such IETs cannot be serialized yet.

(Currently, the following crashes with "terminate called after throwing an
instance of 'cereal::Exception'" see https://github.com/wlav/cppyy/issues/13)::

    # >>> loads(dumps(iet))
    # Traceback (most recent call last):
    # ...
    # cppyy.gbl.cereal.Exception: ...

You can also use algebraic lengths with pyeantic::

    >>> from pyeantic import eantic
    >>> L = eantic.renf("a^3 - a^2 - a - 1", "a", "[1.84 +/- 0.01]")
    >>> lengths = [L.gen(), eantic.renf_elem(L, "2*a^2 - 3")]
    >>> iet = IntervalExchangeTransformation(lengths, [1, 0])

And these IETs can be serialized and deserialized::

    >>> loads(dumps(iet))
    [a: (a ~ 1.8392868)] [b: (2*a^2 - 3 ~ 3.7659515)] / [b] [a]

We also support IETs with transcendental lengths provided by pyexactreal::

    >>> from pyexactreal import NumberFieldModule, RealNumber
    >>> M = NumberFieldModule(L, RealNumber.rational(1), RealNumber.random())
    >>> lengths = [M.gen(0), M.gen(1)]
    >>> iet = IntervalExchangeTransformation(lengths, [1, 0])

These can also be serialized and deserialized::

    >>> loads(dumps(iet))
    [a: 1] [b: ℝ(0...)] / [b] [a]

TESTS:

Check that #85 has been resolved.

(Currently, the following crashes with "terminate called after throwing an
exception see https://github.com/wlav/cppyy/issues/13)::

    # >>> lengths = [L.gen(), eantic.renf_elem(L, "-3*a^2 + 2*a - 1")]
    # >>> IntervalExchangeTransformation(lengths, [1, 0])
    # Traceback (most recent call last):
    # ...
    # TypeError: ...all lengths must be non-negative...

Check that #33 has been resolved::

    >>> IntervalExchangeTransformation((1, 1, 1), (1, 0, 2)) == IntervalExchangeTransformation((1, 1, 1), (2, 1, 0))
    False
    >>> IntervalExchangeTransformation((3, 2, 1), (1, 0, 2)) == IntervalExchangeTransformation((1, 2, 3), (1, 0, 2))
    False
    >>> IntervalExchangeTransformation((3, 2, 1), (2, 1, 0)) == IntervalExchangeTransformation((3, 2, 1), (1, 2, 0))
    False
    >>> IntervalExchangeTransformation((1, 2, 3), (0, 1, 2)) == IntervalExchangeTransformation((1, 2, 3), (0, 1, 2))
    True

Note that such equality comparisons takes the names of the labels into account
(actually, they use `Label::operator==` which is more complicated but behaves
very predictably from the Python interface.) To ignore label naming completely,
use `equivalent`::

    >>> iet = IntervalExchangeTransformation((1, 1), (0, 1))
    >>> jet = iet.reduce().value()
    >>> iet
    [a: 1] / [a]
    >>> jet
    [b: 1] / [b]
    >>> iet == jet
    False
    >>> iet.equivalent(jet)
    True

"""

######################################################################
#  This file is part of intervalxt.
#
#        Copyright (C) 2019-2020 Vincent Delecroix
#        Copyright (C) 2019-2021 Julian Rüth
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

from .cppyy_intervalxt import intervalxt, IntervalExchangeTransformation, Lengths
