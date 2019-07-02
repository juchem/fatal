/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_functional_identity_h
#define FATAL_INCLUDE_fatal_functional_identity_h

#include <fatal/portability.h>

#include <utility>

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

namespace fatal {
namespace fn {

/**
 * An identity operation that receives any parameter and returns it, unaltered.
 *
 * Example:
 *
 *  identity i;
 *
 *  // yields an `int &&` containing `10`
 *  auto result1 = i(10);
 *
 *  // yields a `char const [6] &` pointing to "hello"
 *  auto result2 = i("hello");
 *
 *  // yields a `std::string &&` containing "world"
 *  auto result3 = i(std::string("world"));
 *
 *  std::string s("test");
 *  // yields a `std::string &` containing "test"
 *  auto result4 = i(s);
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
struct FATAL_HIDE_SYMBOL identity {
  template <typename T>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  constexpr T &&operator ()(T &&value) const noexcept {
    return std::forward<T>(value);
  }
};

} // namespace fn {
} // namespace fatal {

FATAL_DIAGNOSTIC_POP

#endif // FATAL_INCLUDE_fatal_functional_identity_h
