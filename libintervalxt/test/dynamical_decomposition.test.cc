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

#include <e-antic/renfxx.h>
#include <boost/logic/tribool.hpp>

#include "external/catch2/single_include/catch2/catch.hpp"

#include "../intervalxt/dynamical_decomposition.hpp"
#include "../intervalxt/interval_exchange_transformation.hpp"
#include "../intervalxt/decomposition_step.hpp"
#include "../intervalxt/connection.hpp"
#include "../intervalxt/sample/lengths.hpp"
#include "../intervalxt/sample/e-antic-arithmetic.hpp"

using eantic::renf_class;
using eantic::renf_elem_class;
using boost::lexical_cast;
using std::string;
using Result = ::intervalxt::DecompositionStep::Result;

namespace intervalxt::test {

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
    REQUIRE(lexical_cast<string>(component) == "[d] [c] [a] [e] [b] -[e] -[d] -[c] -[a] -[b]");

    auto step0 = component.decompositionStep();
    CAPTURE(step0);
    REQUIRE(step0.result == Result::NON_SEPARATING_CONNECTION);
    REQUIRE(indeterminate(component.cylinder()));
    REQUIRE(indeterminate(component.withoutPeriodicTrajectory()));
    REQUIRE(indeterminate(component.keane()));
    REQUIRE(lexical_cast<string>(*step0.connection) == "[d+ ⚯ b-]");
    REQUIRE(lexical_cast<string>(component) == "[c] [a] [e] [d] [d+ ⚯ b-] -[e] -[d] -[c] -[a] [b- ⚯ d+]");

    auto step1 = component.decompositionStep();
    CAPTURE(step1);
    REQUIRE(step1.result == Result::SEPARATING_CONNECTION);
    REQUIRE(lexical_cast<string>(*step1.connection) == "[a+ ⚯ c-]");
    REQUIRE(lexical_cast<string>(component) == "[c] [a] [a+ ⚯ c-] -[c] -[a] [b- ⚯ d+]");

    {
      auto cylinder = *step1.additionalComponent;
      REQUIRE(lexical_cast<std::string>(cylinder) == "[e] [d] [d+ ⚯ b-] -[e] -[d] [c- ⚯ a+]");
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

      REQUIRE(lexical_cast<string>(cylinder) == "[e] [e+ ⚯ d-] [d+ ⚯ b-] -[e] [d- ⚯ e+] [c- ⚯ a+]");
    }

    {
      auto step2 = component.decompositionStep();
      REQUIRE(step2.result == Result::WITHOUT_PERIODIC_TRAJECTORY);
      REQUIRE(not component.cylinder());
      REQUIRE(component.withoutPeriodicTrajectory());

      REQUIRE(lexical_cast<string>(component) == "[c] [a] [a+ ⚯ c-] -[c] -[a] [b- ⚯ d+]");
    }
  }

  SECTION("Automatic Decomposition") {
    decomposition.decompose();

    REQUIRE(decomposition.components().size() == 2);

    {
      auto keane = decomposition.components()[0];
      CAPTURE(keane);
      REQUIRE(keane.withoutPeriodicTrajectory());

      REQUIRE(lexical_cast<string>(keane) == "[c] [a] [a+ ⚯ c-] -[c] -[a] [b- ⚯ d+]");
    }

    {
      auto cylinder = decomposition.components()[1];
      CAPTURE(cylinder);
      REQUIRE(cylinder.cylinder());

      REQUIRE(lexical_cast<string>(cylinder) == "[e] [e+ ⚯ d-] [d+ ⚯ b-] -[e] [d- ⚯ e+] [c- ⚯ a+]");
    }
  }
}


TEST_CASE("Decomposition of a Trivial IET") {
  using IntLengths = sample::Lengths<int>;

  auto&& [lengths, a] = IntLengths::make(1);
  auto iet = IntervalExchangeTransformation(std::make_shared<Lengths>(lengths), {a}, {a});
  auto decomposition = DynamicalDecomposition(iet);

  REQUIRE(decomposition.decompose());
  CAPTURE(decomposition);

  auto cylinder = decomposition.components()[0];

  REQUIRE(lexical_cast<string>(cylinder) == "[a] -[a]");
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
    REQUIRE(lexical_cast<string>(component) == "[b] [a] -[b] -[a]");

    auto step0 = component.decompositionStep();
    CAPTURE(step0);
    REQUIRE(step0.result == Result::NON_SEPARATING_CONNECTION);
    REQUIRE(indeterminate(component.cylinder()));
    REQUIRE(indeterminate(component.withoutPeriodicTrajectory()));
    REQUIRE(indeterminate(component.keane()));
    REQUIRE(lexical_cast<string>(component) == "[b] [b+ ⚯ a-] -[b] [a- ⚯ b+]");

    auto step1 = component.decompositionStep();
    CAPTURE(step1);
    REQUIRE(step1.result == Result::CYLINDER);
    REQUIRE(lexical_cast<string>(component) == "[b] [b+ ⚯ a-] -[b] [a- ⚯ b+]");
  }

  SECTION("Automatic Decomposition") {
    REQUIRE(decomposition.decompose());

    CAPTURE(decomposition);

    auto component = decomposition.components()[0];

    REQUIRE(lexical_cast<string>(component) == "[b] [b+ ⚯ a-] -[b] [a- ⚯ b+]");
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
    REQUIRE(lexical_cast<string>(component) == "[c] [b] [a] -[c] -[b] -[a]");

    auto step0 = component.decompositionStep();
    CAPTURE(step0);
    REQUIRE(step0.result == Result::NON_SEPARATING_CONNECTION);
    REQUIRE(indeterminate(component.cylinder()));
    REQUIRE(indeterminate(component.withoutPeriodicTrajectory()));
    REQUIRE(indeterminate(component.keane()));
    REQUIRE(lexical_cast<string>(component) == "[b] [c] [c+ ⚯ a-] -[c] -[b] [a- ⚯ c+]");

    auto step1 = component.decompositionStep();
    CAPTURE(step1);
    REQUIRE(step1.result == Result::SEPARATING_CONNECTION);

    {
      auto cylinder = *step1.additionalComponent;
      auto step2 = cylinder.decompositionStep();
      CAPTURE(step2);
      REQUIRE(step2.result == Result::CYLINDER);
      REQUIRE(cylinder.cylinder());
      REQUIRE(lexical_cast<string>(cylinder) == "[c] [c+ ⚯ a-] -[c] [b- ⚯ b+]");
    }

    {
      REQUIRE(indeterminate(component.cylinder()));

      auto step2 = component.decompositionStep();
      REQUIRE(step2.result == Result::CYLINDER);

      REQUIRE(component.cylinder());
      REQUIRE(lexical_cast<string>(component) == "[b] [b+ ⚯ b-] -[b] [a- ⚯ c+]");
    }
  }

  SECTION("Automatic Decomposition") {
    REQUIRE(decomposition.decompose());

    REQUIRE(lexical_cast<string>(decomposition.components()[0]) == "[b] [b+ ⚯ b-] -[b] [a- ⚯ c+]");
    REQUIRE(lexical_cast<string>(decomposition.components()[1]) == "[c] [c+ ⚯ a-] -[c] [b- ⚯ b+]");
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
    REQUIRE(lexical_cast<string>(component) == "[d] [a] [c] [b] -[d] -[c] -[b] -[a]");

    auto step0 = component.decompositionStep();
    CAPTURE(step0);
    REQUIRE(step0.result == Result::NON_SEPARATING_CONNECTION);
    REQUIRE(indeterminate(component.cylinder()));
    REQUIRE(indeterminate(component.withoutPeriodicTrajectory()));
    REQUIRE(indeterminate(component.keane()));
    REQUIRE(lexical_cast<string>(component) == "[d] [d+ ⚯ a-] [c] [b] -[d] -[c] -[b] [a- ⚯ d+]");

    auto step1 = component.decompositionStep();
    CAPTURE(step1);
    REQUIRE(step1.result == Result::NON_SEPARATING_CONNECTION);
    REQUIRE(indeterminate(component.cylinder()));
    REQUIRE(indeterminate(component.withoutPeriodicTrajectory()));
    REQUIRE(indeterminate(component.keane()));
    REQUIRE(lexical_cast<string>(component) == "[c] [a- ⚯ d+] [d] [d+ ⚯ a-] [a+ ⚯ b-] -[d] -[c] [b- ⚯ a+]");

    auto step2 = component.decompositionStep();
    CAPTURE(step2);
    REQUIRE(step2.result == Result::SEPARATING_CONNECTION);
    REQUIRE(indeterminate(component.cylinder()));
    REQUIRE(indeterminate(component.withoutPeriodicTrajectory()));
    REQUIRE(indeterminate(component.keane()));
    REQUIRE(lexical_cast<string>(component) == "[c] [c+ ⚯ c-] -[c] [b- ⚯ a+]");
  }

  SECTION("Automatic Decomposition") {
    REQUIRE(decomposition.decompose());

    REQUIRE(lexical_cast<string>(decomposition.components()[0]) == "[c] [c+ ⚯ c-] -[c] [b- ⚯ a+]");
    REQUIRE(lexical_cast<string>(decomposition.components()[1]) == "[d] [d+ ⚯ a-] [a+ ⚯ b-] -[d] [c- ⚯ c+] [a- ⚯ d+]");
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

  REQUIRE(lexical_cast<string>(component) == "[b] [b+ ⚯ e-] [e+ ⚯ f-] [a] [a+ ⚯ c-] -[b] [e- ⚯ b+] [d+ ⚯ a-] -[a] [c- ⚯ a+] [f- ⚯ e+] [a- ⚯ d+]");

  component.decompose();

  REQUIRE(lexical_cast<string>(component) == "[b] [b+ ⚯ e-] [e+ ⚯ f-] [f+ ⚯ d-] [d+ ⚯ a-] [a+ ⚯ c-] -[b] [e- ⚯ b+] [d- ⚯ f+] [c- ⚯ a+] [f- ⚯ e+] [a- ⚯ d+]");
}

TEST_CASE("Decomposition Coming From a Case on the 1221 Surface") {
  using namespace eantic;
  using EAnticLengths = sample::Lengths<renf_elem_class>;
  auto K = renf_class::make("x^2 - 3", "x", "1.73 +/- 0.1");
  auto x = K->gen();

  auto&& [lengths, a, b, c, d] = EAnticLengths::make(17528509747*x/5000000000, 150057*x/100000, 50057*x/100000, 150057*x/100000);
  auto iet = IntervalExchangeTransformation(std::make_shared<Lengths>(lengths), {a, b, c, d}, {d, b, a, c});
  auto decomposition = DynamicalDecomposition(iet);

  REQUIRE(decomposition.components().size() == 1);

  auto component = decomposition.components()[0];

  component.decompose();

  REQUIRE(lexical_cast<string>(component) == "[d] [d+ ⚯ b-] [b+ ⚯ a-] [a+ ⚯ c-] -[d] [b- ⚯ d+] [a- ⚯ b+] [c- ⚯ a+]");
}

}  // namespace intervalxt::test
