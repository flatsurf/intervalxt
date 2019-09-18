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

#include <gmpxx.h>
#include <boost/numeric/conversion/cast.hpp>
#include <iostream>
#include <ppl.hh>
#include <vector>

#include "intervalxt/detail/rational_linear_subspace.hpp"

using boost::numeric_cast;
using Parma_Polyhedra_Library::Constraint;
using Parma_Polyhedra_Library::Constraint_System;
using Parma_Polyhedra_Library::Generator_System;
using Parma_Polyhedra_Library::Linear_Expression;
using Parma_Polyhedra_Library::Linear_System;
using Parma_Polyhedra_Library::NNC_Polyhedron;
using Parma_Polyhedra_Library::NOT_NECESSARILY_CLOSED;
using Parma_Polyhedra_Library::SPARSE;
using Parma_Polyhedra_Library::Variable;
// Note: there are global point and ray in the ppl header.
using Parma_Polyhedra_Library::point;
using Parma_Polyhedra_Library::ray;
using std::ostream;
using std::vector;

namespace {
struct SwapDimensions {
  size_t dimension, i, j;

  bool has_empty_codomain() const { return false; }
  size_t max_in_codomain() const { return dimension - 1; }
  bool maps(size_t x, size_t& y) const {
    y = x == i ? j : i;
    return true;
  }
};
}  // namespace

Linear_Expression linearExpressionFromVector(const std::vector<mpq_class>& vec) {
  auto den = mpz_class(1);
  for (auto c : vec) {
    den = lcm(den, c.get_den());
  }

  Linear_Expression linear;
  for (size_t i = 0; i < vec.size(); i++) {
    mpq_class num = den * vec[i];
    assert(num.get_den() == 1);
    linear = linear + num.get_num() * Variable(i);
  }

  return linear;
}

namespace intervalxt {
namespace detail {
class RationalLinearSubspace::Implementation {
 public:
  Implementation() {}

  Implementation(const vector<vector<mpq_class>>& vectors, bool eqns) {
    Generator_System generators;
    Constraint_System constraints;

    if (eqns) {
      // Initialize from equations
      for (auto equation : vectors) {
        Linear_Expression linear = linearExpressionFromVector(equation);
        constraints.insert(linear == 0);
      }
      subspace = NNC_Polyhedron(constraints);

    } else {
      // Initialize from generators
      generators.insert(point());
      for (auto generator : vectors) {
        Linear_Expression linear = linearExpressionFromVector(generator);
        if (!linear.is_zero()) generators.insert(ray(linear));
      }
      subspace = NNC_Polyhedron(generators);
    }

    constraints.clear();
    for (size_t i = 0; i < subspace.space_dimension(); i++) {
      constraints.insert(Variable(i) >= 0);
    }
    nonNegative = NNC_Polyhedron(constraints);

    constraints.clear();
    for (size_t i = 0; i < subspace.space_dimension(); i++) {
      constraints.insert(Variable(i) > 0);
    }
    positive = NNC_Polyhedron(constraints);
  }

  NNC_Polyhedron subspace;
  NNC_Polyhedron positive;
  NNC_Polyhedron nonNegative;
};

RationalLinearSubspace::RationalLinearSubspace() : impl(spimpl::make_impl<Implementation>()) {}

RationalLinearSubspace RationalLinearSubspace::fromEquations(const vector<vector<mpq_class>>& equations) {
  RationalLinearSubspace R;
  R.impl = spimpl::make_impl<Implementation>(equations, true);
  return R;
}

RationalLinearSubspace RationalLinearSubspace::fromGenerators(const vector<vector<mpq_class>>& generators) {
  RationalLinearSubspace R;
  R.impl = spimpl::make_impl<Implementation>(generators, false);
  return R;
}

bool RationalLinearSubspace::hasNonZeroNonNegativeVector() const {
  if (impl->subspace.space_dimension() == 0) {
    return false;
  }

  auto polyhedron = NNC_Polyhedron(impl->subspace);
  polyhedron.intersection_assign(impl->nonNegative);
  return !polyhedron.is_bounded();
}

bool RationalLinearSubspace::hasPositiveVector() const {
  if (impl->subspace.space_dimension() == 0) {
    return false;
  }

  auto polyhedron = NNC_Polyhedron(impl->subspace);
  polyhedron.intersection_assign(impl->positive);
  return !polyhedron.is_empty();
}

bool RationalLinearSubspace::operator==(const RationalLinearSubspace& rhs) const {
  return impl->subspace == rhs.impl->subspace;
}

void RationalLinearSubspace::swap(int i, int j) {
  impl->subspace.map_space_dimensions(SwapDimensions{impl->subspace.space_dimension(), numeric_cast<size_t>(i), numeric_cast<size_t>(j)});
}

void RationalLinearSubspace::elementaryTransformation(int i, int j, mpq_class c) {
  impl->subspace.affine_image(Variable(numeric_cast<size_t>(i)), c.get_den() * Variable(numeric_cast<size_t>(i)) + c.get_num() * Variable(numeric_cast<size_t>(j)), c.get_den());
}

ostream& operator<<(std::ostream& os, const RationalLinearSubspace& self) {
  return Parma_Polyhedra_Library::IO_Operators::operator<<(os, self.impl->subspace);
}
}  // namespace detail
}  // namespace intervalxt
