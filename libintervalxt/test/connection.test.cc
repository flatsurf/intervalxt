/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2021 Julian Rüth
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

#include "../intervalxt/connection.hpp"
#include "../intervalxt/sample/lengths.hpp"
#include "../intervalxt/sample/integer_coefficients.hpp"
#include "../intervalxt/sample/integer_floor_division.hpp"
#include "../intervalxt/interval_exchange_transformation.hpp"
#include "../intervalxt/dynamical_decomposition.hpp"

#include "external/catch2/single_include/catch2/catch.hpp"

namespace intervalxt::test {

using IntLengths = sample::Lengths<int>;

TEST_CASE("Connection in a Decomposition") {
  auto&& [lengths, a, b] = IntLengths::make(1, 1);

  auto decomposition = DynamicalDecomposition(IntervalExchangeTransformation(std::make_shared<Lengths>(lengths), {a, b}, {b, a}));
  decomposition.decompose();

  const auto component = decomposition.components()[0];

  REQUIRE(std::get<Connection>(component.left()[0]) == -std::get<Connection>(component.right()[0]));
}

TEST_CASE("Injected Connections in Decompositions") {
  auto&& [lengths, a, b, c, d, e] = IntLengths::make(1, 1, 1, 1, 0);

  Component components[] = {
    // We start from two unrelated IETs which have a single component each:
    //
    // a b
    // / ╲
    // ╲ /
    // b a
    //
    // c d
    // / ╲
    // ╲ /
    // d c
    DynamicalDecomposition(IntervalExchangeTransformation(std::make_shared<Lengths>(lengths), {a, b}, {b, a})).components()[0],
    DynamicalDecomposition(IntervalExchangeTransformation(std::make_shared<Lengths>(lengths), {c, d}, {d, c})).components()[0],
  };

  // Now we inject a vertical connection in each that connects these components:
  //
  // a b   c d
  // /r    / ╲
  //  r    ╲
  //   ╲    l
  // ╲ /    l/
  // b a   d c

  const auto [_, r] = components[0].inject(components[0].topContour()[0], {}, { std::make_pair(e, a) });
  CAPTURE(r);
  CAPTURE(components[0]);
  const auto [l, __] = components[1].inject(components[1].bottomContour()[1], { std::make_pair(a, e) }, {});
  CAPTURE(l);
  CAPTURE(components[1]);

  REQUIRE(*begin(r) == -*begin(l));
}

}

