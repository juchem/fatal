/*
 *  Copyright (c) 2019, Marcelo Juchem
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#ifndef FATAL_INCLUDE_fatal_string_char_range_h
#define FATAL_INCLUDE_fatal_string_char_range_h

#include <cassert>

namespace fatal {

template <typename Char = char>
struct char_range {
  using char_type = Char;

  constexpr char_range(char_type min, char_type max):
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

} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_string_char_range_h
