/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_impl_apply_h
#define FATAL_INCLUDE_fatal_type_impl_apply_h

namespace fatal {
namespace i_ap {

template <template <typename...> typename T, typename... Args>
struct FATAL_HIDE_SYMBOL a { using type = T<Args...>; };

template <template <typename> typename T, typename U>
struct FATAL_HIDE_SYMBOL a<T, U> { using type = T<U>; };

template <template <typename, typename> typename T, typename U0, typename U1>
struct FATAL_HIDE_SYMBOL a<T, U0, U1> { using type = T<U0, U1>; };

template <
  template <typename, typename, typename> typename T,
  typename U0, typename U1, typename U2
>
struct FATAL_HIDE_SYMBOL a<T, U0, U1, U2> { using type = T<U0, U1, U2>; };

template <
  template <typename, typename, typename, typename> typename T,
  typename U0, typename U1, typename U2, typename U3
>
struct FATAL_HIDE_SYMBOL a<T, U0, U1, U2, U3> { using type = T<U0, U1, U2, U3>; };

template <
  template <typename, typename, typename, typename, typename> typename T,
  typename U0, typename U1, typename U2, typename U3, typename U4
>
struct FATAL_HIDE_SYMBOL a<T, U0, U1, U2, U3, U4> {
  using type = T<U0, U1, U2, U3, U4>;
};

template <
  template <typename, typename, typename, typename, typename, typename> typename T,
  typename U0, typename U1, typename U2, typename U3, typename U4, typename U5
>
struct FATAL_HIDE_SYMBOL a<T, U0, U1, U2, U3, U4, U5> {
  using type = T<U0, U1, U2, U3, U4, U5>;
};

template <
  template <
    typename, typename, typename, typename, typename, typename, typename
  > typename T,
  typename U0, typename U1, typename U2, typename U3, typename U4, typename U5,
  typename V0
>
struct FATAL_HIDE_SYMBOL a<T, U0, U1, U2, U3, U4, U5, V0> {
  using type = T<U0, U1, U2, U3, U4, U5, V0>;
};

template <
  template <
    typename, typename, typename, typename, typename, typename,
    typename, typename
  > typename T,
  typename U0, typename U1, typename U2, typename U3, typename U4, typename U5,
  typename V0, typename V1
>
struct FATAL_HIDE_SYMBOL a<T, U0, U1, U2, U3, U4, U5, V0, V1> {
  using type = T<U0, U1, U2, U3, U4, U5, V0, V1>;
};

template <
  template <
    typename, typename, typename, typename, typename, typename,
    typename, typename, typename
  > typename T,
  typename U0, typename U1, typename U2, typename U3, typename U4, typename U5,
  typename V0, typename V1, typename V2
>
struct FATAL_HIDE_SYMBOL a<T, U0, U1, U2, U3, U4, U5, V0, V1, V2> {
  using type = T<U0, U1, U2, U3, U4, U5, V0, V1, V2>;
};

template <
  template <
    typename, typename, typename, typename, typename, typename,
    typename, typename, typename, typename
  > typename T,
  typename U0, typename U1, typename U2, typename U3, typename U4, typename U5,
  typename V0, typename V1, typename V2, typename V3
>
struct FATAL_HIDE_SYMBOL a<T, U0, U1, U2, U3, U4, U5, V0, V1, V2, V3> {
  using type = T<U0, U1, U2, U3, U4, U5, V0, V1, V2, V3>;
};

template <
  template <
    typename, typename, typename, typename, typename, typename,
    typename, typename, typename, typename, typename
  > typename T,
  typename U0, typename U1, typename U2, typename U3, typename U4, typename U5,
  typename V0, typename V1, typename V2, typename V3, typename V4
>
struct FATAL_HIDE_SYMBOL a<T, U0, U1, U2, U3, U4, U5, V0, V1, V2, V3, V4> {
  using type = T<U0, U1, U2, U3, U4, U5, V0, V1, V2, V3, V4>;
};

template <
  template <
    typename, typename, typename, typename, typename, typename,
    typename, typename, typename, typename, typename, typename
  > typename T,
  typename U0, typename U1, typename U2, typename U3, typename U4, typename U5,
  typename V0, typename V1, typename V2, typename V3, typename V4, typename V5
>
struct FATAL_HIDE_SYMBOL a<T, U0, U1, U2, U3, U4, U5, V0, V1, V2, V3, V4, V5> {
  using type = T<U0, U1, U2, U3, U4, U5, V0, V1, V2, V3, V4, V5>;
};

template <template <typename...> typename, typename...> struct FATAL_HIDE_SYMBOL LS;
template <
  template <typename...> typename To,
  template <typename...> typename List,
  typename... Args,
  typename... Suffix
>
struct FATAL_HIDE_SYMBOL LS<To, List<Args...>, Suffix...> {
  using type = typename a<To, Args..., Suffix...>::type;
};

template <template <typename...> typename, typename...> struct FATAL_HIDE_SYMBOL lsf;

template <
  template <typename...> typename To,
  template <typename...> typename List,
  typename... Args,
  typename... Prefix
>
struct FATAL_HIDE_SYMBOL lsf<To, List<Args...>, Prefix...> {
  using type = typename a<To, Prefix..., Args...>::type;
};

template <typename> struct FATAL_HIDE_SYMBOL sq;

template <
  template <typename V, V...> typename Sequence,
  typename T,
  T... Values
>
struct FATAL_HIDE_SYMBOL sq<Sequence<T, Values...>> {
  template<template <typename V, V...> typename To, T... Suffix>
  using apply = To<T, Values..., Suffix...>;

  template<template <typename V, V...> typename To, T... Prefix>
  using front = To<T, Prefix..., Values...>;
};

template <template <typename...> typename T, typename... Args>
struct FATAL_HIDE_SYMBOL app {
  using type = T<Args...>;
};

template <template <typename> typename T, typename U>
struct FATAL_HIDE_SYMBOL app<T, U> {
  using type = T<U>;
};

template <template <typename, typename> typename T, typename U0, typename U1>
struct FATAL_HIDE_SYMBOL app<T, U0, U1> {
  using type = T<U0, U1>;
};

template <
  template <typename, typename, typename> typename T,
  typename U0, typename U1, typename U2
>
struct FATAL_HIDE_SYMBOL app<T, U0, U1, U2> {
  using type = T<U0, U1, U2>;
};

template <
  template <typename, typename, typename, typename> typename T,
  typename U0, typename U1, typename U2, typename U3
>
struct FATAL_HIDE_SYMBOL app<T, U0, U1, U2, U3> {
  using type = T<U0, U1, U2, U3>;
};

template <
  template <typename, typename, typename, typename, typename> typename T,
  typename U0, typename U1, typename U2, typename U3, typename U4
>
struct FATAL_HIDE_SYMBOL app<T, U0, U1, U2, U3, U4> {
  using type = T<U0, U1, U2, U3, U4>;
};

template <
  template <typename, typename, typename, typename, typename, typename> typename T,
  typename U0, typename U1, typename U2, typename U3, typename U4, typename U5
>
struct FATAL_HIDE_SYMBOL app<T, U0, U1, U2, U3, U4, U5> {
  using type = T<U0, U1, U2, U3, U4, U5>;
};

template <
  template <
    typename, typename, typename, typename, typename, typename, typename
  > typename T,
  typename U0, typename U1, typename U2, typename U3, typename U4, typename U5,
  typename V0
>
struct FATAL_HIDE_SYMBOL app<T, U0, U1, U2, U3, U4, U5, V0> {
  using type = T<U0, U1, U2, U3, U4, U5, V0>;
};

template <
  template <
    typename, typename, typename, typename, typename, typename,
    typename, typename
  > typename T,
  typename U0, typename U1, typename U2, typename U3, typename U4, typename U5,
  typename V0, typename V1
>
struct FATAL_HIDE_SYMBOL app<T, U0, U1, U2, U3, U4, U5, V0, V1> {
  using type = T<U0, U1, U2, U3, U4, U5, V0, V1>;
};

template <
  template <
    typename, typename, typename, typename, typename, typename,
    typename, typename, typename
  > typename T,
  typename U0, typename U1, typename U2, typename U3, typename U4, typename U5,
  typename V0, typename V1, typename V2
>
struct FATAL_HIDE_SYMBOL app<T, U0, U1, U2, U3, U4, U5, V0, V1, V2> {
  using type = T<U0, U1, U2, U3, U4, U5, V0, V1, V2>;
};

template <
  template <
    typename, typename, typename, typename, typename, typename,
    typename, typename, typename, typename
  > typename T,
  typename U0, typename U1, typename U2, typename U3, typename U4, typename U5,
  typename V0, typename V1, typename V2, typename V3
>
struct FATAL_HIDE_SYMBOL app<T, U0, U1, U2, U3, U4, U5, V0, V1, V2, V3> {
  using type = T<U0, U1, U2, U3, U4, U5, V0, V1, V2, V3>;
};

template <
  template <
    typename, typename, typename, typename, typename, typename,
    typename, typename, typename, typename, typename
  > typename T,
  typename U0, typename U1, typename U2, typename U3, typename U4, typename U5,
  typename V0, typename V1, typename V2, typename V3, typename V4
>
struct FATAL_HIDE_SYMBOL app<T, U0, U1, U2, U3, U4, U5, V0, V1, V2, V3, V4> {
  using type = T<U0, U1, U2, U3, U4, U5, V0, V1, V2, V3, V4>;
};

template <
  template <
    typename, typename, typename, typename, typename, typename,
    typename, typename, typename, typename, typename, typename
  > typename T,
  typename U0, typename U1, typename U2, typename U3, typename U4, typename U5,
  typename V0, typename V1, typename V2, typename V3, typename V4, typename V5
>
struct FATAL_HIDE_SYMBOL app<T, U0, U1, U2, U3, U4, U5, V0, V1, V2, V3, V4, V5> {
  using type = T<U0, U1, U2, U3, U4, U5, V0, V1, V2, V3, V4, V5>;
};

} // namespace i_ap {
} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_type_impl_apply_h
