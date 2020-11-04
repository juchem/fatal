/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_impl_type_h
#define FATAL_INCLUDE_fatal_type_impl_type_h

#include <fatal/type/tag.h>

#include <fatal/portability.h>

#include <type_traits>

namespace ftl {
namespace impl_t {

template <typename T, typename = typename T::type>
FATAL_HIDE_SYMBOL
std::true_type s(T *);

FATAL_HIDE_SYMBOL
std::false_type s(...);

template <typename T, bool = decltype(s(static_cast<T *>(nullptr)))::value>
struct FATAL_HIDE_SYMBOL to;

template <typename T>
struct FATAL_HIDE_SYMBOL to<T, true> {
  using type = typename T::type;
};

template <template <typename V, V...> typename Variadics, typename T, T... Values>
struct FATAL_HIDE_SYMBOL to<Variadics<T, Values...>, false> {
  using type = T;
};

template <typename T, typename = typename T::value_type>
FATAL_HIDE_SYMBOL
std::true_type S(T *);
FATAL_HIDE_SYMBOL
std::false_type S(...);

template <typename T, bool = decltype(S(static_cast<T *>(nullptr)))::value>
struct FATAL_HIDE_SYMBOL vto;

template <typename T>
struct FATAL_HIDE_SYMBOL vto<T, true> {
  using type = typename T::value_type;
};

template <template <typename V, V...> typename Variadics, typename T, T... Values>
struct FATAL_HIDE_SYMBOL vto<Variadics<T, Values...>, false> {
  using type = T;
};

} // namespace impl_t {
} // namespace ftl {

#endif // FATAL_INCLUDE_fatal_type_impl_type_h
