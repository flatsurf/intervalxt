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

#ifndef LIBINTERVALXT_ERASED_BOOST_HPP
#define LIBINTERVALXT_ERASED_BOOST_HPP

#include <memory>

#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/free.hpp>

#include "forward.hpp"

namespace intervalxt::erased {

BOOST_TYPE_ERASURE_FREE((has_free_serializable), serializable, 2)

// Concept that asserts the existence of a free serializable(const Erased&)
// function which returns a pointer to a polymorphic object that can be handled
// by cereal.
template <typename Erased>
using is_serializable = ::intervalxt::erased::has_free_serializable<std::unique_ptr<::intervalxt::erased::Serializable<Erased>>(const boost::type_erasure::_self&, const Tag<Erased>&)>;

}  // namespace intervalxt::erased

#endif
