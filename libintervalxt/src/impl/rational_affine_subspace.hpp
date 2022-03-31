/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019      Vincent Delecroix
 *        Copyright (C) 2019-2022 Julian Rüth
 *
 *  intervalxt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  intervalxt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with intervalxt. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#ifndef LIBINTERVALXT_RATIONAL_AFFINE_SUBSPACE_HPP
#define LIBINTERVALXT_RATIONAL_AFFINE_SUBSPACE_HPP

#include <gmpxx.h>

#include <boost/operators.hpp>
#include <iosfwd>
#include <ppl.hh>
#include <vector>

#include "../../intervalxt/forward.hpp"

namespace intervalxt {

// An affine rational subspace of ℚ^d.
class RationalAffineSubspace : boost::equality_comparable<RationalAffineSubspace> {
 public:
  // The space ℚ^0.
  RationalAffineSubspace();

  // The subspace generated by the vectors (a_0, …, a_n)
  RationalAffineSubspace(const std::vector<std::vector<mpq_class>>& generators);

  // The subspace which satisfies Σa_i x_i = y for each vector (a_0, …, a_n) in
  // constraints.
  RationalAffineSubspace(const std::vector<std::vector<mpq_class>>& equations, const std::vector<mpq_class>& y);

  // Act with the elementary matrix: x_i ↦ x_i + c * x_j
  void elementaryTransformation(int i, int j, mpq_class c);

  // Act with the permutation: x_i ↔ x_j
  void swap(int i, int j);

  enum class HAS_NON_ZERO_NON_NEGATIVE_VECTOR_IMPLEMENTATION {
    DEFAULT = 0,
    PPL_POLYHEDRON = 1,
    PPL_QUOTIENT = 2,
    PPL_MIP = 3,
  };

  template <HAS_NON_ZERO_NON_NEGATIVE_VECTOR_IMPLEMENTATION algorithm = HAS_NON_ZERO_NON_NEGATIVE_VECTOR_IMPLEMENTATION::DEFAULT>
  bool hasNonZeroNonNegativeVector() const;
  bool hasNonNegativeVector() const;
  bool hasPositiveVector() const;

  bool operator==(const RationalAffineSubspace&) const;

  friend std::ostream& operator<<(std::ostream&, const RationalAffineSubspace&);

 private:
  Parma_Polyhedra_Library::NNC_Polyhedron subspace;

  // Whether the right hand side was 0 when creating this subspace, i.e.,
  // whether it is an actual linear subspace or just an affine subspace.
  bool homogeneous;

  Parma_Polyhedra_Library::NNC_Polyhedron positive() const;
  Parma_Polyhedra_Library::NNC_Polyhedron nonNegative() const;
};

}  // namespace intervalxt

#endif
