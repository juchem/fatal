/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#include <fatal/type/fold.h>

#include <fatal/type/arithmetic.h>

#include <fatal/test/driver.h>

namespace fatal {

FATAL_TEST(fold, sanity check) {
  FATAL_EXPECT_SAME<
    list<>,
    fold<
      list<>,
      std::integral_constant<int, 0>, add
    >
  >();

  FATAL_EXPECT_SAME<
    int_list<5>,
    fold<
      int_list<5>,
      std::integral_constant<int, 0>, add
    >
  >();

  FATAL_EXPECT_SAME<
    int_list<6>,
    fold<
      int_list<5>,
      std::integral_constant<int, 1>, add
    >
  >();

  FATAL_EXPECT_SAME<
    int_list<1, 2, 3, 4, 5>,
    fold<
      int_list<1, 1, 1, 1, 1>,
      std::integral_constant<int, 0>, add
    >
  >();

  FATAL_EXPECT_SAME<
    int_list<2, 4, 6, 8, 10>,
    fold<
      int_list<2, 2, 2, 2, 2>,
      std::integral_constant<int, 0>, add
    >
  >();

  FATAL_EXPECT_SAME<
    int_list<4, 6, 8, 10, 12>,
    fold<
      int_list<2, 2, 2, 2, 2>,
      std::integral_constant<int, 2>, add
    >
  >();

  FATAL_EXPECT_SAME<
    int_list<1, 3, 6, 10, 15>,
    fold<
      int_list<1, 2, 3, 4, 5>,
      std::integral_constant<int, 0>, add
    >
  >();

  FATAL_EXPECT_SAME<
    int_list<4, 8, 14, 22, 32>,
    fold<
      int_list<2, 4, 6, 8, 10>,
      std::integral_constant<int, 2>, add
    >
  >();
}

} // namespace fatal {
