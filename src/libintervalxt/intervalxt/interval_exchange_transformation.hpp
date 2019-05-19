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
#include <vector>

#include "intervalxt/forward.hpp"
#include "intervalxt/intervalxt.hpp"

#include "intervalxt/label.hpp"

namespace intervalxt {

using Permutation = std::vector<int>;

template <typename Tlen, typename Tmat>
class IntervalExchangeTransformation {
  size_t n;  // number of intervals (= pairs of Label)

  Interval<Tlen, Tmat> *top;  // pointer to the first interval on top
  Interval<Tlen, Tmat> *bot;  // pointer to the first interval on bottom

  std::vector<Label<Tlen, Tmat>> labels;  // the labels

  void reset(size_t nintervals);

 public:
  IntervalExchangeTransformation(size_t n);
  IntervalExchangeTransformation(const Permutation &topperm, const Permutation &botperm);

  // check whether data is consistent
  void check();

  // data access
  Permutation topPermutation() const;
  Permutation botPermutation() const;
  std::vector<Tlen> lengths() const;
  std::vector<std::vector<Tmat>> KontsevichZorichCocycle() const;

  // set or reset data
  void setIdentityPermutation();
  void setTop(const Permutation &p);
  void setBot(const Permutation &p);
  void setLengths(const std::vector<Tlen> &lengths);

  // test whether the permutation is irreducible. If it is reducible
  // return true and set argument to the labels on top and bot
  // after which we cut. Otherwise returns fals.
  bool isReducible() const;
  bool isReducible(Label<Tlen, Tmat> *&, Label<Tlen, Tmat> *&) const;

  // check for periodic trajectory via Boshernitzan's algorithm
  bool hasNoPeriodicTrajectory() const;

  // assuming that top[0] > bot[0] and perform one step of Zorich induction
  // (if top[0] <= bot[0] the iet is left unchanged)
  void zorichInductionStep();
  void zorichInductions(size_t n);

  // swap the top and bottom intervals
  void swapTopBot();

  // remove the first pair of intervals (assuming that it corresponds to
  // a cylinder, that is the leftmost singularity is a connection of length
  // one). After such cylinder is removed the permutation could be reducible.
  void dropSaddleConnection();

  // Warning: destructive!
  // each component found is put as a vector component
  std::vector<IntervalExchangeTransformation<Tlen, Tmat>> periodicNonPeriodicDecomposition();

  template <typename TTlen, typename TTmat>
  friend std::ostream &operator<<(std::ostream &, const IntervalExchangeTransformation<TTlen, TTmat> &);

  static Tmat fdiv(Tlen &a, Tlen &b);
};

}  // namespace intervalxt

#endif
