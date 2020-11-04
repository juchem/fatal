/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_impl_fold_h
#define FATAL_INCLUDE_fatal_type_impl_fold_h

#include <fatal/type/list.h>

namespace ftl {
namespace i_fl {

// TODO: Implement a version for  sequences
template <typename...> struct FATAL_HIDE_SYMBOL fold;

template <typename Fn, typename Seed, typename... R>
struct FATAL_HIDE_SYMBOL fold<Fn, Seed, list<R...>> {
  template <template <typename...> class V>
  using apply = V<R...>;
};

template <
  typename Fn,
  typename Seed,
  typename... R,
  typename T,
  typename... Args
>
struct FATAL_HIDE_SYMBOL fold<Fn, Seed, list<R...>, T, Args...>:
  fold<
    Fn,
    typename Fn::template apply<Seed, T>,
    list<R..., typename Fn::template apply<Seed, T>>,
    Args...
  >
{};

template <typename...> struct FATAL_HIDE_SYMBOL F;

template <
  typename Seed,
  typename Fn,
  template <typename...> class V,
  typename... Args
>
struct FATAL_HIDE_SYMBOL F<Seed, Fn, V<Args...>> {
  using type = typename fold<Fn, Seed, list<>, Args...>::template apply<V>;
};

} // namespace i_fl {
} // namespace ftl {

#endif // FATAL_INCLUDE_fatal_type_impl_fold_h
