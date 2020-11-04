/*
 *  Copyright (c) 2019, Marcelo Juchem
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#ifndef FATAL_INCLUDE_fatal_test_double_move_h
#define FATAL_INCLUDE_fatal_test_double_move_h

#include <fatal/portability.h>

#include <atomic>

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

namespace ftl {

/**
 * A helper class to test against double moves.
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
// TODO: ADD UNIT TESTS
template <typename...>
struct FATAL_HIDE_SYMBOL move_tracker {
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  move_tracker() = default;

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  move_tracker(move_tracker const &) {}

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  move_tracker(move_tracker &&rhs) {
    ++rhs.moves_;
  }

  bool moved() const { return moves_ > 0; }
  bool double_moved() const { return moves_ > 1; }

  std::size_t moves() const { return moves_; }

private:
  FATAL_HIDE_SYMBOL
  std::atomic_size_t moves_ = 0;
};

} // namespace ftl {

FATAL_DIAGNOSTIC_POP

#endif // FATAL_INCLUDE_fatal_test_double_move_h
