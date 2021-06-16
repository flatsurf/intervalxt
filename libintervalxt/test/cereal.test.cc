/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019-2020 Vincent Delecroix
 *        Copyright (C) 2019-2021 Julian Rüth
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

#include <boost/type_erasure/any_cast.hpp>
#include <cereal/archives/json.hpp>
#include <e-antic/cereal.hpp>
#include <exact-real/cereal.hpp>

#include "../intervalxt/cereal.hpp"
#include "../intervalxt/interval_exchange_transformation.hpp"
#include "../intervalxt/label.hpp"
#include "../intervalxt/length.hpp"
#include "../intervalxt/lengths.hpp"
#include "../intervalxt/sample/cereal.hpp"
#include "../intervalxt/sample/integer_coefficients.hpp"
#include "../intervalxt/sample/integer_floor_division.hpp"
// Since mpz_class and mpq_class define no standard cereal interface, serializing IETs with such coefficients is not supported.
// #include "../intervalxt/sample/mpz_coefficients.hpp"
// #include "../intervalxt/sample/mpz_floor_division.hpp"
// #include "../intervalxt/sample/mpq_coefficients.hpp"
// #include "../intervalxt/sample/mpq_floor_division.hpp"
#include "../intervalxt/sample/element_coefficients.hpp"
#include "../intervalxt/sample/element_floor_division.hpp"
#include "../intervalxt/sample/lengths.hpp"
#include "../intervalxt/sample/renf_elem_coefficients.hpp"
#include "../intervalxt/sample/renf_elem_floor_division.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"

LIBINTERVALXT_ERASED_REGISTER((::intervalxt::Lengths), (::intervalxt::sample::Lengths<int>))
// Since mpz_class and mpq_class define no standard cereal interface, serializing IETs with such coefficients is not supported.
// LIBINTERVALXT_ERASED_REGISTER((::intervalxt::Lengths), (::intervalxt::sample::Lengths<mpz_class>))
// LIBINTERVALXT_ERASED_REGISTER((::intervalxt::Lengths), (::intervalxt::sample::Lengths<mpq_class>))
LIBINTERVALXT_ERASED_REGISTER((::intervalxt::Lengths), (::intervalxt::sample::Lengths<eantic::renf_elem_class>))
LIBINTERVALXT_ERASED_REGISTER((::intervalxt::Lengths), (::intervalxt::sample::Lengths<exactreal::Element<exactreal::IntegerRing>>))
LIBINTERVALXT_ERASED_REGISTER((::intervalxt::Lengths), (::intervalxt::sample::Lengths<exactreal::Element<exactreal::RationalField>>))
LIBINTERVALXT_ERASED_REGISTER((::intervalxt::Lengths), (::intervalxt::sample::Lengths<exactreal::Element<exactreal::NumberField>>))

using cereal::JSONInputArchive;
using cereal::JSONOutputArchive;

namespace intervalxt::test {

template <typename T>
T test_serialization(
    const T& x, const std::function<bool(const T&, const T&)> equality = [](const T& lhs, const T& rhs) { return lhs == rhs; }) {
  std::stringstream s;

  {
    JSONOutputArchive archive(s);
    archive(cereal::make_nvp("serializedTo", x));
  }

  CAPTURE(s.str());

  T y;
  {
    JSONInputArchive archive(s);
    archive(cereal::make_nvp("serializedTo", y));
  }

  CAPTURE(x);
  CAPTURE(y);
  REQUIRE(equality(x, y));

  return y;
}

TEST_CASE("Serialization of Label", "[cereal][label]") {
  test_serialization(Label());
  test_serialization(Label(1337));
}

TEMPLATE_TEST_CASE("Serialization of Lengths", "[cereal][lengths]", (int), (eantic::renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using TLengths = ::intervalxt::sample::Lengths<T>;

  SECTION("Serialization of Lengths Without Type Erasure") {
    TLengths lengths({T(1), T(2), T(3)});
    test_serialization(lengths);
  }

  SECTION("Serialization of Lengths With Type Erasure") {
    const auto equality = [](const Lengths& lhs, const Lengths& rhs) {
      return boost::type_erasure::any_cast<TLengths>(lhs) == boost::type_erasure::any_cast<TLengths>(rhs);
    };

    TLengths unerased({T(1), T(2), T(3)});
    Lengths erased = unerased;
    test_serialization<Lengths>(erased, equality);
  }
}

TEMPLATE_TEST_CASE("Serialization of IntervalExchangeTransformation", "[cereal][interval_exchange_transformation]", (int), (eantic::renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using TLengths = ::intervalxt::sample::Lengths<T>;

  auto&& [lengths, a, b, c, d] = TLengths::make(T(18), T(3), T(1), T(1));

  test_serialization(IntervalExchangeTransformation());
  test_serialization(IntervalExchangeTransformation(std::make_shared<Lengths>(lengths), {a, b, c, d}, {d, a, b, c}));
}

}  // namespace intervalxt::test
