/*
 *  Copyright (c) 2019, Marcelo Juchem.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#ifndef FATAL_INCLUDE_fatal_type_string_h
#define FATAL_INCLUDE_fatal_type_string_h

#include <fatal/portability.h>

#include <utility>

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

namespace ftl::udl::string {

template <typename T, T... Values>
constexpr std::integer_sequence<T, Values...> operator ""_str() {
  return {};
}

} // namespace ftl::udl::string

FATAL_DIAGNOSTIC_POP

#endif // FATAL_INCLUDE_fatal_type_string_h
