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

#ifndef LIBINTERVALXT_MAYBE_SADDLE_CONNECTION_HPP
#define LIBINTERVALXT_MAYBE_SADDLE_CONNECTION_HPP

#include <memory>
#include <utility>

#include "intervalxt/forward.hpp"

#include "intervalxt/label.hpp"

namespace intervalxt {

// The types in this file make up MaybeConnection defined in forward.hpp.

// the iet has no periodic trajectory
struct NoPeriodicTrajectoryGuarantee {};

// the given label appears both on top and bottom at the left
// hand side of the iet
// Note: Label is already removed when this message is returned.
template <typename Length>
struct Cylinder {
  Label<Length> label;
};

// the pair of labels have the same lengths.
// Note: top is already removed when this message is returned.
template <typename Length>
struct NonSeparatingConnection {
  Label<Length> bottom, top;
};

// Note: top is already removed when this message is returned.
template <typename Length>
struct SeparatingConnection {
  std::unique_ptr<IntervalExchangeTransformation<Length>> addedIET;
  Label<Length> bottom, top;
};

}  // namespace intervalxt

#endif
