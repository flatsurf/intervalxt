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

#ifndef LIBINTERVALXT_COMPONENT_HPP
#define LIBINTERVALXT_COMPONENT_HPP

#include <list>
#include <utility>

#include "external/spimpl/spimpl.h"

#include "intervalxt/forward.hpp"

namespace intervalxt {

template <typename Length>
class Component {
 public:
  using Boundary = std::list<Connection<Length>>;

  bool cylinder();
  bool certified();

  // Returns the left and the right boundary of this component as circular linked lists;
  // note that the right boundary goes "bottom to top" whereas the left
  // boundary goes "top to bottom".
  std::pair<Boundary, Boundary> boundary();

  template <typename T>
  friend std::ostream& operator<<(std::ostream&, const Component<T>&);

 private:
  class Implementation;
  spimpl::unique_impl_ptr<Implementation> impl;
};

}

#endif
