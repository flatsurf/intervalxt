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

#include "catch.hpp"

#include <intervalxt/component.hpp>
#include <intervalxt/connection.hpp>
#include <intervalxt/decomposition_step.hpp>
#include <intervalxt/dynamical_decomposition.hpp>
#include <intervalxt/interval_exchange_transformation.hpp>
#include <intervalxt/maybe_connection.hpp>

using namespace intervalxt;
using eantic::renf_class;
using eantic::renf_elem_class;

namespace intervalxt::test {
template <typename Length>
std::vector<std::vector<Label<Length>>> asLabels(const std::vector<std::list<Connection<Length>>>& boundaries) {
  std::vector<std::vector<Label<Length>>> ret;
  for (auto& boundary : boundaries) {
    std::vector<Label<Length>> labels;
    for (auto& connection : boundary) {
      labels.push_back(connection.source().before());
      labels.push_back((-connection).source().before());
    }
    ret.push_back(labels);
  }
  return ret;
}

TEST_CASE("Decomposition of an IET") {
  using Length = Length<renf_elem_class>;
  using Label = Label<Length>;
  using boost::logic::indeterminate;
  using Result = DecompositionStep<Length>::Result;
  using Boundaries = std::vector<std::vector<Label>>;

  auto K = renf_class::make("a^2 - 2", "a", "1.41 +/- 0.01");

  auto a = Label(renf_elem_class(K, 1));
  auto b = Label(renf_elem_class(K, 3));
  auto c = Label(K->gen());
  auto d = Label(renf_elem_class(K, 3));
  auto e = Label(renf_elem_class(K, 3));

  auto decomposition = DynamicalDecomposition<Length>(IntervalExchangeTransformation<Length>({b, a, c, d, e}, {d, c, a, e, b}));

  REQUIRE(decomposition.components().size() == 1);

  SECTION("Manual Decomposition") {
    auto component = decomposition.components()[0];
    CAPTURE(component);
    REQUIRE(indeterminate(component.cylinder()));
    REQUIRE(indeterminate(component.withoutPeriodicTrajectory()));
    REQUIRE(indeterminate(component.keane()));
    REQUIRE(asLabels(component.left()) == Boundaries{});
    REQUIRE(asLabels(component.right()) == Boundaries{});

    auto step0 = component.decompositionStep();
    CAPTURE(step0);
    REQUIRE(step0.result == Result::NON_SEPARATING_CONNECTION);
    REQUIRE(indeterminate(component.cylinder()));
    REQUIRE(indeterminate(component.withoutPeriodicTrajectory()));
    REQUIRE(indeterminate(component.keane()));
    REQUIRE(asLabels(component.left()) == Boundaries{{a, d}});
    REQUIRE(asLabels(component.right()) == Boundaries{{d, a}});

    auto step1 = component.decompositionStep();
    CAPTURE(step1);
    REQUIRE(step1.result == Result::SEPARATING_CONNECTION);
    REQUIRE(asLabels(component.left()) == Boundaries{{a, d}});
    REQUIRE(asLabels(component.right()) == Boundaries{{a, d}});

    {
      auto cylinder = *step1.additionalComponent;
      CAPTURE(cylinder);
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
      REQUIRE(!cylinder.withoutPeriodicTrajectory());
      REQUIRE(!cylinder.keane());
    }

    {
      auto step2 = component.decompositionStep();
      CAPTURE(step2);
      REQUIRE(step2.result == Result::WITHOUT_PERIODIC_TRAJECTORY);
      REQUIRE(!component.cylinder());
      REQUIRE(component.withoutPeriodicTrajectory());
    }
  }

  SECTION("Automatic Decomposition") {
    decomposition.decompose();

    REQUIRE(decomposition.components().size() == 2);

    {
      auto keane = decomposition.components()[0];
      CAPTURE(keane);
      REQUIRE(keane.withoutPeriodicTrajectory());
    }

    {
      auto cylinder = decomposition.components()[1];
      CAPTURE(cylinder);
      REQUIRE(cylinder.cylinder());
    }
  }
}


TEST_CASE("Decomposition of a Trivial IET") {
  using Length = Length<int>;
  using Label = Label<Length>;

  auto a = Label(1);

  auto decomposition = DynamicalDecomposition<Length>(IntervalExchangeTransformation<Length>({a}, {a}));

  REQUIRE(decomposition.decompose());
  REQUIRE(decomposition.components().size() == 1);
  REQUIRE(decomposition.components()[0].cylinder());
  REQUIRE(decomposition.components()[0].left().size() == 0);
  REQUIRE(decomposition.components()[0].right().size() == 0);
}

TEST_CASE("Decomposition of obvious Cylinders") {
  using Length = Length<int>;
  using Label = Label<Length>;
  using boost::logic::indeterminate;
  using Result = DecompositionStep<Length>::Result;
  using Boundaries = std::vector<std::vector<Label>>;
  using Labels = std::vector<Label>;

  auto a = Label(1);
  auto b = Label(1);

  auto decomposition = DynamicalDecomposition<Length>(IntervalExchangeTransformation<Length>({a, b}, {b, a}));

  REQUIRE(decomposition.components().size() == 1);

  SECTION("Manual Decomposition") {
    auto component = decomposition.components()[0];
    CAPTURE(component);
    REQUIRE(indeterminate(component.cylinder()));
    REQUIRE(indeterminate(component.withoutPeriodicTrajectory()));
    REQUIRE(indeterminate(component.keane()));
    REQUIRE(asLabels(component.left()) == Boundaries{});
    REQUIRE(asLabels(component.right()) == Boundaries{});

    auto step0 = component.decompositionStep();
    CAPTURE(step0);
    REQUIRE(step0.result == Result::NON_SEPARATING_CONNECTION);
    REQUIRE(indeterminate(component.cylinder()));
    REQUIRE(indeterminate(component.withoutPeriodicTrajectory()));
    REQUIRE(indeterminate(component.keane()));
    REQUIRE(component.bottom() == Labels{b});
    REQUIRE(component.top() == Labels{b});

    REQUIRE(asLabels(component.right()) == Boundaries{{b, b}});
    auto right = *component.right().begin()->begin();
    REQUIRE(right.nextInBoundary() == right.source());
    REQUIRE(right.source().previousAtSingularity() == (-right).source());
    REQUIRE((-right).source().after() == a);

    REQUIRE(asLabels(component.left()) == Boundaries{{b, b}});
    auto left = *component.left().begin()->begin();
    REQUIRE(left.nextInBoundary() == left.source());
    REQUIRE(left.source().previousAtSingularity() == (-left).source());
    REQUIRE((-left).source().after() == a);

    auto step1 = component.decompositionStep();
    CAPTURE(step1);
    REQUIRE(step1.result == Result::CYLINDER);
    REQUIRE(asLabels(component.left()) == Boundaries{{b, b}});
    REQUIRE(asLabels(component.right()) == Boundaries{{b, b}});
  }

  SECTION("Automatic Decomposition") {
    REQUIRE(decomposition.decompose());

    CAPTURE(decomposition);

    REQUIRE(decomposition.components().size() == 1);
    REQUIRE(decomposition.components()[0].cylinder());
    REQUIRE(asLabels(decomposition.components()[0].left()) == Boundaries{{b, b}});
    REQUIRE(asLabels(decomposition.components()[0].right()) == Boundaries{{b, b}});
  }
}

TEST_CASE("Decomposition of Nested Cylinders") {
  using Length = Length<int>;
  using Label = Label<Length>;
  using boost::logic::indeterminate;
  using Result = DecompositionStep<Length>::Result;
  using Boundaries = std::vector<std::vector<Label>>;
  using Labels = std::vector<Label>;

  auto a = Label(1);
  auto b = Label(1);
  auto c = Label(1);

  auto decomposition = DynamicalDecomposition<Length>(IntervalExchangeTransformation<Length>({a, b, c}, {c, b, a}));

  REQUIRE(decomposition.components().size() == 1);

  SECTION("Manual Decomposition") {
    auto component = decomposition.components()[0];
    CAPTURE(component);
    REQUIRE(indeterminate(component.cylinder()));
    REQUIRE(indeterminate(component.withoutPeriodicTrajectory()));
    REQUIRE(indeterminate(component.keane()));
    REQUIRE(asLabels(component.left()) == Boundaries{});
    REQUIRE(asLabels(component.right()) == Boundaries{});

    auto step0 = component.decompositionStep();
    CAPTURE(step0);
    REQUIRE(step0.result == Result::NON_SEPARATING_CONNECTION);
    REQUIRE(indeterminate(component.cylinder()));
    REQUIRE(indeterminate(component.withoutPeriodicTrajectory()));
    REQUIRE(indeterminate(component.keane()));
    REQUIRE(component.bottom() == Labels{b, c});
    REQUIRE(component.top() == Labels{c, b});

    REQUIRE(asLabels(component.right()) == Boundaries{{c, b}});
    auto right = *component.right().begin()->begin();
    REQUIRE(right.nextInBoundary() == right.source());
    REQUIRE(right.source().previousAtSingularity() == (-right).source());
    REQUIRE((-right).source().after() == a);

    REQUIRE(asLabels(component.left()) == Boundaries{{b, c}});
    auto left = *component.left().begin()->begin();
    REQUIRE(left.nextInBoundary() == left.source());
    REQUIRE(left.source().previousAtSingularity() == (-left).source());
    REQUIRE((-left).source().after() == b);

    auto step1 = component.decompositionStep();
    CAPTURE(step1);
    REQUIRE(step1.result == Result::CYLINDER);
    REQUIRE(step1.additionalComponent);

    {
      auto cylinder = *step1.additionalComponent;
      REQUIRE(cylinder.cylinder());
      REQUIRE(asLabels(cylinder.left()) == Boundaries{{b, c}});
      REQUIRE(asLabels(cylinder.right()) == Boundaries{{b, c}});
    }

    {
      REQUIRE(indeterminate(component.cylinder()));

      auto step2 = component.decompositionStep();
      REQUIRE(step2.result == Result::CYLINDER);

      REQUIRE(component.cylinder());
      REQUIRE(asLabels(component.left()) == Boundaries{{c, b}});
      REQUIRE(asLabels(component.right()) == Boundaries{{c, b}});
    }
  }

  SECTION("Automatic Decomposition") {
  }
}

}  // namespace intervalxt::test
