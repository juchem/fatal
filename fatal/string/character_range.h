/*
 *  Copyright (c) 2019, Marcelo Juchem
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#ifndef FATAL_INCLUDE_fatal_string_character_range_h
#define FATAL_INCLUDE_fatal_string_character_range_h

#include <cassert>

namespace ftl {

template <typename Char = char>
struct character_range {
  using char_type = Char;

  constexpr character_range(char_type min, char_type max):
    min_(min),
    max_(max)
  {
    assert(min_ <= max_);
  }

  constexpr bool operator ()(char_type c) const {
    return c >= min_ && c <= max_;
  }

private:
  char_type const min_;
  char_type const max_;
};

} // namespace ftl {

#endif // FATAL_INCLUDE_fatal_string_character_range_h
