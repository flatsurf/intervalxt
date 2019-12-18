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

// This file forward declares all the types in the intervalxt namespace.

#ifndef LIBINTERVALXT_FORWARD_HPP
#define LIBINTERVALXT_FORWARD_HPP

#include <optional>
#include <variant>

#include "intervalxt.hpp"

namespace intervalxt {

template <typename T>
class Length;

template <typename Length>
class Label;

template <typename Length>
class IntervalExchangeTransformation;

template <typename Length>
class DynamicalDecomposition;

template <typename Length>
class Connection;

template <typename Length>
class MaybeConnection;

template <typename Length>
class Component;

template <typename Length>
struct DecompositionStep;

template <typename Length>
struct InductionStep;

}  // namespace intervalxt

#endif
