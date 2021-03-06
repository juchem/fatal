/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_conditional_h
#define FATAL_INCLUDE_fatal_type_conditional_h

#include <fatal/portability.h>

#include <fatal/type/impl/conditional.h>

namespace ftl {

/**
 * Equivalent to `std::conditional_t`.
 *
 * Implemented with fewer template instantiations, can often be used to improve
 * build times compared to using `std::conditional` or `std::conditional_t`.
 */
template <bool C, typename T, typename F>
using conditional = typename i_cnd::c<C>::template apply<T, F>;

} // namespace ftl {

#endif // FATAL_INCLUDE_fatal_type_conditional_h
