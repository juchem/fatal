/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_list_h
#define FATAL_INCLUDE_fatal_type_list_h

#include <fatal/portability.h>

#include <type_traits>

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

namespace ftl {

template <typename...> struct FATAL_HIDE_SYMBOL list {};

template <typename T, T... Values>
using value_list = list<std::integral_constant<T, Values>...>;

template <std::size_t... Values>
using index_list = list<std::integral_constant<std::size_t, Values>...>;

template <bool... Values>
using bool_list = list<std::integral_constant<bool, Values>...>;

template <char... Values>
using char_list = list<std::integral_constant<char, Values>...>;

template <int... Values>
using int_list = list<std::integral_constant<int, Values>...>;

template <typename... Args>
FATAL_HIDE_SYMBOL
constexpr list<Args...> list_of(Args &&...) { return {}; }

} // namespace ftl {

FATAL_DIAGNOSTIC_POP

#endif // FATAL_INCLUDE_fatal_type_list_h
