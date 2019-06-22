/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_impl_size_h
#define FATAL_INCLUDE_fatal_type_impl_size_h

namespace fatal {
namespace i_sz {

// size //
template <typename> struct FATAL_HIDE_SYMBOL s;

template <template <typename...> typename T, typename... Args>
struct FATAL_HIDE_SYMBOL s<T<Args...>> {
  using type = std::integral_constant<std::size_t, sizeof...(Args)>;
};

template <typename T, template <typename V, V...> typename Sequence, T... Values>
struct FATAL_HIDE_SYMBOL s<Sequence<T, Values...>> {
  using type = std::integral_constant<std::size_t, sizeof...(Values)>;
};

} // namespace i_sz {
} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_type_impl_size_h
