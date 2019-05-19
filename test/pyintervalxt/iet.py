#!/usr/bin/env python

import sys
import pytest

from pyintervalxt import intervalxt

IET = intervalxt.IntervalExchangeTransformation['ulong', 'ulong']

def test_IntervalExchangeTransformation():
    iet = IET(2)
    iet.setLenghts([18, 3])
    assert iet == ""
