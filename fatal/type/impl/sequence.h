/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_impl_sequence_h
#define FATAL_INCLUDE_fatal_type_impl_sequence_h

namespace ftl {
namespace impl_seq {

template <typename...> struct FATAL_HIDE_SYMBOL cat;

template <
  template <typename T, T...> class Sequence,
  typename T, T... V0, T... V1, T... Tail
>
struct FATAL_HIDE_SYMBOL cat<Sequence<T, V0...>, Sequence<T, V1...>, Sequence<T, Tail...>> {
  using type = Sequence<
    T,
    V0...,
    (sizeof...(V0) + V1)...,
    (sizeof...(V0) + sizeof...(V1) + Tail)...
  >;
};

template <std::size_t Size>
struct FATAL_HIDE_SYMBOL make {
  template <typename S0, typename S1>
  using apply = typename cat<
    typename make<Size / 2>::template apply<S0, S1>,
    typename make<Size / 2>::template apply<S0, S1>,
    typename make<Size % 2>::template apply<S0, S1>
  >::type;
};

template <> struct FATAL_HIDE_SYMBOL make<1> {
  template <typename S0, typename S1>
  using apply = S1;
};
template <> struct FATAL_HIDE_SYMBOL make<0> {
  template <typename S0, typename S1>
  using apply = S0;
};

template <
  template <typename T, T...> class Sequence, typename T, std::size_t Size
>
using make_sequence = typename make<Size>::template apply<
  Sequence<T>, Sequence<T, 0>
>;

template <typename T, T, T, typename> struct FATAL_HIDE_SYMBOL i;

template <
  template <typename T, T...> class Sequence,
  typename T, T Offset, T Multiplier, T... Values
>
struct FATAL_HIDE_SYMBOL i<T, Offset, Multiplier, Sequence<T, Values...>> {
  using type = Sequence<T, (Offset + (Values * Multiplier))...>;
};

template <typename T, std::size_t Size>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static constexpr std::size_t size(T const (&)[Size]) {
  static_assert(
    Size > 0,
    "expecting a string containing at least the null terminator"
  );
  return Size - 1;
}

#define FATAL_IMPL_BUILD_STRING(Sequence, Id, Helper, Indexes, ...) \
  template <::std::size_t... Indexes> \
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL \
  static Sequence< \
    typename ::std::decay<decltype(*(__VA_ARGS__))>::type, \
    (__VA_ARGS__)[Indexes]... \
  > Helper(Sequence<::std::size_t, Indexes...>); \
  \
  using Id = decltype(Helper( \
    typename ::ftl::impl_seq::make_sequence< \
      Sequence, ::std::size_t, ::ftl::impl_seq::size(__VA_ARGS__) \
    >() \
  ))

} // namespace impl_seq {
} // namespace ftl {

#endif // FATAL_INCLUDE_fatal_type_impl_sequence_h
