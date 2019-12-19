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

#include <boost/type_erasure/member.hpp>
#include <boost/type_erasure/any.hpp>

#include "forward.hpp"

namespace intervalxt {

BOOST_TYPE_ERASURE_MEMBER((has_member_push), push, 1)
BOOST_TYPE_ERASURE_MEMBER((has_member_pop), pop, 0) 
 
namespace te = boost::type_erasure;
 
using LengthsSpecification = boost::mpl::vector<
  boost::type_erasure::copy_constructible<>,
  has_member_push<void(const Label&)>,
  has_member_pop<void()>,
  boost::type_erasure::relaxed
>;
 
using Lengths = boost::type_erasure::any<LengthsSpecification>;

}

#endif

