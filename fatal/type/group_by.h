/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_group_by_h
#define FATAL_INCLUDE_fatal_type_group_by_h

#include <fatal/type/list.h>
#include <fatal/type/pair.h>

#include <fatal/portability.h>

#include <fatal/type/impl/group_by.h>

namespace fatal {

// O(n) on the size of the variadic template T
// `Group` is guaranteed to be instantiated exactly once per group
template <
  typename T,
  typename Key,
  template <typename...> typename Group = list,
  template <typename...> typename Outer = list
>
using group_by = typename i_g::G<T, Key, Group, Outer>::type;

// O(n) on the size of the variadic template T
// `Pair` is guaranteed to be instantiated exactly once
// `Group` is guaranteed to be instantiated exactly once per group
template <
  typename T,
  typename Key,
  typename Filter,
  template <typename...> typename Pair = pair,
  template <typename...> typename Group = list
>
using filtered_group_by = typename i_g::F<T, Key, Filter, Pair, Group>::type;

} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_type_group_by_h
