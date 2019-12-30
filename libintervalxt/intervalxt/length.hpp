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

#ifndef LIBINTERVALXT_LENGTH_HPP
#define LIBINTERVALXT_LENGTH_HPP

#include <string>

#include <boost/type_erasure/operators.hpp>
#include <boost/type_erasure/member.hpp>
#include <boost/type_erasure/any.hpp>

#include "forward.hpp"

namespace intervalxt {

template<class T = boost::type_erasure::_self>
struct boolable { static bool apply(const T& arg) { return static_cast<bool>(arg); } };

}

template<class T, class Base>
struct boost::type_erasure::concept_interface<intervalxt::boolable<T>, Base, T> : Base {
  explicit operator bool() const { return ::boost::type_erasure::call(intervalxt::boolable<T>(), *this); }
};

namespace intervalxt {

// Arguably, this is a bit incomplete. One could imagine more relevant
// operations here but we want the implementation to rely on Lengths and not on
// Length.
using LengthInterface = boost::mpl::vector<
  boost::type_erasure::copy_constructible<>,
  boost::type_erasure::equality_comparable<>,
  boost::type_erasure::less_than_comparable<>,
  boost::type_erasure::ostreamable<>,
  intervalxt::boolable<>,
  boost::type_erasure::typeid_<>,
  boost::type_erasure::relaxed
>;
 
using Length = boost::type_erasure::any<LengthInterface>;

}

#endif

