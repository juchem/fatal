/*
 *  Copyright (c) 2019, Marcelo Juchem
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#ifndef FATAL_INCLUDE_fatal_container_impl_bit_set_h
#define FATAL_INCLUDE_fatal_container_impl_bit_set_h

namespace ftl {
namespace i_bs {

template <std::size_t Size, bool = (Size > data_bits_v<last<known_fast_unsigned_integers>>)>
FATAL_HIDE_SYMBOL
struct bit_set_type_selector {
  using type = last<known_fast_unsigned_integers>;
};

template <std::size_t Size>
FATAL_HIDE_SYMBOL
struct bit_set_type_selector<Size, false> {
  using type = smallest_fast_unsigned_integral<Size>;
};

template <typename T>
FATAL_HIDE_SYMBOL
struct bit_set_decrement {
  static inline T post_decrement(T &value) { return value--; }
};

template <>
FATAL_HIDE_SYMBOL
struct bit_set_decrement<bool> {
  static inline bool post_decrement(bool &value) {
    bool result = value;
    value = !value;
    return result;
  }
};

} // namespace i_bs {
} // namespace ftl {

#endif // FATAL_INCLUDE_fatal_container_impl_bit_set_h
