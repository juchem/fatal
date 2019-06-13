/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_time_time_h
#define FATAL_INCLUDE_fatal_time_time_h

#include <fatal/type/array.h>
#include <fatal/type/get.h>
#include <fatal/type/list.h>
#include <fatal/type/slice.h>

#include <chrono>
#include <ratio>
#include <string_view>

namespace fatal::time {

/**
 * TODO: DOCUMENT
 *
 * @author: Marcelo Juchem <juchem at gmail dot com>
 */

using suffixes = list<
  //list<std::yocto, std::integer_sequence<char, 'y', 's'>>,
  //list<std::zepto, std::integer_sequence<char, 'z', 's'>>,
  list<std::atto, std::integer_sequence<char, 'a', 's'>>,
  list<std::femto, std::integer_sequence<char, 'f', 's'>>,
  list<std::pico, std::integer_sequence<char, 'p', 's'>>,
  list<std::nano, std::integer_sequence<char, 'n', 's'>>,
  list<std::micro, std::integer_sequence<char, 'u', 's'>>,
  list<std::milli, std::integer_sequence<char, 'm', 's'>>,
  list<std::centi, std::integer_sequence<char, 'c', 's'>>,
  list<std::deci, std::integer_sequence<char, 'd', 's'>>,
  list<std::chrono::seconds::period, std::integer_sequence<char, 's'>>,
  list<std::deca, std::integer_sequence<char, 'd', 'a', 's'>>,
  list<std::chrono::minutes::period, std::integer_sequence<char, 'm', 'i', 'n'>>,
  list<std::hecto, std::integer_sequence<char, 'h', 's'>>,
  list<std::kilo, std::integer_sequence<char, 'k', 's'>>,
  list<std::chrono::hours::period, std::integer_sequence<char, 'h'>>,
  list<std::ratio_multiply<std::chrono::hours::period, std::ratio<24, 1>>, std::integer_sequence<char, 'd'>>,
  list<std::ratio_multiply<std::chrono::hours::period, std::ratio<24 * 7, 1>>, std::integer_sequence<char, 'w', 'k'>>,
  list<std::mega, std::integer_sequence<char, 'M', 's'>>,
  list<std::giga, std::integer_sequence<char, 'G', 's'>>,
  list<std::tera, std::integer_sequence<char, 'T', 's'>>,
  list<std::peta, std::integer_sequence<char, 'P', 's'>>,
  list<std::exa, std::integer_sequence<char, 'E', 's'>>
  //list<std::zetta, std::integer_sequence<char, 'Z', 's'>>,
  //list<std::yotta, std::integer_sequence<char, 'Y', 's'>>
>;

template <typename Period>
using suffix_t = second<get<suffixes, Period>>;

template <typename T>
inline std::string_view suffix_of() {
  using suffix = suffix_t<suffix_t<typename T::period>>;
  return {z_data<suffix>(), size<suffix>::value};
}

template <typename T>
inline std::string_view suffix_of(T const &) {
  return suffix_of<T>();
}

} // namespace fatal::time {

#endif // FATAL_INCLUDE_fatal_time_time_h
