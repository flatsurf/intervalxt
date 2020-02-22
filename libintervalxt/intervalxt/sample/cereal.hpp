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

#ifndef LIBINTERVALXT_SAMPLE_CEREAL_HPP
#define LIBINTERVALXT_SAMPLE_CEREAL_HPP

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

#include "lengths.hpp"

namespace intervalxt {

template <typename T>
struct Serialization<sample::Lengths<T>> {
  template <typename Archive>
  void save(Archive& archive, const sample::Lengths<T>& self) {
    archive(cereal::make_nvp("lengths", self.lengths));
    archive(cereal::make_nvp("stack", self.stack));
  }

  template <typename Archive>
  void load(Archive& archive, sample::Lengths<T>& self) {
    archive(cereal::make_nvp("lengths", self.lengths));
    archive(cereal::make_nvp("stack", self.stack));
  }
};

}

#endif
