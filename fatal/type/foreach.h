/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_foreach_h
#define FATAL_INCLUDE_fatal_type_foreach_h

#include <fatal/type/sequence.h>
#include <fatal/type/size.h>

#include <fatal/portability.h>

#include <utility>

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

#include <fatal/type/impl/foreach.h>

namespace ftl {

// TODO: ADD EXAMPLES TO THE DOCUMENTATION

/**
 * Given a list `L<V0, V1, ..., Vn>`, calls the visitor for each element in the list, in order,
 * as if calling:
 *
 *  visitor(indexed<V0, 0>(), args...);
 *  visitor(indexed<V1, 1>(), args...);
 *  ...
 *  visitor(indexed<Vn, N>(), args...);
 *
 * Visitor signature:
 *  T visitor(indexed<Vn, N>, Args...)
 *
 * Notes:
 * - the result of the visitor is ignored.
 *
 * Author: Marcelo Juchem <juchem at gmail dot com>
 */
template <typename List, typename Visitor, typename... Args>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static  void foreach(Visitor &&visitor, Args &&...args) {
  i_fe::f<List, make_index_sequence<size<List>::value>>::g(
    std::forward<Visitor>(visitor),
    std::forward<Args>(args)...
  );
}

/**
 * Given a list `L<V0, V1, V2, V3>`, returns a value as if evaluating the expression
 *
 *  visitor(
 *    indexed<V3, 3>(),
 *    visitor(
 *      indexed<V2, 2>(),
 *      visitor(
 *        indexed<V1, 1>(),
 *        visitor(
 *          indexed<V0, 0>(),
 *          seed,
 *          args...
 *        ),
 *        args...
 *      ),
 *      args...
 *    ),
 *    args...
 *  )
 *
 * Visitor signature:
 *  T visitor(indexed<Vn, N>, Seed, Args...)
 *
 * Notes:
 * - `T` doesn't need to have any relation to `Seed`
 * - the return type of `foreach_accumulate` is the `T` returned by the visitor to the last
 *   element of the list
 *
 * Author: Marcelo Juchem <juchem at gmail dot com>
 */
template <typename List, typename Visitor, typename Seed, typename... Args>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static constexpr auto foreach_accumulate(Visitor &&visitor, Seed &&seed, Args &&...args) {
  return i_fe::a<List>::g(
    std::forward<Visitor>(visitor),
    std::forward<Seed>(seed),
    std::forward<Args>(args)...
  );
}

} // namespace ftl {

FATAL_DIAGNOSTIC_POP

#endif // FATAL_INCLUDE_fatal_type_foreach_h
