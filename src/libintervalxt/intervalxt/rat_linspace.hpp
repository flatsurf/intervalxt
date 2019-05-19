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

#ifndef FLATSURF_RAT_LINSPACE
#define FLATSURF_RAT_LINSPACE

#include "intervalxt/intervalxt.hpp"

namespace intervalxt {
// Class that models a Q-vector subspace of Q^d
// ideally there should be several implementation
// - a generator based version (ie, store generators)
// - a constraint based version (ie, store equations)
// of course they are dual to each other, but depending on the codimension
// of the space it might be more efficient to do one or the other.
//
// The most important non-trivial functionalities to focus on are
// - hasNonZeroNonNegativeVector()
// - hasPositiveVector()

class RationalLinearSubspace {
 public:
  // sadly FLINT imposes matrix dimension
  // (no resize available)
  RatLinSpace();
  RatLinSpace(int dim);

  // action by the elementary matrix on R^d
  //   ei -> ei + x ej
  void elementaryTransformation(size_t i, size_t j, Coeff x);

  // swap action on R^d
  //   ei <-> ej
  void swap(size_t i, size_t j);

  // canonical form
  // This provides canonical generators (or equations) for the subspace
  // so that we can use
  void canonicalize();

  // test for non-negative or positive vectors
  // (NOTE: this can be done via linear programming, eg call to PPL)
  bool hasNonZeroNonNegativeVector();
  bool hasPositiveVector();
}
}  // namespace intervalxt

#endif
