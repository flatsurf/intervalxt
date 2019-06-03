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

#include <memory>
#include <map>

#include "intervalxt/label.hpp"
#include "intervalxt/length.hpp"

namespace intervalxt {
namespace {
// A unique object relating labels that are considered equal.
class Id {};
}

template <typename Coordinate>
class Label<Coordinate>::Implementation {
  friend Label<Coordinate>;

  std::shared_ptr<Id> id;
  Length length;
  // Kontsevich-Zorich cocycle (= coordinate of core curves)
  std::map<std::shared_ptr<Id>, Quotient<Coordinate>> cocycle;

 public:
  Implementation() : Implementation(Length(0)) {}
  Implementation(const Length& length) : id(std::make_shared<Id>()), length(length) {
    cocycle[id] = 1;
  }
};

template <typename Coordinate>
Label<Coordinate>::Label() : impl(spimpl::make_impl<Implementation>()) {}

template <typename Coordinate>
Label<Coordinate>::Label(const Length& length) : impl(spimpl::make_impl<Implementation>(length)) {}

template <typename Coordinate>
bool Label<Coordinate>::operator==(const Label<Coordinate>& rhs) const noexcept {
  return impl->id == rhs.impl->id;
}

template <typename Coordinate>
bool Label<Coordinate>::operator<(const Label<Coordinate>& rhs) const noexcept {
  return impl->id < rhs.impl->id;
}

template <typename Coordinate>
void Label<Coordinate>::subtract(Label<Coordinate>& rhs, const Quotient<Coordinate>& multiplicity) noexcept {
  assert(multiplicity >= 0);
  
  impl->length -= rhs.impl->length * multiplicity;

  for (auto source = impl->cocycle.begin(); source != impl->cocycle.end(); ++source) {
    rhs.impl->cocycle[source->first] += source->second * multiplicity;
  }
}

template <typename Coordinate>
typename Label<Coordinate>::Length& Label<Coordinate>::length() noexcept { return impl->length; }

template <typename Coordinate>
const typename Label<Coordinate>::Length& Label<Coordinate>::length() const noexcept { return impl->length; }

template <typename Coordinate>
std::ostream& operator<<(std::ostream& os, const Label<Coordinate>& self) {
  return os << self.length();
}

}  // namespace intervalxt

// Explicit instantiations of templates so that code is generated for the linker.
#include <gmpxx.h>

namespace intervalxt {
template class Label<int>;
template std::ostream& operator<<(std::ostream&, const Label<int>&);
template class Label<mpz_class>;
template std::ostream& operator<<(std::ostream&, const Label<mpz_class>&);
}
