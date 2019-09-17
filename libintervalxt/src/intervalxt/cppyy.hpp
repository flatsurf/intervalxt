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

#ifndef LIBINTERVALXT_CPPYY_HPP
#define LIBINTERVALXT_CPPYY_HPP

#include <iosfwd>

#include "intervalxt/interval_exchange_transformation.hpp"
#include "intervalxt/label.hpp"
#include "intervalxt/length.hpp"

namespace intervalxt {
template <typename T>
std::ostream &operator<<(std::ostream &, const Length<T> &);
template <typename Length>
std::ostream &operator<<(std::ostream &, const Label<Length> &);
template <typename Length>
std::ostream &operator<<(std::ostream &, const IntervalExchangeTransformation<Length> &);
}  // namespace intervalxt

#endif
