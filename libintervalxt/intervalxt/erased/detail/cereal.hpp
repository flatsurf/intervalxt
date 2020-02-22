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

#ifndef LIBINTERVALXT_ERASED_DETAIL_CEREAL_HPP
#define LIBINTERVALXT_ERASED_DETAIL_CEREAL_HPP

#include <stdexcept>

#include "serializable.hpp"

namespace cereal {

// Forward declare cereal's base_class struct so IDEs do not complain about
// base_class usage below. We choose not to include
// <cereal/types/base_class.hpp> here explicitly since we do not know how the
// user brought the cereal headers on their search path.
template <typename Base>
struct base_class;

}  // namespace cereal

namespace intervalxt::erased::detail {

template <typename Unerased, typename Base>
template <typename Archive>
void Serializable<Unerased, Base>::save_(Archive& archive) const {
  archive(::cereal::base_class<Base>(this));
  archive(unerased);
}

template <typename Unerased, typename Base>
template <typename Archive>
void Serializable<Unerased, Base>::load_(Archive& archive) {
  archive(::cereal::base_class<Base>(this));
  archive(unerased);
}

}  // namespace intervalxt::erased::detail

#endif
