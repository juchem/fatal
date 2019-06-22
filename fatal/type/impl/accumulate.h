/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_impl_accumulate_h
#define FATAL_INCLUDE_fatal_type_impl_accumulate_h

namespace fatal {
namespace i_A {

// TODO: Implement a version for  sequences
template <typename...> struct FATAL_HIDE_SYMBOL a;

template <typename Fn, typename Seed>
struct FATAL_HIDE_SYMBOL a<Fn, Seed> {
  using type = Seed;
};

template <
  typename Fn,
  typename Seed,
  typename T,
  typename... Args
>
struct FATAL_HIDE_SYMBOL a<Fn, Seed, T, Args...>:
  a<Fn, typename Fn::template apply<Seed, T>, Args...>
{};

template <typename...> struct FATAL_HIDE_SYMBOL F;

template <
  typename Seed,
  typename Fn,
  template <typename...> typename V,
  typename... Args
>
struct FATAL_HIDE_SYMBOL F<Seed, Fn, V<Args...>>:
  a<Fn, Seed, Args...>
{};

// default metafunction //
// TODO: TEST DEFAULT METAFUNCTION
struct FATAL_HIDE_SYMBOL f {
  template <typename Seed, typename T>
  using apply = typename Seed::template apply<T>;
};

} // namespace i_A {
} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_type_impl_accumulate_h
