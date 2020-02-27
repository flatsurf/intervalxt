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

#ifndef LIBINTERVALXT_ERASED_SERIALIZABLE_HPP
#define LIBINTERVALXT_ERASED_SERIALIZABLE_HPP

#include <memory>
#include <type_traits>

#include "detail/serializable.hpp"

namespace intervalxt::erased {

template <typename T>
struct Serializable {
  using Erased = T;

  virtual ~Serializable() {}

  virtual Erased erased() const = 0;

 private:
  // Serialize the erased type through this polymorphic wrapper type.
  template <typename Archive>
  friend void save(Archive& archive, const Erased& self) {
    saveErased(archive, self);
  }

  // Deserialize the erased type through this polymorphic wrapper type.
  template <typename Archive>
  friend void load(Archive& archive, Erased& self) {
    loadErased(archive, self);
  }

  template <typename Archive>
  friend void save(Archive&, const Serializable&) {
    // Intentionally empty. There is nothing to serialize for this virtual
    // base type, the subtypes do the actual (de)serialization.
  }

  template <typename Archive>
  friend void load(Archive&, Serializable&) {
    // Intentionally empty. There is nothing to deserialize for this virtual
    // base type, the subtypes do the actual (de)serialization.
  }
};

// Serialize the erased type through a polymorphic wrapper type.
template <typename Archive, typename T>
void saveErased(Archive& archive, const T& self) {
  archive(serializable(self, Tag<T>()));
}

// Deserialize the erased type through a polymorphic wrapper type.
template <typename Archive, typename T>
void loadErased(Archive& archive, T& self) {
  std::unique_ptr<Serializable<T>> tmp;
  archive(tmp);
  self = tmp->erased();
}

// Turns an unerased object into a polymorphic serilizable
// wrapper. This method is picked up automatically through
// some ADL magic (it appears.) 
template <typename Erased, typename T>
std::unique_ptr<Serializable<Erased>> serializable(const T& unerased, const Tag<Erased>&) {
  return std::make_unique<detail::Serializable<T, Serializable<Erased>>>(unerased);
}

}  // namespace intervalxt::erased

#endif
