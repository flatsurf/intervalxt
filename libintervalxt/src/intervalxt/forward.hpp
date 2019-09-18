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

#include "intervalxt/intervalxt.hpp"

namespace intervalxt {

template <typename T>
class Length;

template <typename Length>
class Label;

template <typename Length>
class IntervalExchangeTransformation;

struct NoPeriodicTrajectoryGuarantee;

template <typename Length>
struct Cylinder;

template <typename Length>
struct NonSeparatingConnection;

template <typename Length>
struct SeparatingConnection;

template <typename Length>
using MaybeConnection = std::optional<std::variant<
    NoPeriodicTrajectoryGuarantee,
    Cylinder<Length>,
    NonSeparatingConnection<Length>,
    SeparatingConnection<Length>>>;

class RationalLinearSubspace;
}  // namespace intervalxt

#endif
