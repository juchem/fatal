/*
 *  Copyright (c) 2019, Marcelo Juchem.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#include <fatal/type/string.h>

#include <fatal/test/driver.h>

namespace fatal {
namespace udl {
namespace string {

FATAL_TEST(make_sequence, make_sequence) {
  FATAL_EXPECT_SAME<std::integer_sequence<char>, decltype(""_str)>();

  FATAL_EXPECT_SAME<std::integer_sequence<char, '0'>, decltype("0"_str)>();

  FATAL_EXPECT_SAME<std::integer_sequence<char, 'a', 'b', 'c'>, decltype("abc"_str)>();

  FATAL_EXPECT_SAME<
    std::integer_sequence<char, 'h', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd'>,
    decltype("hello, world"_str)
  >();
}

} // namespace string {
} // namespace udl {
} // namespace fatal {
