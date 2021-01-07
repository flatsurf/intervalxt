/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019-2020 Vincent Delecroix
 *        Copyright (C) 2019-2021 Julian Rüth
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

#include "impl/rational_linear_subspace.hpp"

#include <boost/numeric/conversion/cast.hpp>
#include <ostream>
#include <ppl.hh>
#include <vector>

#include "external/rx-ranges/include/rx/ranges.hpp"
#include "util/assert.ipp"

using boost::numeric_cast;

using Parma_Polyhedra_Library::C_Polyhedron;
using Parma_Polyhedra_Library::Constraint_System;
using Parma_Polyhedra_Library::Generator_System;
using Parma_Polyhedra_Library::Linear_Expression;
using Parma_Polyhedra_Library::MIP_Problem;
using Parma_Polyhedra_Library::NNC_Polyhedron;
using Parma_Polyhedra_Library::Variable;
using Parma_Polyhedra_Library::Variables_Set;

// Note: there are global point and ray in the ppl header.
using Parma_Polyhedra_Library::line;
using Parma_Polyhedra_Library::point;
using Parma_Polyhedra_Library::ray;

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

RationalLinearSubspace::RationalLinearSubspace() {}

RationalLinearSubspace::RationalLinearSubspace(const std::vector<std::vector<mpq_class>>& vectors, bool equations) {
  Generator_System generators;
  Constraint_System constraints;

  if (equations) {
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
      if (!linear.is_zero()) generators.insert(line(linear));
    }
    subspace = NNC_Polyhedron(generators);
  }
}

NNC_Polyhedron RationalLinearSubspace::positive() const {
  Constraint_System constraints;

  for (size_t i = 0; i < subspace.space_dimension(); i++)
    constraints.insert(Variable(i) > 0);

  return NNC_Polyhedron(constraints);
}

NNC_Polyhedron RationalLinearSubspace::nonNegative() const {
  Constraint_System constraints;

  for (size_t i = 0; i < subspace.space_dimension(); i++)
    constraints.insert(Variable(i) >= 0);

  return NNC_Polyhedron(constraints);
}

RationalLinearSubspace RationalLinearSubspace::fromEquations(const std::vector<std::vector<mpq_class>>& equations) {
  return RationalLinearSubspace(equations, true);
}

RationalLinearSubspace RationalLinearSubspace::fromGenerators(const std::vector<std::vector<mpq_class>>& generators) {
  return RationalLinearSubspace(generators, false);
}

template <RationalLinearSubspace::HAS_NON_ZERO_NON_NEGATIVE_VECTOR_IMPLEMENTATION implementation>
bool RationalLinearSubspace::hasNonZeroNonNegativeVector() const {
  if (implementation == HAS_NON_ZERO_NON_NEGATIVE_VECTOR_IMPLEMENTATION::DEFAULT)
    return hasNonZeroNonNegativeVector<HAS_NON_ZERO_NON_NEGATIVE_VECTOR_IMPLEMENTATION::PPL_MIP>();

  if (subspace.space_dimension() == 0)
    return false;

  if (implementation == HAS_NON_ZERO_NON_NEGATIVE_VECTOR_IMPLEMENTATION::PPL_POLYHEDRON) {
    auto polyhedron = NNC_Polyhedron(subspace);
    polyhedron.intersection_assign(nonNegative());
    return !polyhedron.is_bounded();
  } else if (implementation == HAS_NON_ZERO_NON_NEGATIVE_VECTOR_IMPLEMENTATION::PPL_QUOTIENT) {
    const auto equations = subspace.minimized_constraints();

    ASSERT(equations | rx::all_of([](const auto& equation) { return equation.is_equality(); }), "equations defining the subspace must be equalities");
    ASSERT(equations | rx::all_of([](const auto& equation) { return equation.inhomogeneous_term() == 0; }), "equations defining the subspace must be homogeneous");
    ASSERT(subspace.affine_dimension() == subspace.space_dimension() - std::distance(std::begin(equations), std::end(equations)), "the number of equations must match the codimension");

    if (equations.empty())
      return true;

    auto generators = Generator_System();
    generators.insert(point(static_cast<Linear_Expression>(0)));
    for (int i = 0; i < subspace.space_dimension(); i++) {
      Linear_Expression linear = static_cast<Linear_Expression>(0);
      int j = 0;
      for (auto equation : equations)
        linear += equation.coefficient(Variable(i)) * Variable(j++);

      // basis vector inside the space
      if (linear.is_zero())
        return true;

      generators.insert(ray(linear));
    }

    return C_Polyhedron(generators).minimized_generators() | rx::any_of([](const auto& g) { return g.is_line(); });
  } else if (implementation == HAS_NON_ZERO_NON_NEGATIVE_VECTOR_IMPLEMENTATION::PPL_MIP) {
    Parma_Polyhedra_Library::MIP_Problem problem;

    problem.add_space_dimensions_and_embed(subspace.space_dimension());

    auto ivar = Variables_Set();
    auto objective = Linear_Expression(0);
    for (size_t i = 0; i < subspace.space_dimension(); i++) {
      ivar.insert(Variable(i));
      objective = objective + Linear_Expression(Variable(i));
      problem.add_constraint(Variable(i) >= 0);
    }
    problem.add_to_integer_space_dimensions(ivar);
    problem.set_objective_function(objective);

    for (auto constraint : subspace.constraints())
      problem.add_constraint(constraint);

    problem.set_optimization_mode(Parma_Polyhedra_Library::Optimization_Mode::MAXIMIZATION);

    return problem.solve() == Parma_Polyhedra_Library::UNBOUNDED_MIP_PROBLEM;
  } else {
    UNREACHABLE("unknown implementation for hasNonZeroNonNegativeVector selected");
  }
}

bool RationalLinearSubspace::hasPositiveVector() const {
  if (subspace.space_dimension() == 0) {
    return false;
  }

  auto polyhedron = NNC_Polyhedron(subspace);
  polyhedron.intersection_assign(positive());
  return !polyhedron.is_empty();
}

bool RationalLinearSubspace::operator==(const RationalLinearSubspace& rhs) const {
  return subspace == rhs.subspace;
}

void RationalLinearSubspace::swap(int i, int j) {
  subspace.map_space_dimensions(SwapDimensions{subspace.space_dimension(), numeric_cast<size_t>(i), numeric_cast<size_t>(j)});
}

void RationalLinearSubspace::elementaryTransformation(int i, int j, mpq_class c) {
  subspace.affine_image(Variable(numeric_cast<size_t>(i)), c.get_den() * Variable(numeric_cast<size_t>(i)) + c.get_num() * Variable(numeric_cast<size_t>(j)), c.get_den());
}

std::ostream& operator<<(std::ostream& os, const RationalLinearSubspace& self) {
  return Parma_Polyhedra_Library::IO_Operators::operator<<(os, self.subspace);
}

// Explicit instantiations so that implementations are generated for the linker.

template bool RationalLinearSubspace::hasNonZeroNonNegativeVector<RationalLinearSubspace::HAS_NON_ZERO_NON_NEGATIVE_VECTOR_IMPLEMENTATION::DEFAULT>() const;
template bool RationalLinearSubspace::hasNonZeroNonNegativeVector<RationalLinearSubspace::HAS_NON_ZERO_NON_NEGATIVE_VECTOR_IMPLEMENTATION::PPL_POLYHEDRON>() const;
template bool RationalLinearSubspace::hasNonZeroNonNegativeVector<RationalLinearSubspace::HAS_NON_ZERO_NON_NEGATIVE_VECTOR_IMPLEMENTATION::PPL_QUOTIENT>() const;
template bool RationalLinearSubspace::hasNonZeroNonNegativeVector<RationalLinearSubspace::HAS_NON_ZERO_NON_NEGATIVE_VECTOR_IMPLEMENTATION::PPL_MIP>() const;

}  // namespace intervalxt
