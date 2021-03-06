/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_longest_common_prefix_h
#define FATAL_INCLUDE_fatal_type_longest_common_prefix_h

#include <fatal/portability.h>

#include <cstdlib>

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

#include <fatal/type/impl/longest_common_prefix.h>

namespace ftl {

template <
  template <typename, std::size_t> typename Key,
  std::size_t Begin,
  std::size_t End,
  typename... Args
>
using longest_common_prefix = typename i_lcp::s<
  true, Key, Begin, End, Args...
>::type;

} // namespace ftl {

FATAL_DIAGNOSTIC_POP

#endif // FATAL_INCLUDE_fatal_type_longest_common_prefix_h
