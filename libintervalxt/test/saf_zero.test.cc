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

#include <valarray>

#include <e-antic/renfxx.h>
#include <fmt/format.h>
#include <boost/logic/tribool.hpp>

#include "external/catch2/single_include/catch2/catch.hpp"

#include "../intervalxt/interval_exchange_transformation.hpp"
#include "../intervalxt/label.hpp"
#include "../intervalxt/connection.hpp"
#include "../intervalxt/decomposition_step.hpp"
#include "../intervalxt/dynamical_decomposition.hpp"
#include "../intervalxt/fmt.hpp"
#include "../intervalxt/sample/e-antic-arithmetic.hpp"
#include "../intervalxt/sample/lengths.hpp"

using eantic::renf_class;
using eantic::renf_elem_class;
using Result = ::intervalxt::DecompositionStep::Result;

namespace intervalxt::test {

TEST_CASE("Cubic IET in H(3,1) with SAF=0", "[saf_zero_interval_exchange_transformation]") {
  using namespace eantic;
  using EAnticLengths = sample::Lengths<renf_elem_class>;
  auto K = renf_class::make("A^3 - 6*A^2 + 9*A - 3", "A", "3.87 +/- 0.01");
  auto&& [lengths, a, b, c, d, e, f, g] = EAnticLengths::make(renf_elem_class(K, "-A^2 + 4*A"),
          renf_elem_class(K, "1/3*A^2 - A - 1"), renf_elem_class(K, "40/3*A^2 - 61*A + 36"),
          renf_elem_class(K, "-25/3*A^2 + 38*A - 22"), renf_elem_class(K, "1/3*A^2 - 5"),
          renf_elem_class(K, "-5*A^2 + 22*A - 10"), renf_elem_class(K, "1/3*A^2 - 2*A + 3"));
  auto iet = IntervalExchangeTransformation(std::make_shared<Lengths>(lengths), {a, b, c, d, e, f, g}, {g, f, d, c, e, b, a});

  SECTION("SAF should be zero") {
    auto s = iet.safInvariant();
    REQUIRE(s.min() == 0);
    REQUIRE(s.max() == 0);
  }

  auto decomposition = DynamicalDecomposition(iet);
  REQUIRE(decomposition.components().size() == 1);
  auto component = decomposition.components()[0];
  auto step0 = component.decompositionStep();
  }
}  // namespace intervalxt::test
