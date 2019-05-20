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
#include <vector>

#include <gmpxx.h>
#include <boost/operators.hpp>
#include <ppl.hh>

#include "intervalxt/external/spimpl/spimpl.h"
#include "intervalxt/intervalxt.hpp"

namespace intervalxt {

// A linear rational subspace of ℚ^d.
class RationalLinearSubspace : boost::equality_comparable<RationalLinearSubspace> {
 public:
  // The space ℚ^0.
  RationalLinearSubspace();
  // The subspace which satisfies Σa_i x_i = 0 for each vector (a_0, …, a_n) in
  // constraints.
  RationalLinearSubspace(const std::vector<std::vector<mpq_class>>& equations);

  // Act with the elementary matrix: x_i ↦ x_i + c * x_j
  void elementaryTransformation(int i, int j, mpq_class c);

  // Act with the permutation: x_i ↔ x_j
  void swap(int i, int j);

  bool hasNonZeroNonNegativeVector() const;
  bool hasPositiveVector() const;

  bool operator==(const RationalLinearSubspace&) const;

  friend std::ostream& operator<<(std::ostream&, const RationalLinearSubspace&);

 private:
  class Implementation;
  spimpl::impl_ptr<Implementation> impl;
};
}  // namespace intervalxt

#endif
