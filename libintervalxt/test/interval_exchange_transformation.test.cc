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

#include <vector>

#include <e-antic/renfxx.h>

#include "external/catch2/single_include/catch2/catch.hpp"

#include "../intervalxt/interval_exchange_transformation.hpp"
#include "../intervalxt/label.hpp"
#include "../intervalxt/sample/lengths.hpp"
#include "../intervalxt/sample/exact-real-arithmetic.hpp"
#include "../intervalxt/sample/e-antic-arithmetic.hpp"
#include "../intervalxt/sample/rational-arithmetic.hpp"
#include "../intervalxt/induction_step.hpp"

using std::vector;
using std::pair;

namespace intervalxt::test {

using IntLengths = sample::Lengths<int>;

IntervalExchangeTransformation IET(Lengths lengths, const std::vector<Label>& top, const std::vector<Label>& bottom) {
  return IntervalExchangeTransformation(std::make_shared<Lengths>(lengths), top, bottom);
}

TEST_CASE("Basic Operations on an Interval Exchange Transformation", "[interval_exchange_transformation]") {
  auto&& [lengths, a, b, c, d] = IntLengths::make(18, 3, 1, 1);

  SECTION("Initialization Works") {
    WHEN("Initialized from Labels") {
      auto iet = IET(lengths, {a, b, c, d}, {d, a, b, c});

      REQUIRE(iet.top() == vector{a, b, c, d});
      REQUIRE(iet.bottom() == vector{d, a, b, c});
    }

    WHEN("Initialized from Permutation") {
      auto iet = IET(lengths, {a, b, c, d}, {d, a , b, c});

      REQUIRE(iet.top() == vector{a, b, c, d});
      REQUIRE(iet.bottom() == vector{d, a, b, c});
    }
  }
}

TEST_CASE("Reduction of an Interval Exchange Transformation", "[interval_exchange_transformation][reduce]") {
  SECTION("An IET Where the Lengths do not Add Up For Reduction") {
    auto&& [lengths, a, b, c] = IntLengths::make(17, 23, 33);

    auto iet = IET(lengths, {a, b, c}, {c, b, a});
    REQUIRE(!iet.reduce());
  }

  SECTION("An IET Where the First and Last Interval Make Things Irreducible") {
    auto&& [lengths, a, b, c] = IntLengths::make(1, 1, 1);

    auto iet = IET(lengths, {a, b, c}, {b, c, a});
    REQUIRE(!iet.reduce());
  }

  SECTION("A Trivial IET Is Irreducible") {
    auto&& [lengths, a] = IntLengths::make(1);

    auto iet = IET(lengths, {a}, {a});
    REQUIRE(!iet.reduce());
  }

  SECTION("A Trivially Reducible IET") {
    auto&& [lengths, a, b, c] = IntLengths::make(1, 2, 3);

    auto iet = IET(lengths, {a, b, c}, {a, b, c});
    REQUIRE(iet.reduce());
    REQUIRE(iet == IET(lengths, {a}, {a}));
  }

  SECTION("Reducible IETs") {
    auto&& [lengths, a, b, c] = IntLengths::make(17, 23, 33);

    auto iet = IET(lengths, {a, b, c}, {b, a, c});
    auto r = iet.reduce();
    REQUIRE(r);
    REQUIRE(*r == IET(lengths, {c}, {c}));
    REQUIRE(iet == IET(lengths, {a, b}, {b, a}));
  }
}

TEST_CASE("Zorich Induction on an Interval Exchange Transformation", "[interval_exchange_transformation][zorich_induction]") {
  SECTION("An IET that Allows Zorich Induction Speedup") {
    auto&& [lengths, a, b] = IntLengths::make(23, 5);
    auto iet = IET(lengths, {a, b}, {b, a});
  
    iet.zorichInduction();
  
    REQUIRE(iet == IET(IntLengths({23 - 4*5, 5}), {a, b}, {b, a}));
  }

  SECTION("An IET that Has No Zorich Induction Speedup") {
    auto&& [lengths, a, b] = IntLengths::make(5, 3);
    auto iet = IET(lengths, {a, b}, {b, a});
  
    iet.zorichInduction();
  
    REQUIRE(iet == IET(IntLengths({2, 3}), {a, b}, {b, a}));
  }

  SECTION("An IET that Produces Two Cylinders After One Iteration") {
    auto&& [lengths, a, b] = IntLengths::make(13, 1);
    auto iet = IET(lengths, {a, b}, {b, a});

    iet.zorichInduction();

    REQUIRE(iet == IET(IntLengths({1, 1}), {a, b}, {b, a}));
  }

  SECTION("Zorich Induction Changes Ordering at the Bottom") {
    auto&& [lengths, a,  b, c, d] = IntLengths::make(15, 2, 3, 7);
    auto iet = IET(lengths, {a, d, b, c}, {b, c, a, d});

    iet.zorichInduction();

    REQUIRE(iet == IET(IntLengths({3, 2, 3, 7}), {a, d, b, c}, {c, b, a, d}));
  }

  SECTION("Zorich Induction with MPZ Coefficients") {
    using MpzLengths = sample::Lengths<mpz_class>;
    auto&& [lengths, a, b] = MpzLengths::make(14328748557375491835455123393141239398243_mpz, 51123145748597134_mpz);
    auto iet = IET(lengths, {a, b}, {b, a});

    iet.zorichInduction();

    REQUIRE(iet == IET(MpzLengths({15560595195676063_mpz, 51123145748597134_mpz}), {a, b}, {b, a}));
  }

  SECTION("Zorich Induction with MPQ Coefficients") {
    using MpqLengths = sample::Lengths<mpq_class>;
    auto&& [lengths, a, b] = MpqLengths::make(mpq_class(3, 5), mpq_class(1, 4));
    auto iet = IET(lengths, {a, b}, {b, a});

    iet.zorichInduction();

    REQUIRE(iet == IET(MpqLengths({mpq_class(1, 10), mpq_class(1, 4)}), {a, b}, {b, a}));
  }

  SECTION("Zorich Induction With e-antic Coefficients") {
    using namespace eantic;
    using EAnticLengths = sample::Lengths<renf_elem_class>;
    auto K = renf_class::make("a^2 - a - 1", "a", "1.618 +/- 0.01");
    auto&& [lengths, a, b] = EAnticLengths::make(K->gen(), renf_elem_class(K, 1));

    auto iet = IET(lengths, {a, b}, {b, a});

    iet.zorichInduction();

    REQUIRE(iet == IET(EAnticLengths({K->gen() - 1, renf_elem_class(K, 1)}), {a, b}, {b, a}));
  }

  SECTION("Several Iterations of Zorich Induction") {
    auto&& [lengths, a, b, c, d, e] = IntLengths::make(977, 351, 143, 321, 12);

    auto iet = IET(lengths, {a, b, c, d, e}, {d, c, a, e, b});

    iet.zorichInduction();

    REQUIRE(iet == IET(IntLengths({49, 351, 143, 321, 12}), {a, b, c, d, e}, {d, c, a, e, b}));

    iet.swap();
    iet.zorichInduction();

    REQUIRE(iet == IET(IntLengths({49, 351, 143, 272, 12}), {d, c, a, e, b}, {b, c, a, d, e}));

    iet.swap();
    iet.zorichInduction();

    REQUIRE(iet == IET(IntLengths({49, 79, 143, 272, 12}), {b, c, a, d, e}, {c, a, e, d, b}));

    iet.swap();
    iet.zorichInduction();

    REQUIRE(iet == IET(IntLengths({49, 79, 64, 272, 12}), {c, a, e, d, b}, {b, c, a, d, e}));

    iet.swap();
    iet.zorichInduction();

    REQUIRE(iet == IET(IntLengths({49, 15, 64, 272, 12}), {b, c, a, d, e}, {a, e, d, c, b}));

    iet.swap();
    iet.zorichInduction();

    REQUIRE(iet == IET(IntLengths({34, 15, 64, 272, 12}), {a, e, d, c, b}, {c, b, a, d, e}));
  }
}

TEST_CASE("Repeated Induction on an Interval Exchange Transformation", "[interval_exchange_transformation][induce]") {
  using Result = InductionStep::Result;

  SECTION("Without Induction Steps") {
    SECTION("No Induction Steps Are Performed") {
      auto&& [lengths, a, b, c] = IntLengths::make(1, 2, 3);
      auto iet = IET(lengths, {a, b, c}, {c, b, a});
      REQUIRE(iet.induce(0).result == Result::LIMIT_REACHED);
    }

    SECTION("But Obvious Cylinders Are Detected") {
      auto&& [lengths, a, b, c] = IntLengths::make(1, 2, 3);
      auto iet = IET(lengths, {a, b, c}, {a, c, b});

      auto step0 = iet.induce(0);
      REQUIRE(step0.result == Result::SEPARATING_CONNECTION);
      REQUIRE(*step0.additionalIntervalExchangeTransformation == IET(lengths, {b, c}, {c, b}));

      auto step1 = iet.induce(0);
      REQUIRE(step1.result == Result::CYLINDER);
      REQUIRE(iet == IET(lengths, {a}, {a}));
    }

    SECTION("But Obvious Non-Separating Connections Are Detected") {
      auto&& [lengths, a, b, c] = IntLengths::make(1, 1, 1);
      auto iet = IET(lengths, {a, b, c}, {c, b, a});
      auto induce = iet.induce(0);
      REQUIRE(induce.result == Result::NON_SEPARATING_CONNECTION);
      REQUIRE(*induce.connection == pair{ c, a });
      REQUIRE(iet == IET(lengths, {b, c}, {b, c}));
    }

    SECTION("Obvious Connections Are Detected") {
      auto&& [lengths, a, b, c, d, e, f] = IntLengths::make(1, 2, 3, 1, 5, 7);
      auto iet = IET(lengths, {a, b, c, d, e, f}, {d, c, b, f, e, a});
    
      {
        auto induce = iet.induce(0);
        REQUIRE(induce.result == Result::NON_SEPARATING_CONNECTION);
      }
    
      {
        auto induce = iet.induce(0);
        REQUIRE(induce.result == Result::SEPARATING_CONNECTION);
        REQUIRE(iet == IET(lengths, {b, c}, {c, b}));
        REQUIRE(induce.additionalIntervalExchangeTransformation == IET(lengths, {d, e, f}, {f, e, d}));
      }
    }

    SECTION("Full Decomposition of the IET") {
      auto&& [lengths, a, b, c] = IntLengths::make(1, 1, 1);
      auto iet = IET(lengths, {a, b, c}, {c, a, b});

      auto step0 = iet.induce(0);
      REQUIRE(step0.result == Result::NON_SEPARATING_CONNECTION);
      REQUIRE(*step0.connection == pair{c, a});

      auto step1 = iet.induce(0);
      REQUIRE(step1.result == Result::NON_SEPARATING_CONNECTION);
      REQUIRE(*step1.connection == pair{c, b});

      auto step2 = iet.induce(0);
      REQUIRE(step2.result == Result::CYLINDER);
    }
  }

  SECTION("With Induction Steps") {
    SECTION("Induction Initially Fails to Terminate Within Limits") {
      auto&& [lengths, a, b] = IntLengths::make(13, 5);

      auto iet = IET(lengths, {a, b}, {b, a});

      REQUIRE(iet.induce(0).result == Result::LIMIT_REACHED);
      REQUIRE(iet.induce(1).result == Result::LIMIT_REACHED);
      REQUIRE(iet.induce(1).result == Result::NON_SEPARATING_CONNECTION);
      REQUIRE(iet.induce(1).result == Result::CYLINDER);
    }
  }
}

TEST_CASE("Computation of SAF Invariant", "[interval_exchange_transformation][saf_invariant]") {
/* TODO: Enable again
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

  int sign = 1;

  for (int i = 0; i < 10; i++) {
    iet.zorichInduction();
    iet.swap();
    std::valarray<mpq_class> vv;
    if (sign)
      vv = -iet.safInvariant();
    else
      vv = iet.safInvariant();
    EXPECT_TRUE(std::equal(std::begin(v), std::end(v), std::begin(vv), std::end(vv)));
    sign ^= 1;
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

  int sign = 1;

  for (int i = 0; i < 10; i++) {
    iet.zorichInduction();
    iet.swap();
    std::valarray<mpq_class> vv;
    if (sign)
      vv = -iet.safInvariant();
    else
      vv = iet.safInvariant();
    EXPECT_TRUE(std::equal(std::begin(v), std::end(v), std::begin(vv), std::end(vv)));
    sign ^= 1;
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
*/
}

TEST_CASE("Boshernitzan Algorithm on Interval Exchange Transformations", "[interval_exchange_transformation][boshernitzan_no_periodic_trajectory]") {
  using namespace eantic;
  using EAnticLengths = sample::Lengths<renf_elem_class>;

  SECTION("Over the Rationals, Boshernitzan Is Trivial") {
    auto&& [lengths, a, b, c] = IntLengths::make(451, 3221, 451);
    auto iet = IET(lengths, {a, b, c}, {c, b, a});
    REQUIRE(!iet.boshernitzanNoPeriodicTrajectory());
  }

  SECTION("Over the Field Q(√2)") {
    auto K = renf_class::make("a^2 - 2", "a", "1.41 +/- 0.01");
    
    SECTION("Examples Without Periodic Trajectories") {
      auto&& [lengths, a, b, c, d] = EAnticLengths::make(K->gen(), renf_elem_class(K, 1), renf_elem_class(K, 1), renf_elem_class(K, 1));

      REQUIRE(IET(lengths, {b, a}, {a, b}).boshernitzanNoPeriodicTrajectory());
      REQUIRE(IET(lengths, {a, b, c, d}, {d, b, a, c}).boshernitzanNoPeriodicTrajectory());
    }

    SECTION("Example With A Cylinder") {
      // Translations are:
      //   t0 = 7*a + 7
      //   t1 = -3*a - 3
      //   t2 = -10*a - 10
      // The space of relations is generated by (3,7,0) and (10,0,7).
      auto&& [lengths, a, b, c] = EAnticLengths::make(renf_elem_class(K, "7*a + 8"), renf_elem_class(K, "3*a + 2"), renf_elem_class(K, "4*a + 5"));
      REQUIRE(!IET(lengths, {a, b, c}, {c, b, a}).boshernitzanNoPeriodicTrajectory());
    }
  }

  SECTION("Over the Field Q(∛2)") {
    auto K = renf_class::make("a^3 - 2", "a", "1.25 +/- 0.01");
    auto&& [lengths, a, b, c] = EAnticLengths::make(K->gen() * K->gen(), K->gen(), renf_elem_class(K, 1));

    REQUIRE(IET(lengths, {a, b}, {b, a}).boshernitzanNoPeriodicTrajectory());
    REQUIRE(IET(lengths, {a, b, c}, {c, b, a}).boshernitzanNoPeriodicTrajectory());
  }
}

}
