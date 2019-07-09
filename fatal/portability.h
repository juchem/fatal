/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_portability_h
#define FATAL_INCLUDE_fatal_portability_h

///////////////////////
// FATAL_HAS_BUILTIN //
///////////////////////

/**
 * TODO: DOCUMENT
 */

#if defined(__has_builtin)
# define FATAL_HAS_BUILTIN(...) __has_builtin(__VA_ARGS__)
#else
# define FATAL_HAS_BUILTIN(...) 0
#endif

#define FATAL_COMPILER_UNKNOWN 0
#define FATAL_COMPILER_CLANG 1
#define FATAL_COMPILER_GCC 1

#if defined(__clang__)
# define FATAL_COMPILER_IS_CLANG 1
# define FATAL_COMPILER FATAL_COMPILER_CLANG
# define FATAL_COMPILER_IS_CLANG_OR_GCC 1
#elif defined(__GNUC__)
# define FATAL_COMPILER_IS_GCC 1
# define FATAL_COMPILER FATAL_COMPILER_GCC
# define FATAL_COMPILER_IS_CLANG_OR_GCC 1
#else
# define FATAL_COMPILER_IS_UNKNOWN 1
# define FATAL_COMPILER FATAL_COMPILER_UNKNOWN
#endif

// Generalize warning push/pop.
#ifdef FATAL_COMPILER_IS_CLANG_OR_GCC
# define FATAL_DIAGNOSTIC_PUSH _Pragma("GCC diagnostic push")
# define FATAL_DIAGNOSTIC_POP _Pragma("GCC diagnostic pop")
# define FATAL_DIAGNOSTIC_TO_STR(warningName) #warningName
# define FATAL_DIAGNOSTIC_IGNORE(warningName) \
  _Pragma(FATAL_DIAGNOSTIC_TO_STR(GCC diagnostic ignored warningName))
# define FATAL_DIAGNOSTIC_WARNING(warningName) \
  _Pragma(FATAL_DIAGNOSTIC_TO_STR(GCC diagnostic warning warningName))
# define FATAL_DIAGNOSTIC_ERROR(warningName) \
  _Pragma(FATAL_DIAGNOSTIC_TO_STR(GCC diagnostic error warningName))
#else
# define FATAL_DIAGNOSTIC_PUSH
# define FATAL_DIAGNOSTIC_POP
# define FATAL_DIAGNOSTIC_IGNORE(warningName)
# define FATAL_DIAGNOSTIC_WARNING(warningName)
# define FATAL_DIAGNOSTIC_ERROR(warningName)
#endif

#if __GNUC__ && __GNUC__ < 5
# define FATAL_DIAGNOSTIC_IGNORE_SHADOW \
  FATAL_DIAGNOSTIC_IGNORE("-Wshadow")
#else
# define FATAL_DIAGNOSTIC_IGNORE_SHADOW
#endif

#if __GNUC__
# define FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES \
  FATAL_DIAGNOSTIC_IGNORE("-Wattributes")
#else
# define FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES
#endif

#define FATAL_

/////////////////////////
// FATAL_ALWAYS_INLINE //
/////////////////////////

/**
 * TODO: DOCUMENT
 */

#if _MSC_VER
# define FATAL_ALWAYS_INLINE __forceinline
#elif FATAL_COMPILER_IS_CLANG_OR_GCC
# define FATAL_ALWAYS_INLINE inline __attribute__((always_inline))
#else
# define FATAL_ALWAYS_INLINE inline
#endif

///////////////////////
// FATAL_HIDE_SYMBOL //
///////////////////////

/**
 * TODO: DOCUMENT
 */

#if _MSC_VER
# define FATAL_HIDE_SYMBOL
#elif FATAL_COMPILER_IS_CLANG_OR_GCC
# define FATAL_HIDE_SYMBOL __attribute__((visibility("hidden")))
#else
# define FATAL_HIDE_SYMBOL
#endif

#if __SIZEOF_INT128__

#include <type_traits>

namespace std {

template <>
struct is_signed<__int128_t>: std::true_type {};

template <>
struct is_signed<__uint128_t>: std::false_type {};

template <>
struct is_unsigned<__int128_t>: std::false_type {};

template <>
struct is_unsigned<__uint128_t>: std::true_type {};

} // namespace std {

#endif // __SIZEOF_INT128__

#endif
