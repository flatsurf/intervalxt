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

#include <e-antic/cereal.hpp>
#include <exact-real/cereal.hpp>

#include <cereal/archives/json.hpp>

#include "../intervalxt/cppyy.hpp"
#include "../intervalxt/sample/cppyy.hpp"

#include "external/catch2/single_include/catch2/catch.hpp"

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

TEMPLATE_TEST_CASE("Serialization of Lengths", "[cppyy][cereal][lengths]", (int), (eantic::renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using TLengths = ::intervalxt::cppyy::Lengths<std::vector<T>>;

  SECTION("Serialization of Lengths Without Type Erasure") {
    TLengths lengths({T(1), T(2), T(3)});
    test_serialization(lengths);
  }
}

}  // namespace intervalxt::test
