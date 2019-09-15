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

#include <boost/operators.hpp>
#include <iosfwd>
#include <optional>
#include <vector>

#include <gmpxx.h>

#include "external/spimpl/spimpl.h"

#include "intervalxt/forward.hpp"
#include "intervalxt/maybe_saddle_connection.hpp"

namespace intervalxt {

template <typename Length>
class IntervalExchangeTransformation {
 public:
  using Label = intervalxt::Label<Length>;
  IntervalExchangeTransformation();
  IntervalExchangeTransformation(const std::vector<Label> &top, const std::vector<size_t> &bottom);
  IntervalExchangeTransformation(const std::vector<Label> &top, const std::vector<Label> &bottom);

  MaybeConnection<Length> induce(int limit = -1);

  // check for reductibility
  std::optional<IntervalExchangeTransformation<Length>> reduce();

  // one step of Zorich induction
  bool zorichInduction(void);

  // Swap the top and bottom intervals.
  void swap();

  // Return the Sah-Arnoux-Fathi invariant
  std::valarray<mpq_class> safInvariant() const;

  // Remove the first pair of intervals (assuming that it corresponds to a
  // cylinder, i.e., the leftmost singularity is a connection of length one).
  // After such cylinder is removed the permutation could be reducible.
  void dropSaddleConnection();

  std::vector<IntervalExchangeTransformation> periodicNonPeriodicDecomposition() const;

  // Return the labels of the top permutation (in order.)
  std::vector<Label> top() const noexcept;
  // Return the labels of the bottom permutation (in order.)
  std::vector<Label> bottom() const noexcept;

  template <typename Length_>
  friend std::ostream &operator<<(std::ostream &, const IntervalExchangeTransformation<Length_> &);

 private:
  class Implementation;
  spimpl::unique_impl_ptr<Implementation> impl;
};

}  // namespace intervalxt

#include "detail/interval_exchange_transformation.ipp"

#endif
