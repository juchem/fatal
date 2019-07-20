/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_impl_trie_h
#define FATAL_INCLUDE_fatal_type_impl_trie_h

#include <fatal/type/array.h>
#include <fatal/type/conditional.h>
#include <fatal/type/longest_common_prefix.h>
#include <fatal/type/search.h>
#include <fatal/type/select.h>
#include <fatal/type/group_by.h>
#include <fatal/type/size.h>
#include <fatal/type/tag.h>

#include <algorithm>
#include <iterator>
#include <type_traits>

#include <cassert>

namespace fatal {
namespace i_t {

// represents a node in the trie //
template <
  typename Haystack,
  bool IsTerminal,
  // the range of the haystack string this node represents //
  std::size_t Begin, std::size_t End,
  typename... Children
>
struct FATAL_HIDE_SYMBOL n {
  using haystack = Haystack;
};

// bound node - to expose it as a transform //
template <typename T, bool IsTerminal, std::size_t Begin, std::size_t End>
struct FATAL_HIDE_SYMBOL N {
  template <typename... Args>
  using apply = n<T, IsTerminal, Begin, End, Args...>;
};

// filtered at
template <std::size_t Index, typename Filter>
struct FATAL_HIDE_SYMBOL a {
  template <typename T>
  using apply = at<typename Filter::template apply<T>, Index>;
};

// node filter for sorted search (lookup) //
template <std::size_t Index, typename Filter>
struct FATAL_HIDE_SYMBOL F {
  template <typename T>
  using apply = at<
    typename Filter::template apply<typename T::haystack>,
    Index
  >;
};

FATAL_HIDE_SYMBOL
enum class M {
  e, // exact
  l // longest
};

// trie lookup implementation //
template <M Mode, std::size_t, typename...> struct FATAL_HIDE_SYMBOL l;

// empty subtrie //
template <M Mode, typename Filter>
struct FATAL_HIDE_SYMBOL l<Mode, 0, Filter> {
  template <typename... Args>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static constexpr bool f(Args &&...) { return false; }
};

// subtrie with single node //
template <
  M Mode,
  std::size_t Offset,
  typename Filter,
  typename Haystack,
  bool IsTerminal,
  std::size_t Begin,
  std::size_t End,
  typename... Children
>
struct FATAL_HIDE_SYMBOL l<Mode, Offset, Filter, n<Haystack, IsTerminal, Begin, End, Children...>> {
  static_assert(Offset + Begin <= End, "internal error");

  template <typename NeedleBegin, typename Visitor, typename... VArgs>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static constexpr bool f(
    std::size_t const size,
    NeedleBegin &&begin,
    Visitor &&visitor,
    VArgs &&...args
  ) {
    using haystack_data = typename Filter::template apply<Haystack>;
    using value_type = typename std::iterator_traits<std::decay_t<NeedleBegin>>::value_type;

    static_assert(Mode == M::e || Mode == M::l, "unsupported mode");

    if constexpr (Mode == M::e) {
      if (size < End - Begin - Offset) { return false; }
      if (!IsTerminal && size == End - Begin - Offset) { return false; }
      if (!std::equal(
        begin,
        std::next(begin, End - Begin - Offset),
        std::next(z_data<haystack_data, value_type>(), Offset + Begin)
      )) {
        return false;
      }

      return (
        IsTerminal && size == End - Begin - Offset
          && (visitor(tag<Haystack>(), std::forward<VArgs>(args)...), true)
      )
      || l<Mode, 0, Filter, Children...>::f(
        size - (End - Begin - Offset),
        std::next(begin, End - Begin - Offset),
        std::forward<Visitor>(visitor),
        std::forward<VArgs>(args)...
      );
    } else if constexpr (Mode == M::l) {
      if (size < End - Begin - Offset) { return false; }

      if (!std::equal(
        begin,
        std::next(begin, End - Begin - Offset),
        std::next(z_data<haystack_data, value_type>(), Offset + Begin)
      )) {
        return false;
      }

      if (l<Mode, 0, Filter, Children...>::f(
        size - (End - Begin - Offset),
        std::next(begin, End - Begin - Offset),
        std::forward<Visitor>(visitor),
        std::forward<VArgs>(args)...
      )) {
        return true;
      }

      return IsTerminal && (visitor(tag<Haystack>(), std::forward<VArgs>(args)...), true);

    }
  }
};

// siblings //
template <
  M Mode,
  typename Filter,
  typename Haystack,
  bool IsTerminal,
  std::size_t Begin,
  std::size_t End,
  typename... Children,
  typename Node,
  typename... Siblings
>
struct FATAL_HIDE_SYMBOL l<
  Mode,
  0,
  Filter,
  n<Haystack, IsTerminal, Begin, End, Children...>,
  Node,
  Siblings...
> {
  template <typename NeedleBegin, typename Visitor, typename... VArgs>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static constexpr bool f(
    std::size_t const size,
    NeedleBegin &&begin,
    Visitor &&visitor,
    VArgs &&...args
  ) {
    return size && sorted_find<
      list<n<Haystack, IsTerminal, Begin, End, Children...>, Node, Siblings...>,
      F<Begin, Filter>
    >(
      *begin,
      false,
      l(),
      size,
      std::forward<NeedleBegin>(begin),
      std::forward<Visitor>(visitor),
      std::forward<VArgs>(args)...
    );
  }

  template <
    typename Match,
    std::size_t Index,
    typename NeedleBegin,
    typename Visitor,
    typename... VArgs
  >
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  constexpr bool operator ()(
    indexed<Match, Index>,
    std::size_t const size,
    NeedleBegin &&begin,
    Visitor &&visitor,
    VArgs &&...args
  ) const {
    return l<Mode, 1, Filter, Match>::f(
      size - 1,
      std::next(begin),
      std::forward<Visitor>(visitor),
      std::forward<VArgs>(args)...
    );
  }
};

// helper to expose the trie lookup implementation as a transform //
template <M Mode, typename Filter>
struct FATAL_HIDE_SYMBOL L {
  template <typename... T>
  using apply = l<Mode, 0, Filter, T...>;
};

// trie build recursion //
template <std::size_t, typename...> struct FATAL_HIDE_SYMBOL r;

// helper to expose the trie build recursion as a transform //
template <std::size_t Depth, typename Filter>
struct FATAL_HIDE_SYMBOL R {
  template <typename... Args>
  using apply = typename r<Depth, Filter, Args...>::type;
};

// leaf //
template <std::size_t Depth, typename Filter, typename T>
struct FATAL_HIDE_SYMBOL r<Depth, Filter, T> {
  using type = n<
    T, true, Depth, size<typename Filter::template apply<T>>::value
  >;
};

// internal node //
template <std::size_t Depth, typename Filter, typename T, typename... Args>
struct FATAL_HIDE_SYMBOL r<Depth, Filter, T, Args...> {
  using common = longest_common_prefix<
    at,
    Depth,
    vmin<
      less,
      size<typename Filter::template apply<T>>,
      size<typename Filter::template apply<Args>>...
    >::value,
    typename Filter::template apply<T>, typename Filter::template apply<Args>...
  >;

  using type = group_by<
    conditional<
      size<typename Filter::template apply<T>>::value == common::value,
      list<Args...>,
      list<T, Args...>
    >,
    a<common::value, Filter>,
    R<common::value, Filter>::template apply,
    N<
      T,
      common::value == size<typename Filter::template apply<T>>::value,
      Depth,
      common::value
    >::template apply
  >;
};

// trie build entry point helper //
template <M Mode, bool, std::size_t, typename...> struct FATAL_HIDE_SYMBOL h;

// no common prefix and no empty string //
template <M Mode, std::size_t Common, typename Filter, typename T, typename... Args>
struct FATAL_HIDE_SYMBOL h<Mode, true, Common, Filter, T, Args...> {
  using type = group_by<
    list<T, Args...>,
    a<0, Filter>,
    R<0, Filter>::template apply,
    L<Mode, Filter>::template apply
  >;
};

// common prefix or empty string //
template <M Mode, std::size_t Common, typename Filter, typename T, typename... Args>
struct FATAL_HIDE_SYMBOL h<Mode, false, Common, Filter, T, Args...> {
  using type = l<
    Mode,
    0,
    Filter,
    group_by<
      conditional<
        size<typename Filter::template apply<T>>::value == Common,
        list<Args...>,
        list<T, Args...>
      >,
      a<Common, Filter>,
      R<Common, Filter>::template apply,
      N<
        T, Common == size<typename Filter::template apply<T>>::value, 0, Common
      >::template apply
    >
  >;
};

// trie build entry point //
template <M Mode, typename...> struct FATAL_HIDE_SYMBOL e;

// non-empty input - find longest common prefix //
template <
  M Mode,
  typename Filter,
  template <typename...> typename Variadic,
  typename T, typename... Args
>
struct FATAL_HIDE_SYMBOL e<Mode, Filter, Variadic<T, Args...>>:
  h<
    Mode,
    (size<typename Filter::template apply<T>>::value > 1)
      && !longest_common_prefix<
        at,
        0,
        vmin<less, size<typename Filter::template apply<Args>>...>::value,
        typename Filter::template apply<Args>...
      >::value,
    longest_common_prefix<
      at,
      0,
      vmin<
        less,
        size<typename Filter::template apply<T>>,
        size<typename Filter::template apply<Args>>...
      >::value,
      typename Filter::template apply<T>,
      typename Filter::template apply<Args>...
    >::value,
    Filter,
    T, Args...
  >
{};

// unitary input //
template <M Mode, typename Filter, template <typename...> typename Variadic, typename T>
struct FATAL_HIDE_SYMBOL e<Mode, Filter, Variadic<T>> {
  using type = l<
    Mode,
    0,
    Filter,
    n<T, true, 0, size<typename Filter::template apply<T>>::value>
  >;
};

// empty input //
template <M Mode, typename Filter, template <typename...> typename Variadic>
struct FATAL_HIDE_SYMBOL e<Mode, Filter, Variadic<>> {
  using type = l<Mode, 0, Filter>;
};

} // namespace i_t {
} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_type_impl_trie_h
