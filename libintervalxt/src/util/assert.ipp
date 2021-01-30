/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019-2021 Julian Rüth
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

#ifndef LIBINTERVALXT_UTIL_ASSERT_IPP
#define LIBINTERVALXT_UTIL_ASSERT_IPP

#include <boost/preprocessor/stringize.hpp>
#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <sstream>

namespace intervalxt {
namespace {

// A throw statement that can be used in noexcept marked blocks without
// triggering compiler warnings.
template <typename E>
void throw_for_assert(const E& e) { throw e; }

// Return whether an environment variable should be considered as set.
bool isSet(const char* env) {
  const auto* ptr = std::getenv(env);
  if (ptr == nullptr) return false;

  std::string value = ptr;
  boost::trim(value);

  if (value == "0") return false;
  if (boost::iequals(value, "no")) return false;
  if (boost::iequals(value, "false")) return false;

  return true;
}

// Return whether all CHECK_ and ASSERT_ macros have been disabled at runtime
// through the LIBINTERVALXT_NOCHECK environment variable.
bool nocheck() {
  static bool value = isSet("LIBINTERVALXT_NOCHECK");
  return value;
}

// Return whether all ASSERT_ macros have been disabled at runtime through the
// LIBINTERVALXT_NOASSERT environment variable.
bool noassert() {
  if (nocheck()) return true;

  static bool value = isSet("LIBINTERVALXT_NOASSERT");
  return value;
}

}
} // namespace intervalxt

#define LIBINTERVALXT_ASSERT_(CONDITION, EXCEPTION, MESSAGE)                                  \
  while (BOOST_UNLIKELY(static_cast<bool>(not(CONDITION)))) {                   \
    std::stringstream user_message, assertion_message;                          \
    user_message << MESSAGE;                                                    \
    assertion_message << (#CONDITION " does not hold");                         \
    if (user_message.str().size())                                              \
      assertion_message << ": " << user_message.str();                          \
    else                                                                        \
      assertion_message << " ";                                                 \
    assertion_message << " in " __FILE__ ":" BOOST_PP_STRINGIZE(__LINE__);      \
    ::intervalxt::throw_for_assert(EXCEPTION(assertion_message.str().c_str())); \
  }

// Run a (cheap) check that a (user provided) argument is valid.
// If the check should be disabled when NDEBUG is defined, e.g., because it
// occurs in a hotspot, use LIBFLATSURF_ASSERT_ARGUMENT instead.
#define LIBINTERVALXT_CHECK_ARGUMENT_(CONDITION) LIBINTERVALXT_ASSERT_(::intervalxt::nocheck() || (CONDITION), std::invalid_argument, "")
#define LIBINTERVALXT_CHECK_ARGUMENT(CONDITION, MESSAGE) LIBINTERVALXT_ASSERT_(::intervalxt::nocheck() || (CONDITION), std::invalid_argument, MESSAGE)

#ifdef NDEBUG

#define LIBINTERVALXT_ASSERT_CONDITION(CONDITION) (true || ::intervalxt::noassert() || (CONDITION))

#else

#define LIBINTERVALXT_ASSERT_CONDITION(CONDITION) (::intervalxt::noassert() || (CONDITION))

#endif

#define LIBINTERVALXT_ASSERT_ARGUMENT_(CONDITION) CHECK_ARGUMENT_(LIBINTERVALXT_ASSERT_CONDITION(CONDITION))
#define LIBINTERVALXT_ASSERT_ARGUMENT(CONDITION, MESSAGE) CHECK_ARGUMENT(LIBINTERVALXT_ASSERT_CONDITION(CONDITION), MESSAGE)
#define LIBINTERVALXT_ASSERT(CONDITION, MESSAGE) LIBINTERVALXT_ASSERT_(LIBINTERVALXT_ASSERT_CONDITION(CONDITION), std::logic_error, MESSAGE)

#define LIBINTERVALXT_UNREACHABLE(MESSAGE)    \
  {                                           \
    LIBINTERVALXT_ASSERT_(false, std::logic_error, MESSAGE) \
    __builtin_unreachable();                  \
  }

#endif
