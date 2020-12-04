/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_cat_h
#define FATAL_INCLUDE_fatal_type_cat_h

#include <fatal/portability.h>

#include <fatal/type/impl/cat.h>

namespace ftl {

// TODO: make it variadic
template <typename... Args>
using cat = typename impl_cat::c<Args...>::type;

template <typename T, typename... Filter>
using inner_cat = typename impl_cat::i<T, Filter...>::type;

template <typename LHS, typename RHS>
using vcat = impl_cat::v<LHS, RHS>;

template <typename LHS, typename RHS, typename... Args>
using lcat = typename impl_cat::l<LHS, RHS, Args...>::type;

} // namespace ftl {

#endif // FATAL_INCLUDE_fatal_type_cat_h
