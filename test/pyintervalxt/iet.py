#!/usr/bin/env python

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
