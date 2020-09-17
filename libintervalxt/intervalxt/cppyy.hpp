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

#ifndef LIBINTERVALXT_CPPYY_HPP
#define LIBINTERVALXT_CPPYY_HPP

#include <iosfwd>
#include <memory>
#include <vector>

#include "dynamical_decomposition.hpp"
#include "interval_exchange_transformation.hpp"
#include "label.hpp"
#include "lengths.hpp"
#include "sample/lengths.hpp"

namespace intervalxt::cppyy {

// We work around
// https://bitbucket.org/wlav/cppyy/issues/268/segfault-with-types-in-unnamed-namespaces
// by moving the Lengths type out of its unnamed namespace.
template <typename V>
class Lengths : public ::intervalxt::sample::Lengths<typename V::value_type> {
 public:
  using T = typename V::value_type;

  using ::intervalxt::sample::Lengths<T>::Lengths;

  using ::intervalxt::sample::Lengths<T>::labels;
  using ::intervalxt::sample::Lengths<T>::push;
  using ::intervalxt::sample::Lengths<T>::pop;
  using ::intervalxt::sample::Lengths<T>::clear;
  using ::intervalxt::sample::Lengths<T>::cmp;
  using ::intervalxt::sample::Lengths<T>::subtract;
  using ::intervalxt::sample::Lengths<T>::subtractRepeated;
  using ::intervalxt::sample::Lengths<T>::coefficients;
  using ::intervalxt::sample::Lengths<T>::render;
  using ::intervalxt::sample::Lengths<T>::get;
  using ::intervalxt::sample::Lengths<T>::only;
  using ::intervalxt::sample::Lengths<T>::forget;
  using ::intervalxt::sample::Lengths<T>::similar;
  using ::intervalxt::sample::Lengths<T>::operator==;
  using ::intervalxt::sample::Lengths<T>::operator bool;
  using ::intervalxt::sample::Lengths<T>::operator T;
};

template <typename L>
auto IntervalExchangeTransformation(L &lengths, const std::vector<int> &permutation) {
  ::intervalxt::Lengths erasedLengths = lengths;
  auto top = lengths.labels();
  std::vector<Label> bottom;
  for (int p : permutation)
    bottom.push_back(top[p]);
  return ::intervalxt::IntervalExchangeTransformation(std::make_shared<::intervalxt::Lengths>(erasedLengths), top, bottom);
}

}  // namespace intervalxt::cppyy

namespace intervalxt {

std::ostream &operator<<(std::ostream &, const IntervalExchangeTransformation &);

}

#endif
