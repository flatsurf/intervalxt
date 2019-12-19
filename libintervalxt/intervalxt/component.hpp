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

#include <functional>
#include <list>
#include <utility>

#include <boost/logic/tribool.hpp>

#include "external/spimpl/spimpl.h"

#include "forward.hpp"

namespace intervalxt {

class Component {
  // Components can not be created directly (other than copying & moving them.)
  // They are created as products of a DynamicalDecomposition.
  Component();

 public:
  using Boundary = std::list<Connection>;

  boost::logic::tribool cylinder() const noexcept;
  boost::logic::tribool withoutPeriodicTrajectory() const noexcept;
  boost::logic::tribool keane() const noexcept;

  /*
  // TODO: What do we want here?

  // The labels on the top, going from right to left.
  std::vector<Label<Length>> top() const;
  // The labels on the bottom, going from right to left.
  std::vector<Label<Length>> bottom() const;

  // The left boundaries of this component as a linked list going from "top to bottom."
  std::vector<Boundary> left() const;
  // The right boundaries of this component as a linked list going from "bottom to top."
  std::vector<Boundary> right() const;
  */

  DecompositionStep decompositionStep(int limit = -1);

  // Return whether all resulting components satisfy target, i.e., target could
  // be established for them all without reaching the limit.
  bool decompose(
      std::function<bool(const Component&)> target = [](const auto& c) {
        return (c.cylinder() || c.withoutPeriodicTrajectory()) ? true : false;
      },
      int limit = -1);

  friend std::ostream& operator<<(std::ostream&, const Component&);

 private:
  class Implementation;
  spimpl::impl_ptr<Implementation> impl;
};

}  // namespace intervalxt

#endif
