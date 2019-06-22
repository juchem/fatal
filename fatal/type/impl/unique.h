/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_impl_unique_h
#define FATAL_INCLUDE_fatal_type_impl_unique_h

#include <fatal/type/list.h>

#include <fatal/portability.h>

namespace fatal {
namespace i_uq {

/////////////////////
// adjacent_unique //
/////////////////////

// TODO: OPTIMIZE
// unique recursion //
template <template <typename...> typename, typename...> struct FATAL_HIDE_SYMBOL a;

// unique //
template <
  template <typename...> typename Variadic,
  typename... Result,
  typename T,
  typename U,
  typename... Args
>
struct FATAL_HIDE_SYMBOL a<Variadic, list<Result...>, T, U, Args...>:
  a<Variadic, list<Result..., T>, U, Args...>
{};

// duplicate //
template <
  template <typename...> typename Variadic,
  typename... Result,
  typename T,
  typename... Args
>
struct FATAL_HIDE_SYMBOL a<Variadic, list<Result...>, T, T, Args...>:
  a<Variadic, list<Result...>, T, Args...>
{};

// base case //
template <
  template <typename...> typename Variadic,
  typename... Result,
  typename T
>
struct FATAL_HIDE_SYMBOL a<Variadic, list<Result...>, T> {
  using type = Variadic<Result..., T>;
};

////////////////////////
// adjacent_unique_by //
////////////////////////

// unique recursion //
template <bool, template <typename...> typename, typename...> struct FATAL_HIDE_SYMBOL A;

// unique //
template <
  template <typename...> typename Variadic,
  typename Comparer,
  typename... Result,
  typename LHS,
  typename RHS,
  typename... Args
>
struct FATAL_HIDE_SYMBOL A<
  false,
  Variadic,
  Comparer,
  list<Result...>,
  LHS, RHS, Args...
>:
  A<
    Comparer::template apply<LHS, RHS>::value,
    Variadic, Comparer, list<Result..., LHS>, RHS, Args...
  >
{};

// duplicate //
template <
  template <typename...> typename Variadic,
  typename Comparer,
  typename... Result,
  typename LHS,
  typename RHS,
  typename... Args
>
struct FATAL_HIDE_SYMBOL A<true, Variadic, Comparer, list<Result...>, LHS, RHS, Args...>:
  A<
    Comparer::template apply<LHS, RHS>::value,
    Variadic, Comparer, list<Result...>, RHS, Args...
  >
{};

// base case //
template <
  template <typename...> typename Variadic,
  typename Comparer,
  typename... Result,
  typename T
>
struct FATAL_HIDE_SYMBOL A<false, Variadic, Comparer, list<Result...>, T> {
  using type = Variadic<Result..., T>;
};

// base case //
template <
  template <typename...> typename Variadic,
  typename Comparer,
  typename... Result,
  typename T
>
struct FATAL_HIDE_SYMBOL A<true, Variadic, Comparer, list<Result...>, T> {
  using type = Variadic<Result...>;
};

/////////////////
// entry_point //
/////////////////

// unique entry point //
template <typename...> struct FATAL_HIDE_SYMBOL au;

template <template <typename...> typename Variadic, typename... Args>
struct FATAL_HIDE_SYMBOL au<Variadic<Args...>>: a<Variadic, list<>, Args...> {};

template <template <typename...> typename Variadic>
struct FATAL_HIDE_SYMBOL au<Variadic<>> {
  using type = Variadic<>;
};

template <
  template <typename...> typename Variadic,
  typename... Args,
  typename Comparer
>
struct FATAL_HIDE_SYMBOL au<Variadic<Args...>, Comparer>:
  A<false, Variadic, Comparer, list<>, Args...>
{};

template <template <typename...> typename Variadic, typename Comparer>
struct FATAL_HIDE_SYMBOL au<Variadic<>, Comparer> {
  using type = Variadic<>;
};

} // namespace i_uq {
} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_type_impl_unique_h
