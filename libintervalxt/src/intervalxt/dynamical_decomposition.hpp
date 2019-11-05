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

#ifndef LIBINTERVALXT_DYNAMICAL_DECOMPOSITION_HPP
#define LIBINTERVALXT_DYNAMICAL_DECOMPOSITION_HPP

#include <iosfwd>
#include <vector>

#include "external/spimpl/spimpl.h"

#include "intervalxt/forward.hpp"

namespace intervalxt {

template <typename Length>
class DynamicalDecomposition {
 public:
  DynamicalDecomposition(const IntervalExchangeTransformation<Length>&);

  // Return the new component if component splits.
  std::optional<const Component<Length>&> findConnection(const Component<Length>& component, int limit = -1);

  // Run findConnection() recursively on every component.
  void findConnections(int limit = -1);
  
  const std::vector<Component<Length>>& components() const noexcept;

  template <typename T>
  friend std::ostream& operator<<(std::ostream&, const DynamicalDecomposition<T>&);

 private:
  class Implementation;
  spimpl::unique_impl_ptr<Implementation> impl;
};

}

#endif
