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

#ifndef LIBINTERVALXT_DETAIL_LABEL_IPP
#define LIBINTERVALXT_DETAIL_LABEL_IPP

#include <memory>

#include <boost/uuid/uuid.hpp>

#include "intervalxt/detail/id.hpp"
#include "intervalxt/label.hpp"
#include "intervalxt/length.hpp"

// Consult the rant on top of interval_exchange_transformation.ipp for why we
// have to put the implementation into this header file.

namespace intervalxt {

template <typename Length>
class Label<Length>::Implementation {
  std::shared_ptr<const detail::Id> id;
  Length length;

 public:
  Implementation() : id(nullptr), length() {}
  Implementation(const Length& length) : id(detail::Id::make()), length(length) {}

  template <typename L>
  friend std::ostream& operator<<(std::ostream&, const Label<L>&);
  friend Label<Length>;
  template <typename Archive, typename Len>
  friend void load(Archive&, Label<Len>&);
  template <typename Archive, typename Len>
  friend void save(Archive&, const Label<Len>&);
};

template <typename Length>
Label<Length>::Label() : impl(spimpl::make_impl<Implementation>()) {}

template <typename Length>
Label<Length>::Label(const Length& length) : impl(spimpl::make_impl<Implementation>(length)) {}

template <typename Length>
bool Label<Length>::operator==(const Label<Length>& rhs) const noexcept {
  return impl->id == rhs.impl->id;
}

template <typename Length>
bool Label<Length>::operator<(const Label<Length>& rhs) const noexcept {
  return impl->id < rhs.impl->id;
}

template <typename Length>
Label<Length>& Label<Length>::operator=(const Length& rhs) noexcept {
  assert(impl->id != nullptr && "invalid label, maybe after the end of a trivial IET, cannot be assigned to");
  impl->length = rhs;
  return *this;
}

template <typename Length>
Length& Label<Length>::length() noexcept {
  assert(impl->id != nullptr && "invalid label, maybe after the end of a trivial IET, has no length");
  return impl->length;
}

template <typename Length>
const Length& Label<Length>::length() const noexcept {
  assert(impl->id != nullptr && "invalid label, maybe after the end of a trivial IET, has no length");
  return impl->length;
}

template <typename Length>
std::ostream& operator<<(std::ostream& os, const Label<Length>& self) {
  if (self.impl->id == nullptr)
    return os << "nil";
  else
    return os << self.length() << "[" << *self.impl->id << "]";
}

}  // namespace intervalxt

#endif
