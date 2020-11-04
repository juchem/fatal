/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_impl_foreach_h
#define FATAL_INCLUDE_fatal_type_impl_foreach_h

#include <fatal/portability.h>
#include <fatal/type/list.h>
#include <fatal/type/tag.h>

namespace ftl {
namespace i_fe {

// foreach

template <typename, typename>
struct FATAL_HIDE_SYMBOL f {
  template <typename... Args>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static void g(Args &&...) {}
};

template <typename... T, std::size_t... Indexes>
struct FATAL_HIDE_SYMBOL f<list<T...>, index_sequence<Indexes...>> {
  static_assert(sizeof...(T) == sizeof...(Indexes), "size mismatch");

  template <typename Visitor, typename... Args>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static void g(Visitor &&visitor, Args &&...args) {
    bool _[sizeof...(T)] = {
      (visitor(indexed<T, Indexes>(), args...), false)...
    };
    (void)_;
  }
};

// foreach_accumulate

template <std::size_t Index, typename...> struct FATAL_HIDE_SYMBOL A {};

template <std::size_t Index, typename T, typename... Args>
struct FATAL_HIDE_SYMBOL A<Index, T, Args...> {
  template <typename Visitor, typename Seed, typename... UArgs>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static constexpr decltype(auto) g(Visitor &&visitor, Seed &&seed, UArgs &&... args) {
    return A<Index + 1, Args...>::g(
      visitor,
      visitor(indexed<T, Index>(), std::forward<Seed>(seed), args...),
      args...
    );
  }
};

template <std::size_t Index>
struct FATAL_HIDE_SYMBOL A<Index> {
  template <typename Visitor, typename Seed, typename... UArgs>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static constexpr decltype(auto) g(Visitor &&, Seed &&seed, UArgs &&...) {
    return std::forward<Seed>(seed);
  }
};

template <typename> struct FATAL_HIDE_SYMBOL a {};

template <template <typename...> typename List, typename... Args>
struct FATAL_HIDE_SYMBOL a<List<Args...>> {
  template <typename Visitor, typename Seed, typename... UArgs>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static constexpr decltype(auto) g(Visitor &&visitor, Seed &&seed, UArgs &&... args) {
    return A<0, Args...>::g(
      std::forward<Visitor>(visitor),
      std::forward<Seed>(seed),
      std::forward<UArgs>(args)...
    );
  }
};

} // namespace i_fe {
} // namespace ftl {

#endif // FATAL_INCLUDE_fatal_type_impl_foreach_h
