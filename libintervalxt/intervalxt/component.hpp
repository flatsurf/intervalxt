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

#include <boost/logic/tribool.hpp>
#include <boost/operators.hpp>
#include <functional>
#include <list>
#include <utility>

#include "connection.hpp"
#include "external/spimpl/spimpl.h"
#include "forward.hpp"
#include "half_edge.hpp"

namespace intervalxt {

class Component : boost::equality_comparable<Component> {
  // Components can not be created directly (other than copying & moving them.)
  // They are created in the process of a DynamicalDecomposition.
  Component();

 public:
  boost::logic::tribool cylinder() const noexcept;
  boost::logic::tribool withoutPeriodicTrajectory() const noexcept;
  boost::logic::tribool keane() const noexcept;

  // The half edges that make up the top contour, left to right.
  std::vector<HalfEdge> topContour() const;
  // The half edges that make up the bottom contour, left to right.
  std::vector<HalfEdge> bottomContour() const;

  // Return a perimeter, walking around this component in counterclockwise order.
  std::vector<Side> perimeter() const;

  // The portion of the perimeter that is on the left end (from top to bottom.)
  std::vector<Side> left() const;
  // The portion of the perimeter that is on the right end (from bottom to top.)
  std::vector<Side> right() const;
  // The portion of the perimeter that is on the bottom (from left to right.)
  std::vector<Side> bottom() const;
  // The portion of the perimeter that is on the top (from right to left.)
  std::vector<Side> top() const;

  DecompositionStep decompositionStep(int limit = -1);

  // Return whether all resulting components satisfy target, i.e., target could
  // be established for them all without reaching the limit.
  bool decompose(
      std::function<bool(const Component&)> target = [](const auto& c) {
        return (c.cylinder() || c.withoutPeriodicTrajectory()) ? true : false;
      },
      int limit = -1);

  // Insert (artificial) connections left and right of the given half edge. The
  // labels specify the source and target separatrices of the new connections;
  // the entries of left and right are proceding towards the interior, the left
  // ones are oriented top to bottom whereas the right ones are oriented bottom
  // to top.
  std::pair<std::list<Connection>, std::list<Connection>> inject(const HalfEdge&, const std::vector<std::pair<Label, Label>>& left, const std::vector<std::pair<Label, Label>>& right);

  bool operator==(const Component& rhs) const;

  friend std::ostream& operator<<(std::ostream&, const Component&);

 private:
  using Implementation = ::intervalxt::Implementation<Component>;
  spimpl::impl_ptr<Implementation> impl;

  friend Implementation;
};

std::ostream& operator<<(std::ostream&, const Side&);

}  // namespace intervalxt

#endif
