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

// This file forward declares all the types in the intervalxt namespace.

#ifndef LIBINTERVALXT_FORWARD_HPP
#define LIBINTERVALXT_FORWARD_HPP

#include <optional>
#include <variant>

#include "local.hpp"

namespace intervalxt {

class Component;

class Connection;

struct DecompositionStep;

class DynamicalDecomposition;

class HalfEdge;

template <typename T>
class ImplementationOf;

struct InductionStep;

class IntervalExchangeTransformation;

class Label;

struct PrivateConstructor {};

class Separatrix;

template <typename T>
class Serializable;

template <typename T>
struct Serialization;

using Side = std::variant<Connection, HalfEdge>;

}  // namespace intervalxt

#endif
