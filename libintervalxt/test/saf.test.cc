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

TEST(IETTest, SAFRational) {
  using Length = Length<int>;
  IntervalExchangeTransformation<Length> iet({Length(18), Length(3), Length(1), Length(1)}, {3, 0, 1, 2});
  std::valarray<mpq_class> v = iet.safInvariant();
}

TEST(IETTest, NonZeroSAF1) {
  using Length = Length<eantic::renf_elem_class>;

  auto K = eantic::renf_class::make("a^3 - 2*a^2 - 3*a - 1", "a", "3.07 +/- 0.01");
  auto a = K->gen();
  auto one = K->one();

  IntervalExchangeTransformation<Length> iet({Length(a / 2), Length(a * a - mpq_class(1, 13)), Length(4 * a / 7 - 1), Length(2 * one)}, {3, 2, 1, 0});

  std::valarray<mpq_class> v = iet.safInvariant();
  EXPECT_TRUE(v.min() != 0 || v.max() != 0);

  int sign = -1;

  for (int i = 0; i < 10; i++) {
    iet.zorichInduction();
    iet.swap();
    std::valarray<mpq_class> vv = sign * iet.safInvariant();
    EXPECT_TRUE(std::equal(std::begin(v), std::end(v), std::begin(vv), std::end(vv)));
    sign *= -1;
  }
}

TEST(IETTest, NonZeroSAF2) {
  using Length = Length<eantic::renf_elem_class>;

  auto K = eantic::renf_class::make("5*a^3 - 2*a^2 - 3*a - 13", "a", "1.68 +/- 0.01");
  auto a = K->gen();
  auto one = K->one();

  IntervalExchangeTransformation<Length> iet({Length(2 * a / 17), Length(5 * a * a / 3 - mpq_class(1, 18)), Length(5 * a * a / 2 - 3), Length(3 * one)}, {3, 2, 1, 0});
  std::valarray<mpq_class> v = iet.safInvariant();
  EXPECT_TRUE(v.min() != 0 || v.max() != 0);

  int sign = -1;

  for (int i = 0; i < 10; i++) {
    iet.zorichInduction();
    iet.swap();
    std::valarray<mpq_class> vv = sign * iet.safInvariant();
    EXPECT_TRUE(std::equal(std::begin(v), std::end(v), std::begin(vv), std::end(vv)));
    sign *= -1;
  }
}

TEST(IETTest, SAFArnouxYoccoz3) {
  using Length = Length<eantic::renf_elem_class>;

  auto K = eantic::renf_class::make("a^3 - a^2 - a - 1", "a", "1.84 +/- 0.01");
  auto a = K->gen();
  auto one = K->one();

  IntervalExchangeTransformation<Length> iet({Length(a + 1), Length(a * a - a - 1), Length(a * a), Length(a), Length(a), Length(one), Length(one)}, {1, 4, 3, 6, 5, 2, 0});
  std::valarray<mpq_class> v = iet.safInvariant();
  EXPECT_EQ(v.min(), mpq_class(0));
  EXPECT_EQ(v.max(), mpq_class(0));

  for (int i = 0; i < 10; i++) {
    iet.zorichInduction();
    iet.swap();
    std::valarray<mpq_class> vv = iet.safInvariant();
    EXPECT_TRUE(std::equal(std::begin(v), std::end(v), std::begin(vv), std::end(vv)));
  }
}

TEST(IETTest, SAFArnouxYoccoz4) {
  using Length = Length<eantic::renf_elem_class>;

  auto K = eantic::renf_class::make("a^4 - a^3 - a^2 - a - 1", "a", "1.92 +/- 0.01");
  auto a = K->gen();
  auto one = K->one();

  IntervalExchangeTransformation<Length> iet({Length(a.pow(4) - a.pow(3)),
                                              Length(2 * a.pow(3) - a.pow(4)),
                                              Length(a.pow(3)), Length(a.pow(2)), Length(a.pow(2)),
                                              Length(a), Length(a), Length(one), Length(one)},
                                             {1, 4, 3, 6, 5, 8, 7, 2, 0});
  std::valarray<mpq_class> v = iet.safInvariant();
  EXPECT_EQ(v.min(), mpq_class(0));
  EXPECT_EQ(v.max(), mpq_class(0));

  for (int i = 0; i < 10; i++) {
    iet.zorichInduction();
    iet.swap();
    std::valarray<mpq_class> vv = iet.safInvariant();
    EXPECT_TRUE(std::equal(std::begin(v), std::end(v), std::begin(vv), std::end(vv)));
  }
}

#include "main.hpp"
