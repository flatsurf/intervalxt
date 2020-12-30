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

#include <vector>

#include "../src/impl/rational_linear_subspace.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"

using std::vector;

namespace intervalxt::test {

TEST_CASE("Rational Linear Subspace Correctly Detects Signs of Vectors", "[rational_linear_subspace]") {
  SECTION("hasNonZeroNonNegativeVector()") {
    REQUIRE(!RationalLinearSubspace().hasNonZeroNonNegativeVector());
    REQUIRE(!RationalLinearSubspace::fromEquations(vector<vector<mpq_class>>{}).hasNonZeroNonNegativeVector());
    REQUIRE(!RationalLinearSubspace::fromEquations(vector<vector<mpq_class>>{{}}).hasNonZeroNonNegativeVector());
    REQUIRE(RationalLinearSubspace::fromEquations({{1, 0}}).hasNonZeroNonNegativeVector());
    REQUIRE(!RationalLinearSubspace::fromEquations({{1, 0}, {0, 1}}).hasNonZeroNonNegativeVector());
    REQUIRE(RationalLinearSubspace::fromEquations({{1, -1}}).hasNonZeroNonNegativeVector());
    REQUIRE(!RationalLinearSubspace::fromEquations({{1, 1}}).hasNonZeroNonNegativeVector());

    REQUIRE(!RationalLinearSubspace::fromGenerators(vector<vector<mpq_class>>{}).hasNonZeroNonNegativeVector());
    REQUIRE(!RationalLinearSubspace::fromGenerators(vector<vector<mpq_class>>{{}}).hasNonZeroNonNegativeVector());
    REQUIRE(RationalLinearSubspace::fromGenerators({{0, 1}}).hasNonZeroNonNegativeVector());
    REQUIRE(RationalLinearSubspace::fromGenerators({{0, -1}}).hasNonZeroNonNegativeVector());
    REQUIRE(!RationalLinearSubspace::fromGenerators({{0, 0}}).hasNonZeroNonNegativeVector());
    REQUIRE(!RationalLinearSubspace::fromGenerators({{1, -1}}).hasNonZeroNonNegativeVector());
  }

  SECTION("hasNonZeroNonNegativeVectorViaQuotient()") {
    REQUIRE(!RationalLinearSubspace().hasNonZeroNonNegativeVectorViaQuotient());
    REQUIRE(!RationalLinearSubspace::fromEquations(vector<vector<mpq_class>>{}).hasNonZeroNonNegativeVectorViaQuotient());
    REQUIRE(!RationalLinearSubspace::fromEquations(vector<vector<mpq_class>>{{}}).hasNonZeroNonNegativeVectorViaQuotient());
    REQUIRE(RationalLinearSubspace::fromEquations({{1, 0}}).hasNonZeroNonNegativeVectorViaQuotient());
    REQUIRE(!RationalLinearSubspace::fromEquations({{1, 0}, {0, 1}}).hasNonZeroNonNegativeVectorViaQuotient());
    REQUIRE(RationalLinearSubspace::fromEquations({{1, -1}}).hasNonZeroNonNegativeVectorViaQuotient());
    REQUIRE(!RationalLinearSubspace::fromEquations({{1, 1}}).hasNonZeroNonNegativeVectorViaQuotient());

    REQUIRE(!RationalLinearSubspace::fromGenerators(vector<vector<mpq_class>>{}).hasNonZeroNonNegativeVectorViaQuotient());
    REQUIRE(!RationalLinearSubspace::fromGenerators(vector<vector<mpq_class>>{{}}).hasNonZeroNonNegativeVectorViaQuotient());
    REQUIRE(RationalLinearSubspace::fromGenerators({{0, 1}}).hasNonZeroNonNegativeVectorViaQuotient());
    REQUIRE(RationalLinearSubspace::fromGenerators({{0, -1}}).hasNonZeroNonNegativeVector());
    REQUIRE(!RationalLinearSubspace::fromGenerators({{0, 0}}).hasNonZeroNonNegativeVectorViaQuotient());
    REQUIRE(!RationalLinearSubspace::fromGenerators({{1, -1}}).hasNonZeroNonNegativeVectorViaQuotient());
  }

  SECTION("hasPositiveVector") {
    REQUIRE(!RationalLinearSubspace().hasPositiveVector());
    REQUIRE(!RationalLinearSubspace::fromEquations(vector<vector<mpq_class>>{}).hasPositiveVector());
    REQUIRE(!RationalLinearSubspace::fromEquations({{1, 0}}).hasPositiveVector());
    REQUIRE(!RationalLinearSubspace::fromEquations({{1, 0}, {0, 1}}).hasPositiveVector());
    REQUIRE(RationalLinearSubspace::fromEquations({{1, -1}}).hasPositiveVector());
    REQUIRE(!RationalLinearSubspace::fromEquations({{1, 1}}).hasPositiveVector());

    REQUIRE(!RationalLinearSubspace::fromGenerators(vector<vector<mpq_class>>{}).hasPositiveVector());
    REQUIRE(!RationalLinearSubspace::fromGenerators({{0, 1}}).hasPositiveVector());
    REQUIRE(RationalLinearSubspace::fromGenerators({{1, 1}}).hasPositiveVector());
    REQUIRE(!RationalLinearSubspace::fromGenerators({{1, -1}}).hasPositiveVector());
  }
}

TEST_CASE("Transformations of Rational Linear Subspace", "[rational_linear_subspace]") {
  // The line B := x_1 = 0
  auto S = RationalLinearSubspace::fromEquations({{0, 1}});
  REQUIRE(!S.hasPositiveVector());
  REQUIRE(S.hasNonZeroNonNegativeVector());

  // The line A := x_0 = 0
  S.swap(0, 1);
  REQUIRE(S == RationalLinearSubspace::fromEquations({{1, 0}}));
  REQUIRE(!S.hasPositiveVector());
  REQUIRE(S.hasNonZeroNonNegativeVector());

  // Acting with A ↦ A - B; the line B = -A
  S.elementaryTransformation(0, 1, -1);
  REQUIRE(S == RationalLinearSubspace::fromEquations({{1, 1}}));
  REQUIRE(!S.hasPositiveVector());
  REQUIRE(!S.hasNonZeroNonNegativeVector());

  // Acting with A ↦ A + B; the line A = 0
  S.elementaryTransformation(0, 1, 1);
  REQUIRE(S == RationalLinearSubspace::fromEquations({{1, 0}}));
}

}  // namespace intervalxt::test
