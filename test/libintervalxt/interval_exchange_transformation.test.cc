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
#include <vector>

#include <intervalxt/interval_exchange_transformation.hpp>

using namespace intervalxt;
using std::vector;

TEST(IETTest, InitializationLengths) {
  IntervalExchangeTransformation<int, int> iet(2);
  iet.setLengths({18, 3});
  EXPECT_EQ(iet.lengths(), (std::vector{18, 3}));
}

TEST(IETTest, InitializationTopBottom) {
  IntervalExchangeTransformation<int, int> iet({0, 1, 3, 2}, {3, 0, 1, 2});
}

TEST(IETTest, IsIrreducible) {
  IntervalExchangeTransformation<int, int> iet(3);
  iet.setBot({2, 1, 0});
  EXPECT_FALSE(iet.isReducible());

  iet.setBot({1, 0, 2});
  EXPECT_TRUE(iet.isReducible());

  iet.setTop({2, 0, 1});
  iet.setBot({0, 2, 1});
  EXPECT_TRUE(iet.isReducible());
}

#include "main.hpp"
