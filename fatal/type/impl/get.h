/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_impl_get_h
#define FATAL_INCLUDE_fatal_type_impl_get_h

#include <fatal/type/inherit.h>
#include <fatal/type/pair.h>

namespace ftl {
namespace impl_gt {

template <typename Key, typename Value>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static Value f(pair<Key, Value>);

template <typename> struct FATAL_HIDE_SYMBOL g;

template <template <typename...> typename List, typename... Args>
struct FATAL_HIDE_SYMBOL g<List<Args...>> {
  template <typename Key, typename KeyFilter, typename PostFilter>
  using apply = typename PostFilter::template apply<decltype(f<Key>(
    inherit<pair<typename KeyFilter::template apply<Args>, Args>...>())
  )>;
};

} // namespace impl_gt {
} // namespace ftl {

#endif // FATAL_INCLUDE_fatal_type_impl_get_h
