/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_convert_h
#define FATAL_INCLUDE_fatal_type_convert_h

#include <fatal/type/identity.h>
#include <fatal/type/list.h>
#include <fatal/type/pair.h>
#include <fatal/type/sequence.h>

#include <fatal/portability.h>

#include <type_traits>
#include <utility>

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

#include <fatal/type/impl/convert.h>

namespace fatal {

template <
  typename From,
  template <typename V, V...> typename Sequence = sequence,
  typename... T
>
using as_sequence = typename impl_cv::s<Sequence<int>, From, T...>::type;

template <typename T, template <typename...> typename List = list>
using as_list = typename impl_cv::l<List, T>::type;

template <typename To, typename T, typename... Args>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static constexpr To to_instance(Args &&...args) {
  return impl_cv::t<To, T>::to(std::forward<Args>(args)...);
}

namespace bound {

template <template <typename V, V...> typename Sequence, typename... T>
struct FATAL_HIDE_SYMBOL as_sequence {
  template <typename From>
  using apply = typename impl_cv::s<Sequence<int>, From, T...>::type;
};

template <template <typename...> typename List>
struct FATAL_HIDE_SYMBOL as_list {
  template <typename T>
  using apply = fatal::as_list<T, List>;
};

} // namespace bound {
} // namespace fatal {

FATAL_DIAGNOSTIC_POP

#endif // FATAL_INCLUDE_fatal_type_convert_h
