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

#include "../intervalxt/interval_exchange_transformation.hpp"

#include <fmt/format.h>
#include <gmpxx.h>

#include <algorithm>
#include <list>
#include <unordered_set>

#include "../intervalxt/fmt.hpp"
#include "../intervalxt/induction_step.hpp"
#include "../intervalxt/label.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/interval.hpp"
#include "impl/interval_exchange_transformation.impl.hpp"
#include "impl/rational_linear_subspace.hpp"
#include "util/assert.ipp"

using std::vector;

namespace intervalxt {

namespace {

template <typename T>
std::vector<T> wedge(std::vector<T> v1, std::vector<T> v2) {
  CHECK_ARGUMENT(v1.size() == v2.size(), "vectors must have same size but " << v1.size() << " != " << v2.size());

  size_t d = v1.size();
  std::vector<T> res(d * (d - 1) / 2);
  if (d == 0) return res;
  size_t k = 0;
  for (size_t i = 0; i < d - 1; i++)
    for (size_t j = i + 1; j < d; j++) {
      res[k] += v1[i] * v2[j] - v1[j] * v2[i];
      k += 1;
    }

  return res;
}

std::vector<mpq_class>& operator+=(std::vector<mpq_class>& lhs, const std::vector<mpq_class>& rhs) {
  ASSERT(lhs.size() == rhs.size(), "cannot add vectors of different size");
  for (size_t i = 0; i < lhs.size(); i++)
    lhs[i] += rhs[i];
  return lhs;
}

std::vector<mpq_class>& operator-=(std::vector<mpq_class>& lhs, const std::vector<mpq_class>& rhs) {
  ASSERT(lhs.size() == rhs.size(), "cannot add vectors of different size");
  for (size_t i = 0; i < lhs.size(); i++)
    lhs[i] -= rhs[i];
  return lhs;
}

}  // namespace

bool IntervalExchangeTransformation::zorichInduction() {
  const auto top = begin(impl->top);
  auto bottom = begin(impl->bottom);
  auto& lengths = *impl->lengths;

  if (*top == *bottom)
    // The IET starts with a Cylinder
    return true;

  while (true) {
    if (*bottom == *top) {
      // Zorich acceleration step: perform m full Dehn twists
      // and a partial twist.
      Label stop = lengths.subtractRepeated(*top);
      while (*bottom != stop) bottom--;

      bottom++;
      break;
    }

    lengths.push(*bottom);
    if (lengths.cmp(*top) >= 0) {
      lengths.pop();

      // partial twist
      lengths.subtract(*top);

      break;
    }

    bottom++;
  }

  impl->bottom.splice(top->twin, impl->bottom, begin(impl->bottom), bottom);

  return lengths.cmp(*begin(impl->top), *begin(impl->bottom)) == 0;
}

std::vector<mpq_class> IntervalExchangeTransformation::safInvariant() const {
  return impl->saf();
}

bool IntervalExchangeTransformation::boshernitzanNoPeriodicTrajectory() const {
  const auto translations = impl->translations();

  if (translations[0].size() <= 1)
    return false;

  // When SAF = 0 the Boshernitzan is never going to report "true".
  // https://github.com/flatsurf/intervalxt/issues/86
  if (impl->saf0())
    return false;

  // Build the QQ-module of relations between translations, that is the space generated
  // by integer vectors (a0, ..., an) such that a0 t0 + a1 t1 + ... + an tn = 0
  std::vector<std::vector<mpq_class>> relations(translations[0].size());
  for (auto& t : translations)
    for (size_t d = 0; d < t.size(); d++)
      relations[d].push_back(t[d]);

  return not RationalLinearSubspace::fromEquations(relations).hasNonZeroNonNegativeVector();
}

InductionStep IntervalExchangeTransformation::induce(int limit) {
  using Result = InductionStep::Result;

  if (size() == 1) {
    return {Result::CYLINDER};
  }

  const bool saf0 = impl->saf0();

  bool foundSaddleConnection = false;

  for (int i = 0; limit == -1 || i < limit; i++) {
    if (saf0) {
      // When SAF=0 the Boshernitzan criterion will not be useful so we try to
      // detect a loop directly.
      if (impl->similarityTracker.loop(*this)) {
        return {Result::WITHOUT_PERIODIC_TRAJECTORY_AUTO_SIMILAR};
      }
    }

    foundSaddleConnection = zorichInduction();
    if (foundSaddleConnection) break;

    swap();
    foundSaddleConnection = zorichInduction();
    swap();
    if (foundSaddleConnection) break;
  }

  const Interval firstTop = *begin(impl->top);
  const Interval firstBottom = *begin(impl->bottom);

  auto reducible = reduce();
  if (reducible) {
    return {
        Result::SEPARATING_CONNECTION,
        std::pair(rbegin(impl->bottom)->label, rbegin(impl->top)->label),
        std::move(*reducible)};
  }

  if (impl->lengths->cmp(firstTop, firstBottom) == 0) {
    auto connection = std::pair(begin(impl->bottom)->label, begin(impl->top)->label);

    // We merge the labels on top and bottom by
    // replacing the top one with the bottom one.
    auto bottomReplacement = impl->bottom.insert(firstTop.twin, firstBottom);
    impl->bottom.erase(begin(impl->bottom));

    // Fix the twin pointer of the bottom's twin to point to its new location.
    bottomReplacement->twin->twin = bottomReplacement;

    // Now drop the top label from top and bottom
    impl->top.erase(begin(impl->top));
    impl->bottom.erase(firstTop.twin);

    return {
        Result::NON_SEPARATING_CONNECTION,
        connection,
    };
  }

  ASSERT(!foundSaddleConnection, "Zorich Induction found a Saddle Connection in " << *this << " but induce() failed to see it.");

  if (boshernitzanNoPeriodicTrajectory()) {
    return {Result::WITHOUT_PERIODIC_TRAJECTORY_BOSHERNITZAN};
  }

  return {Result::LIMIT_REACHED};
}

IntervalExchangeTransformation::IntervalExchangeTransformation() :
  impl(spimpl::make_unique_impl<Implementation>(std::shared_ptr<Lengths>(nullptr), vector<Label>(), vector<Label>())) {}

IntervalExchangeTransformation::IntervalExchangeTransformation(std::shared_ptr<Lengths> lengths, const vector<Label>& top, const vector<Label>& bottom) :
  impl(spimpl::make_unique_impl<Implementation>(std::move(lengths), top, bottom)) {
  ASSERT(top.size() != 0, "IntervalExchangeTransformation cannot be empty");
}

vector<Label> IntervalExchangeTransformation::top() const noexcept {
  return impl->top | rx::transform([](auto& i) { return i.label; }) | rx::to_vector();
}

vector<Label> IntervalExchangeTransformation::bottom() const noexcept {
  return impl->bottom | rx::transform([](auto& i) { return i.label; }) | rx::to_vector();
}

void IntervalExchangeTransformation::swap() {
  impl->top.swap(impl->bottom);
  impl->swap = not impl->swap;
}

bool IntervalExchangeTransformation::swapped() const noexcept {
  return impl->swap;
}

std::shared_ptr<const Lengths> IntervalExchangeTransformation::lengths() const {
  return impl->lengths;
}

size_t IntervalExchangeTransformation::size() const noexcept {
  return impl->top.size();
}

std::optional<IntervalExchangeTransformation> IntervalExchangeTransformation::reduce() {
  // NOTE: using a set here is algorithmically worse than the bitarray that
  // used to be. We know exactly how many labels we have to fit. Instead of
  // constant time searches and additions we have here logarithmic search and
  // addition...then again many consider logarithms just big constants.
  std::unordered_set<Label> seen;

  int top_ahead = 0;
  int bottom_ahead = 0;

  auto topIterator = begin(impl->top);
  auto bottomIterator = begin(impl->bottom);
  while (true) {
    ASSERT(topIterator != end(impl->top), "top_ahead == 0 && bottom_ahead == 0 must hold eventually.");
    ASSERT(bottomIterator != end(impl->bottom), "top_ahead == 0 && bottom_ahead == 0 must hold eventually.");

    if (seen.find(*topIterator) != end(seen)) {
      bottom_ahead--;
    } else {
      top_ahead++;
      seen.insert(*topIterator);
    }

    if (seen.find(*bottomIterator) != end(seen)) {
      top_ahead--;
    } else {
      bottom_ahead++;
      seen.insert(*bottomIterator);
    }

    if (top_ahead == 0 && bottom_ahead == 0) {
      break;
    }

    ++topIterator;
    ++bottomIterator;
  }

  ++topIterator;
  ++bottomIterator;

  if (topIterator == end(impl->top)) {
    ASSERT(bottomIterator == end(impl->bottom), "top & bottom do not have the same size");
    return {};
  } else {
    vector<Label> newComponentTop;
    vector<Label> newComponentBottom;

    for (; topIterator != end(impl->top); topIterator = impl->top.erase(topIterator))
      newComponentTop.push_back(topIterator->label);
    for (; bottomIterator != end(impl->bottom); bottomIterator = impl->bottom.erase(bottomIterator))
      newComponentBottom.push_back(bottomIterator->label);

    ASSERT(impl->top.size() == impl->bottom.size(), "top and bottom must have the same length after splitting of a component");

    return IntervalExchangeTransformation(impl->lengths, newComponentTop, newComponentBottom);
  }
}

bool IntervalExchangeTransformation::equivalent(const IntervalExchangeTransformation& rhs) const {
  if (size() != rhs.size())
    return false;

  const auto permutation = [](const std::vector<Label>& top, const std::vector<Label>& bottom) {
    return bottom | rx::transform([&](const auto& b) { return std::find(begin(top), end(top), b) - begin(top); }) | rx::to_vector();
  };

  if (permutation(top(), bottom()) != permutation(rhs.top(), rhs.bottom()))
    return false;

  const auto topLengths = [](const std::vector<Label>& top, const auto& lengths) {
    return top | rx::transform([&](const auto& label) { return lengths->get(label); }) | rx::to_vector();
  };

  if (topLengths(top(), impl->lengths) != topLengths(rhs.top(), rhs.impl->lengths))
    return false;

  return true;
}

bool IntervalExchangeTransformation::operator==(const IntervalExchangeTransformation& rhs) const {
  const std::vector<Label> labels = impl->top | rx::transform([](const auto& interval) { return interval.label; }) | rx::to_vector();
  if (impl->top != rhs.impl->top || impl->bottom != rhs.impl->bottom)
    return false;

  for (auto label : impl->top)
    if (impl->lengths->get(label) != rhs.impl->lengths->get(label))
      return false;

  return true;
}

Implementation<IntervalExchangeTransformation>::Implementation(std::shared_ptr<Lengths> lengths, const vector<Label>& top, const vector<Label>& bottom) :
  top(top | rx::transform([](const Label label) { return Interval(label); }) | rx::to_list()),
  bottom(bottom | rx::transform([](const Label label) { return Interval(label); }) | rx::to_list()),
  lengths(std::move(lengths)) {
  ASSERT(top.size() == bottom.size(), "top and bottom must have the same length");

  ASSERT(std::unordered_set<Label>(begin(top), end(top)).size() == std::unordered_set<Label>(begin(bottom), end(bottom)).size(), "top and bottom must consist of the same labels");

  ASSERT(std::unordered_set<Label>(begin(top), end(top)).size() == top.size(), "top and bottom must not contain duplicates");

  for (auto t = begin(this->top); t != end(this->top); t++) {
    for (auto b = begin(this->bottom); b != end(this->bottom); b++) {
      if (t->label == b->label) {
        t->twin = b;
        b->twin = t;
      }
    }
  }

  ASSERT(std::all_of(top.begin(), top.end(), [&](Label label) { return static_cast<bool>(this->lengths->get(label)); }), "all lengths must be positive");
}

std::vector<mpq_class> Implementation<IntervalExchangeTransformation>::saf() const {
  const auto coefficients = this->coefficients();
  const auto translations = this->translations();

  const auto degree = coefficients[0].size();
  if (degree <= 1)
    return {};

  std::vector<mpq_class> w(degree * (degree - 1) / 2);

  for (const auto& [c, t] : rx::zip(coefficients, translations))
    w += wedge(c, t);

  return w;
}

bool Implementation<IntervalExchangeTransformation>::saf0() const {
  auto saf = this->saf();
  return std::none_of(begin(saf), end(saf), [](const auto& x) { return x; });
}

std::vector<std::vector<mpq_class>> Implementation<IntervalExchangeTransformation>::coefficients() const {
  return lengths->coefficients(top | rx::transform([](const auto& interval) { return static_cast<Label>(interval); }) | rx::to_vector());
}

std::vector<std::vector<mpq_class>> Implementation<IntervalExchangeTransformation>::translations() const {
  const auto coefficients = this->coefficients();
  std::unordered_map<Label, std::vector<mpq_class>> labelToCoefficient;
  for (const auto& [label, coefficient] : rx::zip(top, coefficients))
    labelToCoefficient[label] = coefficient;

  std::vector<std::vector<mpq_class>> translations;
  for (const auto& label : top) {
    std::vector<mpq_class> translation(coefficients[0].size());
    for (auto& t : top) {
      if (t == label)
        break;
      translation -= labelToCoefficient.at(t);
    }

    for (auto& b : bottom) {
      if (b == label)
        break;
      translation += labelToCoefficient.at(b);
    }

    translations.push_back(translation);
  }

  return translations;
}

IntervalExchangeTransformation Implementation<IntervalExchangeTransformation>::withLengths(const IntervalExchangeTransformation& iet, const std::function<std::shared_ptr<Lengths>(std::shared_ptr<Lengths>)>& createLengths) {
  return IntervalExchangeTransformation(createLengths(iet.impl->lengths), iet.top(), iet.bottom());
}

std::string Implementation<IntervalExchangeTransformation>::render(const IntervalExchangeTransformation& iet, Label label) {
  return iet.impl->lengths->render(label);
}

std::ostream& operator<<(std::ostream& os, const IntervalExchangeTransformation& self) {
  return os << fmt::format("{} / {}",
             fmt::join(self.impl->top | rx::transform([&](const auto& interval) { return fmt::format("[{}: {}]", self.impl->lengths->render(interval), self.impl->lengths->get(interval)); }) | rx::to_vector(), " "),
             fmt::join(self.impl->bottom | rx::transform([&](const auto& interval) { return fmt::format("[{}]", self.impl->lengths->render(interval)); }) | rx::to_vector(), " "));
}

}  // namespace intervalxt
