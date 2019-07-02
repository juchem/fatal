/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_trie_h
#define FATAL_INCLUDE_fatal_type_trie_h

#include <fatal/functional/no_op.h>
#include <fatal/type/identity.h>
#include <fatal/type/sort.h>

#include <fatal/portability.h>

#include <utility>

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

#include <fatal/type/impl/trie.h>

namespace fatal {

// TODO: INVERT COMPARER AND FILTER
template <
  typename T,
  typename Filter = get_identity,
  typename Comparer = less,
  typename Begin,
  typename End,
  typename Visitor,
  typename... VArgs
>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static bool trie_find(
  Begin &&begin,
  End &&end,
  Visitor &&visitor,
  VArgs &&...args
) {
  assert(begin <= end);
  return i_t::e<Filter, sort<T, sequence_compare<Comparer>, Filter>>::type::f(
    static_cast<std::size_t>(std::distance(begin, end)),
    std::forward<Begin>(begin),
    std::forward<Visitor>(visitor),
    std::forward<VArgs>(args)...
  );
}

template <
  typename T,
  typename Filter = get_identity,
  typename Comparer = less,
  typename Begin,
  typename End
>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static bool trie_find(Begin &&begin, End &&end) {
  return trie_find<T, Filter, Comparer>(
    std::forward<Begin>(begin),
    std::forward<End>(end),
    fn::no_op()
  );
}

} // namespace fatal {

FATAL_DIAGNOSTIC_POP

#endif // FATAL_INCLUDE_fatal_type_trie_h
