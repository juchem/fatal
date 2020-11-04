/*
 *  Copyright (c) 2020, Marcelo Juchem
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#ifndef FATAL_INCLUDE_fatal_type_interleave_h
#define FATAL_INCLUDE_fatal_type_interleave_h

#include <fatal/portability.h>

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

#include <fatal/type/impl/interleave.h>

namespace ftl {

/**
 * Inserts the element `D` between each pair of consecutive elements of the list.
 *
 * If any of the elements in the pair is `D`, no insertion takes place at that point.
 */
template <typename D, typename T>
using interleave = typename i_i::i<D, T>::type;

/**
 * Adds the given element `D` to the beginning of the list if first element is not `D` already.
 * Adds the given element `D` to the end of the list if last element is not `D` already.
 * If the list is empty, it returns a list with a single element `D`.
 */
template <typename D, typename T>
using surround = typename i_i::s<D, T>::type;

} // namespace ftl {

#endif // FATAL_INCLUDE_fatal_type_interleave_h
