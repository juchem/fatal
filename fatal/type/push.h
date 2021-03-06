/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_push_h
#define FATAL_INCLUDE_fatal_type_push_h

#include <fatal/portability.h>

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

#include <fatal/type/impl/push.h>

namespace ftl {

template <typename T>
using push = typename i_P::p<T>;

template <typename T, typename... Args>
using push_back = typename i_P::b<T, Args...>::type;

template <typename T, typename... Args>
using push_front = typename i_P::f<T, Args...>::type;

template <bool Condition, typename T, typename... Args>
using push_back_if = typename i_P::B<Condition, T, Args...>::type;

template <bool Condition, typename T, typename... Args>
using push_front_if = typename i_P::F<Condition, T, Args...>::type;

} // namespace ftl {

FATAL_DIAGNOSTIC_POP

#endif // FATAL_INCLUDE_fatal_type_push_h
