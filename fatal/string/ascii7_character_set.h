/*
 *  Copyright (c) 2019, Marcelo Juchem
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#ifndef FATAL_INCLUDE_fatal_string_ascii7_character_set_h
#define FATAL_INCLUDE_fatal_string_ascii7_character_set_h

#include <fatal/math/numerics.h>

#include <cassert>
#include <cstdint>

namespace fatal {

class ascii7_character_set {
  using set_type = smallest_unsigned_integral<128>;

public:
  template <typename... Chars>
  constexpr ascii7_character_set(Chars... c):
    set_((static_cast<set_type>(set_type(1) << set_type(c)) | ... | 0))
  {
    assert(((set_type(c) < 128) && ... && true));
  }

  constexpr bool operator ()(char c) const {
    return (set_ & static_cast<set_type>(set_type(1) << set_type(c))) != 0;
  }

private:
  set_type set_ = 0;
};

} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_string_ascii7_character_set_h
