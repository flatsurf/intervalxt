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

#include <gtest/gtest.h>
#include <boost/lexical_cast.hpp>
#include <cereal/archives/json.hpp>
#include <memory>
#include <vector>

#include <intervalxt/cereal.hpp>
#include <intervalxt/interval_exchange_transformation.hpp>
#include <intervalxt/label.hpp>
#include <intervalxt/length.hpp>

using namespace intervalxt;
using cereal::JSONInputArchive;
using cereal::JSONOutputArchive;

namespace {

template <typename T>
struct is_shared_ptr : std::false_type {};
template <typename T>
struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};

template <typename T>
std::string toString(const T& x) {
  if constexpr (is_shared_ptr<T>::value) {
    if (x)
      return boost::lexical_cast<std::string>(x) + "->" + boost::lexical_cast<std::string>(*x);
    else
      return "null";
  } else {
    return boost::lexical_cast<std::string>(x);
  }
}

template <typename T>
T test_serialization(const T& x) {
  std::stringstream s;

  {
    JSONOutputArchive archive(s);
    archive(cereal::make_nvp("test", x));
  }

  T y;
  {
    JSONInputArchive archive(s);
    archive(cereal::make_nvp("test", y));
  }

  if constexpr (std::is_same_v<IntervalExchangeTransformation<Length<int>>, T>) {
    bool eq = true;
    if (x.top().size() != y.top().size())
      eq = false;
    for (int i = 0; i < x.top().size(); i++) {
      if (x.top()[i] != y.top()[i])
        eq = false;
      if (x.top()[i].length() != y.top()[i].length())
        eq = false;
      if (x.bottom()[i] != y.bottom()[i])
        eq = false;
      if (x.bottom()[i].length() != y.bottom()[i].length())
        eq = false;
    }
    if (eq)
      return y;
  } else if (x == y) {
    return y;
  }

  if constexpr (is_shared_ptr<T>::value) {
    if (!x && !y) return y;
    if (x && y && *x == *y) return y;
  }
  throw std::runtime_error("deserialization failed to reconstruct element, the original value " + toString(x) + " had serialized to " + s.str() + " which deserialized to " + toString(y));
}

TEST(CerealTest, Label) {
  test_serialization(Label<Length<int>>(Length<int>(1337)));
}

TEST(CerealTest, Length) {
  test_serialization(Length<int>(1337));
}

TEST(CerealTest, IntervalExchangeTransformation) {
  using Length = Length<int>;

  IntervalExchangeTransformation<Length> iet({Length(977), Length(351), Length(143), Length(321), Length(12)}, {3, 2, 0, 4, 1});

  test_serialization(iet);
}

}  // namespace

#include "main.hpp"
