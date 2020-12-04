/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_accumulate_h
#define FATAL_INCLUDE_fatal_type_accumulate_h

#include <fatal/portability.h>

#include <fatal/type/impl/accumulate.h>

namespace ftl {

template <typename T, typename Seed, typename Fn = i_A::f>
using accumulate = typename i_A::F<Seed, Fn, T>::type;

} // namespace ftl {

#endif // FATAL_INCLUDE_fatal_type_accumulate_h
