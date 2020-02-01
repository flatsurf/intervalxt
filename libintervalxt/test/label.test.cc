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

#include <unordered_set>

#include "external/catch2/single_include/catch2/catch.hpp"

#include "../intervalxt/label.hpp"

namespace intervalxt::test {

TEST_CASE("Labels Can Be Created and Compared", "[label]") {
  WHEN("Labels Are Created With Different Values") {
    Label a(1);
    Label b(2);
    Label c;
    Label d;

    THEN("They Compare Accordingly") {
      REQUIRE(a == a);
      REQUIRE(a != b);
      REQUIRE(a != c);
      REQUIRE(a != d);
      REQUIRE(b == b);
      REQUIRE(b != c);
      REQUIRE(b != d);
      REQUIRE(c == c);
      REQUIRE(c != d);
      REQUIRE(d == d);
    }

    THEN("They Can Be Used in Unordered Data Structures") {
      REQUIRE(std::unordered_set<Label>{a, a, b, c, d}.size() == 4);
    }
  }
}

}  // namespace intervalxt::test
