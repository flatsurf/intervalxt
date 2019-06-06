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

#include "intervalxt/label.hpp"
#include "intervalxt/length.hpp"

// Consult the rant on top of interval_exchange_transformation.ipp for the
// rationale of this file.

namespace intervalxt {
namespace {
// A unique object relating labels that are considered equal.
class Id {};
}

template <typename Length>
class Label<Length>::Implementation {
  friend Label<Length>;

  std::shared_ptr<Id> id;
  Length length;

 public:
  Implementation() : Implementation(Length()) {}
  Implementation(const Length& length) : id(std::make_shared<Id>()), length(length) {}
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
Length& Label<Length>::length() noexcept { return impl->length; }

template <typename Length>
const Length& Label<Length>::length() const noexcept { return impl->length; }

template <typename Length>
std::ostream& operator<<(std::ostream& os, const Label<Length>& self) {
  return os << self.length();
}
}  // namespace intervalxt

#endif
