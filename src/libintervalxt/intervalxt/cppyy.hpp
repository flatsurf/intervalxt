#ifndef LIBINTERVALXT_CPPYY_HPP
#define LIBINTERVALXT_CPPYY_HPP

#include <iosfwd>

#include "intervalxt/iet.hpp"

namespace intervalxt {
template <typename TTlen, typename TTmat>
std::ostream &operator<<(std::ostream &, const IntervalExchangeTransformation<TTlen, TTmat> &);
}

extern template std::ostream &intervalxt::operator<<(std::ostream &, const IntervalExchangeTransformation<unsigned long, unsigned long> &);

#endif
