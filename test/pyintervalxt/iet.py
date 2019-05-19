#!/usr/bin/env python

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

IET = intervalxt.IntervalExchangeTransformation['ulong', 'ulong']

expected="""01
01
interval 0 length 18
interval 1 length 3
1 0 
0 1 
"""

def test_IntervalExchangeTransformation():
    iet = IET(2)
    iet.setLengths([18, 3])
    assert repr(iet) == expected

if __name__ == '__main__': sys.exit(pytest.main(sys.argv))
