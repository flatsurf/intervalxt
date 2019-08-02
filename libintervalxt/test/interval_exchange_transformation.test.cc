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
#include <intervalxt/label.hpp>
#include <intervalxt/length.hpp>

using namespace intervalxt;
using std::vector;

TEST(IETTest, Initialization) {
  using Length = Length<int>;
  IntervalExchangeTransformation<Length> iet({Length(18), Length(3), Length(1), Length(1)}, {3, 0, 1, 2});
}

TEST(IETTest, Reduction) {
  using Length = Length<int>;
  IntervalExchangeTransformation<Length> iet({Length(1), Length(1), Length(1)}, {2, 1, 0});
  EXPECT_FALSE(iet.reduce());

  iet = IntervalExchangeTransformation<Length>({Length(1), Length(1), Length(1)}, {1, 0, 2});
  EXPECT_TRUE(iet.reduce());

  iet = IntervalExchangeTransformation<Length>({Length(1), Length(1), Length(1)}, {1, 2, 0});
  EXPECT_FALSE(iet.reduce());
}

#include "main.hpp"
