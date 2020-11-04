/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_search_h
#define FATAL_INCLUDE_fatal_type_search_h

#include <fatal/functional/no_op.h>
#include <fatal/type/compare.h>
#include <fatal/type/identity.h>
#include <fatal/type/slice.h>
#include <fatal/type/sort.h>

#include <fatal/portability.h>

#include <utility>

#include <cassert>

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

#include <fatal/type/impl/search.h>

namespace ftl {

// TODO: ADD EXAMPLES TO THE DOCUMENTATION

/**
 * Given a sorted list `L<V0, V1, ..., Vn>`, looks for the element that matches the needle.
 *
 * If found, calls the visitor for it and returns `true`. Otherwise, the visitor is never
 * called and `false` is returned.
 *
 * `sorted_search` requires the input list to be pre-sorted. For a convenient alternative that
 * accepts an unsorted list and sorts it using the given comparer, see `unsorted_search`.
 *
 * The match between list element and needle is given by first applying the filter to the list
 * element, then applying the comparer to both the filtered element and the needle, as if
 * evaluating the expressions:
 *
 *  Comparer::equal<Filter::apply<ListElement>>(needle)
 *  Comparer::less<Filter::apply<ListElement>>(needle)
 *  Comparer::greater<Filter::apply<ListElement>>(needle)
 *
 * Visitor signature:
 *
 *  T visitor(indexed<Vn, N>, Args...)
 *
 * Notes:
 * - the result of the visitor is ignored.
 *
 * Author: Marcelo Juchem <juchem at gmail dot com>
 */
template <
  typename SortedInputList,
  typename Filter = get_identity,
  typename Comparer = value_comparer,
  typename Needle,
  typename Visitor,
  typename... Args
>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static constexpr bool sorted_search(Needle &&needle, Visitor &&visitor, Args &&...args) {
  return i_S::s<SortedInputList>::template S<Comparer, Filter>(
    std::forward<Needle>(needle),
    std::forward<Visitor>(visitor),
    std::forward<Args>(args)...
  );
}

/**
 * Given a sorted list `L<V0, V1, ..., Vn>`, looks for the element that matches the needle.
 *
 * If found, returns `true`. Otherwise, `false` is returned.
 *
 * `sorted_search` requires the input list to be pre-sorted. For a convenient alternative that
 * accepts an unsorted list and sorts it using the given comparer, see `unsorted_search`.
 *
 * The match between list element and needle is given by first applying the filter to the list
 * element, then applying the comparer to both the filtered element and the needle, as if
 * evaluating the expressions:
 *
 *  Comparer::equal<Filter::apply<ListElement>>(needle)
 *  Comparer::less<Filter::apply<ListElement>>(needle)
 *  Comparer::greater<Filter::apply<ListElement>>(needle)
 *
 * Author: Marcelo Juchem <juchem at gmail dot com>
 */
template <
  typename SortedInputList,
  typename Filter = get_identity,
  typename Comparer = value_comparer,
  typename Needle
>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static constexpr bool sorted_search(Needle &&needle) {
  return sorted_search<SortedInputList, Filter, Comparer>(
    std::forward<Needle>(needle),
    fn::no_op()
  );
}

/**
 * Given a sorted list `L<V0, V1, ..., Vn>`, looks for the element that matches the needle.
 *
 * If found, calls the visitor for it and returns the result of that call. Otherwise, the
 * visitor is never called and `fallback` is returned.
 *
 * `sorted_find` requires the input list to be pre-sorted. For a convenient alternative that
 * accepts an unsorted list and sorts it according to the given comparer, see `unsorted_find`.
 *
 * The match between list element and needle is given by first applying the filter to the list
 * element, then applying the comparer to both the filtered element and the needle, as if
 * evaluating the expressions:
 *
 *  Comparer::equal<Filter::apply<ListElement>>(needle)
 *  Comparer::less<Filter::apply<ListElement>>(needle)
 *  Comparer::greater<Filter::apply<ListElement>>(needle)
 *
 * Visitor signature:
 *
 *  T visitor(indexed<Vn, N>, Args...)
 *
 * Notes:
 * - `T` must have a common type with `Fallback` as returned by `std::common_type_t<T, Fallback>`
 * - the return type of `sorted_find` is the common type described above
 *
 * Author: Marcelo Juchem <juchem at gmail dot com>
 */
template <
  typename SortedInputList,
  typename Filter = get_identity,
  typename Comparer = value_comparer,
  typename Needle,
  typename Fallback,
  typename Visitor,
  typename... Args
>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static constexpr decltype(auto) sorted_find(
  Needle &&needle,
  Fallback &&fallback,
  Visitor &&visitor,
  Args &&...args
) {
  return i_S::f<SortedInputList>::template S<Comparer, Filter>(
    std::forward<Needle>(needle),
    std::forward<Fallback>(fallback),
    std::forward<Visitor>(visitor),
    std::forward<Args>(args)...
  );
}

/**
 * Given a list `L<V0, V1, ..., Vn>`, looks for the element that matches the needle.
 *
 * If found, calls the visitor for it and returns `true`. Otherwise, the visitor is never
 * called and `false` is returned.
 *
 * `unsorted_search` always sorts the input list using to the given comparer. If the
 * list is pre-sorted, use `sorted_search` instead to save on compile time.
 *
 * For the semantics of how the list is sorted refer to `sort` from `fatal/sort.h`.
 *
 * The match between list element and needle is given by first applying the filter to the list
 * element, then applying the comparer to both the filtered element and the needle, as if
 * evaluating the expressions:
 *
 *  Comparer::equal<Filter::apply<ListElement>>(needle)
 *  Comparer::less<Filter::apply<ListElement>>(needle)
 *  Comparer::greater<Filter::apply<ListElement>>(needle)
 *
 * Visitor signature:
 *
 *  T visitor(indexed<Vn, N>, Args...)
 *
 * Notes:
 * - the result of the visitor is ignored.
 *
 * Author: Marcelo Juchem <juchem at gmail dot com>
 */
template <
  typename UnsortedInputList,
  typename Filter = get_identity,
  typename Comparer = value_comparer,
  typename Needle,
  typename Visitor,
  typename... Args
>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static constexpr bool unsorted_search(Needle &&needle, Visitor &&visitor, Args &&...args) {
  return sorted_search<sort<UnsortedInputList, Comparer, Filter>, Filter, Comparer>(
    std::forward<Needle>(needle),
    std::forward<Visitor>(visitor),
    std::forward<Args>(args)...
  );
}

/**
 * Given a list `L<V0, V1, ..., Vn>`, looks for the element that matches the needle.
 *
 * If found, returns `true`. Otherwise, `false` is returned.
 *
 * `unsorted_search` always sorts the input list using to the given comparer. If the
 * list is pre-sorted, use `sorted_search` instead to save on compile time.
 *
 * For the semantics of how the list is sorted refer to `sort` from `fatal/sort.h`.
 *
 * The match between list element and needle is given by first applying the filter to the list
 * element, then applying the comparer to both the filtered element and the needle, as if
 * evaluating the expressions:
 *
 *  Comparer::equal<Filter::apply<ListElement>>(needle)
 *  Comparer::less<Filter::apply<ListElement>>(needle)
 *  Comparer::greater<Filter::apply<ListElement>>(needle)
 *
 * Author: Marcelo Juchem <juchem at gmail dot com>
 */
template <
  typename UnsortedInputList,
  typename Filter = get_identity,
  typename Comparer = value_comparer,
  typename Needle
>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static constexpr bool unsorted_search(Needle &&needle) {
  return unsorted_search<UnsortedInputList, Filter, Comparer>(
    std::forward<Needle>(needle),
    fn::no_op()
  );
}

/**
 * Given a list `L<V0, V1, ..., Vn>`, looks for the element that matches the needle.
 *
 * If found, calls the visitor for it and returns the result of that call. Otherwise, the
 * visitor is never called and `fallback` is returned.
 *
 * `unsorted_search` always sorts the input list using to the given comparer. If the
 * list is pre-sorted, use `sorted_search` instead to save on compile time.
 *
 * For the semantics of how the list is sorted refer to `sort` from `fatal/sort.h`.
 *
 * The match between list element and needle is given by first applying the filter to the list
 * element, then applying the comparer to both the filtered element and the needle, as if
 * evaluating the expressions:
 *
 *  Comparer::equal<Filter::apply<ListElement>>(needle)
 *  Comparer::less<Filter::apply<ListElement>>(needle)
 *  Comparer::greater<Filter::apply<ListElement>>(needle)
 *
 * Visitor signature:
 *
 *  T visitor(indexed<Vn, N>, Args...)
 *
 * Notes:
 * - `T` must have a common type with `Fallback` as returned by `std::common_type_t<T, Fallback>`
 * - the return type of `sorted_find` is the common type described above
 *
 * Author: Marcelo Juchem <juchem at gmail dot com>
 */
template <
  typename UnsortedInputList,
  typename Filter = get_identity,
  typename Comparer = value_comparer,
  typename Needle,
  typename Fallback,
  typename Visitor,
  typename... Args
>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static constexpr decltype(auto) unsorted_find(
  Needle &&needle,
  Fallback &&fallback,
  Visitor &&visitor,
  Args &&...args
) {
  return sorted_find<sort<UnsortedInputList, Comparer, Filter>, Filter, Comparer>(
    std::forward<Needle>(needle),
    std::forward<Fallback>(fallback),
    std::forward<Visitor>(visitor),
    std::forward<Args>(args)...
  );
}

/**
 * Given a list `L<V0, V1, ..., Vn>`, looks for the element at the index given by needle.
 *
 * If found, calls the visitor for it and returns `true`. Otherwise, the visitor is never
 * called and `false` is returned.
 *
 * Visitor signature:
 *
 *  T visitor(indexed<Vn, N>, Args...)
 *
 * Notes:
 * - the result of the visitor is ignored.
 *
 * Author: Marcelo Juchem <juchem at gmail dot com>
 */
template <typename InputList, typename Visitor, typename... Args>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static constexpr bool index_search(std::size_t needle, Visitor &&visitor, Args &&...args) {
  return sorted_search<InputList, index<InputList>>(
    needle,
    std::forward<Visitor>(visitor),
    std::forward<Args>(args)...
  );
}

/**
 * Given a list `L<V0, V1, ..., Vn>`, looks for the element at the index given by needle.
 *
 * If found, calls the visitor for it and returns the result of that call. Otherwise, the
 * visitor is never called and `fallback` is returned.
 *
 * Visitor signature:
 *
 *  T visitor(indexed<Vn, N>, Args...)
 *
 * Notes:
 * - `T` must have a common type with `Fallback` as returned by `std::common_type_t<T, Fallback>`
 * - the return type of `sorted_find` is the common type described above
 *
 * Author: Marcelo Juchem <juchem at gmail dot com>
 */
template <typename InputList, typename Fallback, typename Visitor, typename... Args>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static constexpr decltype(auto) index_find(
  std::size_t needle,
  Fallback &&fallback,
  Visitor &&visitor,
  Args &&...args
) {
  return sorted_find<InputList, index<InputList>>(
    needle,
    std::forward<Fallback>(fallback),
    std::forward<Visitor>(visitor),
    std::forward<Args>(args)...
  );
}

} // namespace ftl {

FATAL_DIAGNOSTIC_POP

#endif // FATAL_INCLUDE_fatal_type_search_h
