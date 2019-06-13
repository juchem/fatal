/*
 *  Copyright (c) 2019, Marcelo Juchem
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#ifndef FATAL_INCLUDE_fatal_type_parser_h
#define FATAL_INCLUDE_fatal_type_parser_h

#include <type_traits>

namespace fatal {

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
struct parser {
  using value_type = T;

private:
  template <typename Char, Char Head, Char... Tail>
  struct hexadecimal_parser {
    static_assert(
      std::is_signed_v<value_type> || Head != static_cast<Char>('-'),
      "can't parse a negative number as an unsigned type"
    );

    static_assert(
      (
        Head == static_cast<Char>('-') || Head == static_cast<Char>('+')
          || (Head >= static_cast<Char>('0') && Head <= static_cast<Char>('9'))
          || (Head >= static_cast<Char>('a') && Head <= static_cast<Char>('f'))
          || (Head >= static_cast<Char>('A') && Head <= static_cast<Char>('F'))
      ),
      "character is not a valid digit"
    );

    static_assert(
      ((
        (Tail >= static_cast<Char>('0') && Tail <= static_cast<Char>('9'))
          || (Tail >= static_cast<Char>('a') && Tail <= static_cast<Char>('f'))
          || (Tail >= static_cast<Char>('A') && Tail <= static_cast<Char>('F'))
      ) && ...),
      "character is not a valid digit"
    );

    constexpr static value_type base() { return 16; }

    constexpr static auto convert(Char digit) {
      return digit >= static_cast<Char>('a') && digit <= static_cast<Char>('f')
        ? static_cast<value_type>(digit - static_cast<Char>('a') + 10)
        : digit >= static_cast<Char>('A') && digit <= static_cast<Char>('F')
          ? static_cast<value_type>(digit - static_cast<Char>('A') + 10)
          : static_cast<value_type>(digit - static_cast<Char>('0'));
    }
  };

  template <typename Char, Char Head, Char... Tail>
  struct decimal_parser {
    static_assert(
      std::is_signed_v<value_type> || Head != static_cast<Char>('-'),
      "can't parse a negative number as an unsigned type"
    );

    static_assert(
      (
        Head == static_cast<Char>('-') || Head == static_cast<Char>('+')
          || (Head >= static_cast<Char>('0') && Head <= static_cast<Char>('9'))
      ),
      "character is not a valid digit"
    );

    static_assert(
      ((Tail >= static_cast<Char>('0') && Tail <= static_cast<Char>('9')) && ...),
      "character is not a valid digit"
    );

    constexpr static value_type base() { return 10; }

    constexpr static auto convert(Char digit) {
      return static_cast<value_type>(digit - static_cast<Char>('0'));
    }
  };

  template <typename Parser>
  constexpr static value_type parse_tail(value_type number) {
    return number;
  }

  template <typename Parser, typename Char, typename... Tail>
  constexpr static value_type parse_tail(value_type number, Char head, Tail... tail) {
    return parse_tail<Parser>(number * Parser::base() + Parser::convert(head), tail...);
  }

public:
  template <typename Parser, typename Char, typename... Tail>
  constexpr static value_type parse(Char head, Tail... tail) {
    return head == static_cast<Char>('-')
      ? -parse_tail<Parser>(0, tail...)
      : head == static_cast<Char>('+')
        ? parse_tail<Parser>(0, tail...)
        : parse_tail<Parser>(0, head, tail...);
  }

  template <typename Char, Char... Digits>
  using hexadecimal = std::integral_constant<
    value_type,
    parse<hexadecimal_parser<Char, Digits...>>(Digits...)
  >;

  template <typename Char, Char... Digits>
  using decimal = std::integral_constant<
    value_type,
    parse<decimal_parser<Char, Digits...>>(Digits...)
  >;
};

} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_type_parser_h
