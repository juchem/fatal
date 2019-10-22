/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_array_h
#define FATAL_INCLUDE_fatal_type_array_h

#include <fatal/type/identity.h>
#include <fatal/type/sequence.h>
#include <fatal/type/size.h>

#include <fatal/portability.h>

#include <string_view>

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

#include <fatal/type/impl/array.h>

namespace fatal {

template <
  typename Array,
  typename OuterFilter,
  typename InnerFilter = get_identity,
  typename... T
>
using as_array_filtered = i_a::CF<
  make_index_sequence<size<typename OuterFilter::template apply<Array>>::value>,
  OuterFilter, InnerFilter, Array, T...
>;

template <typename Array, typename... T>
using as_array = i_a::C<Array, T...>;

template <typename Array, typename... T>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static constexpr auto z_data() {
  return i_a::z<Array, T...>::data;
}

template <typename Array, typename... T>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static constexpr auto z_view() {
  return std::basic_string_view<std::decay_t<decltype(*z_data<Array, T...>())>>(
    z_data<Array, T...>(),
    size_v<Array> + sizeof...(T)
  );
}

template <typename List, typename Factory, typename... T>
using as_array_from = i_a::A<i_a::c, List, Factory, T...>;

template <typename Array, typename Factory, typename... T>
using as_runtime_array_from = i_a::A<i_a::n, Array, Factory, T...>;

template <typename Array, typename Filter, typename... T>
using z_array_filtered = i_a::ZAF<Array, Filter, T...>;

template <typename Array, typename... T>
using z_array = i_a::ZA<Array, T...>;

template <
  typename Array,
  typename StringView,
  typename OuterFilter,
  typename InnerFilter = get_identity
>
using string_view_array_filtered = i_a::SF<
  make_index_sequence<size<typename OuterFilter::template apply<Array>>::value>,
  Array, OuterFilter, InnerFilter, StringView
>;

template <typename Array, typename StringView>
using string_view_array = i_a::S<Array, StringView>;

} // namespace fatal {

FATAL_DIAGNOSTIC_POP

#endif // FATAL_INCLUDE_fatal_type_array_h
