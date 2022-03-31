/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019 Vincent Delecroix
 *        Copyright (C) 2019-2022 Julian Rüth
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
#include <unordered_map>
#include <unordered_set>

#include "../intervalxt/fmt.hpp"
#include "../intervalxt/induction_step.hpp"
#include "../intervalxt/label.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/interval.hpp"
#include "impl/interval_exchange_transformation.impl.hpp"
#include "impl/rational_affine_subspace.hpp"
#include "util/assert.ipp"

using std::vector;

namespace intervalxt {

namespace {

template <typename T>
std::vector<T> wedge(std::vector<T> v1, std::vector<T> v2) {
  LIBINTERVALXT_CHECK_ARGUMENT(v1.size() == v2.size(), "vectors must have same size but " << v1.size() << " != " << v2.size());

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
  LIBINTERVALXT_ASSERT(lhs.size() == rhs.size(), "cannot add vectors of different size");
  for (size_t i = 0; i < lhs.size(); i++)
    lhs[i] += rhs[i];
  return lhs;
}

std::vector<mpq_class>& operator-=(std::vector<mpq_class>& lhs, const std::vector<mpq_class>& rhs) {
  LIBINTERVALXT_ASSERT(lhs.size() == rhs.size(), "cannot add vectors of different size");
  for (size_t i = 0; i < lhs.size(); i++)
    lhs[i] -= rhs[i];
  return lhs;
}

}  // namespace

bool IntervalExchangeTransformation::zorichInduction() {
  const auto top = begin(self->top);
  auto bottom = begin(self->bottom);
  auto& lengths = *self->lengths;

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

  self->bottom.splice(top->twin, self->bottom, begin(self->bottom), bottom);

  return lengths.cmp(*begin(self->top), *begin(self->bottom)) == 0;
}

std::vector<mpq_class> IntervalExchangeTransformation::safInvariant() const {
  auto saf = self->saf();
  if (self->swap)
    for (auto& c : saf)
      c = -c;
  return saf;
}

bool IntervalExchangeTransformation::boshernitzanNoPeriodicTrajectory() const {
  // When SAF = 0 the Boshernitzan is never going to report "true".
  // https://github.com/flatsurf/intervalxt/issues/86
  if (self->saf0())
    return false;

  if (size() <= 1)
    return false;

  const auto relations = boshernitzanEquations();

  const auto space = RationalAffineSubspace(relations, std::vector<mpq_class>(relations.size()));
  return not space.hasNonZeroNonNegativeVector();
}

bool IntervalExchangeTransformation::boshernitzanNoSaddleConnection() const {
  // Probably this wil always return false when SAF=0. But we are not entirely
  // sure that this is actually the case.

  if (self->top.size() <= 1)
    return false;

  for (auto t = ++self->top.rbegin(); t != self->top.rend(); t++)
    for (auto b = ++self->bottom.rbegin(); b != self->bottom.rend(); b++)
      if (!boshernitzanNoSaddleConnection(*t, *b))
        return false;

  return true;
}

bool IntervalExchangeTransformation::boshernitzanNoSaddleConnection(const Label& top, const Label& bottom) const {
  // Probably this wil always return false when SAF=0. But we are not entirely
  // sure that this is actually the case.

  if (size() <= 1)
    return false;

  const auto values = boshernitzanSaddleConnectionValues(top, bottom);

  if (values | rx::all_of([](const auto& t){ return t == 0; }))
    // Since the translation between top and bottom is zero, there is an
    // obvious saddle connection here.
    return false;

  const auto space = RationalAffineSubspace(boshernitzanEquations(), values);
  return not space.hasNonNegativeVector();
}

std::vector<std::vector<mpq_class>> IntervalExchangeTransformation::boshernitzanEquations() const {
  const auto translations = self->translations();

  // Build the space generated by integer vectors (a0, ..., an) such that
  // a0 t0 + a1 t1 + ... + an tn = top - bottom.
  std::vector<std::vector<mpq_class>> relations(translations[0].size());
  for (auto& t : translations)
    for (size_t d = 0; d < t.size(); d++)
      relations[d].push_back(t[d]);

  return relations;
}

std::vector<mpq_class> IntervalExchangeTransformation::boshernitzanSaddleConnectionValues(const Label &top, const Label &bottom) const {
  auto topIt = std::find(begin(self->top), end(self->top), top);
  auto bottomIt = std::find(begin(self->bottom), end(self->bottom), bottom);

  LIBINTERVALXT_CHECK_ARGUMENT(++topIt != end(self->top), "Cannot select right end point of last top interval for Boshernitzan saddle connection values.");
  LIBINTERVALXT_CHECK_ARGUMENT(++bottomIt != end(self->bottom), "Cannot select right end point of last bottom interval for Boshernitzan saddle connection values.");

  std::vector<mpq_class> values = self->translation(*topIt, *bottomIt, self->labelToCoefficients());

  for (mpq_class& v : values)
    v = -v;

  return values;
}

InductionStep IntervalExchangeTransformation::induce(int limit) {
  using Result = InductionStep::Result;

  if (size() == 1) {
    return {Result::CYLINDER};
  }

  bool foundSaddleConnection = false;

  if (limit != 0) {
    const bool saf0 = self->saf0();

    for (int i = 0; limit == -1 || i < limit; i++) {
      if (saf0) {
        // When SAF=0 the Boshernitzan criterion will not be useful so we try to
        // find a loop in the IETs we see.
        if (self->saf().size() == 0) {
          // When all lengths are rational, then there cannot be any loops.
          ;
        } else if (self->similarityTracker.loop(*this)) {
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
  }

  auto reducible = reduce();
  if (reducible) {
    return {
        Result::SEPARATING_CONNECTION,
        std::pair(rbegin(self->bottom)->label, rbegin(self->top)->label),
        std::move(*reducible)};
  }

  const Interval firstTop = *begin(self->top);
  const Interval firstBottom = *begin(self->bottom);

  if (self->lengths->cmp(firstTop, firstBottom) == 0) {
    auto connection = std::pair(begin(self->bottom)->label, begin(self->top)->label);

    // We merge the labels on top and bottom by
    // replacing the top one with the bottom one.
    auto bottomReplacement = self->bottom.insert(firstTop.twin, firstBottom);
    self->bottom.erase(begin(self->bottom));

    // Fix the twin pointer of the bottom's twin to point to its new location.
    bottomReplacement->twin->twin = bottomReplacement;

    // Now drop the top label from top and bottom
    self->top.erase(begin(self->top));
    self->bottom.erase(firstTop.twin);

    return {
        Result::NON_SEPARATING_CONNECTION,
        connection,
    };
  }

  LIBINTERVALXT_ASSERT(!foundSaddleConnection, "Zorich Induction found a Saddle Connection in " << *this << " but induce() failed to see it.");

  if (boshernitzanNoPeriodicTrajectory()) {
    return {Result::WITHOUT_PERIODIC_TRAJECTORY_BOSHERNITZAN};
  }

  return {Result::LIMIT_REACHED};
}

IntervalExchangeTransformation::IntervalExchangeTransformation() :
  self(spimpl::make_unique_impl<ImplementationOf<IntervalExchangeTransformation>>(std::shared_ptr<Lengths>(nullptr), vector<Label>(), vector<Label>())) {}

IntervalExchangeTransformation::IntervalExchangeTransformation(std::shared_ptr<Lengths> lengths, const vector<Label>& top, const vector<Label>& bottom) :
  self(spimpl::make_unique_impl<ImplementationOf<IntervalExchangeTransformation>>(std::move(lengths), top, bottom)) {
  LIBINTERVALXT_ASSERT(top.size() != 0, "IntervalExchangeTransformation cannot be empty");
}

vector<Label> IntervalExchangeTransformation::top() const {
  return self->top | rx::transform([](auto& i) { return i.label; }) | rx::to_vector();
}

vector<Label> IntervalExchangeTransformation::bottom() const {
  return self->bottom | rx::transform([](auto& i) { return i.label; }) | rx::to_vector();
}

void IntervalExchangeTransformation::swap() {
  self->top.swap(self->bottom);
  self->swap = not self->swap;
}

bool IntervalExchangeTransformation::swapped() const {
  return self->swap;
}

std::shared_ptr<const Lengths> IntervalExchangeTransformation::lengths() const {
  return self->lengths;
}

size_t IntervalExchangeTransformation::size() const {
  return self->top.size();
}

std::optional<IntervalExchangeTransformation> IntervalExchangeTransformation::reduce() {
  int topAhead = 0;
  int bottomAhead = 0;

  auto topIterator = begin(self->top);
  auto bottomIterator = begin(self->bottom);
  while (true) {
    LIBINTERVALXT_ASSERT(topIterator != end(self->top), "top_ahead == 0 && bottom_ahead == 0 must hold eventually.");
    LIBINTERVALXT_ASSERT(bottomIterator != end(self->bottom), "top_ahead == 0 && bottom_ahead == 0 must hold eventually.");

    if (topIterator->label.id != topIterator->twin->label.id) {
      bottomAhead--;
      topIterator->twin->label.id ^= 1;
    } else {
      topAhead++;
      topIterator->label.id ^= 1;
    }

    if (bottomIterator->label.id != bottomIterator->twin->label.id) {
      topAhead--;
      bottomIterator->twin->label.id ^= 1;
    } else {
      bottomAhead++;
      bottomIterator->label.id ^= 1;
    }

    if (topAhead == 0 && bottomAhead == 0) {
      break;
    }

    ++topIterator;
    ++bottomIterator;
  }

  ++topIterator;
  ++bottomIterator;

  if (topIterator == end(self->top)) {
    LIBINTERVALXT_ASSERT(bottomIterator == end(self->bottom), "top & bottom do not have the same size");
    return {};
  } else {
    vector<Label> newComponentTop;
    vector<Label> newComponentBottom;

    for (; topIterator != end(self->top); topIterator = self->top.erase(topIterator))
      newComponentTop.push_back(topIterator->label);
    for (; bottomIterator != end(self->bottom); bottomIterator = self->bottom.erase(bottomIterator))
      newComponentBottom.push_back(bottomIterator->label);

    LIBINTERVALXT_ASSERT(self->top.size() == self->bottom.size(), "top and bottom must have the same length after splitting of a component");

    self->safCache = std::nullopt;
    return IntervalExchangeTransformation(self->lengths, newComponentTop, newComponentBottom);
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

  if (topLengths(top(), self->lengths) != topLengths(rhs.top(), rhs.self->lengths))
    return false;

  return true;
}

bool IntervalExchangeTransformation::operator==(const IntervalExchangeTransformation& rhs) const {
  const std::vector<Label> labels = self->top | rx::transform([](const auto& interval) { return interval.label; }) | rx::to_vector();
  if (self->top != rhs.self->top || self->bottom != rhs.self->bottom)
    return false;

  for (auto label : self->top)
    if (self->lengths->get(label) != rhs.self->lengths->get(label))
      return false;

  return true;
}

ImplementationOf<IntervalExchangeTransformation>::ImplementationOf(std::shared_ptr<Lengths> lengths, const vector<Label>& top, const vector<Label>& bottom) :
  top(top | rx::transform([](const Label label) { return Interval(label); }) | rx::to_list()),
  bottom(bottom | rx::transform([](const Label label) { return Interval(label); }) | rx::to_list()),
  lengths(std::move(lengths)) {
  LIBINTERVALXT_ASSERT(top.size() == bottom.size(), "top and bottom must have the same length");

  LIBINTERVALXT_ASSERT(std::unordered_set<Label>(begin(top), end(top)).size() == std::unordered_set<Label>(begin(bottom), end(bottom)).size(), "top and bottom must consist of the same labels");

  LIBINTERVALXT_ASSERT(std::unordered_set<Label>(begin(top), end(top)).size() == top.size(), "top and bottom must not contain duplicates");

  for (auto t = begin(this->top); t != end(this->top); t++) {
    for (auto b = begin(this->bottom); b != end(this->bottom); b++) {
      if (t->label == b->label) {
        t->twin = b;
        b->twin = t;
      }
    }
  }

  LIBINTERVALXT_ASSERT(std::all_of(top.begin(), top.end(), [&](Label label) { return static_cast<bool>(this->lengths->get(label)); }), "all lengths must be positive");
}

const std::vector<mpq_class>& ImplementationOf<IntervalExchangeTransformation>::saf() const {
  if (!safCache) {
    const auto coefficients = this->coefficients();

    const auto degree = coefficients[0].size();
    if (degree <= 1)
      safCache = std::vector<mpq_class>();
    else {
      const auto translations = this->translations();

      safCache = std::vector<mpq_class>(degree * (degree - 1) / 2);

      for (const auto& [c, t] : rx::zip(coefficients, translations))
        *safCache += wedge(c, t);
    }

    if (swap)
      for (auto& c : *safCache)
        c = -c;
  }
  return *safCache;
}

bool ImplementationOf<IntervalExchangeTransformation>::saf0() const {
  const auto& saf = this->saf();
  return std::none_of(begin(saf), end(saf), [](const auto& x) { return x; });
}

std::vector<std::vector<mpq_class>> ImplementationOf<IntervalExchangeTransformation>::coefficients() const {
  return lengths->coefficients(top | rx::transform([](const auto& interval) { return static_cast<Label>(interval); }) | rx::to_vector());
}

std::unordered_map<Label, std::vector<mpq_class>> ImplementationOf<IntervalExchangeTransformation>::labelToCoefficients() const {
  const auto coefficients = this->coefficients();
  std::unordered_map<Label, std::vector<mpq_class>> labelToCoefficient;
  for (const auto& [label, coefficient] : rx::zip(top, coefficients))
    labelToCoefficient[label] = coefficient;
  return labelToCoefficient;
}

std::vector<std::vector<mpq_class>> ImplementationOf<IntervalExchangeTransformation>::translations() const {
  const auto labelToCoefficients = this->labelToCoefficients();

  std::vector<std::vector<mpq_class>> translations;
  for (const auto& label : top) {
    translations.push_back(this->translation(label, label, labelToCoefficients));
  }

  return translations;
}

std::vector<mpq_class> ImplementationOf<IntervalExchangeTransformation>::translation(const Label& top, const Label& bottom, const std::unordered_map<Label, std::vector<mpq_class>>& labelToCoefficients) const {
  std::vector<mpq_class> translation(labelToCoefficients.begin()->second.size());

  for (auto& t : this->top) {
    if (t == top)
      break;
    translation -= labelToCoefficients.at(t);
  }

  for (auto& b : this->bottom) {
    if (b == bottom)
      break;
    translation += labelToCoefficients.at(b);
  }

  return translation;
}

IntervalExchangeTransformation ImplementationOf<IntervalExchangeTransformation>::withLengths(const IntervalExchangeTransformation& iet, const std::function<std::shared_ptr<Lengths>(std::shared_ptr<Lengths>)>& createLengths) {
  return IntervalExchangeTransformation(createLengths(iet.self->lengths), iet.top(), iet.bottom());
}

std::string ImplementationOf<IntervalExchangeTransformation>::render(const IntervalExchangeTransformation& iet, Label label) {
  return iet.self->lengths->render(label);
}

std::ostream& operator<<(std::ostream& os, const IntervalExchangeTransformation& self) {
  return os << fmt::format("{} / {}",
             fmt::join(self.self->top | rx::transform([&](const auto& interval) { return fmt::format("[{}: {}]", self.self->lengths->render(interval), self.self->lengths->get(interval)); }) | rx::to_vector(), " "),
             fmt::join(self.self->bottom | rx::transform([&](const auto& interval) { return fmt::format("[{}]", self.self->lengths->render(interval)); }) | rx::to_vector(), " "));
}

}  // namespace intervalxt
