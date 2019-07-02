/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_slice_h
#define FATAL_INCLUDE_fatal_type_slice_h

#include <fatal/type/size.h>
#include <fatal/type/tag.h>

#include <fatal/portability.h>

#include <cstdlib>

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

#include <fatal/type/impl/slice.h>

namespace fatal {

template <typename T, std::size_t Index>
using at = typename i_at::a<Index, T>::type;

template <typename T, std::size_t Index, typename Default = not_found>
using try_at = typename i_at::A<
  (Index < size<T>::value),
  Index, T, Default
>::type;

template <typename T>
using first = at<T, 0>;

struct FATAL_HIDE_SYMBOL get_first {
  template <typename T>
  using apply = first<T>;
};

template <typename T>
using second = at<T, 1>;

struct FATAL_HIDE_SYMBOL get_second {
  template <typename T>
  using apply = second<T>;
};

template <typename T>
using third = at<T, 2>;

struct FATAL_HIDE_SYMBOL get_third {
  template <typename T>
  using apply = third<T>;
};

template <typename T>
using last = at<T, size<T>::value - 1>;

struct FATAL_HIDE_SYMBOL get_last {
  template <typename T>
  using apply = last<T>;
};

template <typename T, std::size_t... Indexes>
using pick = typename i_at::p<T, Indexes...>::type;

template <typename T, std::size_t Offset>
using tail = typename i_at::t<T, make_index_sequence<Offset>>::type;

template <typename T, std::size_t Offset>
using head = typename i_at::h<Offset, T>::type;

template <typename T, std::size_t Begin, std::size_t End>
using slice = head<tail<T, Begin>, End - Begin>;

template <typename Container>
using index = i_at::i<
  Container,
  make_index_sequence<size<Container>::value>
>;

template <typename Container>
using try_index = i_at::ti<
  Container,
  make_index_sequence<size<Container>::value>
>;

template <typename T0, typename...>
using first_argument = T0;

struct FATAL_HIDE_SYMBOL get_first_argument {
  template <typename T0, typename...>
  using apply = T0;
};

template <typename T0, typename T1, typename...>
using second_argument = T1;

struct FATAL_HIDE_SYMBOL get_second_argument {
  template <typename T0, typename T1, typename...>
  using apply = T1;
};

template <typename T0, typename T1, typename T2, typename...>
using third_argument = T2;

struct FATAL_HIDE_SYMBOL get_third_argument {
  template <typename T0, typename T1, typename T2, typename...>
  using apply = T2;
};

// TODO: REDUCE TEMPLATE INSTANTIATIONS
template <typename T, typename Of>
using index_of = typename index<T>::template apply<Of>;

template <typename T, typename Of>
using try_index_of = typename try_index<T>::template apply<Of>;

namespace bound {

template <std::size_t Index>
struct FATAL_HIDE_SYMBOL at {
  template <typename T>
  using apply = typename i_at::a<Index, T>::type;
};

template <std::size_t Index, typename Default = not_found>
struct FATAL_HIDE_SYMBOL try_at {
  template <typename T>
  using apply = typename i_at::A<
    (Index < size<T>::value),
    Index, T, Default
  >::type;
};

template <std::size_t... Indexes>
struct FATAL_HIDE_SYMBOL pick {
  template <typename T>
  using apply = typename i_at::p<T, Indexes...>::type;
};

template <std::size_t Offset>
struct FATAL_HIDE_SYMBOL tail {
  template <typename T>
  using apply = typename i_at::t<T, make_index_sequence<Offset>>::type;
};

template <std::size_t Offset>
struct FATAL_HIDE_SYMBOL head {
  template <typename T>
  using apply = typename i_at::h<Offset, T>::type;
};

template <std::size_t Begin, std::size_t End>
struct FATAL_HIDE_SYMBOL slice {
  template <typename T>
  using apply = fatal::slice<T, Begin, End>;
};

} // namespace bound {
} // namespace fatal {

FATAL_DIAGNOSTIC_POP

#endif // FATAL_INCLUDE_fatal_type_slice_h
