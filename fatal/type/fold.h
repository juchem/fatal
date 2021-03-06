/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_fold_h
#define FATAL_INCLUDE_fatal_type_fold_h

#include <fatal/portability.h>

#include <fatal/type/impl/fold.h>

namespace ftl {

template <typename T, typename Seed, typename Fn>
using fold = typename i_fl::F<Seed, Fn, T>::type;

} // namespace ftl {

#endif // FATAL_INCLUDE_fatal_type_fold_h
