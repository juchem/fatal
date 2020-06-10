/*
 *  Copyright (c) 2020, Marcelo Juchem
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#include <fatal/type/interleave.h>

#include <fatal/type/list.h>

#include <fatal/test/driver.h>

namespace fatal {

FATAL_TEST(interleave, interleave) {
  // size 0
  FATAL_EXPECT_SAME<list<>, interleave<int, list<>>>();

  // size 1
  FATAL_EXPECT_SAME<list<int>, interleave<int, list<int>>>();
  FATAL_EXPECT_SAME<list<double>, interleave<int, list<double>>>();

  // size 2
  FATAL_EXPECT_SAME<list<int, int>, interleave<int, list<int, int>>>();
  FATAL_EXPECT_SAME<list<int, double>, interleave<int, list<int, double>>>();
  FATAL_EXPECT_SAME<list<double, int>, interleave<int, list<double, int>>>();
  FATAL_EXPECT_SAME<list<double, int, double>, interleave<int, list<double, double>>>();

  // size 3
  FATAL_EXPECT_SAME<list<int, int, int>, interleave<int, list<int, int, int>>>();
  FATAL_EXPECT_SAME<list<int, int, double>, interleave<int, list<int, int, double>>>();
  FATAL_EXPECT_SAME<list<int, double, int>, interleave<int, list<int, double, int>>>();
  FATAL_EXPECT_SAME<list<double, int, int>, interleave<int, list<double, int, int>>>();
  FATAL_EXPECT_SAME<list<int, double, int, double>, interleave<int, list<int, double, double>>>();
  FATAL_EXPECT_SAME<list<double, int, double>, interleave<int, list<double, int, double>>>();
  FATAL_EXPECT_SAME<list<double, int, double, int>, interleave<int, list<double, double, int>>>();
  FATAL_EXPECT_SAME<
    list<double, int, double, int, double>,
    interleave<int, list<double, double, double>>
  >();
}

FATAL_TEST(surround, surround) {
  // size 0
  FATAL_EXPECT_SAME<list<int>, surround<int, list<>>>();

  // size 1
  FATAL_EXPECT_SAME<list<int>, surround<int, list<int>>>();
  FATAL_EXPECT_SAME<list<int, double, int>, surround<int, list<double>>>();

  // size 2
  FATAL_EXPECT_SAME<list<int, int>, surround<int, list<int, int>>>();
  FATAL_EXPECT_SAME<list<int, double, int>, surround<int, list<int, double>>>();
  FATAL_EXPECT_SAME<list<int, double, int>, surround<int, list<double, int>>>();
  FATAL_EXPECT_SAME<list<int, double, double, int>, surround<int, list<double, double>>>();

  // size 3
  FATAL_EXPECT_SAME<list<int, int, int>, surround<int, list<int, int, int>>>();
  FATAL_EXPECT_SAME<list<int, int, double, int>, surround<int, list<int, int, double>>>();
  FATAL_EXPECT_SAME<list<int, double, int>, surround<int, list<int, double, int>>>();
  FATAL_EXPECT_SAME<list<int, double, int, int>, surround<int, list<double, int, int>>>();
  FATAL_EXPECT_SAME<list<int, double, double, int>, surround<int, list<int, double, double>>>();
  FATAL_EXPECT_SAME<
    list<int, double, int, double, int>,
    surround<int, list<double, int, double>>
  >();
  FATAL_EXPECT_SAME<list<int, double, double, int>, surround<int, list<double, double, int>>>();
  FATAL_EXPECT_SAME<
    list<int, double, double, double, int>,
    surround<int, list<double, double, double>>
  >();
}

} // namespace fatal {
