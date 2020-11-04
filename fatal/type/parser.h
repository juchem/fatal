/*
 *  Copyright (c) 2019, Marcelo Juchem
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#ifndef FATAL_INCLUDE_fatal_type_parser_h
#define FATAL_INCLUDE_fatal_type_parser_h

#include <fatal/portability.h>

#include <type_traits>

namespace ftl {

/**
 * Provides mechanisms to parse the integral value of type `T`
 * represented by a sequence of characters.
 *
 * Parameters:
 *
 *  - T: the type of the integral value that will be parsed.
 *
 * @author: Marcelo Juchem <juchem at gmail dot com>
 */
template <typename T>
struct FATAL_HIDE_SYMBOL parser {
  using value_type = T;

private:
  template <typename Char, value_type Acc, Char Head, Char... Tail>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static constexpr value_type parse_octal_digits() {
    static_assert(Head >= '0' && Head <= '7', "invalid octal digit");

    constexpr value_type Result = Acc * 8
      + static_cast<value_type>(Head - static_cast<Char>('0'));

    if constexpr (!sizeof...(Tail)) {
      return Result;
    } else {
      return parse_octal_digits<Char, Result, Tail...>();
    }
  }

  template <typename Char, value_type Acc, Char Head, Char... Tail>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static constexpr value_type parse_decimal_digits() {
    static_assert(Head >= '0' && Head <= '9', "invalid decimal digit");

    constexpr value_type Result = Acc * 10
      + static_cast<value_type>(Head - static_cast<Char>('0'));

    if constexpr (!sizeof...(Tail)) {
      return Result;
    } else {
      return parse_decimal_digits<Char, Result, Tail...>();
    }
  }

  template <typename Char, value_type Acc, Char Head, Char... Tail>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static constexpr value_type parse_hexadecimal_digits() {
    static_assert(
      (Head >= '0' && Head <= '9')
       || (Head >= 'a' && Head <= 'f')
       || (Head >= 'A' && Head <= 'F'),
      "invalid hexadecimal digit"
    );

    constexpr value_type Result = Acc * 16 + (
      Head >= static_cast<Char>('a') && Head <= static_cast<Char>('f')
        ? static_cast<value_type>(Head - static_cast<Char>('a') + 10)
        : Head >= static_cast<Char>('A') && Head <= static_cast<Char>('F')
          ? static_cast<value_type>(Head - static_cast<Char>('A') + 10)
          : static_cast<value_type>(Head - static_cast<Char>('0'))
    );

    if constexpr (!sizeof...(Tail)) {
      return Result;
    } else {
      return parse_hexadecimal_digits<Char, Result, Tail...>();
    }

  }

  template <typename Char, std::size_t Depth, Char Head, Char... Tail>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static constexpr value_type parse_literal_digits() {
    // first character being parsed
    if constexpr (Depth == 0) {
      if constexpr (Head == '0') {
        if constexpr (!sizeof...(Tail)) {
          return 0;
        } else {
          return parse_literal_digits<Char, 1, Tail...>();
        }
      } else {
        return parse_decimal_digits<Char, 0, Head, Tail...>();
      }
    // initial zero, checking whether octal or hexadecimal
    } else {
      if constexpr (Head == 'x' || Head == 'X') {
        return parse_hexadecimal_digits<Char, 0, Tail...>();
      } else {
        return parse_octal_digits<Char, 0, Head, Tail...>();
      }
    }
  }

public:
  template <typename Char, Char Head, Char... Tail>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static constexpr value_type parse_octal() {
    if constexpr (Head == '+') {
      return parse_octal_digits<Char, 0, Tail...>();
    } else if constexpr (Head == '-') {
      static_assert(
        std::is_signed_v<value_type>,
        "can't parse a negative number as an unsigned type"
      );

      return -parse_octal_digits<Char, 0, Tail...>();
    } else {
      return parse_octal_digits<Char, 0, Head, Tail...>();
    }
  }

  template <typename Char, Char... Digits>
  using octal = std::integral_constant<value_type, parse_octal<Char, Digits...>()>;

  template <typename Char, Char Head, Char... Tail>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static constexpr value_type parse_decimal() {
    if constexpr (Head == '+') {
      return parse_decimal_digits<Char, 0, Tail...>();
    } else if constexpr (Head == '-') {
      static_assert(
        std::is_signed_v<value_type>,
        "can't parse a negative number as an unsigned type"
      );

      return -parse_decimal_digits<Char, 0, Tail...>();
    } else {
      return parse_decimal_digits<Char, 0, Head, Tail...>();
    }
  }

  template <typename Char, Char... Digits>
  using decimal = std::integral_constant<value_type, parse_decimal<Char, Digits...>()>;

  template <typename Char, Char Head, Char... Tail>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static constexpr value_type parse_hexadecimal() {
    if constexpr (Head == '+') {
      return parse_hexadecimal_digits<Char, 0, Tail...>();
    } else if constexpr (Head == '-') {
      static_assert(
        std::is_signed_v<value_type>,
        "can't parse a negative number as an unsigned type"
      );

      return -parse_hexadecimal_digits<Char, 0, Tail...>();
    } else {
      return parse_hexadecimal_digits<Char, 0, Head, Tail...>();
    }
  }

  template <typename Char, Char... Digits>
  using hexadecimal = std::integral_constant<value_type, parse_hexadecimal<Char, Digits...>()>;

  template <typename Char, Char Head, Char... Tail>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static constexpr value_type parse_literal() {
    if constexpr (Head == '+') {
      return parse_literal_digits<Char, 0, Tail...>();
    } else if constexpr (Head == '-') {
      static_assert(
        std::is_signed_v<value_type>,
        "can't parse a negative number as an unsigned type"
      );

      return -parse_literal_digits<Char, 0, Tail...>();
    } else {
      return parse_literal_digits<Char, 0, Head, Tail...>();
    }
  }

  template <typename Char, Char... Digits>
  using literal = std::integral_constant<value_type, parse_literal<Char, Digits...>()>;
};

} // namespace ftl {

#endif // FATAL_INCLUDE_fatal_type_parser_h
