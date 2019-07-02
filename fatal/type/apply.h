/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_apply_h
#define FATAL_INCLUDE_fatal_type_apply_h

#include <fatal/portability.h>

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

#include <fatal/type/impl/apply.h>

namespace fatal {

template <template <typename...> typename To, typename... Args>
using apply_args = typename i_ap::a<To, Args...>::type;

template <typename T, template <typename...> typename To, typename... Suffix>
using apply_to = typename i_ap::LS<To, T, Suffix...>::type;

template <typename T, template <typename...> typename To, typename... Prefix>
using apply_to_front = typename i_ap::lsf<To, T, Prefix...>::type;

template <typename T>
using sequence_apply = i_ap::sq<T>;

template <template <typename...> typename T>
struct FATAL_HIDE_SYMBOL applier {
  template <typename... Args>
  using apply = typename i_ap::app<T, Args...>::type;
};

// TODO: ADD FILTERED VARIANT
template <typename T, typename... Bound>
struct FATAL_HIDE_SYMBOL curry {
  template <typename... Args>
  using apply = typename i_ap::app<
    T::template apply, Bound..., Args...
  >::type;
};

// TODO: ADD FILTERED VARIANT
template <typename T, typename... Bound>
struct FATAL_HIDE_SYMBOL curry_back {
  template <typename... Args>
  using apply = typename i_ap::app<
    T::template apply, Args..., Bound...
  >::type;
};

} // namespace fatal {

FATAL_DIAGNOSTIC_POP

#endif // FATAL_INCLUDE_fatal_type_apply_h
