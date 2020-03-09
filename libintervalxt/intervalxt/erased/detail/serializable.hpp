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

#ifndef LIBINTERVALXT_ERASED_DETAIL_SERIALIZABLE_HPP
#define LIBINTERVALXT_ERASED_DETAIL_SERIALIZABLE_HPP

#include <type_traits>

#include "../forward.hpp"

namespace intervalxt::erased::detail {

template <typename Unerased, typename Base>
struct Serializable : public Base {
  using Erased = typename Base::Erased;

  static_assert(!std::is_same_v<Unerased, Erased>);

  Serializable() :
    unerased() {}
  explicit Serializable(const Unerased& unerased) :
    unerased(unerased) {}

  Erased erased() const override { return unerased; }

 private:
  Unerased unerased;

  // A free save() function to serialize this type.  Since templatized save()
  // has been causing trouble, we use the Barton-Nackman trick to redirect to a
  // private member implementation.
  template <typename Archive>
  friend void save(Archive& archive, const Serializable& self) { self.save_(archive); }
  // A free save() function to serialize this type.  Since templatized save()
  // has been causing trouble, we use the Barton-Nackman trick to redirect to a
  // private member implementation.
  template <typename Archive>
  friend void load(Archive& archive, Serializable& self) { self.load_(archive); }

  // Write unerased to an archive. The implementation lives in cereal.hpp so we
  // do not need to pull in all the cereal machinery unless this is actually
  // used.
  template <typename Archive>
  void save_(Archive&) const;
  // Load unerased from an archive. The implementation lives in cereal.hpp so we
  // do not need to pull in all the cereal machinery unless this is actually
  // used.
  template <typename Archive>
  void load_(Archive&);
};

}  // namespace intervalxt::erased::detail

#endif
