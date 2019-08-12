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
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({Length(23 - 4 * 5), Length(5)}, {1, 0}));
}

TEST(InductionTest, Induction3) {
  using Length = Length<int>;

  IntervalExchangeTransformation<Length> iet({Length(13), Length(1)}, {1, 0});
  iet.zorichInduction();
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({Length(1), Length(1)}, {1, 0}));
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

  IntervalExchangeTransformation<Length> iet({Length("14328748557375491835455123393141239398243"), Length("51123145748597134")}, {1, 0});

  iet.zorichInduction();
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({Length("15560595195676063"), Length("51123145748597134")}, {1, 0}));
}

TEST(InductionTest, InductionMpqClass) {
  using Length = Length<mpq_class>;

  IntervalExchangeTransformation<Length> iet({Length("3/5"), Length("1/4")}, {1, 0});

  iet.zorichInduction();
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({Length("1/10"), Length("1/4")}, {1, 0}));
}

TEST(InductionTest, InductionRenfElemClass) {
  using Length = Length<eantic::renf_elem_class>;

  auto K = eantic::renf_class::make("a^2 - a - 1", "a", "1.618 +/- 0.01");

  eantic::renf_elem_class a(K, "a");
  eantic::renf_elem_class b(K, 1);

  IntervalExchangeTransformation<Length> iet({Length(a), Length(b)}, {1, 0});

  iet.zorichInduction();
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({Length(a - 1), Length(b)}, {1, 0}));
}

TEST(InductionTest, Induction5) {
  using Length = Length<int>;

  IntervalExchangeTransformation<Length> iet({Length(977), Length(351), Length(143), Length(321), Length(12)}, {3, 2, 0, 4, 1});

  iet.zorichInduction();
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({Length(49), Length(351), Length(143), Length(321), Length(12)}, {3, 2, 0, 4, 1}));

  iet.swap();
  iet.zorichInduction();
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({Length(272), Length(143), Length(49), Length(12), Length(351)}, {4, 1, 2, 0, 3}));

  iet.swap();
  iet.zorichInduction();
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({Length(79), Length(143), Length(49), Length(272), Length(12)}, {1, 2, 4, 3, 0}));

  iet.swap();
  iet.zorichInduction();
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({Length(64), Length(49), Length(12), Length(272), Length(79)}, {4, 0, 1, 3, 2}));

  iet.swap();
  iet.zorichInduction();
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({Length(15), Length(64), Length(49), Length(272), Length(12)}, {2, 4, 3, 1, 0}));

  iet.swap();
  iet.zorichInduction();
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({Length(34), Length(12), Length(272), Length(64), Length(15)}, {3, 4, 0, 2, 1}));
}

TEST(InductionTest, InductionAndReport1) {
  using Length = Length<int>;

  IntervalExchangeTransformation<Length> iet({Length(1), Length(2), Length(3)}, {2, 1, 0});
  EXPECT_FALSE(iet.induce(0));
}

TEST(InductionTest, InductionAndReport2) {
  using Length = Length<int>;
  using Label = Label<Length>;

  Label l0(1), l1(1), l2(1);
  IntervalExchangeTransformation<Length> iet({l0, l1, l2}, {l0, l2, l1});
  auto r = iet.induce(0);
  EXPECT_TRUE(r);
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({Length(1), Length(1)}, {1, 0}));

  EXPECT_TRUE(std::holds_alternative<Cylinder<Length>>(r.value()));
  auto u = &std::get<Cylinder<Length>>(r.value());
  EXPECT_EQ(u->label, l0);
}

TEST(InductionTest, InductionAndReport3) {
  using Length = Length<int>;
  using Label = Label<Length>;

  Label l0(1), l1(1), l2(1);
  IntervalExchangeTransformation<Length> iet({l0, l1, l2}, {l2, l1, l0});

  auto r = iet.induce(0);

  EXPECT_TRUE(r);
  EXPECT_TRUE(std::holds_alternative<SeparatingConnection<Length>>(r.value()));
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({Length(1)}, std::vector<unsigned long>({0})));

  auto u = &std::get<SeparatingConnection<Length>>(r.value());

  EXPECT_EQ(u->top, l0);
  EXPECT_EQ(u->bottom, l2);
  EXPECT_EQ(*(u->addedIET), IntervalExchangeTransformation<Length>({Length(1)}, std::vector<unsigned long>({0})));
}

TEST(InductionTest, InductionAndReport4) {
  using Length = Length<int>;
  using Label = Label<Length>;

  // 0 1 2 3 4 5
  // 3 2 1 5 4 0
  // ->
  // label 0 get dropped and we have the two iet
  // 1 2  and  3 4 5
  // 2 1       5 4 3
  Label l0(1), l1(2), l2(3), l3(1), l4(5), l5(7);
  IntervalExchangeTransformation<Length> iet({l0, l1, l2, l3, l4, l5}, {l3, l2, l1, l5, l4, l0});

  auto r = iet.induce(0);

  EXPECT_TRUE(r);
  EXPECT_TRUE(std::holds_alternative<SeparatingConnection<Length>>(r.value()));
  EXPECT_EQ(iet.top(), std::vector<Label>({l1, l2}));
  EXPECT_EQ(iet.bottom(), std::vector<Label>({l2, l1}));

  auto u = &std::get<SeparatingConnection<Length>>(r.value());
  EXPECT_EQ(u->addedIET->top(), std::vector<Label>({l3, l4, l5}));
  EXPECT_EQ(u->addedIET->bottom(), std::vector<Label>({l5, l4, l3}));
}

TEST(InductionTest, InductionAndReport5) {
  using Length = Length<int>;
  using Label = Label<Length>;

  Label l0(1), l1(3), l2(1);
  IntervalExchangeTransformation<Length> iet({l0, l1, l2}, {l2, l0, l1});
  auto r = iet.induce(0);
  EXPECT_TRUE(r);
  EXPECT_TRUE(std::holds_alternative<NonSeparatingConnection<Length>>(r.value()));
  EXPECT_EQ(iet, IntervalExchangeTransformation<Length>({Length(3), Length(1)}, {1, 0}));

  auto u = &std::get<NonSeparatingConnection<Length>>(r.value());
  EXPECT_EQ(u->top, l0);
  EXPECT_EQ(u->bottom, l2);
}

TEST(InductionTest, InductionAndReport6) {
  using Length = Length<int>;
  using Label = Label<Length>;

  Label l0(13);
  Label l1(5);
  IntervalExchangeTransformation<Length> iet({l0, l1}, {l1, l0});
  auto r = iet.induce(0);
  EXPECT_FALSE(r);
  r = iet.induce(1);
  EXPECT_FALSE(r);
  r = iet.induce(1);
  EXPECT_TRUE(r);
}

}  // namespace

#include "main.hpp"
