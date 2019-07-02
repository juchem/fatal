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

// Generalize warning push/pop.
#if defined(__clang__) || defined(__GNUC__)
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
#elif __clang__ || __GNUC__
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
#elif __clang__ || __GNUC__
# define FATAL_HIDE_SYMBOL __attribute__((visibility("hidden")))
#else
# define FATAL_HIDE_SYMBOL
#endif

#endif
