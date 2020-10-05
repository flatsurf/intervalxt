/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019 Vincent Delecroix
 *        Copyright (C) 2019-2020 Julian Rüth
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

#ifndef LIBINTERVALXT_CONNECTION_IMPL_HPP
#define LIBINTERVALXT_CONNECTION_IMPL_HPP

#include <memory>

#include "../../intervalxt/connection.hpp"
#include "../../intervalxt/separatrix.hpp"
#include "implementation_of_decomposition.hpp"

namespace intervalxt {

template <>
class ImplementationOf<Connection> : public ImplementationOfDecomposition {
 public:
  ImplementationOf(const DynamicalDecomposition&, DecompositionState::Connection);
  static Connection make(const DynamicalDecomposition&, DecompositionState::Connection);

  // The underlying data of this connection.
  // We could have stored a pointer into the DecompositionState here instead
  // but this would make operator- much harder to implement.
  DecompositionState::Connection connection;
};

template <typename... Args>
Connection::Connection(PrivateConstructor, Args&&... args) :
  self(spimpl::make_impl<ImplementationOf<Connection>>(std::forward<Args>(args)...)) {}

}  // namespace intervalxt

#endif
