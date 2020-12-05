/*
 *  Copyright (c) 2021, Marcelo Juchem
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#include <fatal/test/driver.h>

#include <fatal/preprocessor.h>
#include <fatal/type/integer.h>

#include <type_traits>

namespace ftl {

FATAL_TEST(integer, literal) {
  using namespace ftl::udl::integer;

# define TEST_IMPL(T, Value, Suffix) \
  do { \
    using expected = std::integral_constant<T, Value>; \
    FATAL_EXPECT_SAME<expected, decltype(FATAL_CAT(#Value, FATAL_CAT(_, Suffix)))>(); \
    FATAL_EXPECT_SAME<expected, decltype(FATAL_CAT(FATAL_CAT(L, #Value), FATAL_CAT(_, Suffix)))>(); \
  } while (false)

  TEST_IMPL(unsigned, 0, u);
  TEST_IMPL(short, 1, s);
  TEST_IMPL(short, -1, s);
  TEST_IMPL(std::size_t, 42, sz);
  TEST_IMPL(int, 56, i);
  TEST_IMPL(int, -56, i);
  TEST_IMPL(unsigned, 0x0, u);
  TEST_IMPL(short, 0x1, s);
  TEST_IMPL(short, -0x1, s);
  TEST_IMPL(std::size_t, 0x42, sz);
  TEST_IMPL(int, 0x56, i);
  TEST_IMPL(int, -0x56, i);
  TEST_IMPL(unsigned, 0xc, u);
  TEST_IMPL(short, 0xa, s);
  TEST_IMPL(short, -0xa, s);
  TEST_IMPL(unsigned, 0xC, u);
  TEST_IMPL(short, -0xA, s);
  TEST_IMPL(std::size_t, 0xE2, sz);
  TEST_IMPL(int, 0x5a, i);
  TEST_IMPL(int, -0x5a, i);
  TEST_IMPL(std::size_t, 0x42Bd, sz);
  TEST_IMPL(int, 0xaF56, i);
  TEST_IMPL(int, -0xaF56, i);

# undef TEST_IMPL
}

} // namespace ftl {
