# -*- coding: utf-8 -*-
r"""
A Python Interface to libintervalxt

EXAMPLES::

    >>> from pyintervalxt import IET
    >>> iet = IET((18, 3), (1, 0))
    >>> iet
    [a: 18] [b: 3] / [b] [a]
    
TESTS:

Pickling works::

    >>> from pickle import loads, dumps
    >>> loads(dumps(iet))
    [a: 18] [b: 3] / [b] [a]

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

from .cppyy_intervalxt import intervalxt, IET
