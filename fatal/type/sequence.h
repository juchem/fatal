/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_sequence_h
#define FATAL_INCLUDE_fatal_type_sequence_h

#include <fatal/portability.h>
#include <fatal/preprocessor.h>

#include <utility>

#include <cstdlib>

#include <fatal/type/impl/sequence.h>

namespace fatal {

template <typename T, T... Values>
using sequence = std::integer_sequence<T, Values...>;

template <typename T, std::size_t Size>
using make_sequence = std::make_integer_sequence<T, Size>;

template <typename T, T Begin, T End, T Step = 1>
using make_interval = typename impl_seq::i<
  T, Begin, Step, make_sequence<T, End - Begin>
>::type;

template <std::size_t... Values>
using index_sequence = sequence<std::size_t, Values...>;

template <std::size_t Size>
using make_index_sequence = make_sequence<std::size_t, Size>;

template <std::size_t Begin, std::size_t End, std::size_t Step = 1>
using make_index_interval = make_interval<std::size_t, Begin, End, Step>;

template <bool... Values>
using bool_sequence = sequence<bool, Values...>;

template <char... Values>
using char_sequence = sequence<char, Values...>;

template <int... Values>
using int_sequence = sequence<int, Values...>;

#define FATAL_S(Id, String) \
  FATAL_IMPL_BUILD_STRING( \
    ::std::integer_sequence, \
    Id, \
    FATAL_UID(FATAL_CAT(fatal_str_, Id)), \
    FATAL_UID(Indexes), \
    String \
  )

} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_type_sequence_h
