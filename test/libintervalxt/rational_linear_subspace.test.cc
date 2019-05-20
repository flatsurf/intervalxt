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

#include <gtest/gtest.h>
#include <ppl.hh>
#include <vector>

#include <intervalxt/rational_linear_subspace.hpp>

using namespace intervalxt;
using namespace Parma_Polyhedra_Library;
using std::vector;

TEST(RationalLinearSubspaceTest, HasNonZeroNonNegativeVector) {
  EXPECT_FALSE(RationalLinearSubspace().hasNonZeroNonNegativeVector());
  EXPECT_FALSE(RationalLinearSubspace({}).hasNonZeroNonNegativeVector());
  EXPECT_TRUE(RationalLinearSubspace({{1, 0}}).hasNonZeroNonNegativeVector());
  EXPECT_FALSE(RationalLinearSubspace({{1, 0}, {0, 1}}).hasNonZeroNonNegativeVector());
  EXPECT_TRUE(RationalLinearSubspace({{1, -1}}).hasNonZeroNonNegativeVector());
  EXPECT_FALSE(RationalLinearSubspace({{1, 1}}).hasNonZeroNonNegativeVector());
}

TEST(RationalLinearSubspaceTest, HasPositiveVector) {
  EXPECT_FALSE(RationalLinearSubspace().hasPositiveVector());
  EXPECT_FALSE(RationalLinearSubspace({}).hasPositiveVector());
  EXPECT_FALSE(RationalLinearSubspace({{1, 0}}).hasPositiveVector());
  EXPECT_FALSE(RationalLinearSubspace({{1, 0}, {0, 1}}).hasPositiveVector());
  EXPECT_TRUE(RationalLinearSubspace({{1, -1}}).hasPositiveVector());
  EXPECT_FALSE(RationalLinearSubspace({{1, 1}}).hasPositiveVector());
}

TEST(RationalLinearSubspaceTest, Transformation) {
  // The line B := x_1 = 0
  auto S = RationalLinearSubspace({{0, 1}});
  EXPECT_FALSE(S.hasPositiveVector());
  EXPECT_TRUE(S.hasNonZeroNonNegativeVector());

  // The line A := x_0 = 0
  S.swap(0, 1);
  EXPECT_EQ(S, RationalLinearSubspace({{1, 0}}));
  EXPECT_FALSE(S.hasPositiveVector());
  EXPECT_TRUE(S.hasNonZeroNonNegativeVector());

  // Acting with A ↦ A - B; the line B = -A
  S.elementaryTransformation(0, 1, -1);
  EXPECT_EQ(S, RationalLinearSubspace({{1, 1}}));
  EXPECT_FALSE(S.hasPositiveVector());
  EXPECT_FALSE(S.hasNonZeroNonNegativeVector());

  // Acting with A ↦ A + B; the line A = 0
  S.elementaryTransformation(0, 1, 1);
  EXPECT_EQ(S, RationalLinearSubspace({{1, 0}}));
}

#include "main.hpp"
