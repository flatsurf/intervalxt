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

#include <boost/uuid/uuid.hpp>
#include <memory>

namespace intervalxt {
namespace detail {
// A object uniquely identifying labels that are considered equal.
class Id : public std::enable_shared_from_this<Id> {
  Id() noexcept;

 public:
  // Helpers for serialization
  operator boost::uuids::uuid() const;
  static std::shared_ptr<const Id> make(const boost::uuids::uuid& uuid);
  static std::shared_ptr<const Id> make();
  // Change the name of this Id for printing
  void rename(std::string);
  // Return a human-readable name for this Id; not preserved by serialization.
  friend std::ostream& operator<<(std::ostream&, const Id&);
};
}  // namespace detail
}  // namespace intervalxt

#endif
