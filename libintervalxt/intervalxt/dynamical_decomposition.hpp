/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019 Vincent Delecroix
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

#ifndef LIBINTERVALXT_DYNAMICAL_DECOMPOSITION_HPP
#define LIBINTERVALXT_DYNAMICAL_DECOMPOSITION_HPP

#include <boost/logic/tribool.hpp>
#include <boost/operators.hpp>
#include <functional>
#include <iosfwd>
#include <vector>

#include "component.hpp"
#include "shared.hpp"

namespace intervalxt {

// Frontend to decompose an IntervalExchangeTransformation into Components.
class DynamicalDecomposition : boost::equality_comparable<DynamicalDecomposition> {
  template <typename... Args>
  DynamicalDecomposition(PrivateConstructor, Args&&... args);

 public:
  DynamicalDecomposition(const IntervalExchangeTransformation&);

  // Return whether all resulting components satisfy target, i.e., target could
  // be established for them all without reaching the limit.
  bool decompose(
      std::function<bool(const Component&)> target = [](const auto& c) {
        return (c.cylinder() || c.withoutPeriodicTrajectory()) ? true : false;
      },
      int limit = -1);

  std::vector<Component> components() const;

  bool operator==(const DynamicalDecomposition&) const;

  friend std::ostream& operator<<(std::ostream&, const DynamicalDecomposition&);

 private:
  Shared<DynamicalDecomposition> self;

  friend ImplementationOf<DynamicalDecomposition>;
};

}  // namespace intervalxt

#endif
