/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_unique_h
#define FATAL_INCLUDE_fatal_type_unique_h

#include <fatal/portability.h>

#include <fatal/type/impl/unique.h>

namespace fatal {

template <typename T>
using adjacent_unique = typename i_uq::au<T>::type;

template <typename T, typename Comparer>
using adjacent_unique_by = typename i_uq::au<T, Comparer>::type;

template <typename T>
using is_adjacent_unique = std::is_same<T, adjacent_unique<T>>;

template <typename T>
constexpr bool is_adjacent_unique_v = std::is_same_v<T, adjacent_unique<T>>;

template <typename T, typename Comparer>
using is_adjacent_unique_by = std::is_same<T, adjacent_unique_by<T, Comparer>>;

template <typename T, typename Comparer>
constexpr bool is_adjacent_unique_by_v = std::is_same_v<T, adjacent_unique_by<T, Comparer>>;

} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_type_unique_h
