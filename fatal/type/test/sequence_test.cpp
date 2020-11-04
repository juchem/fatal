/*
 *  Copyright (c) 2019, Marcelo Juchem.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#include <fatal/type/sequence.h>

#include <fatal/test/driver.h>

namespace ftl {

FATAL_TEST(make_sequence, make_sequence) {
  FATAL_EXPECT_SAME<sequence<int>, make_sequence<int, 0>>();
  FATAL_EXPECT_SAME<index_sequence<>, make_index_sequence<0>>();

  FATAL_EXPECT_SAME<sequence<int, 0>, make_sequence<int, 1>>();
  FATAL_EXPECT_SAME<index_sequence<0>, make_index_sequence<1>>();

  FATAL_EXPECT_SAME<sequence<int, 0, 1, 2, 3, 4>, make_sequence<int, 5>>();
  FATAL_EXPECT_SAME<index_sequence<0, 1, 2, 3, 4>, make_index_sequence<5>>();
}

} // namespace ftl {
