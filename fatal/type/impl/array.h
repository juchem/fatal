/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_impl_array_h
#define FATAL_INCLUDE_fatal_type_impl_array_h

#include <fatal/type/list.h>
#include <fatal/type/slice.h>

#include <type_traits>

namespace ftl {
namespace i_a {

// constexpr statically allocated array //

template <std::size_t Excess, typename T, T... Values>
struct FATAL_HIDE_SYMBOL a {
  static_assert(Excess <= sizeof...(Values), "internal error");
  using size = std::integral_constant<std::size_t, sizeof...(Values) - Excess>;

  FATAL_HIDE_SYMBOL
  static constexpr T const data[sizeof...(Values)] = {Values...};
};

template <std::size_t Excess, typename T, T... Values>
FATAL_HIDE_SYMBOL
constexpr T const a<Excess, T, Values...>::data[sizeof...(Values)];

template <
  std::size_t Excess,
  typename T,
  typename OuterFilter,
  typename InnerFilter,
  typename Array,
  std::size_t... Indexes
>
struct FATAL_HIDE_SYMBOL aF {
  static_assert(Excess <= sizeof...(Indexes), "internal error");
  using size = std::integral_constant<std::size_t, sizeof...(Indexes) - Excess>;

  FATAL_HIDE_SYMBOL
  static constexpr T const data[sizeof...(Indexes)] = {
    static_cast<T>(
      InnerFilter::template apply<
        at<typename OuterFilter::template apply<Array>, Indexes>
      >::value
    )...
  };
};

template <
  std::size_t Excess,
  typename T,
  typename OuterFilter,
  typename InnerFilter,
  typename Array,
  std::size_t... Indexes
>
FATAL_HIDE_SYMBOL
constexpr T const aF<
  Excess, T, OuterFilter, InnerFilter, Array, Indexes...
>::data[sizeof...(Indexes)];

template <typename...> struct FATAL_HIDE_SYMBOL C;

// constexpr statically allocated array from a list or sequence //
template <template <typename...> typename Variadics, typename... Args, typename T>
struct FATAL_HIDE_SYMBOL C<Variadics<Args...>, T>: a<0, T, Args::value...> {};

template <template <typename...> typename Variadics, typename T, typename... Args>
struct FATAL_HIDE_SYMBOL C<Variadics<T, Args...>>:
  a<
    0,
    typename std::decay<decltype(T::value)>::type,
    T::value,
    Args::value...
  >
{};

template <
  template <typename V, V...> typename Variadics,
  typename V,
  V... Values,
  typename T
>
struct FATAL_HIDE_SYMBOL C<Variadics<V, Values...>, T>: a<0, T, Values...> {};

template <template <typename V, V...> typename Variadics, typename T, T... Values>
struct FATAL_HIDE_SYMBOL C<Variadics<T, Values...>>: a<0, T, Values...> {};

template <typename...> struct FATAL_HIDE_SYMBOL CF;

// constexpr statically allocated array from a list or sequence //
template <
  std::size_t... Indexes,
  typename OuterFilter, typename InnerFilter,
  typename Array, typename T
>
struct FATAL_HIDE_SYMBOL CF<index_sequence<Indexes...>, OuterFilter, InnerFilter, Array, T>:
  aF<0, T, OuterFilter, InnerFilter, Array, Indexes...>
{};

template <
  std::size_t... Indexes,
  typename OuterFilter, typename InnerFilter,
  typename Array
>
struct FATAL_HIDE_SYMBOL CF<index_sequence<Indexes...>, OuterFilter, InnerFilter, Array>:
  aF<
    0,
    typename std::decay<
      decltype(first<typename OuterFilter::template apply<Array>>::value)
    >::type,
    OuterFilter, InnerFilter, Array, Indexes...
  >
{};

// statically allocated array from a list or sequence with null terminator //

template <typename...> struct FATAL_HIDE_SYMBOL z;

template <template <typename...> typename Variadics, typename T, typename... Args>
struct FATAL_HIDE_SYMBOL z<Variadics<T, Args...>>:
  a<
    1,
    typename std::decay<decltype(T::value)>::type,
    T::value,
    Args::value...,
    static_cast<typename std::decay<decltype(T::value)>::type>(0)
  >
{};

template <template <typename...> typename Variadics, typename... Args, typename T>
struct FATAL_HIDE_SYMBOL z<Variadics<Args...>, T>: a<1, T, Args::value..., static_cast<T>(0)> {};

template <template <typename V, V...> typename Variadics, typename T, T... Args>
struct FATAL_HIDE_SYMBOL z<Variadics<T, Args...>>: a<1, T, Args..., static_cast<T>(0)> {};

template <
  template <typename V, V...> typename Variadics,
  typename Value,
  Value... Args,
  typename T
>
struct FATAL_HIDE_SYMBOL z<Variadics<Value, Args...>, T>:
  a<1, T, static_cast<T>(Args)..., static_cast<T>(0)>
{};

template <typename...> struct FATAL_HIDE_SYMBOL zF;

template <
  std::size_t... Indexes,
  template <typename...> typename Variadic,
  typename T, typename... Args
>
struct FATAL_HIDE_SYMBOL zF<index_sequence<Indexes...>, Variadic<T, Args...>>:
  aF<
    1, typename std::decay<decltype(T::value)>::type,
    get_identity, get_identity,
    list<
      T, Args...,
      std::integral_constant<
        typename std::decay<decltype(T::value)>::type,
        static_cast<typename std::decay<decltype(T::value)>::type>(0)
      >
    >,
    Indexes..., sizeof...(Indexes)
  >
{};

template <
  std::size_t... Indexes,
  template <typename...> typename Variadic,
  typename... Args, typename T
>
struct FATAL_HIDE_SYMBOL zF<index_sequence<Indexes...>, Variadic<Args...>, T>:
  aF<
    1, T,
    get_identity, get_identity,
    list<Args..., std::integral_constant<T, static_cast<T>(0)>>,
    Indexes..., sizeof...(Indexes)
  >
{};

template <
  std::size_t... Indexes,
  template <typename V, V...> typename Variadic,
  typename T, T... Args
>
struct FATAL_HIDE_SYMBOL zF<index_sequence<Indexes...>, Variadic<T, Args...>>:
  aF<
    1, T, get_identity, get_identity,
    sequence<T, Args..., static_cast<T>(0)>,
    Indexes..., sizeof...(Indexes)
  >
{};

template <
  std::size_t... Indexes,
  template <typename V, V...> typename Variadic,
  typename Value,
  Value... Args,
  typename T
>
struct FATAL_HIDE_SYMBOL zF<index_sequence<Indexes...>, Variadic<Value, Args...>, T>:
  aF<
    1, T, get_identity, get_identity,
    sequence<Value, Args..., static_cast<Value>(0)>,
    Indexes..., sizeof...(Indexes)
  >
{};

// z_array

template <typename T, typename... Args>
struct FATAL_HIDE_SYMBOL Z {
  using value_type = T;
  using size = std::integral_constant<std::size_t, sizeof...(Args)>;

  FATAL_HIDE_SYMBOL
  static constexpr T const data[sizeof...(Args)] = { z<Args>::data... };
};

template <typename T, typename... Args>
FATAL_HIDE_SYMBOL
constexpr T const Z<T, Args...>::data[sizeof...(Args)];

template <typename...> struct FATAL_HIDE_SYMBOL ZA;

template <template <typename...> typename Variadics, typename... Args, typename T>
struct FATAL_HIDE_SYMBOL ZA<Variadics<Args...>, T>: Z<T, Args...> {};

template <template <typename...> typename Variadics, typename T, typename... Args>
struct FATAL_HIDE_SYMBOL ZA<Variadics<T, Args...>>:
  Z<typename std::decay<decltype(z<T>::data)>::type, T, Args...>
{};

template <typename T, typename Filter, typename... Args>
class FATAL_HIDE_SYMBOL ZF {
  template <typename Arg>
  using indexes = make_index_sequence<
    size<typename Filter::template apply<Arg>>::value
  >;

public:
  using value_type = T;
  using size = std::integral_constant<std::size_t, sizeof...(Args)>;

  FATAL_HIDE_SYMBOL
  static constexpr T const data[sizeof...(Args)] = {
    zF<
      indexes<Args>,
      typename Filter::template apply<Args>
    >::data...
  };
};

template <typename T, typename Filter, typename... Args>
FATAL_HIDE_SYMBOL
constexpr T const ZF<T, Filter, Args...>::data[sizeof...(Args)];

template <typename...> struct FATAL_HIDE_SYMBOL ZAF;

template <
  template <typename...> typename Variadic,
  typename... Args,
  typename Filter,
  typename T
>
struct FATAL_HIDE_SYMBOL ZAF<Variadic<Args...>, Filter, T>: ZF<T, Filter, Args...> {};

template <
  template <typename...> typename Variadic,
  typename T,
  typename... Args,
  typename Filter
>
struct FATAL_HIDE_SYMBOL ZAF<Variadic<T, Args...>, Filter>:
  ZF<
    typename std::decay<
      decltype(
        zF<
          make_index_sequence<size<typename Filter::template apply<T>>::value>,
          typename Filter::template apply<T>
        >::data
      )
    >::type,
    Filter,
    T, Args...
  >
{};

// string_view_array

template <typename T, typename... Args>
struct FATAL_HIDE_SYMBOL s {
  using value_type = T;
  using size = std::integral_constant<std::size_t, sizeof...(Args)>;

  FATAL_HIDE_SYMBOL
  static constexpr T const data[sizeof...(Args)] = {
    T(z<Args, typename T::value_type>::data, ftl::size<Args>::value)...
  };
};

template <typename T, typename... Args>
FATAL_HIDE_SYMBOL
constexpr T const s<T, Args...>::data[sizeof...(Args)];

template <typename...> struct FATAL_HIDE_SYMBOL S;

template <template <typename...> typename Variadics, typename... Args, typename T>
struct FATAL_HIDE_SYMBOL S<Variadics<Args...>, T>: s<T, Args...> {};

template <
  typename T,
  typename OuterFilter,
  typename InnerFilter,
  typename Array,
  std::size_t... Indexes
>
class FATAL_HIDE_SYMBOL sF {
  template <typename Element>
  using str = zF<
    make_index_sequence<size<Element>::value>,
    Element,
    typename T::value_type
  >;

public:
  using value_type = T;
  using size = std::integral_constant<std::size_t, sizeof...(Indexes)>;

  FATAL_HIDE_SYMBOL
  static constexpr T const data[sizeof...(Indexes)] = {
    T(
      str<
        typename InnerFilter::template apply<
          at<typename OuterFilter::template apply<Array>, Indexes>
        >
      >::data,
      ftl::size<
        typename InnerFilter::template apply<
          at<typename OuterFilter::template apply<Array>, Indexes>
        >
      >::value
    )...
  };
};

template <
  typename T,
  typename OuterFilter,
  typename InnerFilter,
  typename Array,
  std::size_t... Indexes
>
FATAL_HIDE_SYMBOL
constexpr T const sF<T, OuterFilter, InnerFilter, Array, Indexes...>::data[
  sizeof...(Indexes)
];

template <typename...> struct FATAL_HIDE_SYMBOL SF;

template <
  std::size_t... Indexes,
  typename Array,
  typename OuterFilter,
  typename InnerFilter,
  typename T
>
struct FATAL_HIDE_SYMBOL SF<index_sequence<Indexes...>, Array, OuterFilter, InnerFilter, T>:
  sF<T, OuterFilter, InnerFilter, Array, Indexes...>
{};

// statically allocated array from an element factory - entry point

template <template <typename...> typename, typename...> struct FATAL_HIDE_SYMBOL A;

template <
  template <typename...> typename Array,
  template <typename...> typename Variadic,
  typename... Args,
  typename Factory,
  typename T
>
struct FATAL_HIDE_SYMBOL A<Array, Variadic<Args...>, Factory, T>:
  Array<T, Factory, Args...>
{};

template <
  template <typename...> typename Array,
  template <typename...> typename Variadic,
  typename T,
  typename... Args,
  typename Factory
>
struct FATAL_HIDE_SYMBOL A<Array, Variadic<T, Args...>, Factory>:
  Array<
    typename std::decay<decltype(Factory::template get<T>())>::type,
    Factory,
    T, Args...
  >
{};

// constexpr statically allocated array from element factory//

template <typename T, typename Factory, typename... Args>
struct FATAL_HIDE_SYMBOL c {
  using value_type = T;
  using size = std::integral_constant<std::size_t, sizeof...(Args)>;

  FATAL_HIDE_SYMBOL
  static constexpr T const data[sizeof...(Args)] = {
    Factory::template get<Args>()...
  };
};

template <typename T, typename Factory, typename... Args>
FATAL_HIDE_SYMBOL
constexpr T const c<T, Factory, Args...>::data[sizeof...(Args)];

// non-constexpr statically allocated array from element factory//

template <typename T, typename Factory, typename... Args>
struct FATAL_HIDE_SYMBOL n {
  using value_type = T;
  using size = std::integral_constant<std::size_t, sizeof...(Args)>;

  FATAL_HIDE_SYMBOL
  static T const data[sizeof...(Args)];
};

template <typename T, typename Factory, typename... Args>
FATAL_HIDE_SYMBOL
T const n<T, Factory, Args...>::data[sizeof...(Args)] = {
  Factory::template get<Args>()...
};

} // namespace i_a {
} // namespace ftl {

#endif // FATAL_INCLUDE_fatal_type_impl_array_h
