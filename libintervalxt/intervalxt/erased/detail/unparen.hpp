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

#ifndef LIBINTERVALXT_ERASED_DETAIL_UNPAREN_HPP
#define LIBINTERVALXT_ERASED_DETAIL_UNPAREN_HPP

// Remove () from macro types, similar to
// https://stackoverflow.com/a/4298441/812379

namespace intervalxt::erased::detail {

template<typename T>
struct unparen;

template<typename R, typename T>
struct unparen<R(T)> {
  typedef T type;
};

}

#endif
