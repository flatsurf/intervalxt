/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2020 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
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

#ifndef LIBINTERVALXT_SERIALIZABLE_HPP
#define LIBINTERVALXT_SERIALIZABLE_HPP

#include "forward.hpp"

namespace intervalxt {

// Common base class to add Cereal serialization.
// This is needed for generic classes since we cannot currently tell cereal to
// pick up free save/load functions if they are generic. (This is essentially
// the Barton-Nackman trick to go from a free function with limited
// specialization possibilities to the more powerful partial specialization
// that types provide.)
template <typename T>
class Serializable {
  template <bool Condition>
  using If = std::enable_if_t<Condition, bool>;

  template <typename Archive>
  static constexpr bool is_minimal = !std::is_same_v<void, decltype(std::declval<Serialization<T>>().save(std::declval<const Archive&>(), std::declval<const T&>()))>;

 public:
  template <typename Archive, bool Enable = !is_minimal<Archive>, If<Enable> = true>
  friend void save(Archive& archive, const T& self) { Serialization<T>().save(archive, self); }
  template <typename Archive, bool Enable = !is_minimal<Archive>, If<Enable> = true>
  friend void load(Archive& archive, T& self) { Serialization<T>().load(archive, self); }

  template <typename Archive, bool Enable = is_minimal<Archive>, If<Enable> = true>
  friend auto save_minimal(const Archive& archive, const T& self) { return Serialization<T>().save(archive, self); }
  template <typename Archive, typename S, bool Enable = is_minimal<Archive>, If<Enable> = true>
  friend void load_minimal(const Archive& archive, T& self, const S& value) { Serialization<T>().load(archive, self, value); }
};

}  // namespace intervalxt

#endif
