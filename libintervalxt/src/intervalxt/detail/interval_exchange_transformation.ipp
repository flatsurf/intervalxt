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

#ifndef LIBINTERVALXT_DETAIL_INTERVAL_EXCHANGE_TRANSFORMATION_IPP
#define LIBINTERVALXT_DETAIL_INTERVAL_EXCHANGE_TRANSFORMATION_IPP

// Unfortunately, we need to ship and include the implementation of
// IntervalExchangeTransformation in the header files, at least until we get
// C++ modules.
// Why not use a pure virtual Length base class instead? Apart from a minor
// performance hit, you would not want to mix different lengths anyway. So, you
// need a lot of casting code that would only exist to work around C++'s
// shortcomings.

#include <algorithm>
#include <iostream>
#include <list>
#include <set>

#include <boost/algorithm/string/join.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include "intervalxt/interval_exchange_transformation.hpp"

namespace intervalxt {
namespace {

template <typename Length>
struct Interval {
  Label<Length>& label;
  const typename std::list<Interval>::iterator twin;
};
}  // namespace

template <typename Length>
class IntervalExchangeTransformation<Length>::Implementation {
  friend IntervalExchangeTransformation<Length>;
  // The labels used by all our intervals (we keep the references alive by adding them to this set.)
  std::set<Label> labels;
  std::list<Interval<Length>> top, bottom;

 public:
  Implementation(const std::vector<Label>& top, const std::vector<Label>& bottom) : labels(top.begin(), top.end()) {
    assert(top.size() == bottom.size() && "top and bottom must have the same length");

    for (const Label& t : top) {
      auto it = labels.find(t);
      assert(it != labels.end() && "top label not found in top labels");
      this->top.push_back(Interval<Length>{const_cast<Label&>(*it), this->bottom.begin()});
    }

    for (const Label& b : bottom) {
      auto it = labels.find(b);
      assert(it != labels.end() && "bottom label not found in top labels");
      this->bottom.push_back(Interval<Length>{const_cast<Label&>(*it), std::find_if(this->top.begin(), this->top.end(), [&](const auto& t) { return t.label == b; })});
    }

    for (auto it = this->bottom.begin(); it != this->bottom.end(); it++) {
      const_cast<typename std::list<Interval<Length>>::iterator&>(it->twin->twin) = it;
    }

    assert(std::all_of(this->top.begin(), this->top.end(), [](auto& t) { return &t == &*t.twin->twin; }) && "twin pointers of top do not point back to the original element.");
    assert(std::all_of(this->bottom.begin(), this->bottom.end(), [](auto& t) { return &t == &*t.twin->twin; }) && "twin pointers of bottom do not point back to the original element.");
    assert(std::all_of(this->labels.begin(), this->labels.end(), [&](auto& l) { return std::count_if(this->top.begin(), this->top.end(), [&](auto& t) { return t.label == l; }); }) && "top does not contain every label exactly once.");
    assert(std::all_of(this->labels.begin(), this->labels.end(), [&](auto& l) { return std::count_if(this->bottom.begin(), this->bottom.end(), [&](auto& t) { return t.label == l; }); }) && "bottom does not contain every label exactly once.");
  }

  // NOTE: using a set here is algorithmically worse than the bitarray that
  // used to be. We know exactly how many labels we have to fit. Instead of
  // constant time searches and additions we have here logarithmic search and
  // addition...then again many consider logarithms just big constants.
  std::optional<IntervalExchangeTransformation<Length>> reduce() {
    std::set<Label> seen;

    int top_ahead = 0;
    int bottom_ahead = 0;

    auto itop = top.begin();
    auto ibottom = bottom.begin();
    while (true) {
      assert(itop != top.end() && "top_ahead == 0 && bottom_ahead == 0 must hold eventually.");
      assert(ibottom != bottom.end() && "top_ahead == 0 && bottom_ahead == 0 must hold eventually.");

      if (seen.find(itop->label) != seen.end()) {
        bottom_ahead--;
      } else {
        top_ahead++;
        seen.insert(itop->label);
      }

      if (seen.find(ibottom->label) != seen.end()) {
        top_ahead--;
      } else {
        bottom_ahead++;
        seen.insert(ibottom->label);
      }

      if (top_ahead == 0 && bottom_ahead == 0) {
        break;
      }

      ++itop;
      ++ibottom;
    }

    ++itop;
    ++ibottom;

    if (itop == top.end()) {
      assert(ibottom == bottom.end() && "top & bottom do not have the same size");
      return {};
    } else {
      std::vector<Label> vtop;
      std::vector<Label> vbottom;

      for (auto j = itop; j != top.end(); j++)
        vtop.push_back(j->label);
      for (auto j = ibottom; j != bottom.end(); j++)
        vbottom.push_back(j->label);
      top.erase(itop, top.end());
      bottom.erase(ibottom, bottom.end());
      assert(top.size() == bottom.size() && "top and bottom must have the same length");
      for (auto it = vtop.begin(); it != vtop.end(); it++) labels.erase(*it);

      return IntervalExchangeTransformation<Length>(vtop, vbottom);
    }
  }

  // Return true if a saddle connection is found and false otherwise
  bool zorichInduction(void) {
    Length length_to_subtract;

    const auto itop = top.begin();
    auto ibottom = bottom.begin();

    if (itop->label == ibottom->label)
      return true;

    while (ibottom->label != itop->label && length_to_subtract + ibottom->label.length() < itop->label.length()) {
      length_to_subtract += ibottom->label.length();
      ++ibottom;
    }

    if (ibottom->label == itop->label) {
      // Zorich acceleration step (= perform m full Dehn twists)
      auto m = itop->label.length() / length_to_subtract;

      for (auto b = bottom.begin(); b != ibottom; ++b) {
        itop->label.length() -= m * b->label.length();
      }

      // recompute from what is left
      ibottom = bottom.begin();
      length_to_subtract = Length();
      while (length_to_subtract + ibottom->label.length() < itop->label.length()) {
        length_to_subtract += ibottom->label.length();
        ++ibottom;
      }
    }

    // partial twist
    for (auto b = bottom.begin(); b != ibottom; ++b) {
      itop->label.length() -= b->label.length();
    }

    bottom.splice(itop->twin, bottom, bottom.begin(), ibottom);

    return top.begin()->label.length() == bottom.begin()->label.length();
  }

  // Return whether there is a periodic trajectory via Boshernitzan's
  // algorithm.
  bool boshernitzanMinimal(void) const {
    return false;
  }
};

template <typename Length>
IntervalExchangeTransformation<Length>::IntervalExchangeTransformation(const std::vector<Label>& top, const std::vector<size_t>& bottom) : impl(spimpl::make_unique_impl<Implementation>(top, [&]() {
  std::vector<Label> bot;
  for (auto b : bottom) bot.push_back(top[b]);
  return bot;
}())) {}

template <typename Length>
IntervalExchangeTransformation<Length>::IntervalExchangeTransformation() : IntervalExchangeTransformation(std::vector<Label>(), std::vector<Label>()) {}

template <typename Length>
IntervalExchangeTransformation<Length>::IntervalExchangeTransformation(const std::vector<Label>& top, const std::vector<Label>& bottom) : impl(spimpl::make_unique_impl<Implementation>(top, bottom)) {}

template <typename Length>
std::vector<Label<Length>> IntervalExchangeTransformation<Length>::top() const noexcept {
  std::vector<Label> ret;

  for (auto& t : impl->top)
    ret.push_back(t.label);

  return ret;
}

template <typename Length>
std::vector<Label<Length>> IntervalExchangeTransformation<Length>::bottom() const noexcept {
  std::vector<Label> ret;

  for (auto& t : impl->bottom)
    ret.push_back(t.label);

  return ret;
}

template <typename Length>
void IntervalExchangeTransformation<Length>::swap() {
  std::swap(impl->top, impl->bottom);
}

template <typename Length>
bool IntervalExchangeTransformation<Length>::zorichInduction(void) {
  return impl->zorichInduction();
}

template <typename Length>
std::optional<IntervalExchangeTransformation<Length>> IntervalExchangeTransformation<Length>::reduce() {
  return impl->reduce();
}

template <typename Length>
std::ostream& operator<<(std::ostream& os, const IntervalExchangeTransformation<Length>& self) {
  using Label = typename IntervalExchangeTransformation<Length>::Label;
  auto id = [&](const Label& query) {
    std::string ret = boost::lexical_cast<std::string>(query);
    int clashes = 0;
    for (auto& t : self.top()) {
      std::string repr = boost::lexical_cast<std::string>(t);
      if (t == query) {
        if (clashes) {
          return ret + boost::lexical_cast<std::string>(clashes);
        } else {
          return ret;
        }
      }
      if (repr == ret) {
        clashes++;
      }
    }

    assert(false && "each label must be contained in top()");
    return std::string();
  };

  os << boost::algorithm::join(self.top() | boost::adaptors::transformed(id), " ") << std::endl;
  os << boost::algorithm::join(self.bottom() | boost::adaptors::transformed(id), " ") << std::endl;

  return os;
}
}  // namespace intervalxt

#endif
