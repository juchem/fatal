/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_benchmark_prevent_optimization_h
#define FATAL_INCLUDE_fatal_benchmark_prevent_optimization_h

#include <fatal/portability.h>

#include <memory>

#include <cstdint>

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

namespace ftl {
namespace detail {

FATAL_HIDE_SYMBOL
static std::uintptr_t prevent_optimization = 0;

} // namespace detail {

template <typename T>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
void prevent_optimization(T const &what) {
  detail::prevent_optimization |= reinterpret_cast<
    decltype(detail::prevent_optimization)
  >(std::addressof(what));
}

} // namespace ftl {

FATAL_DIAGNOSTIC_POP

#endif // FATAL_INCLUDE_fatal_benchmark_prevent_optimization_h
