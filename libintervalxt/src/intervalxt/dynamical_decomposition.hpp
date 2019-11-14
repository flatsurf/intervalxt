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

// Frontend to a decomposition of an IntervalExchangeTransformation into
// Components.
template <typename Length>
class DynamicalDecomposition {
 public:
  DynamicalDecomposition(const IntervalExchangeTransformation<Length>&);
  std::unique_ptr<DynamicalDecomposition<Length>> clone() const;

  void decompose(std::function<bool(const Component<Length>&)> target= [](const auto& c) {
      return c.cylinder() || c.withoutPeriodicTrajectovy();
    }, int limit = -1);
  
  std::vector<Component<Length>> components() const noexcept;

  template <typename T>
  friend std::ostream& operator<<(std::ostream&, const DynamicalDecomposition<T>&);

 private:
  class Implementation;
  spimpl::impl_ptr<Implementation> impl;

  friend class Component<Length>;
  friend class Connection<Length>;
  friend class MaybeConnection<Length>;
};

}

#include "detail/dynamical_decomposition.ipp"

#endif
