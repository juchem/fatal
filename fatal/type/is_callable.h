/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_is_callable_h
#define FATAL_INCLUDE_fatal_type_is_callable_h

#include <fatal/portability.h>

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

#include <fatal/type/impl/is_callable.h>

namespace ftl {

/**
 * Tells whether an instance of a given type supports the
 * call operator, when passing arguments of given types.
 *
 * Example:
 *
 *  void noop();
 *
 *  int fn(std::string const &s, bool b);
 *
 *  struct non_callable {};
 *
 *  struct callable {
 *    void operator ()(int i, double d);
 *  };
 *
 *  // yields `true`
 *  is_callable_t<noop>::value
 *
 *  // yields `false`
 *  is_callable_t<decltype(fn), int, double>::value
 *
 *  // yields `true`
 *  is_callable_t<decltype(fn), char const *, bool>::value
 *
 *  // yields `false`
 *  is_callable_t<non_callable>::value
 *
 *  // yields `false`
 *  is_callable_t<non_callable, int>::value
 *
 *  // yields `true`
 *  is_callable_t<callable, int, double>::value
 *
 *  // yields `false`
 *  is_callable_t<callable, int, double, std::string>::value
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <typename T, typename... Args>
using is_callable_t = typename i_icl::icl<Args...>::template type<T>;

template <typename T, typename... Args>
FATAL_HIDE_SYMBOL
constexpr bool is_callable_v = is_callable_t<T, Args...>::value;

template <typename... Args, typename T>
constexpr bool is_callable(T &&) {
  return is_callable_t<T &&, Args...>::value;
}

template <typename T, typename Arg, typename... Args>
constexpr bool is_callable(T &&, Arg &&, Args &&...) {
  return is_callable_t<T &&, Arg &&, Args &&...>::value;
}

} // namespace ftl {

FATAL_DIAGNOSTIC_POP

#endif // FATAL_INCLUDE_fatal_type_is_callable_h
