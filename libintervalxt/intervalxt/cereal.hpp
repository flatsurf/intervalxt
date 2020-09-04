/* ********************************************************************
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
 * *******************************************************************/

#ifndef LIBINTERVALXT_CEREAL_HPP
#define LIBINTERVALXT_CEREAL_HPP

#include <fmt/format.h>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/push_back.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>
#include <map>

#include "erased/cereal.hpp"
#include "interval_exchange_transformation.hpp"
#include "label.hpp"
#include "length.hpp"

namespace intervalxt {

template <typename T>
struct Serialization {
  template <typename Archive>
  auto save(Archive&, const T&) {
    static_assert(!std::is_same_v<T, T>, "no Serialization specialization for this type defined");
  }

  template <typename Archive>
  void load(Archive&, T&) {
    static_assert(!std::is_same_v<T, T>, "no Serialization specialization for this type defined");
  }
};

template <>
struct Serialization<Label> {
  template <typename Archive>
  size_t save(Archive&, const Label& self) {
    return self.id;
  }

  template <typename Archive>
  void load(const Archive&, Label& self, const size_t& id) {
    self.id = id;
  }
};

template <>
struct Serialization<IntervalExchangeTransformation> {
  template <typename Archive>
  void save(Archive& archive, const IntervalExchangeTransformation& self) {
    archive(cereal::make_nvp("top", self.top()));
    archive(cereal::make_nvp("bottom", self.bottom()));
    archive(cereal::make_nvp("lengths", self.lengths()));
  }

  template <typename Archive>
  void load(Archive& archive, IntervalExchangeTransformation& self) {
    std::vector<Label> top, bottom;
    std::shared_ptr<Lengths> lengths;

    archive(cereal::make_nvp("top", top));
    archive(cereal::make_nvp("bottom", bottom));
    archive(cereal::make_nvp("lengths", lengths));

    if (lengths == nullptr) {
      assert(top.empty() && bottom.empty() && "There must not be top & bottom labels when there are no lengths to give meaning to them.");
      // The default initialized self is already the correct deserialization.
      return;
    }

    self = IntervalExchangeTransformation(lengths, top, bottom);
  }
};

}  // namespace intervalxt

#endif
