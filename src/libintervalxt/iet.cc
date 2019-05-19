#include <gmpxx.h>
#include <boost/numeric/conversion/cast.hpp>
#include <iostream>
#include <stdexcept>

#include "intervalxt/iet.hpp"

using boost::numeric_cast;

namespace intervalxt {
// Constructors

template <typename Tlen, typename Tmat>
Label<Tlen, Tmat>::Label() {
  i1.twin = &i2;
  i2.twin = &i1;
  i1.prev = i1.next = nullptr;
  i2.prev = i2.next = nullptr;
  i1.lab = this;
  i2.lab = this;
  length = 0;
}

template <typename Tlen, typename Tmat>
void IntervalExchangeTransformation<Tlen, Tmat>::reset(size_t nintervals) {
  n = nintervals;
  labels.resize(n);
  for (size_t i = 0; i < n; i++) {
    labels[i].index = i;
    labels[i].v.assign(n, 0);
    labels[i].v[i] = 1;
  }
}

template <typename Tlen, typename Tmat>
IntervalExchangeTransformation<Tlen, Tmat>::IntervalExchangeTransformation(size_t nintervals) {
  reset(nintervals);
  setIdentityPermutation();
}

template <typename Tlen, typename Tmat>
IntervalExchangeTransformation<Tlen, Tmat>::IntervalExchangeTransformation(const Permutation& topperm, const Permutation& botperm) {
  n = topperm.size();

  if (n == 0 || n != botperm.size())
    throw std::runtime_error("invalid paramters");

  reset(n);
  setTop(topperm);
  setBot(botperm);
}

// check

template <typename Tlen, typename Tmat>
void IntervalExchangeTransformation<Tlen, Tmat>::check() {
  std::vector<int> seen(n);

  for (Interval<Tlen, Tmat>* i = top; i != NULL; i = i->next) {
    if (i->next != nullptr && i->next->prev != i)
      throw std::runtime_error("invalid top");
    seen[i->lab->index] += 1;
  }
  for (Interval<Tlen, Tmat>* i = bot; i != NULL; i = i->next) {
    if (i->next != nullptr && i->next->prev != i)
      throw std::runtime_error("invalid bot");
    if (seen[i->lab->index] != 1)
      throw std::runtime_error("some interval appears on bot but not on top");
    seen[i->lab->index] -= 1;
  }

  for (size_t i = 0; i < n; ++i)
    if (seen[i])
      throw std::runtime_error("some interval appears on top but not on bot");
}

// setter getter

template <typename Tlen, typename Tmat>
void IntervalExchangeTransformation<Tlen, Tmat>::setIdentityPermutation() {
  top = &(labels[0].i1);
  bot = &(labels[0].i2);
  top->prev = nullptr;
  bot->prev = nullptr;

  for (size_t i = 0; i < n - 1; ++i) {
    labels[i].i1.next = &(labels[i + 1].i1);
    labels[i + 1].i1.prev = &(labels[i].i1);

    labels[i].i2.next = &(labels[i + 1].i2);
    labels[i + 1].i2.prev = &(labels[i].i2);
  }

  labels[n - 1].i1.next = nullptr;
  labels[n - 1].i2.next = nullptr;
}

template <typename Tlen, typename Tmat>
void IntervalExchangeTransformation<Tlen, Tmat>::setTop(const Permutation& p) {
  if (p.size() != n)
    throw std::invalid_argument("wrong size of permutations");

  top = &(labels[p[0]].i1);
  top->prev = nullptr;

  for (size_t i = 0; i < n - 1; i++) {
    labels[p[i]].i1.next = &(labels[p[i + 1]].i1);
    labels[p[i + 1]].i1.prev = &(labels[p[i]].i1);
  }

  labels[p[n - 1]].i1.next = nullptr;
}

template <typename Tlen, typename Tmat>
void IntervalExchangeTransformation<Tlen, Tmat>::setBot(const Permutation& p) {
  bot = &(labels[p[0]].i2);
  bot->prev = nullptr;

  for (size_t i = 0; i < n - 1; i++) {
    labels[p[i]].i2.next = &(labels[p[i + 1]].i2);
    labels[p[i + 1]].i2.prev = &(labels[p[i]].i2);
  }

  labels[p[n - 1]].i2.next = nullptr;
}

template <typename Tlen, typename Tmat>
Permutation IntervalExchangeTransformation<Tlen, Tmat>::topPermutation() const {
  Permutation p;
  for (Interval<Tlen, Tmat>* t = top; t != nullptr; t = t->next)
    p.push_back(numeric_cast<unsigned int>(t->lab->index));
  return p;
}

template <typename Tlen, typename Tmat>
Permutation IntervalExchangeTransformation<Tlen, Tmat>::botPermutation() const {
  Permutation p;
  for (Interval<Tlen, Tmat>* b = bot; b != nullptr; b = b->next)
    p.push_back(numeric_cast<unsigned int>(b->lab->index));
  return p;
}

template <typename Tlen, typename Tmat>
void IntervalExchangeTransformation<Tlen, Tmat>::setLengths(const std::vector<Tlen>& lengths) {
  if (lengths.size() != n)
    throw std::invalid_argument("wrong size vector 'lengths'");

  typename std::vector<Tlen>::const_iterator i = lengths.begin();
  typename std::vector<Label<Tlen, Tmat>>::iterator j = labels.begin();
  for (size_t count = 0; count < n; count++) {
    (*j).length = *i;
    i++;
    j++;
  }
}

template <typename Tlen, typename Tmat>
std::vector<Tlen> IntervalExchangeTransformation<Tlen, Tmat>::lengths() const {
  std::vector<Tlen> res;
  for (typename std::vector<Label<Tlen, Tmat>>::const_iterator i = labels.begin(); i != labels.end(); i++)
    res.push_back((*i).length);
  return res;
}

// output stream

template <typename Tlen, typename Tmat>
std::ostream& operator<<(std::ostream& os, const IntervalExchangeTransformation<Tlen, Tmat>& t) {
  for (Interval<Tlen, Tmat>* i = t.top; i != nullptr; i = i->next)
    os << (i->lab->index);
  os << std::endl;
  for (Interval<Tlen, Tmat>* i = t.bot; i != nullptr; i = i->next)
    os << (i->lab->index);
  os << std::endl;

  for (typename std::vector<Label<Tlen, Tmat>>::const_iterator i = t.labels.begin(); i != t.labels.end(); i++) {
    os << "interval " << (*i).index << " length " << (*i).length << std::endl;
  }
  for (typename std::vector<Label<Tlen, Tmat>>::const_iterator i = t.labels.begin(); i != t.labels.end(); i++) {
    for (const auto j : (*i).v) os << j << " ";
    os << std::endl;
  }

  return os;
}

// Properties

template <typename Tlen, typename Tmat>
bool IntervalExchangeTransformation<Tlen, Tmat>::isReducible() const {
  std::vector<bool> seen(n, false);
  int topahead = 0;
  int botahead = 0;

  Interval<Tlen, Tmat>* t = top;
  Interval<Tlen, Tmat>* b = bot;

  for (size_t count = 0; count < n - 1; count++) {
    if (seen[t->lab->index])
      botahead--;
    else {
      topahead++;
      seen[t->lab->index] = true;
    }

    if (seen[b->lab->index])
      topahead--;
    else {
      botahead++;
      seen[b->lab->index] = true;
    }

    if (topahead == 0 && botahead == 0)
      return true;

    t = t->next;
    b = b->next;
  }

  if (botahead != 1 || topahead != 1)
    throw std::runtime_error("unexpected behavior");

  return false;
}

// set v1 = v1 + m * v2
template <typename T>
static inline void vectorAdd(std::vector<T>& v1, const std::vector<T>& v2, const T& m) {
  for (size_t i = 0; i < v1.size(); i++)
    v1[i] += m * v2[i];
}

// move the labels in the chain between i (included) and j (excluded) just
// before k. It is assumed that i is the start of the chain.
template <typename Tlen, typename Tmat>
static inline Interval<Tlen, Tmat>* move(Interval<Tlen, Tmat>* i, Interval<Tlen, Tmat>* j, Interval<Tlen, Tmat>* k) {
  // nothing to move
  if (i == j || j == k) return i;

  //  i ... jj j ... kk k
  //  ->
  //  j .. kk i .. jj k
  Interval<Tlen, Tmat>* jj = j->prev;
  Interval<Tlen, Tmat>* kk = k->prev;

  j->prev = nullptr;

  k->prev = jj;
  jj->next = k;

  kk->next = i;
  i->prev = kk;

  i = j;

  return j;
}

template <typename Tlen, typename Tmat>
void IntervalExchangeTransformation<Tlen, Tmat>::zorichInductionStep() {
  if (top->lab == bot->lab)
    throw std::invalid_argument("top and bot point toward the same label");

  Interval<Tlen, Tmat>* b = bot;
  Tlen l = 0;  // length to be subtracted
  while (b->lab != top->lab && l + b->lab->length < top->lab->length) {
    l += b->lab->length;
    b = b->next;
  }
  // std::cout << "l = " << l << std::endl;

  if (b->lab == top->lab) {
    // Zorich acceleration step (= perform m full Dehn twists)

    // vdelecroix had written here: here we want a floor division...
    // [isn't that what fdiv is doing?]
    Tmat m = fdiv<Tlen, Tmat>(top->lab->length, l);

    // std::cout << "m = " << m << std::endl;

    top->lab->length -= m * l;

    for (Interval<Tlen, Tmat>* bb = bot; bb != b; bb = bb->next)
      vectorAdd<Tmat>(bb->lab->v, top->lab->v, m);

    // recompute from what is left
    b = bot;
    l = 0;
    while (l + b->lab->length < top->lab->length) {
      l += b->lab->length;
      b = b->next;
    }
  }

  // partial twist
  top->lab->length -= l;
  for (Interval<Tlen, Tmat>* bb = bot; bb != b; bb = bb->next)
    vectorAdd<Tmat>(bb->lab->v, top->lab->v, 1);

  bot = move(bot, b, top->twin);
}

template <typename Tlen, typename Tmat>
void IntervalExchangeTransformation<Tlen, Tmat>::swapTopBot() {
  Interval<Tlen, Tmat>* tmp = top;
  top = bot;
  bot = tmp;
}
}  // namespace intervalxt

// template instantiations
template class intervalxt::IntervalExchangeTransformation<unsigned long, unsigned long>;
template std::ostream& intervalxt::operator<<(std::ostream& os, const intervalxt::IntervalExchangeTransformation<unsigned long, unsigned long>&);

template class intervalxt::IntervalExchangeTransformation<mpz_class, mpz_class>;
template std::ostream& intervalxt::operator<<(std::ostream& os, const intervalxt::IntervalExchangeTransformation<mpz_class, mpz_class>&);

template class intervalxt::IntervalExchangeTransformation<mpz_class, unsigned long>;
template std::ostream& intervalxt::operator<<(std::ostream& os, const intervalxt::IntervalExchangeTransformation<mpz_class, unsigned long>&);
