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
#include <boost/lexical_cast.hpp>
#include <vector>

#include <intervalxt/interval_exchange_transformation.hpp>
#include <intervalxt/label.hpp>
#include <intervalxt/length.hpp>

using namespace intervalxt;

namespace intervalxt {
template <typename Length>
bool operator==(const IntervalExchangeTransformation<Length>& lhs, const IntervalExchangeTransformation<Length>& rhs) {
  return boost::lexical_cast<std::string>(lhs) == boost::lexical_cast<std::string>(rhs);
}
}  // namespace intervalxt

namespace {

using v = std::vector<int>;

TEST(InductionTest, Induction2) {
  using Length = Length<int>;

  IntervalExchangeTransformation<Length> iet({Length(23), Length(5)}, {1, 0});

  iet.zorichInduction();
  EXPECT_EQ(IntervalExchangeTransformation<Length>({Length(23 - 4 * 5), Length(5)}, {1, 0}), iet);
}

TEST(InductionTest, InductionMpzClass) {
  using Length = Length<mpz_class>;

  IntervalExchangeTransformation<Length> iet({Length("14328748557375491835455123393141239398243"), Length("51123145748597134")}, {1, 0});

  iet.zorichInduction();
  EXPECT_EQ(IntervalExchangeTransformation<Length>({Length("15560595195676063"), Length("51123145748597134")}, {1, 0}), iet);
}

TEST(InductionTest, InductionMpqClass) {
  using Length = Length<mpq_class>;

  IntervalExchangeTransformation<Length> iet({Length("3/5"), Length("1/4")}, {1, 0});

  iet.zorichInduction();
  EXPECT_EQ(IntervalExchangeTransformation<Length>({Length("1/10"), Length("1/4")}, {1, 0}), iet);
}

TEST(InductionTest, InductionRenfElemClass) {
  using Length = Length<eantic::renf_elem_class>;

  auto K = eantic::renf_class::make("a^2 - a - 1", "a", "1.618 +/- 0.01");

  eantic::renf_elem_class a(K, "a");
  eantic::renf_elem_class b(K, 1);

  IntervalExchangeTransformation<Length> iet({Length(a), Length(b)}, {1, 0});

  iet.zorichInduction();
  EXPECT_EQ(IntervalExchangeTransformation<Length>({Length(a - 1), Length(b)}, {1, 0}), iet);
}

TEST(InductionTest, Induction5) {
  using Length = Length<int>;

  IntervalExchangeTransformation<Length> iet({Length(977), Length(351), Length(143), Length(321), Length(12)}, {3, 2, 0, 4, 1});

  iet.zorichInduction();
  EXPECT_EQ(IntervalExchangeTransformation<Length>({Length(49), Length(351), Length(143), Length(321), Length(12)}, {3, 2, 0, 4, 1}), iet);

  iet.swap();
  iet.zorichInduction();
  EXPECT_EQ(IntervalExchangeTransformation<Length>({Length(272), Length(143), Length(49), Length(12), Length(351)}, {4, 1, 2, 0, 3}), iet);

  iet.swap();
  iet.zorichInduction();
  EXPECT_EQ(IntervalExchangeTransformation<Length>({Length(79), Length(143), Length(49), Length(272), Length(12)}, {1, 2, 4, 3, 0}), iet);

  iet.swap();
  iet.zorichInduction();
  EXPECT_EQ(IntervalExchangeTransformation<Length>({Length(64), Length(49), Length(12), Length(272), Length(79)}, {4, 0, 1, 3, 2}), iet);

  iet.swap();
  iet.zorichInduction();
  EXPECT_EQ(IntervalExchangeTransformation<Length>({Length(15), Length(64), Length(49), Length(272), Length(12)}, {2, 4, 3, 1, 0}), iet);

  iet.swap();
  iet.zorichInduction();
  EXPECT_EQ(IntervalExchangeTransformation<Length>({Length(34), Length(12), Length(272), Length(64), Length(15)}, {3, 4, 0, 2, 1}), iet);
}

}  // namespace

#include "main.hpp"
