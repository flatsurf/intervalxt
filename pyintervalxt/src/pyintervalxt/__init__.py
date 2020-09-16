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
    >>> IntervalExchangeTransformation((mpz(str(2**128)), mpz(1)), (1, 0))
    [a: 340282366920938463463374607431768211456] [b: 1] / [b] [a]

Rational numbers can be modeled with mpq fractions::

    >>> from gmpxxyy import mpq
    >>> IntervalExchangeTransformation((mpq(13, 37), mpq(23, 32)), (1, 0))
    [a: 13/37] [b: 23/32] / [b] [a]

You can also use number algebraic lengths with pyeantic::

    >>> from pyeantic import eantic
    >>> L = eantic.renf("a^3 - a^2 - a - 1", "a", "[1.84 +/- 0.01]")
    >>> lengths = [L.gen(), eantic.renf_elem(L, "2*a^2 - 3")]
    >>> IntervalExchangeTransformation(lengths, [1, 0])
    [a: (a ~ 1.8392868)] [b: (2*a^2 - 3 ~ 3.7659515)] / [b] [a]

"""

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

from .cppyy_intervalxt import intervalxt, IntervalExchangeTransformation, Lengths
