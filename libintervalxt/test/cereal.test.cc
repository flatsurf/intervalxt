/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019-2020 Vincent Delecroix
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

#include <memory>
#include <vector>

#include <boost/type_erasure/any_cast.hpp>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

#include "external/catch2/single_include/catch2/catch.hpp"

#include "../intervalxt/cereal.hpp"
#include "../intervalxt/interval_exchange_transformation.hpp"
#include "../intervalxt/label.hpp"
#include "../intervalxt/length.hpp"
#include "../intervalxt/lengths.hpp"
#include "../intervalxt/sample/cereal.hpp"
#include "../intervalxt/sample/lengths.hpp"

#include "../intervalxt/erased/cereal.hpp"

LIBINTERVALXT_ERASED_REGISTER((::intervalxt::LengthsSerialization), (::intervalxt::sample::Lengths<int>))

using cereal::JSONInputArchive;
using cereal::JSONOutputArchive;

using IntLengths = ::intervalxt::sample::Lengths<int>;

namespace intervalxt::test {

template <typename T>
T test_serialization(
    const T& x, const std::function<bool(const T&, const T&)> equality = [](const T& lhs, const T& rhs) { return lhs == rhs; }) {
  std::stringstream s;

  {
    JSONOutputArchive archive(s);
    archive(cereal::make_nvp("serializedTo", x));
  }

  T y;
  {
    JSONInputArchive archive(s);
    archive(cereal::make_nvp("serializedTo", y));
  }

  CAPTURE(x);
  CAPTURE(s.str());
  CAPTURE(y);
  REQUIRE(equality(x, y));

  return y;
}

TEST_CASE("Serialization of Label", "[cereal][label]") {
  test_serialization(Label());
  test_serialization(Label(1337));
}

TEST_CASE("Serialization of Lengths", "[cereal][lengths]") {
  SECTION("Serialization of Lengths Without Type Erasure") {
    IntLengths lengths({1, 2, 3});
    test_serialization(lengths);
  }

  SECTION("Serialization of Lengths With Type Erasure") {
    const auto equality = [](const Lengths& lhs, const Lengths& rhs) {
      return boost::type_erasure::any_cast<IntLengths>(lhs) == boost::type_erasure::any_cast<IntLengths>(rhs);
    };

    IntLengths unerased({1, 2, 3});
    Lengths erased = unerased;
    test_serialization<Lengths>(erased, equality);
  }
}

TEST_CASE("Serialization of IntervalExchangeTransformation", "[cereal][interval_exchange_transformation]") {
  auto&& [lengths, a, b, c, d] = IntLengths::make(18, 3, 1, 1);

  test_serialization(IntervalExchangeTransformation());
  test_serialization(IntervalExchangeTransformation(std::make_shared<Lengths>(lengths), {a, b, c, d}, {d, a, b, c}));
}

}  // namespace intervalxt::test
