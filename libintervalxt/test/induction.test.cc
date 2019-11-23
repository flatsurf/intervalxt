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

#include <e-antic/renfxx.h>

#include <intervalxt/induction_step.hpp>
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
  using Label = Label<Length>;
  auto a = Label(23);
  auto b = Label(5);

  IntervalExchangeTransformation<Length> iet({a, b}, {1, 0});

  iet.zorichInduction();
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({a = 23 - 4 * 5, b}, {1, 0}));
}

TEST(InductionTest, Induction3) {
  using Length = Length<int>;
  using Label = Label<Length>;
  auto a = Label(13);
  auto b = Label(1);

  IntervalExchangeTransformation<Length> iet({a, b}, {1, 0});
  iet.zorichInduction();
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({a = 1, b = 1}, {1, 0}));
}

TEST(InductionTest, Induction4) {
  using Length = Length<int>;
  using Label = Label<Length>;

  // 0 3 1 2
  // 1 2 0 3
  Label l0(15), l1(2), l2(3), l3(7);
  IntervalExchangeTransformation<Length> iet({l0, l3, l1, l2}, {l1, l2, l0, l3});
  iet.zorichInduction();
  EXPECT_EQ(iet.top(), std::vector<Label>({l0, l3, l1, l2}));
  EXPECT_EQ(iet.bottom(), std::vector<Label>({l2, l1, l0, l3}));
  EXPECT_EQ(iet.top().begin()->length(), 3);
}

TEST(InductionTest, InductionMpzClass) {
  using Length = Length<mpz_class>;
  using Label = Label<Length>;
  Label a = Length(mpz_class("14328748557375491835455123393141239398243"));
  Label b = Length(mpz_class("51123145748597134"));

  IntervalExchangeTransformation<Length> iet({a, b}, {1, 0});

  iet.zorichInduction();
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({a = Length(mpz_class("15560595195676063")), b = Length(mpz_class("51123145748597134"))}, {1, 0}));
}

TEST(InductionTest, InductionMpqClass) {
  using Length = Length<mpq_class>;
  using Label = Label<Length>;
  Label a = Length(mpq_class("3/5"));
  Label b = Length(mpq_class("1/4"));

  IntervalExchangeTransformation<Length> iet({a, b}, {1, 0});

  iet.zorichInduction();
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({a = Length(mpq_class("1/10")), b = Length(mpq_class("1/4"))}, {1, 0}));
}

TEST(InductionTest, InductionRenfElemClass) {
  using Length = Length<eantic::renf_elem_class>;
  using Label = Label<Length>;

  auto K = eantic::renf_class::make("a^2 - a - 1", "a", "1.618 +/- 0.01");

  eantic::renf_elem_class a(K, "a");
  eantic::renf_elem_class b(K, 1);
  Label aa = Length(a);
  Label bb = Length(b);

  IntervalExchangeTransformation<Length> iet({aa, bb}, {1, 0});

  iet.zorichInduction();
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({aa = a - 1, bb}, {1, 0}));
}

TEST(InductionTest, Induction5) {
  using Length = Length<int>;
  using Label = Label<Length>;
  Label a(977), b(351), c(143), d(321), e(12);

  IntervalExchangeTransformation<Length> iet({a, b, c, d, e}, {3, 2, 0, 4, 1});

  iet.zorichInduction();
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({a = 49, b, c, d, e}, {3, 2, 0, 4, 1}));

  iet.swap();
  iet.zorichInduction();
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({d = 272, c, a, e, b}, {4, 1, 2, 0, 3}));

  iet.swap();
  iet.zorichInduction();
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({b = 79, c, a, d, e}, {1, 2, 4, 3, 0}));

  iet.swap();
  iet.zorichInduction();
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({c = 64, a, e, d, b}, {4, 0, 1, 3, 2}));

  iet.swap();
  iet.zorichInduction();
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({b = 15, c, a, d, e}, {2, 4, 3, 1, 0}));

  iet.swap();
  iet.zorichInduction();
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({a = 34, e, d, c, b}, {3, 4, 0, 2, 1}));
}

TEST(InductionTest, InductionAndReport1) {
  using Length = Length<int>;
  using Result = InductionStep<Length>::Result;

  IntervalExchangeTransformation<Length> iet({Length(1), Length(2), Length(3)}, {2, 1, 0});
  EXPECT_EQ(iet.induce(0).result, Result::LIMIT_REACHED);
}

TEST(InductionTest, InductionAndReport2) {
  using Length = Length<int>;
  using Label = Label<Length>;
  using Result = InductionStep<Length>::Result;
  Label a(1), b(1), c(1);

  IntervalExchangeTransformation<Length> iet({a, b, c}, {a, c, b});
  auto r = iet.induce(0);
  EXPECT_EQ(r.result, Result::CYLINDER);
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({b, c}, {1, 0}));
  EXPECT_EQ(r.cylinder, a);
}

TEST(InductionTest, InductionAndReport3) {
  using Length = Length<int>;
  using Label = Label<Length>;
  using Result = InductionStep<Length>::Result;

  Label l0(1), l1(1), l2(1);
  IntervalExchangeTransformation<Length> iet({l0, l1, l2}, {l2, l1, l0});

  auto r = iet.induce(0);

  EXPECT_EQ(r.result, Result::NON_SEPARATING_CONNECTION);
  EXPECT_EQ(r.connection->first, l0);
  EXPECT_EQ(r.connection->second, l2);
}

TEST(InductionTest, InductionAndReport4) {
  using Length = Length<int>;
  using Label = Label<Length>;
  using Result = InductionStep<Length>::Result;

  // 0 1 2 3 4 5
  // 3 2 1 5 4 0
  // ->
  // label 0 gets dropped and we have the two IETs
  // 1 2  and  3 4 5
  // 2 1       5 4 3
  Label l0(1), l1(2), l2(3), l3(1), l4(5), l5(7);
  IntervalExchangeTransformation<Length> iet({l0, l1, l2, l3, l4, l5}, {l3, l2, l1, l5, l4, l0});

  auto r = iet.induce(0);
  EXPECT_EQ(r.result, Result::NON_SEPARATING_CONNECTION);

  r = iet.induce(0);
  EXPECT_EQ(r.result, Result::SEPARATING_CONNECTION);
  EXPECT_EQ(iet.top(), std::vector<Label>({l1, l2}));
  EXPECT_EQ(iet.bottom(), std::vector<Label>({l2, l1}));
  EXPECT_EQ(r.additionalIntervalExchangeTransformation->top(), std::vector<Label>({l3, l4, l5}));
  EXPECT_EQ(r.additionalIntervalExchangeTransformation->bottom(), std::vector<Label>({l5, l4, l3}));
}

TEST(InductionTest, InductionAndReport5) {
  using Length = Length<int>;
  using Result = InductionStep<Length>::Result;

  auto a = Label<Length>(1);
  auto b = Label<Length>(1);
  auto c = Label<Length>(1);
  IntervalExchangeTransformation<Length> iet({a, b, c}, {2, 0, 1});

  auto step0 = iet.induce(0);
  EXPECT_EQ(step0.result, Result::NON_SEPARATING_CONNECTION);
  EXPECT_EQ(step0.connection->first, a);
  EXPECT_EQ(step0.connection->second, c);

  auto step1 = iet.induce(0);
  EXPECT_EQ(step1.result, Result::NON_SEPARATING_CONNECTION);
  EXPECT_EQ(step1.connection->first, b);
  EXPECT_EQ(step1.connection->second, c);

  auto step2 = iet.induce(0);
  EXPECT_EQ(step2.result, Result::CYLINDER);
}

TEST(InductionTest, InductionAndReport6) {
  using Length = Length<int>;
  using Label = Label<Length>;
  using Result = InductionStep<Length>::Result;

  Label l0(13);
  Label l1(5);
  IntervalExchangeTransformation<Length> iet({l0, l1}, {l1, l0});
  auto step0 = iet.induce(0);
  EXPECT_EQ(step0.result, Result::LIMIT_REACHED);
  auto step1 = iet.induce(1);
  EXPECT_EQ(step1.result, Result::LIMIT_REACHED);
  auto step2 = iet.induce(1);
  EXPECT_NE(step2.result, Result::LIMIT_REACHED);
}

}  // namespace

#include "main.hpp"
