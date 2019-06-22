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

#include <fatal/type/impl/foreach.h>

namespace fatal {

template <typename List, typename Visitor, typename... Args>
FATAL_HIDE_SYMBOL
static inline void foreach(Visitor &&visitor, Args &&...args) {
  i_fe::f<List, make_index_sequence<size<List>::value>>::g(
    std::forward<Visitor>(visitor),
    std::forward<Args>(args)...
  );
}

template <typename List, typename Visitor, typename Seed, typename... Args>
FATAL_HIDE_SYMBOL
static inline constexpr auto foreach_accumulate(Visitor &&visitor, Seed &&seed, Args &&...args) {
  return i_fe::a<List>::g(
    std::forward<Visitor>(visitor),
    std::forward<Seed>(seed),
    std::forward<Args>(args)...
  );
}

} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_type_foreach_h
