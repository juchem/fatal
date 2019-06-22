/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_size_h
#define FATAL_INCLUDE_fatal_type_size_h

#include <fatal/portability.h>

#include <type_traits>

#include <fatal/type/impl/size.h>

namespace fatal {

template <typename T>
using size = typename i_sz::s<T>::type;

template <typename T>
FATAL_HIDE_SYMBOL
constexpr auto size_v = size<T>::value;

template <typename T>
using empty = std::integral_constant<bool, size<T>::value == 0>;

template <typename T>
FATAL_HIDE_SYMBOL
constexpr auto empty_v = empty<T>::value;

} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_type_size_h
