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

#ifndef LIBINTERVALXT_SHARED
#define LIBINTERVALXT_SHARED

#include "external/spimpl/spimpl.h"
#include "forward.hpp"

namespace intervalxt {

// A pointer-to-implementation (pimpl) for types that have shared pointer
// semantics, i.e., they are copyable and moveable but the underlying object
// never changes and is shared between all instances.
template <typename T>
using Shared = std::shared_ptr<ImplementationOf<T>>;

}  // namespace intervalxt

#endif
