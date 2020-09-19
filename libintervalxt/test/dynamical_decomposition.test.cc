/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019 Vincent Delecroix
 *        Copyright (C) 2019-2020 Julian Rüth
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

#include <e-antic/renfxx.h>
#include <fmt/format.h>

#include <boost/logic/tribool.hpp>

#include "../intervalxt/connection.hpp"
#include "../intervalxt/decomposition_step.hpp"
#include "../intervalxt/dynamical_decomposition.hpp"
#include "../intervalxt/fmt.hpp"
#include "../intervalxt/interval_exchange_transformation.hpp"
#include "../intervalxt/sample/integer_coefficients.hpp"
#include "../intervalxt/sample/integer_floor_division.hpp"
#include "../intervalxt/sample/lengths.hpp"
#include "../intervalxt/sample/renf_elem_coefficients.hpp"
#include "../intervalxt/sample/renf_elem_floor_division.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"

using Result = ::intervalxt::DecompositionStep::Result;

namespace intervalxt::test {

using eantic::renf_class;
using eantic::renf_elem_class;
TEST_CASE("Decomposition of an IET") {
  using namespace eantic;
  using EAnticLengths = sample::Lengths<renf_elem_class>;
  auto K = renf_class::make("a^2 - 2", "a", "1.414 +/- 0.01");

  auto&& [lengths, a, b, c, d, e] = EAnticLengths::make(renf_elem_class(K, 1), renf_elem_class(K, 3), K->gen(), renf_elem_class(K, 3), renf_elem_class(K, 3));
  auto iet = IntervalExchangeTransformation(std::make_shared<Lengths>(lengths), {b, a, c, d, e}, {d, c, a, e, b});
  auto decomposition = DynamicalDecomposition(iet);

  REQUIRE(decomposition.components().size() == 1);

  SECTION("Manual Decomposition") {
    auto component = decomposition.components()[0];
    CAPTURE(component);
    REQUIRE(indeterminate(component.cylinder()));
    REQUIRE(indeterminate(component.withoutPeriodicTrajectory()));
    REQUIRE(indeterminate(component.keane()));
    REQUIRE(fmt::format("{}", component) == "[d] [c] [a] [e] [b] -[e] -[d] -[c] -[a] -[b]");

    auto step0 = component.decompositionStep();
    CAPTURE(step0);
    REQUIRE(step0.result == Result::NON_SEPARATING_CONNECTION);
    REQUIRE(indeterminate(component.cylinder()));
    REQUIRE(indeterminate(component.withoutPeriodicTrajectory()));
    REQUIRE(indeterminate(component.keane()));
    REQUIRE(fmt::format("{}", *step0.connection) == "[d+ ⚯ b-]");
    REQUIRE(fmt::format("{}", component) == "[c] [a] [e] [d] [d+ ⚯ b-] -[e] -[d] -[c] -[a] [b- ⚯ d+]");

    auto step1 = component.decompositionStep();
    CAPTURE(step1);
    REQUIRE(step1.result == Result::SEPARATING_CONNECTION);
    REQUIRE(fmt::format("{}", *step1.connection) == "[a+ ⚯ c-]");
    REQUIRE(fmt::format("{}", component) == "[c] [a] [a+ ⚯ c-] -[c] -[a] [b- ⚯ d+]");

    {
      auto cylinder = *step1.additionalComponent;
      REQUIRE(fmt::format("{}", cylinder) == "[e] [d] [d+ ⚯ b-] -[e] -[d] [c- ⚯ a+]");
      REQUIRE(indeterminate(cylinder.cylinder()));
      REQUIRE(indeterminate(cylinder.withoutPeriodicTrajectory()));
      REQUIRE(indeterminate(cylinder.keane()));

      auto step2 = cylinder.decompositionStep();
      CAPTURE(step2);
      REQUIRE(step2.result == Result::NON_SEPARATING_CONNECTION);
      REQUIRE(indeterminate(cylinder.cylinder()));
      REQUIRE(indeterminate(cylinder.withoutPeriodicTrajectory()));
      REQUIRE(indeterminate(cylinder.keane()));

      auto step3 = cylinder.decompositionStep();
      CAPTURE(step3);
      REQUIRE(step3.result == Result::CYLINDER);
      REQUIRE(cylinder.cylinder());
      REQUIRE(not cylinder.withoutPeriodicTrajectory());
      REQUIRE(not cylinder.keane());

      REQUIRE(fmt::format("{}", cylinder) == "[e] [e+ ⚯ d-] [d+ ⚯ b-] -[e] [d- ⚯ e+] [c- ⚯ a+]");
    }

    {
      auto step2 = component.decompositionStep();
      REQUIRE(step2.result == Result::WITHOUT_PERIODIC_TRAJECTORY_BOSHERNITZAN);
      REQUIRE(not component.cylinder());
      REQUIRE(component.withoutPeriodicTrajectory());

      REQUIRE(fmt::format("{}", component) == "[c] [a] [a+ ⚯ c-] -[c] -[a] [b- ⚯ d+]");
    }
  }

  SECTION("Automatic Decomposition") {
    decomposition.decompose();

    REQUIRE(decomposition.components().size() == 2);

    {
      auto keane = decomposition.components()[0];
      CAPTURE(keane);
      REQUIRE(keane.withoutPeriodicTrajectory());

      REQUIRE(fmt::format("{}", keane) == "[c] [a] [a+ ⚯ c-] -[c] -[a] [b- ⚯ d+]");
    }

    {
      auto cylinder = decomposition.components()[1];
      CAPTURE(cylinder);
      REQUIRE(cylinder.cylinder());

      REQUIRE(fmt::format("{}", cylinder) == "[e] [e+ ⚯ d-] [d+ ⚯ b-] -[e] [d- ⚯ e+] [c- ⚯ a+]");
    }
  }
}

TEST_CASE("Decomposition of Periodic IETs") {
  using namespace eantic;
  using EAnticLengths = sample::Lengths<renf_elem_class>;

  SECTION("An Auto-Similar IET") {
    auto K = renf_class::make("a^3 - a^2 - a - 1", "a", "1.84 +/- 0.1");
    auto x = K->gen();

    auto&& [lengths, a, b, c, d, e, f, g] = EAnticLengths::make(x + 1, x * x - x - 1, x * x, x, x, K->one(), K->one());
    auto iet = IntervalExchangeTransformation(std::make_shared<Lengths>(lengths), {a, b, c, d, e, f, g}, {b, e, d, g, f, c, a});
    auto decomposition = DynamicalDecomposition(iet);

    REQUIRE(decomposition.components().size() == 1);

    auto component = decomposition.components()[0];

    const auto step = component.decompositionStep();

    REQUIRE(step.result == DecompositionStep::Result::WITHOUT_PERIODIC_TRAJECTORY_AUTO_SIMILAR);
  }

  SECTION("A Marked Point at an Unrelated Coordinate") {
    auto K = renf_class::make("y^6 - y^4 - y^2 - 1", "y", "[1.356 +/- .1]");
    auto y = K->gen();
    auto x = y * y;

    auto&& [lengths, a, b, c, d, e, f, g, h] = EAnticLengths::make(y, x + 1 - y, x * x - x - 1, x * x, x, x, K->one(), K->one());
    auto iet = IntervalExchangeTransformation(std::make_shared<Lengths>(lengths), {a, b, c, d, e, f, g, h}, {c, f, e, h, g, d, a, b});
    auto decomposition = DynamicalDecomposition(iet);

    REQUIRE(decomposition.components().size() == 1);

    auto component = decomposition.components()[0];

    // We can not detect the periodicity in this case yet, #87.
    const auto step = component.decompositionStep(1024);

    REQUIRE(step.result == DecompositionStep::Result::LIMIT_REACHED);
  }

  SECTION("An Unsolved Hard Case") {
    using namespace eantic;

    const auto K = renf_class::make("a^8 - 7*a^6 + 14*a^4 - 8*a^2 + 1", "a", "-0.81 +/- .1");
    const auto a = K->gen();

    const auto frac = [&](const auto& num, const auto& den) { return renf_elem_class(num) / renf_elem_class(den); };

    auto&& [lengths, l1, l43, l18, l12, l27, l35, l41, l21, l40, l14, l28, l29, l23, l38, l37, l16] = EAnticLengths::make((-frac(3, 2) * a * a * a * a * a * a * a + frac(19, 2) * a * a * a * a * a - frac(31, 2) * a * a * a + frac(13, 2) * a), (3 * a * a * a * a * a * a * a - frac(39, 2) * a * a * a * a * a + 35 * a * a * a - frac(31, 2) * a), (-5 * a * a * a * a * a * a * a + frac(65, 2) * a * a * a * a * a - frac(113, 2) * a * a * a + frac(49, 2) * a), (-frac(7, 2) * a * a * a * a * a * a * a + frac(47, 2) * a * a * a * a * a - 45 * a * a * a + frac(41, 2) * a), (-frac(1, 2) * a * a * a * a * a * a * a + 5 * a * a * a * a * a - frac(23, 2) * a * a * a + frac(11, 2) * a), (a * a * a * a * a * a * a - 9 * a * a * a * a * a + frac(39, 2) * a * a * a - frac(19, 2) * a), (frac(21, 2) * a * a * a * a * a * a * a - frac(143, 2) * a * a * a * a * a + frac(271, 2) * a * a * a - frac(123, 2) * a), (-2 * a * a * a * a * a * a * a + 15 * a * a * a * a * a - 31 * a * a * a + frac(29, 2) * a), (2 * a * a * a * a * a * a * a - frac(27, 2) * a * a * a * a * a + frac(49, 2) * a * a * a - 11 * a), (-3 * a * a * a * a * a * a * a + frac(39, 2) * a * a * a * a * a - 36 * a * a * a + 16 * a), (-2 * a * a * a * a * a * a * a + 15 * a * a * a * a * a - 31 * a * a * a + frac(29, 2) * a), (frac(3, 2) * a * a * a * a * a * a * a - 11 * a * a * a * a * a + frac(47, 2) * a * a * a - 12 * a), (-5 * a * a * a * a * a * a * a + frac(71, 2) * a * a * a * a * a - frac(137, 2) * a * a * a + 31 * a), (frac(1, 2) * a * a * a * a * a * a * a - 3 * a * a * a * a * a + frac(9, 2) * a * a * a - frac(5, 2) * a), (6 * a * a * a * a * a * a * a - 42 * a * a * a * a * a + 80 * a * a * a - frac(73, 2) * a), (-a * a * a * a * a * a * a + frac(15, 2) * a * a * a * a * a - frac(29, 2) * a * a * a + frac(11, 2) * a));
    auto iet = IntervalExchangeTransformation(std::make_shared<Lengths>(lengths), {l1, l43, l18, l12, l27, l35, l41, l21, l40, l14, l28, l29, l23, l38, l37, l16}, {l29, l23, l35, l16, l43, l38, l14, l27, l37, l40, l21, l18, l12, l41, l28, l1});
    auto decomposition = DynamicalDecomposition(iet);

    REQUIRE(decomposition.components().size() == 1);

    auto component = decomposition.components()[0];

    REQUIRE(component.decompositionStep().result == DecompositionStep::Result::NON_SEPARATING_CONNECTION);
    REQUIRE(component.decompositionStep().result == DecompositionStep::Result::NON_SEPARATING_CONNECTION);
    REQUIRE(component.decompositionStep().result == DecompositionStep::Result::NON_SEPARATING_CONNECTION);
    // We have currently no way to detect that this example is (presumably)
    // without periodic trajectories. This in particular goes beyond #86 and #87.
    REQUIRE(component.decompositionStep(1024).result == DecompositionStep::Result::LIMIT_REACHED);
  }
}

TEST_CASE("Decomposition Coming From a Case on the 1221 Surface") {
  using namespace eantic;
  using EAnticLengths = sample::Lengths<renf_elem_class>;
  auto K = renf_class::make("x^2 - 3", "x", "1.73 +/- 0.1");
  auto x = K->gen();

  auto&& [lengths, a, b, c, d] = EAnticLengths::make(17528509747 * x / 5000000000, 150057 * x / 100000, 50057 * x / 100000, 150057 * x / 100000);
  auto iet = IntervalExchangeTransformation(std::make_shared<Lengths>(lengths), {a, b, c, d}, {d, b, a, c});
  auto decomposition = DynamicalDecomposition(iet);

  REQUIRE(decomposition.components().size() == 1);

  auto component = decomposition.components()[0];

  component.decompose();

  REQUIRE(fmt::format("{}", component) == "[d] [d+ ⚯ b-] [b+ ⚯ a-] [a+ ⚯ c-] -[d] [b- ⚯ d+] [a- ⚯ b+] [c- ⚯ a+]");
}

TEST_CASE("Decomposition of a Trivial IET") {
  using IntLengths = sample::Lengths<int>;

  auto&& [lengths, a] = IntLengths::make(1);
  auto iet = IntervalExchangeTransformation(std::make_shared<Lengths>(lengths), {a}, {a});
  auto decomposition = DynamicalDecomposition(iet);

  REQUIRE(decomposition.decompose());
  CAPTURE(decomposition);

  auto cylinder = decomposition.components()[0];

  REQUIRE(fmt::format("{}", cylinder) == "[a] -[a]");
}

TEST_CASE("Decomposition of obvious Cylinders") {
  using IntLengths = sample::Lengths<int>;

  auto&& [lengths, a, b] = IntLengths::make(1, 1);
  auto iet = IntervalExchangeTransformation(std::make_shared<Lengths>(lengths), {a, b}, {b, a});
  auto decomposition = DynamicalDecomposition(iet);

  REQUIRE(decomposition.components().size() == 1);

  SECTION("Manual Decomposition") {
    auto component = decomposition.components()[0];
    CAPTURE(component);
    REQUIRE(indeterminate(component.cylinder()));
    REQUIRE(indeterminate(component.withoutPeriodicTrajectory()));
    REQUIRE(indeterminate(component.keane()));
    REQUIRE(fmt::format("{}", component) == "[b] [a] -[b] -[a]");

    auto step0 = component.decompositionStep();
    CAPTURE(step0);
    REQUIRE(step0.result == Result::NON_SEPARATING_CONNECTION);
    REQUIRE(indeterminate(component.cylinder()));
    REQUIRE(indeterminate(component.withoutPeriodicTrajectory()));
    REQUIRE(indeterminate(component.keane()));
    REQUIRE(fmt::format("{}", component) == "[b] [b+ ⚯ a-] -[b] [a- ⚯ b+]");

    auto step1 = component.decompositionStep();
    CAPTURE(step1);
    REQUIRE(step1.result == Result::CYLINDER);
    REQUIRE(fmt::format("{}", component) == "[b] [b+ ⚯ a-] -[b] [a- ⚯ b+]");
  }

  SECTION("Automatic Decomposition") {
    REQUIRE(decomposition.decompose());

    CAPTURE(decomposition);

    auto component = decomposition.components()[0];

    REQUIRE(fmt::format("{}", component) == "[b] [b+ ⚯ a-] -[b] [a- ⚯ b+]");
  }
}

TEST_CASE("Decomposition of Nested Cylinders") {
  using IntLengths = sample::Lengths<int>;

  auto&& [lengths, a, b, c] = IntLengths::make(1, 1, 1);
  auto iet = IntervalExchangeTransformation(std::make_shared<Lengths>(lengths), {a, b, c}, {c, b, a});
  auto decomposition = DynamicalDecomposition(iet);

  REQUIRE(decomposition.components().size() == 1);

  SECTION("Manual Decomposition") {
    auto component = decomposition.components()[0];
    CAPTURE(component);
    REQUIRE(indeterminate(component.cylinder()));
    REQUIRE(indeterminate(component.withoutPeriodicTrajectory()));
    REQUIRE(indeterminate(component.keane()));
    REQUIRE(fmt::format("{}", component) == "[c] [b] [a] -[c] -[b] -[a]");

    auto step0 = component.decompositionStep();
    CAPTURE(step0);
    REQUIRE(step0.result == Result::NON_SEPARATING_CONNECTION);
    REQUIRE(indeterminate(component.cylinder()));
    REQUIRE(indeterminate(component.withoutPeriodicTrajectory()));
    REQUIRE(indeterminate(component.keane()));
    REQUIRE(fmt::format("{}", component) == "[b] [c] [c+ ⚯ a-] -[c] -[b] [a- ⚯ c+]");

    auto step1 = component.decompositionStep();
    CAPTURE(step1);
    REQUIRE(step1.result == Result::SEPARATING_CONNECTION);

    {
      auto cylinder = *step1.additionalComponent;
      auto step2 = cylinder.decompositionStep();
      CAPTURE(step2);
      REQUIRE(step2.result == Result::CYLINDER);
      REQUIRE(cylinder.cylinder());
      REQUIRE(fmt::format("{}", cylinder) == "[c] [c+ ⚯ a-] -[c] [b- ⚯ b+]");
    }

    {
      REQUIRE(indeterminate(component.cylinder()));

      auto step2 = component.decompositionStep();
      REQUIRE(step2.result == Result::CYLINDER);

      REQUIRE(component.cylinder());
      REQUIRE(fmt::format("{}", component) == "[b] [b+ ⚯ b-] -[b] [a- ⚯ c+]");
    }
  }

  SECTION("Automatic Decomposition") {
    REQUIRE(decomposition.decompose());

    REQUIRE(fmt::format("{}", decomposition.components()[0]) == "[b] [b+ ⚯ b-] -[b] [a- ⚯ c+]");
    REQUIRE(fmt::format("{}", decomposition.components()[1]) == "[c] [c+ ⚯ a-] -[c] [b- ⚯ b+]");
  }
}

TEST_CASE("Decomposition of More Deeply Nested Cylinders") {
  using IntLengths = sample::Lengths<int>;

  auto&& [lengths, a, b, c, d] = IntLengths::make(1, 1, 1, 1);
  auto iet = IntervalExchangeTransformation(std::make_shared<Lengths>(lengths), {a, b, c, d}, {d, a, c, b});
  auto decomposition = DynamicalDecomposition(iet);

  REQUIRE(decomposition.components().size() == 1);

  SECTION("Manual Decomposition") {
    auto component = decomposition.components()[0];
    CAPTURE(component);
    REQUIRE(indeterminate(component.cylinder()));
    REQUIRE(indeterminate(component.withoutPeriodicTrajectory()));
    REQUIRE(indeterminate(component.keane()));
    REQUIRE(fmt::format("{}", component) == "[d] [a] [c] [b] -[d] -[c] -[b] -[a]");

    auto step0 = component.decompositionStep();
    CAPTURE(step0);
    REQUIRE(step0.result == Result::NON_SEPARATING_CONNECTION);
    REQUIRE(indeterminate(component.cylinder()));
    REQUIRE(indeterminate(component.withoutPeriodicTrajectory()));
    REQUIRE(indeterminate(component.keane()));
    REQUIRE(fmt::format("{}", component) == "[d] [d+ ⚯ a-] [c] [b] -[d] -[c] -[b] [a- ⚯ d+]");

    auto step1 = component.decompositionStep();
    CAPTURE(step1);
    REQUIRE(step1.result == Result::NON_SEPARATING_CONNECTION);
    REQUIRE(indeterminate(component.cylinder()));
    REQUIRE(indeterminate(component.withoutPeriodicTrajectory()));
    REQUIRE(indeterminate(component.keane()));
    REQUIRE(fmt::format("{}", component) == "[c] [a- ⚯ d+] [d] [d+ ⚯ a-] [a+ ⚯ b-] -[d] -[c] [b- ⚯ a+]");

    auto step2 = component.decompositionStep();
    CAPTURE(step2);
    REQUIRE(step2.result == Result::SEPARATING_CONNECTION);
    REQUIRE(indeterminate(component.cylinder()));
    REQUIRE(indeterminate(component.withoutPeriodicTrajectory()));
    REQUIRE(indeterminate(component.keane()));
    REQUIRE(fmt::format("{}", component) == "[c] [c+ ⚯ c-] -[c] [b- ⚯ a+]");
  }

  SECTION("Automatic Decomposition") {
    REQUIRE(decomposition.decompose());

    REQUIRE(fmt::format("{}", decomposition.components()[0]) == "[c] [c+ ⚯ c-] -[c] [b- ⚯ a+]");
    REQUIRE(fmt::format("{}", decomposition.components()[1]) == "[d] [d+ ⚯ a-] [a+ ⚯ b-] -[d] [c- ⚯ c+] [a- ⚯ d+]");
  }
}

TEST_CASE("Decomposition With Injected Connections") {
  using IntLengths = sample::Lengths<int>;

  auto&& [lengths, a, b, c, d, e, f] = IntLengths::make(1, 1, 0, 0, 0, 0);
  auto iet = IntervalExchangeTransformation(std::make_shared<Lengths>(lengths), {a, b}, {b, a});
  auto decomposition = DynamicalDecomposition(iet);

  REQUIRE(decomposition.components().size() == 1);

  auto component = decomposition.components()[0];

  component.inject(HalfEdge(component, a), {{c, a}, {f, e}, {a, d}}, {{d, a}});
  component.inject(HalfEdge(component, b), {{e, b}}, {});
  component.inject(-HalfEdge(component, b), {}, {{b, e}, {e, f}});
  component.inject(-HalfEdge(component, a), {}, {{a, c}});

  REQUIRE(fmt::format("{}", component) == "[b] [b+ ⚯ e-] [e+ ⚯ f-] [a] [a+ ⚯ c-] -[b] [e- ⚯ b+] [d+ ⚯ a-] -[a] [c- ⚯ a+] [f- ⚯ e+] [a- ⚯ d+]");

  component.decompose();

  REQUIRE(fmt::format("{}", component) == "[b] [b+ ⚯ e-] [e+ ⚯ f-] [f+ ⚯ d-] [d+ ⚯ a-] [a+ ⚯ c-] -[b] [e- ⚯ b+] [d- ⚯ f+] [c- ⚯ a+] [f- ⚯ e+] [a- ⚯ d+]");
}

}  // namespace intervalxt::test
