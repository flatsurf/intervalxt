#!/usr/bin/env python3
# -*- coding: utf-8 -*-

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

import sys
import pytest

from pyintervalxt import intervalxt

def test_IntervalExchangeTransformation():
    iet = intervalxt.IET((18, 3), (1, 0))
    assert repr(iet) == "[a: 18] [b: 3] / [b] [a]"
    iet.swap()
    assert repr(iet) == "[b: 3] [a: 18] / [a] [b]"

def test_reduce():
    iet = intervalxt.IET((18, 3), (1, 0))
    r = iet.reduce()
    assert not r.has_value()

    iet = intervalxt.IET((18, 3), (0, 1))
    r = iet.reduce()
    assert r.has_value()
    iet2 = r.value()
    assert repr(iet) == "[a: 18] / [a]"
    assert repr(iet2) == "[b: 3] / [b]"

    iet = intervalxt.IET((4, 56, 23, 11, 21, 9, 65),(1, 0, 4, 3, 2, 6, 5))
    r = iet.reduce()
    assert r.has_value()
    iet2 = r.value()
    assert repr(iet) == "[a: 4] [b: 56] / [b] [a]"
    assert repr(iet2) == "[c: 23] [d: 11] [e: 21] [f: 9] [g: 65] / [e] [d] [c] [g] [f]"
    r2 = iet2.reduce()
    assert r2.has_value()
    iet3 = r2.value()
    assert repr(iet2) == "[c: 23] [d: 11] [e: 21] / [e] [d] [c]"
    assert repr(iet3) == "[f: 9] [g: 65] / [g] [f]"

def iet_check(iet):
    saf = iet.safInvariant()
    decomposition = intervalxt.DynamicalDecomposition(iet)
    decomposition.decompose()
    ncyls = sum(bool(component.cylinder() == True) for component in decomposition.components())

def test_mpz():
    from gmpxxyy import mpz
    lengths = intervalxt.sample.Lengths[mpz]([1, 1])
    iet = intervalxt.makeIET(lengths, [1, 0])
    iet_check(iet)

def test_mpq_constructor():
    from gmpxxyy import mpq
    lengths = intervalxt.sample.Lengths[mpq]([1, 1])
    iet = intervalxt.makeIET(lengths, [1, 0])
    iet_check(iet)

def test_eantic_constructor():
    from pyeantic import eantic

    L = eantic.renf("a^3 - a^2 - a - 1", "a", "[1.84 +/- 0.01]")
    lengths = []
    lengths.append(eantic.renf_elem(L, "a"))
    lengths.append(eantic.renf_elem(L, "2*a^2 - 3"))
    lengths = intervalxt.sample.Lengths[eantic.renf_elem_class](lengths)
    iet = intervalxt.makeIET(lengths, [1, 0])
    iet_check(iet)

if __name__ == '__main__': sys.exit(pytest.main(sys.argv))
