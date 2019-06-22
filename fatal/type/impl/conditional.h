/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_impl_conditional_h
#define FATAL_INCLUDE_fatal_type_impl_conditional_h

namespace fatal {
namespace i_cnd {

template <bool>
struct FATAL_HIDE_SYMBOL c;

template <>
struct FATAL_HIDE_SYMBOL c<false> {
  template <typename T, typename F>
  using apply = F;
};

template <>
struct FATAL_HIDE_SYMBOL c<true> {
  template <typename T, typename F>
  using apply = T;
};

} // namespace i_cnd {
} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_type_impl_conditional_h
