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

namespace intervalxt {
template <typename Length>
bool operator==(const IntervalExchangeTransformation<Length>& lhs, const IntervalExchangeTransformation<Length>& rhs) {
  return boost::lexical_cast<std::string>(lhs) == boost::lexical_cast<std::string>(rhs);
}
}  // namespace intervalxt

// initialisation from lengths and permutation
TEST(IETTest, Initialization1) {
  using Length = Length<int>;
  IntervalExchangeTransformation<Length> iet({Length(18), Length(3), Length(1), Length(1)}, {3, 0, 1, 2});
  auto top = iet.top();
  auto bot = iet.bottom();

  EXPECT_TRUE(bot[0] == top[3]);
  EXPECT_TRUE(bot[1] == top[0]);
  EXPECT_TRUE(bot[2] == top[1]);
  EXPECT_TRUE(bot[3] == top[2]);
}

// initialisation from a pair of list of labels
TEST(IETTest, Initialisation2) {
  using Length = Length<int>;
  Label<Length> l0(3);
  Label<Length> l1(13);
  Label<Length> l2(2);

  IntervalExchangeTransformation<Length> iet({l0, l1, l2}, {l1, l2, l0});

  auto top = iet.top();
  auto bot = iet.bottom();

  EXPECT_EQ(top[0], l0);
  EXPECT_EQ(top[1], l1);
  EXPECT_EQ(top[2], l2);
  EXPECT_EQ(bot[0], l1);
  EXPECT_EQ(bot[1], l2);
  EXPECT_EQ(bot[2], l0);
}

TEST(IETTest, Reduce1) {
  using Length = Length<int>;
  IntervalExchangeTransformation<Length> iet({Length(17), Length(23), Length(33)}, {2, 1, 0});
  EXPECT_FALSE(iet.reduce());
}

TEST(IETTest, Reduce2) {
  using Length = Length<int>;
  IntervalExchangeTransformation<Length> iet({Length(1), Length(1), Length(1)}, {1, 2, 0});
  EXPECT_FALSE(iet.reduce());
}

TEST(IETTest, Reduce3) {
  using Length = Length<int>;
  IntervalExchangeTransformation<Length> iet({Length(17), Length(23), Length(33)}, {1, 0, 2});
  auto r = iet.reduce();
  EXPECT_TRUE(r);
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({Length(17), Length(23)}, {1, 0}));
  EXPECT_EQ(r.value(), IntervalExchangeTransformation<Length>({Length(33)}, std::vector<unsigned long>{0}));
}

TEST(IETTest, Reduce4) {
  using Length = Length<int>;
  IntervalExchangeTransformation<Length> iet({Length(12), Length(3), Length(41), Length(7)}, {1, 0, 3, 2});
  auto r = iet.reduce();
  EXPECT_TRUE(r);
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({Length(12), Length(3)}, {1, 0}));
  EXPECT_EQ(r.value(), IntervalExchangeTransformation<Length>({Length(41), Length(7)}, {1, 0}));
}

#include "main.hpp"
