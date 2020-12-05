/*
 *  Copyright (c) 2021, Marcelo Juchem
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#ifndef FATAL_INCLUDE_fatal_type_integer_h
#define FATAL_INCLUDE_fatal_type_integer_h

#include <fatal/preprocessor.h>
#include <fatal/type/parser.h>

#include <cstdint>

namespace ftl {

#define FATAL_DECLARE_INTEGER_LITERAL_IMPL(Type, LiteralSuffix) \
  namespace udl::integer { \
  \
  template <typename T, T... Digits> \
  constexpr ftl::parser<Type>::literal<T, Digits...> \
  operator FATAL_CAT("", FATAL_CAT(_, LiteralSuffix))() \
  { return {}; } \
  \
  } /* namespace udl::integer */

#define FATAL_DECLARE_INTEGER_TYPE_IMPL(Alias, Type, LiteralSuffix) \
  using Alias = Type; \
  \
  FATAL_DECLARE_INTEGER_LITERAL_IMPL(Type, LiteralSuffix)

FATAL_DECLARE_INTEGER_TYPE_IMPL(uint1, bool, u1)
FATAL_DECLARE_INTEGER_TYPE_IMPL(int8, std::int8_t, i8)
FATAL_DECLARE_INTEGER_TYPE_IMPL(uint8, std::uint8_t, u8)
FATAL_DECLARE_INTEGER_TYPE_IMPL(int16, std::int16_t, i16)
FATAL_DECLARE_INTEGER_TYPE_IMPL(uint16, std::uint16_t, u16)
FATAL_DECLARE_INTEGER_TYPE_IMPL(int32, std::int32_t, i32)
FATAL_DECLARE_INTEGER_TYPE_IMPL(uint32, std::uint32_t, u32)
FATAL_DECLARE_INTEGER_TYPE_IMPL(int64, std::int64_t, i64)
FATAL_DECLARE_INTEGER_TYPE_IMPL(uint64, std::uint64_t, u64)

#if __SIZEOF_INT128__
FATAL_DECLARE_INTEGER_TYPE_IMPL(int128, __int128, i128)
FATAL_DECLARE_INTEGER_TYPE_IMPL(uint128, unsigned __int128, u128)
#endif // __SIZEOF_INT128__

FATAL_DECLARE_INTEGER_LITERAL_IMPL(char, c)
FATAL_DECLARE_INTEGER_LITERAL_IMPL(unsigned char, uc)
FATAL_DECLARE_INTEGER_LITERAL_IMPL(short, s)
FATAL_DECLARE_INTEGER_LITERAL_IMPL(unsigned short, us)
FATAL_DECLARE_INTEGER_LITERAL_IMPL(int, i)
FATAL_DECLARE_INTEGER_LITERAL_IMPL(unsigned int, u)
FATAL_DECLARE_INTEGER_LITERAL_IMPL(long, l)
FATAL_DECLARE_INTEGER_LITERAL_IMPL(unsigned long, ul)
FATAL_DECLARE_INTEGER_LITERAL_IMPL(long long, ll)
FATAL_DECLARE_INTEGER_LITERAL_IMPL(unsigned long long, ull)
FATAL_DECLARE_INTEGER_LITERAL_IMPL(std::size_t, sz)

} // namespace ftl

#endif // FATAL_INCLUDE_fatal_type_integer_h
