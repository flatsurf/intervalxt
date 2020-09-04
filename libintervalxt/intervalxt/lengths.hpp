/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019-2020 Vincent Delecroix
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

#ifndef LIBINTERVALXT_LENGTHS_HPP
#define LIBINTERVALXT_LENGTHS_HPP

#include <gmpxx.h>

#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/member.hpp>
#include <boost/type_erasure/operators.hpp>
#include <string>
#include <type_traits>
#include <unordered_set>
#include <vector>

#include "erased/boost.hpp"
#include "erased/serializable.hpp"
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
BOOST_TYPE_ERASURE_MEMBER((has_member_similar), similar, 5)
BOOST_TYPE_ERASURE_MEMBER((has_member_get), get, 1)
BOOST_TYPE_ERASURE_MEMBER((has_member_render), render, 1)
BOOST_TYPE_ERASURE_MEMBER((has_member_only), only, 1)
BOOST_TYPE_ERASURE_MEMBER((has_member_forget), forget, 0)

struct LengthsInterface;

using Lengths = boost::type_erasure::any<LengthsInterface>;

struct LengthsInterface : boost::mpl::vector<
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
                              // Return Lengths ignoring any labels not in the passed set of labels.
                              has_member_only<Lengths(const std::unordered_set<Label>&) const>,
                              // Return Lengths without any additional tracking of structure such as
                              // keeping track of detected/injected connections.
                              has_member_forget<Lengths() const>,
                              // Return whether for this Lenghts and labels a, b the quotient of
                              // length(a)/length(b) is the same as the quotient of lengths(aa)/lengths(bb)
                              // for the other Lengths.
                              has_member_similar<bool(Label, Label, const Lengths&, Label, Label) const>,
                              intervalxt::erased::is_serializable<Lengths>,
                              boost::type_erasure::typeid_<>,
                              boost::type_erasure::relaxed> {
  template <typename Archive>
  friend void save(Archive& archive, const Lengths& self) {
    ::intervalxt::erased::saveErased(archive, self);
  }

  template <typename Archive>
  friend void load(Archive& archive, Lengths& self) {
    ::intervalxt::erased::loadErased(archive, self);
  }
};

}  // namespace intervalxt

#endif
