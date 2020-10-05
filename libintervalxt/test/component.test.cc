/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2020 Vincent Delecroix
 *        Copyright (C) 2020 Julian Rüth
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

#include <fmt/format.h>

#include "../intervalxt/connection.hpp"
#include "../intervalxt/dynamical_decomposition.hpp"
#include "../intervalxt/fmt.hpp"
#include "../intervalxt/half_edge.hpp"
#include "../intervalxt/interval_exchange_transformation.hpp"
#include "../intervalxt/sample/integer_coefficients.hpp"
#include "../intervalxt/sample/integer_floor_division.hpp"
#include "../intervalxt/sample/lengths.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"

namespace intervalxt::test {

TEST_CASE("Components of a Decomposition", "[component]") {
  using IntLengths = sample::Lengths<int>;

  auto&& [lengths, a] = IntLengths::make(1);
  auto iet = IntervalExchangeTransformation(std::make_shared<Lengths>(lengths), {a}, {a});
  auto decomposition = DynamicalDecomposition(iet);

  SECTION("iet() works") {
    REQUIRE(fmt::format("{}", iet) == fmt::format("{}", decomposition.components()[0].iet()));
  }
}

}  // namespace intervalxt::test
