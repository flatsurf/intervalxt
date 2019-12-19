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

#ifndef LIBINTERVALXT_LENGTHS_HPP
#define LIBINTERVALXT_LENGTHS_HPP

#include <vector>
#include <string>

#include <gmpxx.h>
#include <boost/type_erasure/member.hpp>
#include <boost/type_erasure/any.hpp>

#include "label.hpp"
#include "length.hpp"

namespace intervalxt {

BOOST_TYPE_ERASURE_MEMBER((has_member_push), push, 1)
BOOST_TYPE_ERASURE_MEMBER((has_member_pop), pop, 0) 
BOOST_TYPE_ERASURE_MEMBER((has_member_subtract), subtract, 1)
BOOST_TYPE_ERASURE_MEMBER((has_member_subtract_repeated), subtractRepeated, 1)
BOOST_TYPE_ERASURE_MEMBER((has_member_coefficients), coefficients, 1)
BOOST_TYPE_ERASURE_MEMBER((has_member_cmp1), cmp, 1)
BOOST_TYPE_ERASURE_MEMBER((has_member_cmp2), cmp, 2)
BOOST_TYPE_ERASURE_MEMBER((has_member_get), get, 1)
BOOST_TYPE_ERASURE_MEMBER((has_member_render), render, 1)
 
using LengthsInterface = boost::mpl::vector<
  boost::type_erasure::copy_constructible<>,
  has_member_push<void(Label)>,
  has_member_pop<void()>,
  has_member_subtract<void(Label)>,
  has_member_subtract_repeated<Label(Label)>,
  has_member_coefficients<std::vector<mpq_class>(Label) const>,
  has_member_cmp1<int(Label) const>,
  has_member_cmp2<int(Label, Label) const>,
  has_member_get<Length(Label) const>,
  has_member_render<std::string(Label) const>,
  boost::type_erasure::typeid_<>,
  boost::type_erasure::relaxed
>;
 
using Lengths = boost::type_erasure::any<LengthsInterface>;

}

#endif
