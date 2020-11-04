/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_tag_h
#define FATAL_INCLUDE_fatal_type_tag_h

#include <fatal/type/pair.h>

#include <fatal/portability.h>

#include <cstddef>

namespace ftl {

template <typename T> struct FATAL_HIDE_SYMBOL tag {
  using type = T;
};

template <typename First, typename Second>
using tag_pair = tag<pair<First, Second>>;

template <typename T, std::size_t Index>
struct FATAL_HIDE_SYMBOL indexed: tag<T> {
  FATAL_HIDE_SYMBOL
  static constexpr std::size_t value = Index;
};

template <typename T, std::size_t Index>
constexpr std::size_t indexed<T, Index>::value;

template <typename First, typename Second, std::size_t Index>
using indexed_pair = indexed<pair<First, Second>, Index>;

template <typename T>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
T tag_type(tag<T> const &);

template <typename T, std::size_t Index>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
constexpr std::size_t tag_index(indexed<T, Index>) { return Index; }

template <typename First, typename Second>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
First tag_first(tag_pair<First, Second> const &);

template <typename First, typename Second>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
Second tag_second(tag_pair<First, Second> const &);

struct FATAL_HIDE_SYMBOL not_found {};

} // namespace ftl {

#endif // FATAL_INCLUDE_fatal_type_tag_h
