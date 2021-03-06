/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_sort_h
#define FATAL_INCLUDE_fatal_type_sort_h

#include <fatal/type/apply.h>
#include <fatal/type/compare.h>
#include <fatal/type/logical.h>
#include <fatal/type/pair.h>
#include <fatal/type/slice.h>

#include <fatal/portability.h>

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

#include <fatal/type/impl/sort.h>

namespace ftl {

// `Pair` is guaranteed to be instantiated at most once
template <typename T, typename Filter, template <typename...> typename Pair = pair>
using partition = typename i_s::P<Pair, T, Filter>::type;

template <typename T, typename Predicate>
using filter = typename i_s::F<Predicate, T>::type;

template <typename T, typename Predicate>
using reject = filter<T, negation<Predicate>>;

template <typename T, typename Less = less, typename... By>
using sort = typename i_s::Q<T, Less, By...>::type;

template <typename T, typename... By>
using sort_by = sort<T, less, By...>;

template <typename T, typename Less = less, typename... By>
using sequence_sort = sort<T, sequence_compare<Less>, By...>;

template <typename T, typename... By>
using sequence_sort_by = sequence_sort<T, less, By...>;

// TODO: ACCEPT A CUSTOM PREDICATE
template  <typename LHS, typename RHS>
using merge = typename i_s::M<LHS, RHS>::type;

} // namespace ftl {

FATAL_DIAGNOSTIC_POP

#endif // FATAL_INCLUDE_fatal_type_sort_h
