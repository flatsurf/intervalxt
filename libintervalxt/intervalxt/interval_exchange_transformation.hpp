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

#ifndef LIBINTERVALXT_INTERVAL_EXCHANGE_TRANSFORMATION_HPP
#define LIBINTERVALXT_INTERVAL_EXCHANGE_TRANSFORMATION_HPP

#include <gmpxx.h>

#include <boost/operators.hpp>
#include <iosfwd>
#include <map>
#include <memory>
#include <optional>
#include <vector>

#include "lengths.hpp"
#include "movable.hpp"
#include "serializable.hpp"

namespace intervalxt {

class IntervalExchangeTransformation : boost::equality_comparable<IntervalExchangeTransformation>,
                                       Serializable<IntervalExchangeTransformation> {
 public:
  IntervalExchangeTransformation();
  IntervalExchangeTransformation(std::shared_ptr<Lengths>, const std::vector<Label> &top, const std::vector<Label> &bottom);

  // Perform up to limit many steps of full Zorich induction until a connection
  // is found. Set to -1 for no limit on the number of steps.
  InductionStep induce(int limit = -1);

  // check for reductibility
  std::optional<IntervalExchangeTransformation> reduce();

  // Perform one step of Zorich induction
  // Return true if a saddle connection is found and false otherwise
  bool zorichInduction();

  // Swap the top and bottom intervals.
  void swap();

  // Return the Sah-Arnoux-Fathi invariant
  std::vector<mpq_class> safInvariant() const;

  // Return whether there is no periodic trajectory via Boshernitzan's
  // algorithm.
  bool boshernitzanNoPeriodicTrajectory() const;

  // Remove the first pair of intervals (assuming that it corresponds to a
  // cylinder, i.e., the leftmost singularity is a connection of length one).
  // After such cylinder is removed the permutation could be reducible.
  void dropSaddleConnection();

  std::vector<IntervalExchangeTransformation> periodicNonPeriodicDecomposition() const;

  // Return the labels of the top permutation (in order.)
  std::vector<Label> top() const;
  // Return the labels of the bottom permutation (in order.)
  std::vector<Label> bottom() const;

  // Return the lengths underlying the intervals of this interval exchange transformation.
  std::shared_ptr<const Lengths> lengths() const;

  // Return the number of intervals in this interval exchange transformation.
  size_t size() const;

  // Return whether swap() has been called an odd number of times.
  bool swapped() const;

  // Return whether this interval exchange transformation and rhs have the same
  // underlying permutation and lengths. Unlike operator==, this ignores the
  // exact labels.
  bool equivalent(const IntervalExchangeTransformation &rhs) const;

  // Return whether this interval exchange transformation and rhs have the same labels and the same lengths.
  bool operator==(const IntervalExchangeTransformation &rhs) const;

  friend std::ostream &operator<<(std::ostream &, const IntervalExchangeTransformation &);

 private:
  Movable<IntervalExchangeTransformation> self;

  friend ImplementationOf<IntervalExchangeTransformation>;
};

std::ostream &operator<<(std::ostream &, const IntervalExchangeTransformation &);

}  // namespace intervalxt

#endif
