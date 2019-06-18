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
#include <optional>
#include <utility>
#include <variant>

#include "intervalxt/forward.hpp"

namespace intervalxt {

struct MinimalityGuarantee {};

template <typename Length>
struct NonSeparatingSaddleConnection {
  // Note: Label is already identified when this message is returned.
  std::pair<Label<Length>, Label<Length>> saddleConnection;
};

template <typename Length>
struct SeparatingSaddleConnection {
  std::unique_ptr<IntervalExchangeTransformation<Length>> addedIET;
  std::pair<Label<Length>, Label<Length>> saddleConnection;
};

template <typename Length>
using MaybeSaddleConnection = std::optional<std::variant<MinimalityGuarantee, NonSeparatingSaddleConnection<Length>, SeparatingSaddleConnection<Length>>>;

}  // namespace intervalxt

#endif
