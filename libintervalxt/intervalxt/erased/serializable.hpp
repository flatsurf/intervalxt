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

template <typename Serialization>
struct Serializable {
  using Erased = typename Serialization::Erased;

  // A serializable wrapper type for Unerased.
  template <typename Unerased>
  using SerializableWrap = std::enable_if_t<
      std::is_assignable_v<Erased, Unerased> && !std::is_same_v<Erased, std::decay_t<Unerased>>,
      std::unique_ptr<Serializable>>;

  virtual ~Serializable() {}

  virtual Erased erased() const = 0;

  // Turn unerased into a pointer to a polymorphic serializable wrapper type.
  template <typename T>
  static SerializableWrap<T> make(const T& unerased) {
    return std::make_unique<detail::Serializable<T, Serializable>>(unerased);
  }

 private:
  // Serialize the erased type through this polymorphic wrapper type.
  template <typename Archive>
  friend void save(Archive& archive, const Erased& self) {
    archive(serializable(self));
  }

  // Deserialize the erased type through this polymorphic wrapper type.
  template <typename Archive>
  friend void load(Archive& archive, Erased& self) {
    std::unique_ptr<Serializable> tmp;
    archive(tmp);
    self = tmp->erased();
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

}  // namespace intervalxt::erased

#endif
