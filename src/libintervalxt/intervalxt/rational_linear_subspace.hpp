/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019 Vincent Delecroix
 *        Copyright (C) 2019 Julian Rüth
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

#ifndef LIBINTERVALXT_RATIONAL_LINEAR_SUBSPACE_HPP
#define LIBINTERVALXT_RATIONAL_LINEAR_SUBSPACE_PP

#include <iosfwd>

#include <gmpxx.h>
#include <boost/operators.hpp>
#include <ppl.hh>

#include "intervalxt/intervalxt.hpp"

namespace intervalxt {

// A linear rational subspace of ℚ^d.
class RationalLinearSubspace : boost::equality_comparable<RationalLinearSubspace> {
 public:
  RationalLinearSubspace(const Parma_Polyhedra_Library::Constraint_System& cs);

  // Act with the elementary matrix: i -> i + x * j
  void elementaryTransformation(const Parma_Polyhedra_Library::Variable& i, const Parma_Polyhedra_Library::Variable& j, mpq_class x);

  // Act with the permutation: i <-> j
  void swap(const Parma_Polyhedra_Library::Variable& i, const Parma_Polyhedra_Library::Variable& j);

  bool hasNonZeroNonNegativeVector() const;
  bool hasPositiveVector() const;

  bool operator==(const RationalLinearSubspace&) const;

  friend std::ostream& operator<<(std::ostream&, const RationalLinearSubspace&);
};
}  // namespace intervalxt

#endif
