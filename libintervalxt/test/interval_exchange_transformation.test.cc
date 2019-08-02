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
} // namespace intervalxt

TEST(IETTest, Initialization) {
  using Length = Length<int>;
  IntervalExchangeTransformation<Length> iet({Length(18), Length(3), Length(1), Length(1)}, {3, 0, 1, 2});
  EXPECT_TRUE(iet.bottom()[0] == iet.top()[3]);
  EXPECT_TRUE(iet.bottom()[1] == iet.top()[0]);
  EXPECT_TRUE(iet.bottom()[2] == iet.top()[1]);
  EXPECT_TRUE(iet.bottom()[3] == iet.top()[2]);
}

TEST(IETTest, Reduction) {
  using Length = Length<int>;
  IntervalExchangeTransformation<Length> iet({Length(17), Length(23), Length(33)}, {2, 1, 0});
  EXPECT_FALSE(iet.reduce());

  iet = IntervalExchangeTransformation<Length>({Length(17), Length(23), Length(33)}, {1, 0, 2});
  auto r = iet.reduce();
  EXPECT_TRUE(r);
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({Length(17), Length(23)}, {1,0}));
  EXPECT_EQ(r.value(), IntervalExchangeTransformation<Length>({Length(33)}, std::vector<size_t> {0}));

  iet = IntervalExchangeTransformation<Length>({Length(1), Length(1), Length(1)}, {1, 2, 0});
  EXPECT_FALSE(iet.reduce());
}

#include "main.hpp"
