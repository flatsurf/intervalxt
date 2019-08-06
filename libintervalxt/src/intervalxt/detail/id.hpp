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

#ifndef LIBINTERVALXT_DETAIL_ID_HPP
#define LIBINTERVALXT_DETAIL_ID_HPP

#include <memory>

#include <boost/uuid/uuid.hpp>

namespace intervalxt {
namespace detail {
// A object uniquely identifying labels that are considered equal.
class Id : public std::enable_shared_from_this<Id> {
  Id() noexcept;

 public:
  // Helpers for serialization
  operator boost::uuids::uuid() const noexcept;
  static std::shared_ptr<const Id> make(const boost::uuids::uuid& uuid) noexcept;
  static std::shared_ptr<const Id> make() noexcept;
};
}
}  // namespace intervalxt

#endif

