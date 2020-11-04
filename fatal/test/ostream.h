/*
 *  Copyright (c) 2019, Marcelo Juchem
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#ifndef FATAL_INCLUDE_fatal_test_ostream_h
#define FATAL_INCLUDE_fatal_test_ostream_h

#include <fatal/portability.h>

#include <vector>

namespace ftl {

template <typename Out, typename T>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
Out &operator <<(Out &&out, std::vector<T> const &what) {
  out << '{';
  for (std::size_t i = 0; i < what.size(); ++i) {
    if (i) { out << ", "; }
    out << what[i];
  }
  out << '}';
  return out;
}

} // namespace ftl {

#endif // FATAL_INCLUDE_fatal_test_ostream_h
