/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019-2020 Vincent Delecroix
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

#ifndef LIBINTERVALXT_INTERVAL_EXCHANGE_TRANSFORMATION_IMPL_HPP
#define LIBINTERVALXT_INTERVAL_EXCHANGE_TRANSFORMATION_IMPL_HPP

#include <list>

#include "../../intervalxt/interval_exchange_transformation.hpp"

#include "interval.hpp"

#include "similarity_tracker.hpp"

namespace intervalxt {

template <>
class Implementation<IntervalExchangeTransformation> {
 public:
  Implementation(std::shared_ptr<Lengths>, const std::vector<Label>&, const std::vector<Label>&);

  static IntervalExchangeTransformation withLengths(const IntervalExchangeTransformation&, const std::function<std::shared_ptr<Lengths>(std::shared_ptr<Lengths>)>&);
  static std::string render(const IntervalExchangeTransformation&, Label);

  // Return the translation vector for the label i
  // The output is a vector of mpq_class with respect to the irrational basis
  // used for the lengths of the iet.
  std::valarray<mpq_class> translation(Label) const;

  std::valarray<mpq_class> coefficients(Label) const;

  std::valarray<mpq_class> saf() const;
  bool saf0() const;

  std::list<Interval> top;
  std::list<Interval> bottom;
  std::shared_ptr<Lengths> lengths;
  const size_t degree;
  SimilarityTracker similarityTracker = {};
  bool swap = false;
};

}  // namespace intervalxt

#endif
