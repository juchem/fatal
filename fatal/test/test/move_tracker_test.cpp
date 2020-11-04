/*
 *  Copyright (c) 2019, Marcelo Juchem
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#include <fatal/test/move_tracker.h>

#include <utility>

#include <fatal/test/driver.h>

namespace ftl {

FATAL_TEST(move_tracker, no move, no copy) {
  move_tracker subject;
  FATAL_EXPECT_EQ(0, subject.moves());
  FATAL_EXPECT_FALSE(subject.moved());
  FATAL_EXPECT_FALSE(subject.double_moved());
}

FATAL_TEST(move_tracker, no move, one copy) {
  move_tracker subject;

  [[maybe_unused]]
  move_tracker copy{subject};
  FATAL_EXPECT_EQ(0, subject.moves());
  FATAL_EXPECT_FALSE(subject.moved());
  FATAL_EXPECT_FALSE(subject.double_moved());
  FATAL_EXPECT_EQ(0, copy.moves());
  FATAL_EXPECT_FALSE(copy.moved());
  FATAL_EXPECT_FALSE(copy.double_moved());
}

FATAL_TEST(move_tracker, one move, no copy) {
  move_tracker subject;

  [[maybe_unused]]
  move_tracker move{std::move(subject)};
  FATAL_EXPECT_EQ(1, subject.moves());
  FATAL_EXPECT_TRUE(subject.moved());
  FATAL_EXPECT_FALSE(subject.double_moved());
  FATAL_EXPECT_EQ(0, move.moves());
  FATAL_EXPECT_FALSE(move.moved());
  FATAL_EXPECT_FALSE(move.double_moved());
}

FATAL_TEST(move_tracker, one copy, one move) {
  move_tracker subject;

  [[maybe_unused]]
  move_tracker copy{subject};
  FATAL_EXPECT_EQ(0, subject.moves());
  FATAL_EXPECT_FALSE(subject.moved());
  FATAL_EXPECT_FALSE(subject.double_moved());
  FATAL_EXPECT_EQ(0, copy.moves());
  FATAL_EXPECT_FALSE(copy.moved());
  FATAL_EXPECT_FALSE(copy.double_moved());

  [[maybe_unused]]
  move_tracker move{std::move(subject)};
  FATAL_EXPECT_EQ(1, subject.moves());
  FATAL_EXPECT_TRUE(subject.moved());
  FATAL_EXPECT_FALSE(subject.double_moved());
  FATAL_EXPECT_EQ(0, move.moves());
  FATAL_EXPECT_FALSE(move.moved());
  FATAL_EXPECT_FALSE(move.double_moved());
}

FATAL_TEST(move_tracker, one move, one copy) {
  move_tracker subject;

  [[maybe_unused]]
  move_tracker move{std::move(subject)};
  FATAL_EXPECT_EQ(1, subject.moves());
  FATAL_EXPECT_TRUE(subject.moved());
  FATAL_EXPECT_FALSE(subject.double_moved());
  FATAL_EXPECT_EQ(0, move.moves());
  FATAL_EXPECT_FALSE(move.moved());
  FATAL_EXPECT_FALSE(move.double_moved());

  [[maybe_unused]]
  move_tracker copy{subject};
  FATAL_EXPECT_EQ(1, subject.moves());
  FATAL_EXPECT_TRUE(subject.moved());
  FATAL_EXPECT_FALSE(subject.double_moved());
  FATAL_EXPECT_EQ(0, copy.moves());
  FATAL_EXPECT_FALSE(copy.moved());
  FATAL_EXPECT_FALSE(copy.double_moved());
}

FATAL_TEST(move_tracker, two moves) {
  move_tracker subject;

  [[maybe_unused]]
  move_tracker first_move{std::move(subject)};
  FATAL_EXPECT_EQ(1, subject.moves());
  FATAL_EXPECT_TRUE(subject.moved());
  FATAL_EXPECT_FALSE(subject.double_moved());
  FATAL_EXPECT_EQ(0, first_move.moves());
  FATAL_EXPECT_FALSE(first_move.moved());
  FATAL_EXPECT_FALSE(first_move.double_moved());

  [[maybe_unused]]
  move_tracker second_move{std::move(subject)};
  FATAL_EXPECT_EQ(2, subject.moves());
  FATAL_EXPECT_TRUE(subject.moved());
  FATAL_EXPECT_TRUE(subject.double_moved());
  FATAL_EXPECT_EQ(0, second_move.moves());
  FATAL_EXPECT_FALSE(second_move.moved());
  FATAL_EXPECT_FALSE(second_move.double_moved());
}

FATAL_TEST(move_tracker, three moves) {
  move_tracker subject;

  [[maybe_unused]]
  move_tracker first_move{std::move(subject)};
  FATAL_EXPECT_EQ(1, subject.moves());
  FATAL_EXPECT_TRUE(subject.moved());
  FATAL_EXPECT_FALSE(subject.double_moved());
  FATAL_EXPECT_EQ(0, first_move.moves());
  FATAL_EXPECT_FALSE(first_move.moved());
  FATAL_EXPECT_FALSE(first_move.double_moved());

  [[maybe_unused]]
  move_tracker second_move{std::move(subject)};
  FATAL_EXPECT_EQ(2, subject.moves());
  FATAL_EXPECT_TRUE(subject.moved());
  FATAL_EXPECT_TRUE(subject.double_moved());
  FATAL_EXPECT_EQ(0, second_move.moves());
  FATAL_EXPECT_FALSE(second_move.moved());
  FATAL_EXPECT_FALSE(second_move.double_moved());

  [[maybe_unused]]
  move_tracker third_move{std::move(subject)};
  FATAL_EXPECT_EQ(3, subject.moves());
  FATAL_EXPECT_TRUE(subject.moved());
  FATAL_EXPECT_TRUE(subject.double_moved());
  FATAL_EXPECT_EQ(0, third_move.moves());
  FATAL_EXPECT_FALSE(third_move.moved());
  FATAL_EXPECT_FALSE(third_move.double_moved());
}

} // namespace ftl {
