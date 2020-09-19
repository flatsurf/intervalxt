/**********************************************************************
 *  This file is part of intervalxt.
 *
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

#include <e-antic/renfxx.h>
#include <gmpxx.h>

#include <exact-real/element.hpp>
#include <exact-real/integer_ring.hpp>
#include <exact-real/module.hpp>
#include <exact-real/number_field.hpp>
#include <exact-real/rational_field.hpp>
#include <vector>

#include "../intervalxt/sample/element_coefficients.hpp"
#include "../intervalxt/sample/integer_coefficients.hpp"
#include "../intervalxt/sample/mpq_coefficients.hpp"
#include "../intervalxt/sample/mpz_coefficients.hpp"
#include "../intervalxt/sample/renf_elem_coefficients.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"

namespace intervalxt::test {

using std::vector;

TEMPLATE_TEST_CASE("Coefficients Can Be Computed", "[coefficients]", (int), (mpz_class), (mpq_class), (eantic::renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  SECTION("Zero has no non-zero coefficients") {
    T zero = T();
    auto coefficients = sample::Coefficients<T>()({zero});
    REQUIRE(coefficients.size() == 1);
    REQUIRE(std::all_of(begin(coefficients[0]), end(coefficients[0]), [](const auto& c) { return c == 0; }));
  }
}

}  // namespace intervalxt::test
