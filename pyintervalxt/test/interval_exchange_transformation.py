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
    assert repr(iet) == "18[a] 3[b]\n3[b] 18[a]\n"
    iet.swap()
    assert repr(iet) == "3[b] 18[a]\n18[a] 3[b]\n"

def test_reduce():
    iet = intervalxt.IET((18, 3), (1, 0))
    r = iet.reduce()
    assert not r.has_value()

    iet = intervalxt.IET((18, 3), (0, 1))
    r = iet.reduce()
    assert r.has_value()
    iet2 = r.value()
    assert repr(iet) == "18[c]\n18[c]\n"
    assert repr(iet2) == "3[d]\n3[d]\n"

    iet = intervalxt.IET((4,56,23,11,21,9,65),(1,0,4,3,2,6,5))
    r = iet.reduce()
    assert r.has_value()
    iet2 = r.value()
    assert repr(iet) == "4[e] 56[f]\n56[f] 4[e]\n"
    assert repr(iet2) == "23[g] 11[h] 21[i] 9[j] 65[k]\n21[i] 11[h] 23[g] 65[k] 9[j]\n"
    r2 = iet2.reduce()
    assert r2.has_value()
    iet3 = r2.value()
    assert repr(iet2) == "23[g] 11[h] 21[i]\n21[i] 11[h] 23[g]\n"
    assert repr(iet3) == "9[j] 65[k]\n65[k] 9[j]\n"

if __name__ == '__main__': sys.exit(pytest.main(sys.argv))
