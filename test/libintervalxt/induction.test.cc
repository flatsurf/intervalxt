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
#include <boost/lexical_cast.hpp>

#include <intervalxt/interval_exchange_transformation.hpp>
#include <intervalxt/length.hpp>
#include <intervalxt/label.hpp>

using namespace intervalxt;

namespace intervalxt {
template <typename Label>
bool operator==(const IntervalExchangeTransformation<Label>& lhs, const IntervalExchangeTransformation<Label>& rhs) {
  return boost::lexical_cast<std::string>(lhs) == boost::lexical_cast<std::string>(rhs);
}
}

namespace {

using v = std::vector<int>;

TEST(InductionTest, Induction2) {
  using IET = IntervalExchangeTransformation<Label<int>>;
  using Length = Length<int>;

  IET iet({Length(23), Length(5)}, {1, 0});

  iet.zorichInduction();
  EXPECT_EQ(IET({Length(23 - 4 * 5), Length(5)}, {1, 0}), iet);
}

TEST(InductionTest, Induction5) {
  using IET = IntervalExchangeTransformation<Label<int>>;
  using Length = Length<int>;

  IET iet({Length(977), Length(351), Length(143), Length(321), Length(12)}, {3, 2, 0, 4, 1});

  iet.zorichInduction();
  EXPECT_EQ(IET({Length(49), Length(351), Length(143), Length(321), Length(12)}, {3, 2, 0, 4, 1}), iet);

  iet.swap();
  iet.zorichInduction();
  EXPECT_EQ(IET({Length(272), Length(143), Length(49), Length(12), Length(351)}, {4, 1, 2, 0, 3}), iet);

  iet.swap();
  iet.zorichInduction();
  EXPECT_EQ(IET({Length(79), Length(143), Length(49), Length(272), Length(12)}, {1, 2, 4, 3, 0}), iet);

  iet.swap();
  iet.zorichInduction();
  EXPECT_EQ(IET({Length(64), Length(49), Length(12), Length(272), Length(79)}, {4, 0, 1, 3, 2}), iet);

  iet.swap();
  iet.zorichInduction();
  EXPECT_EQ(IET({Length(15), Length(64), Length(49), Length(272), Length(12)}, {2, 4, 3, 1, 0}), iet);

  iet.swap();
  iet.zorichInduction();
  EXPECT_EQ(IET({Length(34), Length(12), Length(272), Length(64), Length(15)}, {3, 4, 0, 2, 1}), iet);
}

}  // namespace

#include "main.hpp"
