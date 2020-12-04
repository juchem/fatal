/*
 *  Copyright (c) 2021, Marcelo Juchem
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#ifndef FATAL_INCLUDE_fatal_type_integer_h
#define FATAL_INCLUDE_fatal_type_integer_h

#include <cstdint>

namespace ftl {

using uint1 = bool;
using int8 = std::int8_t;
using uint8 = std::uint8_t;
using int16 = std::int16_t;
using uint16 = std::uint16_t;
using int32 = std::int32_t;
using uint32 = std::uint32_t;
using int64 = std::int64_t;
using uint64 = std::uint64_t;

#if __SIZEOF_INT128__
using int128 = __int128;
using uint128 = unsigned int128;
#endif // __SIZEOF_INT128__

} // namespace ftl

#endif // FATAL_INCLUDE_fatal_type_integer_h
