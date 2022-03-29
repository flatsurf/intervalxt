#!/usr/bin/env python3

######################################################################
#  This file is part of intervalxt.
#
#        Copyright (C) 2022 Julian Rüth
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

from pyintervalxt import intervalxt, IntervalExchangeTransformation


def test_printing():
    # Labels can print their internal id which is usually just a random integer.
    assert str(intervalxt.Label(1337)) == "Label(1337)"

    # Labels automatically print properly when created through Lengths
    iet = IntervalExchangeTransformation((18, 3), (1, 0))
    a = iet.top()[0]
    assert isinstance(a, intervalxt.Label)
    assert str(a) == "a"


if __name__ == '__main__':
    import sys
    import pytest
    sys.exit(pytest.main(sys.argv))
