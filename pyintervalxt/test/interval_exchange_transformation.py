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

expected="""18 3
3 18
"""
expected_swapped="""3 18
18 3
"""

def test_IntervalExchangeTransformation():
    iet = intervalxt.IET((18, 3), (1, 0))
    assert repr(iet) == expected
    iet.swap()
    assert repr(iet) == expected_swapped

def test_reduce():
    iet = intervalxt.IET((18, 3), (1, 0))
    r = iet.reduce()
    assert not r.has_value()

    iet = intervalxt.IET((18, 3), (0, 1))
    r = iet.reduce()
    assert r.has_value()
    iet2 = r.value()
    assert repr(iet) == "18\n18\n"
    assert repr(iet2) == "3\n3\n"

    iet = intervalxt.IET((4,56,23,11,21,9,65),(1,0,4,3,2,6,5))
    r = iet.reduce()
    assert r.has_value()
    iet2 = r.value()
    assert repr(iet) == "4 56\n56 4\n"
    assert repr(iet2) == "23 11 21 9 65\n21 11 23 65 9\n"
    r2 = iet2.reduce()
    assert r2.has_value()
    iet3 = r2.value()
    assert repr(iet2) == "23 11 21\n21 11 23\n"
    assert repr(iet3) == "9 65\n65 9\n"

if __name__ == '__main__': sys.exit(pytest.main(sys.argv))
