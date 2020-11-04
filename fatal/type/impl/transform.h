/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_impl_transform_h
#define FATAL_INCLUDE_fatal_type_impl_transform_h

namespace ftl {
namespace i_t {

// transform //
template <typename...> struct FATAL_HIDE_SYMBOL t;

template <template <typename...> typename Variadic, typename... Args>
struct FATAL_HIDE_SYMBOL t<Variadic<Args...>> {
  using type = Variadic<Args...>;
};

template <
  template <typename...> typename Variadic, typename... Args, typename T0
>
struct FATAL_HIDE_SYMBOL t<Variadic<Args...>, T0> {
  using type = Variadic<typename T0::template apply<Args>...>;
};

template <
  template <typename...> typename Variadic, typename... Args,
  typename T0, typename T1
>
struct FATAL_HIDE_SYMBOL t<Variadic<Args...>, T0, T1> {
  using type = Variadic<
    typename T1::template apply<
      typename T0::template apply<Args>
    >...
  >;
};

template <
  template <typename...> typename Variadic, typename... Args,
  typename T0, typename T1, typename T2
>
struct FATAL_HIDE_SYMBOL t<Variadic<Args...>, T0, T1, T2> {
  using type = Variadic<
    typename T2::template apply<
      typename T1::template apply<
        typename T0::template apply<Args>
      >
    >...
  >;
};

template <
  template <typename...> typename Variadic, typename... Args,
  typename T0, typename T1, typename T2, typename T3
>
struct FATAL_HIDE_SYMBOL t<Variadic<Args...>, T0, T1, T2, T3> {
  using type = Variadic<
    typename T3::template apply<
      typename T2::template apply<
        typename T1::template apply<
          typename T0::template apply<Args>
        >
      >
    >...
  >;
};

template <
  template <typename...> typename Variadic, typename... Args,
  typename T0, typename T1, typename T2, typename T3, typename T4,
  typename... Tn
>
struct FATAL_HIDE_SYMBOL t<Variadic<Args...>, T0, T1, T2, T3, T4, Tn...>:
  t<
    Variadic<
      typename T4::template apply<
        typename T3::template apply<
          typename T2::template apply<
            typename T1::template apply<
              typename T0::template apply<Args>
            >
          >
        >
      >...
    >,
    Tn...
  >
{};

// transform_if //
template <bool, typename WhenTrue, typename>
struct FATAL_HIDE_SYMBOL C: WhenTrue {};

template <typename WhenTrue, typename WhenFalse>
struct FATAL_HIDE_SYMBOL C<false, WhenTrue, WhenFalse>: WhenFalse {};

template <typename Predicate, typename WhenTrue, typename WhenFalse>
struct FATAL_HIDE_SYMBOL c {
  template <typename T>
  using apply = typename C<
    Predicate::template apply<T>::value,
    WhenTrue,
    WhenFalse
  >::template apply<T>;
};

} // namespace i_t {
} // namespace ftl {

#endif // FATAL_INCLUDE_fatal_type_impl_transform_h
