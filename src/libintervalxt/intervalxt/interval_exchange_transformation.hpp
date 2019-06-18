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
#include <optional>
#include <vector>

#include "external/spimpl/spimpl.h"

#include "intervalxt/forward.hpp"
#include "intervalxt/maybe_saddle_connection.hpp"

namespace intervalxt {

template <typename Length>
class IntervalExchangeTransformation {
 public:
  using Label = intervalxt::Label<Length>;
  IntervalExchangeTransformation(const std::vector<Label> &top, const std::vector<size_t> &bottom);
  IntervalExchangeTransformation(const std::vector<Label> &top, const std::vector<Label> &bottom);

  // Test whether this permutation is reducible. If it is, return the top and
  // bottom labels after which we cut.
  std::optional<std::pair<const Label &, const Label &>> reduce() const;

  // Return whether there is a periodic trajectory via Boshernitzan's
  // algorithm.
  std::optional<bool> periodicTrajectory() const;

  // Perform n zorich induction steps while the length of the first interval on
  // the top is smaller than the one on the bottom.
  void zorichInduction(int n = 1);

  MaybeSaddleConnection<Length> induce(int limit = -1);

  // Swap the top and bottom intervals.
  void swap();

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
