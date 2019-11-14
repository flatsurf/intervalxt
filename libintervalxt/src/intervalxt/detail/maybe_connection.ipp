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

#ifndef LIBINTERVALXT_DETAIL_MAYBE_CONNECTION_IPP
#define LIBINTERVALXT_DETAIL_MAYBE_CONNECTION_IPP

#include <iostream>

#include "../dynamical_decomposition.hpp"

namespace intervalxt {

template <typename Length>
MaybeConnection<Length>::MaybeConnection() {
  // we assume the caller to be aware that the impl needs to be initialized.
}

template <typename Length>
MaybeConnection<Length> MaybeConnection<Length>::Implementation::make(std::shared_ptr<const DecompositionState> state, ConnectionState* const connection) {
  MaybeConnection<Length> ret;
  ret.impl = spimpl::make_impl<Implementation>(state, connection);
  return ret;
}

template <typename Length>
Label<Length> MaybeConnection<Length>::before() const noexcept { return impl->connection->before; }

template <typename Length>
Label<Length> MaybeConnection<Length>::after() const noexcept { return impl->connection->after; }

template <typename Length>
std::optional<Connection<Length>> MaybeConnection<Length>::connection() const noexcept {
  if (impl->connection->twin == nullptr) return {};
  return impl->state->make(impl->connection);
}

template <typename Length>
bool MaybeConnection<Length>::operator==(const MaybeConnection<Length>& rhs) const noexcept {
  return impl->connection == rhs.impl->connection;
}

template <typename Length>
std::ostream& operator<<(std::ostream& os, const MaybeConnection<Length>& self) {
  return os << self.impl->connection->label << " → ?";
}

}

#endif
