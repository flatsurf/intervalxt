/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019      Vincent Delecroix
 *        Copyright (C) 2019-2021 Julian Rüth
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
#include "copyable.hpp"
#include "half_edge.hpp"

namespace intervalxt {

class LIBINTERVALXT_API Component : boost::equality_comparable<Component> {
  // Components can not be created directly (other than copying & moving them.)
  // They are created in the process of a DynamicalDecomposition.
  template <typename... Args>
  Component(PrivateConstructor, Args&&... args);

 public:
  boost::logic::tribool cylinder() const;
  boost::logic::tribool withoutPeriodicTrajectory() const;
  boost::logic::tribool keane() const;

  // The half edges that make up the top contour, left to right.
  std::vector<HalfEdge> topContour() const;

  // The half edges that make up the bottom contour, left to right.
  std::vector<HalfEdge> bottomContour() const;

  HalfEdge topContour(Label) const;

  HalfEdge bottomContour(Label) const;

  // Return a perimeter, walking around this component in counterclockwise order.
  std::vector<Side> perimeter() const;

  // The portion of the perimeter that is on the left end (from top to bottom.)
  // Note that each entry is a Connection.
  std::vector<Side> left() const;
  // The portion of the perimeter that is on the right end (from bottom to top.)
  // Note that each entry is a Connection.
  std::vector<Side> right() const;
  // The portion of the perimeter that is on the bottom (from left to right.)
  std::vector<Side> bottom() const;
  // The portion of the perimeter that is on the top (from right to left.)
  std::vector<Side> top() const;

  DynamicalDecomposition& decomposition() const;

  DecompositionStep decompositionStep(int limit = -1);

  // Return a copy of the Interval Exchange Transformation that is underlying
  // this component. Note that the Lengths of that copy might not be identical
  // to the current Lengths object but rather its forget().
  IntervalExchangeTransformation iet() const;

  // Return whether all resulting components satisfy target, i.e., target could
  // be established for them all without reaching the limit.
  bool decompose(
      std::function<bool(const Component&)> target = [](const auto& c) {
        return (c.cylinder() || c.withoutPeriodicTrajectory()) ? true : false;
      },
      int limit = -1);

  // Insert (artificial) connections left and right of the given half edge. The
  // labels specify the source and target separatrices of the new connections;
  // the entries of left and right are proceeding towards the interior, the left
  // ones are oriented top to bottom whereas the right ones are oriented bottom
  // to top.
  std::pair<std::list<Connection>, std::list<Connection>> inject(const HalfEdge&, const std::vector<std::pair<Label, Label>>& left, const std::vector<std::pair<Label, Label>>& right);

  bool operator==(const Component& rhs) const;

  LIBINTERVALXT_API friend std::ostream& operator<<(std::ostream&, const Component&);

 private:
  Copyable<Component> self;

  friend ImplementationOf<Component>;
};

LIBINTERVALXT_API std::ostream& operator<<(std::ostream&, const Side&);

}  // namespace intervalxt

#endif
