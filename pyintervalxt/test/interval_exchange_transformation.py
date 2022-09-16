#!/usr/bin/env python

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

import pytest

from pyintervalxt import intervalxt, IntervalExchangeTransformation


@pytest.fixture(params=["mpz", "mpq", "renf_elem"])
def coefficients(request):
    r"""
    Iterates over the supported coefficient types.
    """
    return request.param


@pytest.fixture(params=[[1, 0], [0, 1], [1, 0, 4, 3, 2, 6, 5]])
def permutation(request):
    r"""
    Iterates over a sample of permutations, i.e., given the top of an IET [0,
    1, 2, …], this produces a corresponding bottom permutation.
    """
    return request.param

     
@pytest.fixture
def lengths(coefficients, permutation):
    r"""
    Produces lengths for the given permutation in a ring represented by coefficients.
    """
    if len(permutation) == 2:
        lengths = [18, 3]
    elif len(permutation) == 7:
        lengths = [4, 56, 23, 11, 21, 9, 65]
    else:
        lengths = [1] * len(permutation)

    if coefficients == "mpz":
        from gmpxxyy import mpz
        return [mpz(l) for l in lengths]
    elif coefficients == "mpq":
        from gmpxxyy import mpq
        return [mpq(l) for l in lengths]
    elif coefficients == "renf_elem":
        from pyeantic import eantic
        L = eantic.renf("a^3 - a^2 - a - 1", "a", "[1.84 +/- 0.01]")
        return [eantic.renf_elem(L, l) for l in lengths]
    else:
        raise NotImplementedError(f"Cannot create lengths for {coefficients} yet.")


@pytest.fixture
def iet(lengths, permutation):
    r"""
    Iterates over several Interval Exchange Transformations.
    """
    return IntervalExchangeTransformation(lengths, permutation)


def test_saf_invariant(iet):
    r"""
    Test that the SAF invariant can be computed from Python.
    """
    iet.safInvariant()


def test_IntervalExchangeTransformation(iet, coefficients, permutation):
    r"""
    Test that Interval Exchange Transformations can be printed.
    """
    if coefficients == "mpz" and permutation == [1, 0]:
        assert repr(iet) == "[a: 18] [b: 3] / [b] [a]"
        iet.swap()
        assert repr(iet) == "[b: 3] [a: 18] / [a] [b]"

    assert "0x" not in repr(iet)


def test_reduce(iet, coefficients, permutation):
    r"""
    Test that reduction of Interval Exchange Transformation works when they have an evident Saddle Connection.
    """
    if permutation == [1, 0]:
        assert not iet.reduce().has_value()
    elif permutation == [0, 1]:
        r = iet.reduce()
        assert r.has_value()
        iet2 = r.value()
        if coefficients == "mpz":
            assert repr(iet) == "[a: 18] / [a]"
            assert repr(iet2) == "[b: 3] / [b]"
    elif permutation == [1, 0, 4, 3, 2, 6, 5]:
        r = iet.reduce()
        assert r.has_value()
        iet2 = r.value()
        if coefficients == "mpz":
            assert repr(iet) == "[a: 4] [b: 56] / [b] [a]"
            assert repr(iet2) == "[c: 23] [d: 11] [e: 21] [f: 9] [g: 65] / [e] [d] [c] [g] [f]"
        r2 = iet2.reduce()
        assert r2.has_value()
        iet3 = r2.value()
        if coefficients == "mpz":
            assert repr(iet2) == "[c: 23] [d: 11] [e: 21] / [e] [d] [c]"
            assert repr(iet3) == "[f: 9] [g: 65] / [g] [f]"
    else:
        r = iet.reduce()
        if r.has_value():
            assert not r.value().reduce().has_value()


def test_boshernitzan_equations(iet):
    r"""
    Test that boshernitzanEquations() can be called from Python.
    """
    equations = iet.boshernitzanEquations()

    if permutation == [1, 0]:
        assert equations == [3, -18]


def test_boshernitzan_saddle_connection_values(iet):
    r"""
    Test that boshernitzanSaddleConnectionValues() can be called from Python.
    """
    for top in iet.top()[:-1]:
        for bottom in iet.bottom()[:-1]:
            assert len(iet.boshernitzanSaddleConnectionValues(top, bottom)) == len(iet.boshernitzanEquations())

    # Cannot be tested due to https://github.com/wlav/cppyy/issues/32
    # with pytest.raises(Exception):
    #     iet.boshernitzanSaddleConnectionValues(iet.top()[-1], iet.bottom()[0])

    # with pytest.raises(Exception):
    #     iet.boshernitzanSaddleConnectionValues(iet.top()[0], iet.bottom()[-1])


def test_decompose(iet):
    r"""
    Test that IETs can be decomposed into cylinders and minimal components.
    """
    decomposition = intervalxt.DynamicalDecomposition(iet)
    decomposition.decompose()

    cyls = sum(bool(component.cylinder() == True) for component in decomposition.components())  # noqa, == True is the correct check here.
    mins = sum(bool(component.withoutPeriodicTrajectory() == True) for component in decomposition.components())  # noqa, == True is the correct check here.

    assert cyls + mins == decomposition.components().size()


def test_lengths(iet, lengths):
    r"""
    Test that the lengths underlying an IET can be accessed.
    """
    for label, length in zip(iet.top(), lengths):
        assert iet.lengths().get(label) == length


if __name__ == '__main__':
    import sys
    import pytest
    sys.exit(pytest.main(sys.argv))
