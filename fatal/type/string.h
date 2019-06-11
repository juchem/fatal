/*
 *  Copyright (c) 2019, Marcelo Juchem.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#ifndef FATAL_INCLUDE_fatal_type_string_h
#define FATAL_INCLUDE_fatal_type_string_h

#include <utility>

namespace fatal {
namespace udl {
namespace string {

template <typename T, T... Values>
constexpr std::integer_sequence<T, Values...> operator ""_str() {
  return {};
}

} // namespace string {
} // namespace udl {
} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_type_string_h
