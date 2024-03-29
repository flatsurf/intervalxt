/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019-2020 Vincent Delecroix
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

#include <vector>

#include "../src/impl/rational_affine_subspace.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"

using std::vector;

namespace intervalxt::test {

TEMPLATE_TEST_CASE("Rational Linear Subspace Correctly Detects Signs of Vectors", "[rational_affine_subspace]", (std::integral_constant<RationalAffineSubspace::HAS_NON_ZERO_NON_NEGATIVE_VECTOR_IMPLEMENTATION, RationalAffineSubspace::HAS_NON_ZERO_NON_NEGATIVE_VECTOR_IMPLEMENTATION::PPL_POLYHEDRON>), (std::integral_constant<RationalAffineSubspace::HAS_NON_ZERO_NON_NEGATIVE_VECTOR_IMPLEMENTATION, RationalAffineSubspace::HAS_NON_ZERO_NON_NEGATIVE_VECTOR_IMPLEMENTATION::PPL_QUOTIENT>), (std::integral_constant<RationalAffineSubspace::HAS_NON_ZERO_NON_NEGATIVE_VECTOR_IMPLEMENTATION, RationalAffineSubspace::HAS_NON_ZERO_NON_NEGATIVE_VECTOR_IMPLEMENTATION::PPL_MIP>)) {
  constexpr auto IMPLEMENTATION = TestType::value;

  SECTION("hasNonZeroNonNegativeVector()") {
    REQUIRE(!RationalAffineSubspace().hasNonZeroNonNegativeVector<IMPLEMENTATION>());
    REQUIRE(!RationalAffineSubspace(vector<vector<mpq_class>>{}, vector<mpq_class>{}).hasNonZeroNonNegativeVector<IMPLEMENTATION>());
    REQUIRE(!RationalAffineSubspace(vector<vector<mpq_class>>{{}}, {0}).hasNonZeroNonNegativeVector<IMPLEMENTATION>());
    REQUIRE(RationalAffineSubspace({{1, 0}}, {0}).hasNonZeroNonNegativeVector<IMPLEMENTATION>());
    REQUIRE(!RationalAffineSubspace({{1, 0}, {0, 1}}, {0, 0}).hasNonZeroNonNegativeVector<IMPLEMENTATION>());
    REQUIRE(RationalAffineSubspace({{1, -1}}, {0}).hasNonZeroNonNegativeVector<IMPLEMENTATION>());
    REQUIRE(!RationalAffineSubspace({{1, 1}}, {0}).hasNonZeroNonNegativeVector<IMPLEMENTATION>());

    REQUIRE(!RationalAffineSubspace(vector<vector<mpq_class>>{}).hasNonZeroNonNegativeVector<IMPLEMENTATION>());
    REQUIRE(!RationalAffineSubspace(vector<vector<mpq_class>>{{}}).hasNonZeroNonNegativeVector<IMPLEMENTATION>());
    REQUIRE(RationalAffineSubspace({{0, 1}}).hasNonZeroNonNegativeVector<IMPLEMENTATION>());
    REQUIRE(RationalAffineSubspace({{0, -1}}).hasNonZeroNonNegativeVector<IMPLEMENTATION>());
    REQUIRE(!RationalAffineSubspace({{0, 0}}).hasNonZeroNonNegativeVector<IMPLEMENTATION>());
    REQUIRE(!RationalAffineSubspace({{1, -1}}).hasNonZeroNonNegativeVector<IMPLEMENTATION>());
  }
}

TEST_CASE("Rational Linear Subspace Correctly Detects Signs of Vectors", "[rational_affine_subspace]") {
  SECTION("hasPositiveVector") {
    REQUIRE(!RationalAffineSubspace().hasPositiveVector());
    REQUIRE(!RationalAffineSubspace(vector<vector<mpq_class>>{}, vector<mpq_class>{}).hasPositiveVector());
    REQUIRE(!RationalAffineSubspace({{1, 0}}, {0}).hasPositiveVector());
    REQUIRE(!RationalAffineSubspace({{1, 0}, {0, 1}}, {0, 0}).hasPositiveVector());
    REQUIRE(RationalAffineSubspace({{1, -1}}, {0}).hasPositiveVector());
    REQUIRE(!RationalAffineSubspace({{1, 1}}, {0}).hasPositiveVector());

    REQUIRE(!RationalAffineSubspace(vector<vector<mpq_class>>{}).hasPositiveVector());
    REQUIRE(!RationalAffineSubspace({{0, 1}}).hasPositiveVector());
    REQUIRE(RationalAffineSubspace({{1, 1}}).hasPositiveVector());
    REQUIRE(!RationalAffineSubspace({{1, -1}}).hasPositiveVector());
  }
}

TEST_CASE("Transformations of Rational Linear Subspace", "[rational_affine_subspace]") {
  // The line B := x_1 = 0
  auto S = RationalAffineSubspace({{0, 1}}, {0});
  REQUIRE(!S.hasPositiveVector());
  REQUIRE(S.hasNonZeroNonNegativeVector());

  // The line A := x_0 = 0
  S.swap(0, 1);
  REQUIRE(S == RationalAffineSubspace({{1, 0}}, {0}));
  REQUIRE(!S.hasPositiveVector());
  REQUIRE(S.hasNonZeroNonNegativeVector());

  // Acting with A ↦ A - B; the line B = -A
  S.elementaryTransformation(0, 1, -1);
  REQUIRE(S == RationalAffineSubspace({{1, 1}}, {0}));
  REQUIRE(!S.hasPositiveVector());
  REQUIRE(!S.hasNonZeroNonNegativeVector());

  // Acting with A ↦ A + B; the line A = 0
  S.elementaryTransformation(0, 1, 1);
  REQUIRE(S == RationalAffineSubspace({{1, 0}}, {0}));
}

TEST_CASE("Inhomogeneous Affine Linear Subspace", "[rational_affine_subspace]") {
  SECTION("hasNonZeroNonNegativeVector") {
    auto space = RationalAffineSubspace({{1}}, {1});
    REQUIRE_THROWS(space.hasNonZeroNonNegativeVector());
  }

  SECTION("hasPositiveVector") {
    REQUIRE(RationalAffineSubspace({{1}}, {1}).hasPositiveVector());
    REQUIRE(!RationalAffineSubspace({{1}}, {-1}).hasPositiveVector());
  }

  SECTION("hasNonNegativeVector") {
    REQUIRE(RationalAffineSubspace({{1}}, {1}).hasNonNegativeVector());
    REQUIRE(!RationalAffineSubspace({{1}}, {-1}).hasNonNegativeVector());
    REQUIRE(RationalAffineSubspace({{1}}, {0}).hasNonNegativeVector());
  }
}

}  // namespace intervalxt::test
