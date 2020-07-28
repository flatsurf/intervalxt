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

#include <ostream>

#include "../intervalxt/connection.hpp"

#include "impl/connection.impl.hpp"

namespace intervalxt {

Connection::Connection() :
  // We assume that the caller sets impl.
  impl(nullptr) {}

Connection Connection::operator-() const noexcept {
  return Implementation::make(impl->decomposition, impl->target, impl->source);
}

bool Connection::operator==(const Connection& rhs) const noexcept {
  // Note that we identify separatrices on different decompositions. This seems
  // wrong. However, it is currently, necessary because Lenghts are not shared
  // in libflatsurf between IETs, see #72.
  return impl->source == rhs.impl->source && impl->target == rhs.impl->target;
}

bool Connection::parallel() const noexcept {
  return impl->source.parallel();
}

bool Connection::antiparallel() const noexcept {
  return not parallel();
}

Separatrix Connection::source() const noexcept {
  return impl->source;
}

Separatrix Connection::target() const noexcept {
  return impl->target;
}

Implementation<Connection>::Implementation(std::shared_ptr<DecompositionState> decomposition, const Separatrix& source, const Separatrix& target) :
  decomposition(decomposition),
  source(source),
  target(target) {}

Connection Implementation<Connection>::make(std::shared_ptr<DecompositionState> decomposition, const Separatrix& source, const Separatrix& target) {
  Connection connection;
  connection.impl = spimpl::make_impl<Implementation>(decomposition, source, target);
  return connection;
}

std::ostream& operator<<(std::ostream& os, const Connection& self) {
  return os << "[" << self.impl->source << " ⚯ " << self.impl->target << "]";
}

}  // namespace intervalxt

namespace std {

using namespace intervalxt;

size_t hash<Connection>::operator()(const Connection& self) const noexcept {
  // We should use hash_combine here, see https://github.com/flatsurf/intervalxt/issues/67
  return hash<Separatrix>()(self.source()) + (hash<Separatrix>()(self.target()) << 32);
}

}  // namespace std
