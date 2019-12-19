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

#include <iosfwd>
#include <map>
#include <optional>
#include <vector>

#include <boost/operators.hpp>
#include <gmpxx.h>

#include "external/spimpl/spimpl.h"

#include "forward.hpp"
#include "lengths.hpp"

namespace intervalxt {

class IntervalExchangeTransformation {
 public:
  IntervalExchangeTransformation();
  IntervalExchangeTransformation(Lengths * const, const std::vector<Label> &top, const std::vector<size_t> &bottom);
  IntervalExchangeTransformation(Lengths * const, const std::vector<Label> &top, const std::vector<Label> &bottom);

  // Perform up to limit many steps of full Zorich induction until a connection
  // is found. Set to -1 for no limit on the number of steps.
  InductionStep induce(int limit = -1);

  // check for reductibility
  std::optional<IntervalExchangeTransformation> reduce();

  // one step of Zorich induction
  bool zorichInduction();

  // Swap the top and bottom intervals.
  void swap();

  // Return the Sah-Arnoux-Fathi invariant
  std::valarray<mpq_class> safInvariant() const;

  // Boshernitzan criterion for absence of periodic trajectories
  bool boshernitzanNoPeriodicTrajectory() const;

  // Remove the first pair of intervals (assuming that it corresponds to a
  // cylinder, i.e., the leftmost singularity is a connection of length one).
  // After such cylinder is removed the permutation could be reducible.
  void dropSaddleConnection();

  std::vector<IntervalExchangeTransformation> periodicNonPeriodicDecomposition() const;

  // Return the labels of the top permutation (in order.)
  std::vector<Label> top() const noexcept;
  // Return the labels of the bottom permutation (in order.)
  std::vector<Label> bottom() const noexcept;

  // Return the number of intervals in this interval exchange transformation.
  size_t size() const noexcept;

  friend std::ostream &operator<<(std::ostream &, const IntervalExchangeTransformation&);

 private:
  class Implementation;
  spimpl::unique_impl_ptr<Implementation> impl;
};

}  // namespace intervalxt

#endif
