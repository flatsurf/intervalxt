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

#ifndef LIBINTERVALXT_SAMPLE_CPPYY_HPP
#define LIBINTERVALXT_SAMPLE_CPPYY_HPP

#include "../cppyy.hpp"
#include "cereal.hpp"

namespace intervalxt {

template <typename V>
struct Serialization<cppyy::Lengths<V>> {
  template <typename Archive>
  void save(Archive& archive, const cppyy::Lengths<V>& self) {
    Serialization<intervalxt::sample::Lengths<typename V::value_type>>{}.save(archive, self);
  }

  template <typename Archive>
  void load(Archive& archive, cppyy::Lengths<V>& self) {
    Serialization<intervalxt::sample::Lengths<typename V::value_type>>{}.load(archive, self);
  }
};

}  // namespace intervalxt

#endif
