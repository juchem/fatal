/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_impl_slice_h
#define FATAL_INCLUDE_fatal_type_impl_slice_h

#include <fatal/type/cat.h>
#include <fatal/type/inherit.h>
#include <fatal/type/sequence.h>

#include <type_traits>

namespace ftl {
namespace i_at {

template <std::size_t, typename> struct FATAL_HIDE_SYMBOL a;

template <template <typename> typename Single, typename T>
struct FATAL_HIDE_SYMBOL a<0, Single<T>> {
  using type = T;
};

template <
  template <typename, typename> typename Pair,
  typename First, typename Second
>
struct FATAL_HIDE_SYMBOL a<0, Pair<First, Second>> {
  using type = First;
};

template <
  template <typename, typename> typename Pair,
  typename First, typename Second
>
struct FATAL_HIDE_SYMBOL a<1, Pair<First, Second>> {
  using type = Second;
};

template <template <typename...> typename List, typename T, typename... Args>
struct FATAL_HIDE_SYMBOL a<0, List<T, Args...>> {
  using type = T;
};

template <
  template <typename...> typename List,
  typename First, typename Second, typename... Args
>
struct FATAL_HIDE_SYMBOL a<1, List<First, Second, Args...>> {
  using type = Second;
};

template <
  template <typename...> typename List,
  typename First, typename Second, typename Third, typename... Args
>
struct FATAL_HIDE_SYMBOL a<2, List<First, Second, Third, Args...>> {
  using type = Third;
};

template <
  template <typename V, V...> typename Sequence,
  typename T, T Value, T... Values
>
struct FATAL_HIDE_SYMBOL a<0, Sequence<T, Value, Values...>> {
  using type = std::integral_constant<T, Value>;
};

template <
  template <typename V, V...> typename Sequence,
  typename T, T First, T Second, T... Values
>
struct FATAL_HIDE_SYMBOL a<1, Sequence<T, First, Second, Values...>> {
  using type = std::integral_constant<T, Second>;
};

template <
  template <typename V, V...> typename Sequence,
  typename T, T First, T Second, T Third, T... Values
>
struct FATAL_HIDE_SYMBOL a<2, Sequence<T, First, Second, Third, Values...>> {
  using type = std::integral_constant<T, Third>;
};

template <bool, std::size_t, typename, typename Default>
struct FATAL_HIDE_SYMBOL A {
  using type = Default;
};

template <std::size_t Index, typename T, typename Default>
struct FATAL_HIDE_SYMBOL A<true, Index, T, Default>: a<Index, T> {};

template <typename, std::size_t...> struct FATAL_HIDE_SYMBOL p;

#ifndef FATAL_AT_USE_VOID_POINTER_TRICK

template <typename...> struct FATAL_HIDE_SYMBOL tindex;

template <std::size_t... Indexes, typename... Args>
struct FATAL_HIDE_SYMBOL tindex<index_sequence<Indexes...>, Args...>:
  indexed<Args, Indexes>...
{};

template <typename, typename T, T...> struct FATAL_HIDE_SYMBOL vindex;

template <std::size_t... Indexes, typename T, T... Values>
struct FATAL_HIDE_SYMBOL vindex<index_sequence<Indexes...>, T, Values...>:
  indexed<std::integral_constant<T, Values>, Indexes>...
{};

template <std::size_t Index, typename T>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static T find(indexed<T, Index>);

template <
  std::size_t Index,
  template <typename...> typename List,
  typename... Args
>
struct FATAL_HIDE_SYMBOL a<Index, List<Args...>> {
  using type = decltype(
    find<Index>(
      tindex<make_index_sequence<sizeof...(Args)>, Args...>()
    )
  );
};

template <
  std::size_t Index,
  template <typename V, V...> typename Sequence,
  typename T, typename std::enable_if<(Index > 2), T>::type... Values
>
struct FATAL_HIDE_SYMBOL a<Index, Sequence<T, Values...>> {
  using type = decltype(
    find<Index>(
      vindex<make_index_sequence<sizeof...(Values)>, T, Values...>()
    )
  );
};

template <
  template <typename...> typename List,
  typename... Args, std::size_t... Indexes
>
struct FATAL_HIDE_SYMBOL p<List<Args...>, Indexes...> {
  using type = List<
    decltype(
      find<Indexes>(
        tindex<make_index_sequence<sizeof...(Args)>, Args...>()
      )
    )...
  >;
};

template <
  template <typename V, V...> typename Sequence,
  typename T,
  T... Values,
  std::size_t... Indexes
>
struct FATAL_HIDE_SYMBOL p<Sequence<T, Values...>, Indexes...> {
  using type = Sequence<
    T,
    decltype(
      find<Indexes>(
        vindex<make_index_sequence<sizeof...(Values)>, T, Values...>()
      )
    )::value...
  >;
};

#else // FATAL_AT_USE_VOID_POINTER_TRICK

// TODO: RE-CHECK AND REMOVE DEFINES
template <typename> struct FATAL_HIDE_SYMBOL tindex;

template <std::size_t... Indexes>
struct FATAL_HIDE_SYMBOL tindex<index_sequence<Indexes...>> {
  template <typename T>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static T find(decltype(reinterpret_cast<void *>(Indexes))..., tag<T> *, ...);
};

template <typename> struct FATAL_HIDE_SYMBOL vindex;

template <std::size_t... Indexes>
struct FATAL_HIDE_SYMBOL vindex<index_sequence<Indexes...>> {
  template <typename T, T Value>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static std::integral_constant<T, Value> find(
    decltype(reinterpret_cast<void *>(Indexes))...,
    std::integral_constant<T, Value> *,
    ...
  );
};

template <
  std::size_t Index,
  template <typename...> typename List,
  typename... Args
>
struct FATAL_HIDE_SYMBOL a<Index, List<Args...>> {
  using type = decltype(
    tindex<make_index_sequence<Index>>::find(
      static_cast<tag<Args> *>(nullptr)...
    )
  );
};

template <
  std::size_t Index,
  template <typename V, V...> typename Sequence,
  typename T,
  T... Values
>
struct FATAL_HIDE_SYMBOL a<Index, Sequence<T, Values...>> {
  using type = decltype(
    vindex<make_index_sequence<Index>>::find(
      static_cast<std::integral_constant<T, Values> *>(nullptr)...
    )
  );
};

template <
  template <typename...> typename List,
  typename... Args,
  std::size_t... Indexes
>
struct FATAL_HIDE_SYMBOL p<List<Args...>, Indexes...> {
  using type = List<
    decltype(
      tindex<make_index_sequence<Indexes>>::find(
        static_cast<tag<Args> *>(nullptr)...
      )
    )...
  >;
};

template <
  template <typename V, V...> typename Sequence,
  typename T,
  T... Values,
  std::size_t... Indexes
>
struct FATAL_HIDE_SYMBOL p<Sequence<T, Values...>, Indexes...> {
  using type = Sequence<
    T,
    decltype(
      vindex<make_index_sequence<Indexes>>::find(
        static_cast<std::integral_constant<T, Values> *>(nullptr)...
      )
    )::value...
  >;
};

#endif // FATAL_AT_USE_VOID_POINTER_TRICK

template <std::size_t...> struct FATAL_HIDE_SYMBOL ttindex;

template <std::size_t Size, std::size_t Left, std::size_t... Indexes>
struct FATAL_HIDE_SYMBOL ttindex<Size, Left, Indexes...> {
  template <template <typename...> typename List, typename T, typename... Tail>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static List<T, Tail...> list(
    decltype(reinterpret_cast<void *>(Indexes))...,
    tag<T> *,
    tag<Tail> *...
  );
};

template <std::size_t Size, std::size_t... Indexes>
struct FATAL_HIDE_SYMBOL ttindex<Size, Size, Indexes...> {
  template <template <typename...> typename List>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static List<> list(...);
};

template <std::size_t...> struct FATAL_HIDE_SYMBOL tvindex;

template <std::size_t Size, std::size_t Left, std::size_t... Indexes>
struct FATAL_HIDE_SYMBOL tvindex<Size, Left, Indexes...> {
  template <
    template <typename V, V...> typename Sequence,
    typename T,
    T Value,
    T... Values
  >
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static Sequence<T, Value, Values...> seq(
    decltype(reinterpret_cast<void *>(Indexes))...,
    std::integral_constant<T, Value> *,
    std::integral_constant<T, Values> *...
  );
};

template <std::size_t Size, std::size_t... Indexes>
struct FATAL_HIDE_SYMBOL tvindex<Size, Size, Indexes...> {
  template <template <typename V, V...> typename Sequence, typename T>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static Sequence<T> seq(...);
};

template <typename...> struct FATAL_HIDE_SYMBOL t;

template <
  template <typename...> typename List,
  typename... Args,
  std::size_t... Indexes
>
struct FATAL_HIDE_SYMBOL t<List<Args...>, index_sequence<Indexes...>> {
  using type = decltype(
    ttindex<sizeof...(Args), sizeof...(Indexes), Indexes...>
      ::template list<List>(static_cast<tag<Args> *>(nullptr)...)
  );
};

template <
  template <typename V, V...> typename Sequence,
  typename T,
  T... Values,
  std::size_t... Indexes
>
struct FATAL_HIDE_SYMBOL t<Sequence<T, Values...>, index_sequence<Indexes...>> {
  using type = decltype(
    tvindex<sizeof...(Values), sizeof...(Indexes), Indexes...>
      ::template seq<Sequence, T>(
        static_cast<std::integral_constant<T, Values> *>(nullptr)...
      )
  );
};

template <std::size_t Size>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static constexpr std::size_t hd_chunk() {
  return Size >= 32 ? 32 : Size >= 16 ? 16 : Size >= 8 ? 8 : Size >= 4 ? 4
    : Size;
}

template <std::size_t, std::size_t, template <typename...> typename, typename...>
struct FATAL_HIDE_SYMBOL th;

template <template <typename...> typename List, typename... Tail>
struct FATAL_HIDE_SYMBOL th<0, 0, List, Tail...> {
  using type = List<>;
};

template <template <typename...> typename List, typename T01, typename... Tail>
struct FATAL_HIDE_SYMBOL th<1, 1, List, T01, Tail...> {
  using type = List<T01>;
};

template <
  template <typename...> typename List,
  typename T01, typename T02,
  typename... Tail
>
struct FATAL_HIDE_SYMBOL th<2, 2, List, T01, T02, Tail...> {
  using type = List<T01, T02>;
};

template <
  template <typename...> typename List,
  typename T01, typename T02, typename T03,
  typename... Tail
>
struct FATAL_HIDE_SYMBOL th<3, 3, List, T01, T02, T03, Tail...> {
  using type = List<T01, T02, T03>;
};

template <
  std::size_t Size,
  template <typename...> typename List,
  typename T01, typename T02, typename T03, typename T04,
  typename... Tail
>
struct FATAL_HIDE_SYMBOL th<4, Size, List, T01, T02, T03, T04, Tail...> {
  using type = cat<
    List<T01, T02, T03, T04>,
    typename th<hd_chunk<Size - 4>(), Size - 4, List, Tail...>::type
  >;
};

template <
  std::size_t Size,
  template <typename...> typename List,
  typename T01, typename T02, typename T03, typename T04, typename T05,
  typename T06, typename T07, typename T08,
  typename... Tail
>
struct FATAL_HIDE_SYMBOL th<8, Size, List, T01, T02, T03, T04, T05, T06, T07, T08, Tail...> {
  using type = cat<
    List<T01, T02, T03, T04, T05, T06, T07, T08>,
    typename th<hd_chunk<Size - 8>(), Size - 8, List, Tail...>::type
  >;
};

template <
  std::size_t Size,
  template <typename...> typename List,
  typename T01, typename T02, typename T03, typename T04, typename T05,
  typename T06, typename T07, typename T08, typename T09, typename T10,
  typename T11, typename T12, typename T13, typename T14, typename T15,
  typename T16,
  typename... Tail
>
struct FATAL_HIDE_SYMBOL th<
  16, Size, List,
  T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15,
  T16,
  Tail...
> {
  using type = cat<
    List<
      T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15,
      T16
    >,
    typename th<hd_chunk<Size - 16>(), Size - 16, List, Tail...>::type
  >;
};

template <
  std::size_t Size,
  template <typename...> typename List,
  typename T01, typename T02, typename T03, typename T04, typename T05,
  typename T06, typename T07, typename T08, typename T09, typename T10,
  typename T11, typename T12, typename T13, typename T14, typename T15,
  typename T16, typename T17, typename T18, typename T19, typename T20,
  typename T21, typename T22, typename T23, typename T24, typename T25,
  typename T26, typename T27, typename T28, typename T29, typename T30,
  typename T31, typename T32,
  typename... Tail
>
struct FATAL_HIDE_SYMBOL th<
  32, Size, List,
  T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15,
  T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
  T31, T32,
  Tail...
> {
  using type = cat<
    List<
      T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
      T31, T32
    >,
    typename th<hd_chunk<Size - 32>(), Size - 32, List, Tail...>::type
  >;
};

template <std::size_t, std::size_t, typename, typename T, T...> struct FATAL_HIDE_SYMBOL vh;

template <template <typename V, V...> typename Variadics, typename T, T... Tail>
struct FATAL_HIDE_SYMBOL vh<0, 0, Variadics<int>, T, Tail...> {
  using type = Variadics<T>;
};

template <
  template <typename V, V...> typename Variadics, typename T, T V01, T... Tail
>
struct FATAL_HIDE_SYMBOL vh<1, 1, Variadics<int>, T, V01, Tail...> {
  using type = Variadics<T, V01>;
};

template <
  template <typename V, V...> typename Variadics,
  typename T,
  T V01, T V02,
  T... Tail
>
struct FATAL_HIDE_SYMBOL vh<2, 2, Variadics<int>, T, V01, V02, Tail...> {
  using type = Variadics<T, V01, V02>;
};

template <
  template <typename V, V...> typename Variadics,
  typename T,
  T V01, T V02, T V03,
  T... Tail
>
struct FATAL_HIDE_SYMBOL vh<3, 3, Variadics<int>, T, V01, V02, V03, Tail...> {
  using type = Variadics<T, V01, V02, V03>;
};

template <
  std::size_t Size,
  template <typename V, V...> typename Variadics,
  typename T,
  T V01, T V02, T V03, T V04,
  T... Tail
>
struct FATAL_HIDE_SYMBOL vh<4, Size, Variadics<int>, T, V01, V02, V03, V04, Tail...> {
  using type = cat<
    Variadics<T, V01, V02, V03, V04>,
    typename vh<
      hd_chunk<Size - 4>(), Size - 4, Variadics<int>, T, Tail...
    >::type
  >;
};

template <
  std::size_t Size,
  template <typename V, V...> typename Variadics,
  typename T,
  T V01, T V02, T V03, T V04, T V05,
  T V06, T V07, T V08,
  T... Tail
>
struct FATAL_HIDE_SYMBOL vh<
  8, Size, Variadics<int>, T, V01, V02, V03, V04, V05, V06, V07, V08, Tail...
> {
  using type = cat<
    Variadics<T, V01, V02, V03, V04, V05, V06, V07, V08>,
    typename vh<
      hd_chunk<Size - 8>(), Size - 8, Variadics<int>, T, Tail...
    >::type
  >;
};

template <
  std::size_t Size,
  template <typename V, V...> typename Variadics,
  typename T,
  T V01, T V02, T V03, T V04, T V05,
  T V06, T V07, T V08, T V09, T V10,
  T V11, T V12, T V13, T V14, T V15,
  T V16,
  T... Tail
>
struct FATAL_HIDE_SYMBOL vh<
  16, Size, Variadics<int>, T,
  V01, V02, V03, V04, V05, V06, V07, V08, V09, V10, V11, V12, V13, V14, V15,
  V16,
  Tail...
> {
  using type = cat<
    Variadics<
      T,
      V01, V02, V03, V04, V05, V06, V07, V08, V09, V10, V11, V12, V13, V14, V15,
      V16
    >,
    typename vh<
      hd_chunk<Size - 16>(), Size - 16, Variadics<int>, T, Tail...
    >::type
  >;
};

template <
  std::size_t Size,
  template <typename V, V...> typename Variadics,
  typename T,
  T V01, T V02, T V03, T V04, T V05,
  T V06, T V07, T V08, T V09, T V10,
  T V11, T V12, T V13, T V14, T V15,
  T V16, T V17, T V18, T V19, T V20,
  T V21, T V22, T V23, T V24, T V25,
  T V26, T V27, T V28, T V29, T V30,
  T V31, T V32,
  T... Tail
>
struct FATAL_HIDE_SYMBOL vh<
  32, Size, Variadics<int>, T,
  V01, V02, V03, V04, V05, V06, V07, V08, V09, V10, V11, V12, V13, V14, V15,
  V16, V17, V18, V19, V20, V21, V22, V23, V24, V25, V26, V27, V28, V29, V30,
  V31, V32,
  Tail...
> {
  using type = cat<
    Variadics<
      T,
      V01, V02, V03, V04, V05, V06, V07, V08, V09, V10, V11, V12, V13, V14, V15,
      V16, V17, V18, V19, V20, V21, V22, V23, V24, V25, V26, V27, V28, V29, V30,
      V31, V32
    >,
    typename vh<
      hd_chunk<Size - 32>(), Size - 32, Variadics<int>, T, Tail...
    >::type
  >;
};

template <std::size_t, typename> struct FATAL_HIDE_SYMBOL h;

template <
  template <typename...> typename List,
  typename... Args,
  std::size_t Offset
>
struct FATAL_HIDE_SYMBOL h<Offset, List<Args...>>:
  th<hd_chunk<Offset>(), Offset, List, Args...>
{
  static_assert(Offset <= sizeof...(Args), "index out of bounds");
};

template <
  template <typename V, V...> typename Sequence,
  typename T,
  T... Values,
  std::size_t Offset
>
struct FATAL_HIDE_SYMBOL h<Offset, Sequence<T, Values...>>:
  vh<hd_chunk<Offset>(), Offset, Sequence<int>, T, Values...>
{
  static_assert(Offset <= sizeof...(Values), "index out of bounds");
};

template <typename T, std::size_t Index>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static std::integral_constant<std::size_t, Index> I(indexed<T, Index>);

template <std::size_t, typename T, std::size_t Index>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static std::integral_constant<std::size_t, Index> TI(indexed<T, Index>);

template <std::size_t Size, typename>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static std::integral_constant<std::size_t, Size> TI(...);

template <typename T, T, std::size_t> struct FATAL_HIDE_SYMBOL vi {};

template <typename T, T Value, std::size_t Index>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static std::integral_constant<std::size_t, Index> IV(vi<T, Value, Index>);

template <std::size_t, typename T, T Value, std::size_t Index>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static std::integral_constant<std::size_t, Index> TIV(vi<T, Value, Index>);

template <std::size_t Size, typename T, T>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static std::integral_constant<std::size_t, Size> TIV(...);

template <typename...> struct FATAL_HIDE_SYMBOL i;

template <
  template <typename...> typename List,
  typename... Args,
  std::size_t... Indexes
>
struct FATAL_HIDE_SYMBOL i<List<Args...>, index_sequence<Indexes...>> {
  template <typename T>
  using apply = decltype(I<T>(inherit<indexed<Args, Indexes>...>()));
};

template <
  template <typename V, V...> typename Sequence,
  typename T,
  T... Values,
  std::size_t... Indexes
>
struct FATAL_HIDE_SYMBOL i<Sequence<T, Values...>, index_sequence<Indexes...>> {
  template <T Value>
  using apply = decltype(IV<T, Value>(inherit<vi<T, Values, Indexes>...>()));
};

template <typename...> struct FATAL_HIDE_SYMBOL ti;

template <
  template <typename...> typename List,
  typename... Args,
  std::size_t... Indexes
>
struct FATAL_HIDE_SYMBOL ti<List<Args...>, index_sequence<Indexes...>> {
  template <typename T>
  using apply = decltype(
    TI<sizeof...(Args), T>(inherit<indexed<Args, Indexes>...>())
  );
};

template <
  template <typename V, V...> typename Sequence,
  typename T,
  T... Values,
  std::size_t... Indexes
>
struct FATAL_HIDE_SYMBOL ti<Sequence<T, Values...>, index_sequence<Indexes...>> {
  template <T Value>
  using apply = decltype(
    TIV<sizeof...(Values), T, Value>(inherit<vi<T, Values, Indexes>...>())
  );
};

} // namespace i_at {
} // namespace ftl {

#endif // FATAL_INCLUDE_fatal_type_impl_slice_h
