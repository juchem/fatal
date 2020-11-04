/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_impl_compare_h
#define FATAL_INCLUDE_fatal_type_impl_compare_h

namespace ftl {
namespace i_c {

template <typename, typename, typename, int = 0> struct FATAL_HIDE_SYMBOL sc;

template <typename Less, typename LHS, typename RHS>
struct FATAL_HIDE_SYMBOL sc<Less, LHS, RHS, -1>: std::true_type {};

template <typename Less, typename LHS, typename RHS>
struct FATAL_HIDE_SYMBOL sc<Less, LHS, RHS, 1>: std::false_type {};

template <typename Less, template <typename...> typename Variadic>
struct FATAL_HIDE_SYMBOL sc<Less, Variadic<>, Variadic<>, 0>: std::false_type {};

template <typename Less, template <typename...> typename Variadic, typename... LHS>
struct FATAL_HIDE_SYMBOL sc<Less, Variadic<LHS...>, Variadic<>, 0>: std::false_type {};

template <typename Less, template <typename...> typename Variadic, typename... RHS>
struct FATAL_HIDE_SYMBOL sc<Less, Variadic<>, Variadic<RHS...>, 0>: std::true_type {};

template <
  typename Less,
  template <typename...> typename Variadic,
  typename L, typename... LHS,
  typename R, typename... RHS
>
struct FATAL_HIDE_SYMBOL sc<Less, Variadic<L, LHS...>, Variadic<R, RHS...>, 0>:
  sc<
    Less,
    Variadic<LHS...>,
    Variadic<RHS...>,
    Less::template apply<L, R>::value
      ? -1
      : Less::template apply<R, L>::value
        ? 1
        : 0
  >
{};

template <typename Less, template <typename V, V...> typename Variadic, typename T>
struct FATAL_HIDE_SYMBOL sc<Less, Variadic<T>, Variadic<T>, 0>: std::false_type {};

template <
  typename Less,
  template <typename V, V...> typename Variadic,
  typename T, T L, T... LHS
>
struct FATAL_HIDE_SYMBOL sc<Less, Variadic<T, L, LHS...>, Variadic<T>, 0>: std::false_type {};

template <
  typename Less,
  template <typename V, V...> typename Variadic,
  typename T, T R, T... RHS
>
struct FATAL_HIDE_SYMBOL sc<Less, Variadic<T>, Variadic<T, R, RHS...>, 0>: std::true_type {};

template <
  typename Less,
  template <typename V, V...> typename Variadic,
  typename T,
  T L, T... LHS,
  T R, T... RHS
>
struct FATAL_HIDE_SYMBOL sc<Less, Variadic<T, L, LHS...>, Variadic<T, R, RHS...>, 0>:
  sc<
    Less,
    Variadic<T, LHS...>,
    Variadic<T, RHS...>,
    Less::template vapply<T, L, R>::value
      ? -1
      : Less::template vapply<T, R, L>::value
        ? 1
        : 0
  >
{};

} // namespace i_c {
} // namespace ftl {

#endif // FATAL_INCLUDE_fatal_type_impl_compare_h
