#!/usr/bin/env python3
# -*- coding: utf-8 -*-

######################################################################
#  This file is part of intervalxt.
#
#        Copyright (C) 2019      Vincent Delecroix
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

import sys
import pytest

from pyintervalxt import intervalxt, IntervalExchangeTransformation


def test_IntervalExchangeTransformation():
    iet = IntervalExchangeTransformation((18, 3), (1, 0))
    assert repr(iet) == "[a: 18] [b: 3] / [b] [a]"
    iet.swap()
    assert repr(iet) == "[b: 3] [a: 18] / [a] [b]"


def test_reduce():
    iet = IntervalExchangeTransformation((18, 3), (1, 0))
    r = iet.reduce()
    assert not r.has_value()

    iet = IntervalExchangeTransformation((18, 3), (0, 1))
    r = iet.reduce()
    assert r.has_value()
    iet2 = r.value()
    assert repr(iet) == "[a: 18] / [a]"
    assert repr(iet2) == "[b: 3] / [b]"

    iet = IntervalExchangeTransformation((4, 56, 23, 11, 21, 9, 65), (1, 0, 4, 3, 2, 6, 5))
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


def iet_10_check(iet):
    assert iet.size() == 2
    assert iet.top()[0] == iet.bottom()[1]
    assert iet.top()[1] == iet.bottom()[0]
    saf = iet.safInvariant()
    decomposition = intervalxt.DynamicalDecomposition(iet)
    decomposition.decompose()
    cyls = sum(bool(component.cylinder() == True) for component in decomposition.components())
    nocyls = sum(bool(component.cylinder() == False) for component in decomposition.components())
    mins = sum(bool(component.withoutPeriodicTrajectory() == True) for component in decomposition.components())
    nomins = sum(bool(component.withoutPeriodicTrajectory() == False) for component in decomposition.components())
    assert cyls + nocyls == decomposition.components().size()
    assert mins + nomins == decomposition.components().size()


def test_mpz():
    from gmpxxyy import mpz
    iet = IntervalExchangeTransformation((mpz(1), mpz(1)), [1, 0])
    iet_10_check(iet)


def test_mpq():
    from gmpxxyy import mpq
    iet = IntervalExchangeTransformation((mpq(1, 3), mpq(2, 5)), [1, 0])
    iet_10_check(iet)


def test_eantic():
    from pyeantic import eantic
    L = eantic.renf("a^3 - a^2 - a - 1", "a", "[1.84 +/- 0.01]")
    lengths = []
    lengths.append(eantic.renf_elem(L, "a"))
    lengths.append(eantic.renf_elem(L, "2*a^2 - 3"))
    iet = IntervalExchangeTransformation(lengths, [1, 0])
    iet_10_check(iet)


if __name__ == '__main__':
    sys.exit(pytest.main(sys.argv))
