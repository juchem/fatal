/*
 *  Copyright (c) 2019, Marcelo Juchem
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#include <fatal/test/driver.h>

#include <fatal/type/parser.h>

#include <fatal/type/test/parse_sequence_input.h>

#include <type_traits>

namespace fatal {

#define FATAL_IMPL_OCTAL_PARSE_TEST_CALLS(Fn) \
  do { \
    Fn(unsigned, 00, char, L'0', '0'); \
    Fn(short, 01, char, L'0', '1'); \
    Fn(std::size_t, 042, char, L'0', '4', '2'); \
    Fn(int, -056, char, '-', L'0', '5', '6'); \
    \
    Fn(unsigned, 00, wchar_t, L'0', L'0'); \
    Fn(short, 01, wchar_t, L'0', L'1'); \
    Fn(std::size_t, 042, wchar_t, L'0', L'4', L'2'); \
    Fn(int, -056, wchar_t, L'-', L'0', L'5', L'6'); \
  } while (false)

FATAL_TEST(parser, octal) {
# define TEST_IMPL(T, Value, Char, ...) \
  do { \
    using expected = std::integral_constant<T, Value>; \
    \
    FATAL_EXPECT_SAME< \
      expected, \
      parser<T>::octal<Char, __VA_ARGS__> \
    >(); \
  } while (false)

  FATAL_IMPL_OCTAL_PARSE_TEST_CALLS(TEST_IMPL);

# undef TEST_IMPL
}

#define FATAL_IMPL_DECIMAL_PARSE_TEST_CALLS(Fn) \
  do { \
    Fn(unsigned, 0, char, '0'); \
    Fn(short, 1, char, '1'); \
    Fn(std::size_t, 42, char, '4', '2'); \
    Fn(int, -56, char, '-', '5', '6'); \
    \
    Fn(unsigned, 0, wchar_t, L'0'); \
    Fn(short, 1, wchar_t, L'1'); \
    Fn(std::size_t, 42, wchar_t, L'4', L'2'); \
    Fn(int, -56, wchar_t, L'-', L'5', L'6'); \
  } while (false)

FATAL_TEST(parser, decimal) {
# define TEST_IMPL(T, Value, Char, ...) \
  do { \
    using expected = std::integral_constant<T, Value>; \
    \
    FATAL_EXPECT_SAME< \
      expected, \
      parser<T>::decimal<Char, __VA_ARGS__> \
    >(); \
  } while (false)

  FATAL_IMPL_DECIMAL_PARSE_TEST_CALLS(TEST_IMPL);

# undef TEST_IMPL
}

#define FATAL_IMPL_HEXADECIMAL_PARSE_TEST_CALLS(Fn) \
  do { \
    Fn(unsigned, 0x0, char, '0'); \
    Fn(short, 0x1, char, '1'); \
    Fn(std::size_t, 0x42, char, '4', '2'); \
    Fn(int, -0x56, char, '-', '5', '6'); \
    \
    Fn(unsigned, 0x0, wchar_t, L'0'); \
    Fn(short, 0x1, wchar_t, L'1'); \
    Fn(unsigned, 0xc, wchar_t, L'c'); \
    Fn(short, 0xa, wchar_t, L'a'); \
    Fn(unsigned, 0xC, wchar_t, L'C'); \
    Fn(short, 0xA, wchar_t, L'A'); \
    Fn(std::size_t, 0x42, wchar_t, L'4', L'2'); \
    Fn(int, -0x56, wchar_t, L'-', L'5', L'6'); \
    Fn(std::size_t, 0xE2, wchar_t, L'E', L'2'); \
    Fn(int, -0x5a, wchar_t, L'-', L'5', L'a'); \
    Fn(std::size_t, 0x42Bd, wchar_t, L'4', L'2', L'B', L'd'); \
    Fn(int, -0xaF56, wchar_t, L'-', L'a', L'F', L'5', L'6'); \
  } while (false)

FATAL_TEST(parser, hexadecimal) {
# define TEST_IMPL(T, Value, Char, ...) \
  do { \
    using expected = std::integral_constant<T, Value>; \
    \
    FATAL_EXPECT_SAME< \
      expected, \
      parser<T>::hexadecimal<Char, __VA_ARGS__> \
    >(); \
  } while (false)

  FATAL_IMPL_HEXADECIMAL_PARSE_TEST_CALLS(TEST_IMPL);

# undef TEST_IMPL
}

#define FATAL_IMPL_DECIMAL_LITERAL_PARSE_TEST_CALLS(Fn) \
  do { \
    Fn(unsigned, 0, char, '0'); \
    Fn(short, 1, char, '1'); \
    Fn(std::size_t, 42, char, '4', '2'); \
    Fn(int, -56, char, '-', '5', '6'); \
    \
    Fn(unsigned, 0, wchar_t, L'0'); \
    Fn(short, 1, wchar_t, L'1'); \
    Fn(std::size_t, 42, wchar_t, L'4', L'2'); \
    Fn(int, -56, wchar_t, L'-', L'5', L'6'); \
  } while (false)

#define FATAL_IMPL_HEXADECIMAL_LITERAL_PARSE_TEST_CALLS(Fn) \
  do { \
    Fn(unsigned, 0x0, char, '0', 'x', '0'); \
    Fn(short, 0x1, char, '0', 'x', '1'); \
    Fn(std::size_t, 0x42, char, '0', 'x', '4', '2'); \
    Fn(int, -0x56, char, '-', '0', 'x', '5', '6'); \
    \
    Fn(unsigned, 0x0, wchar_t, L'0', L'x', L'0'); \
    Fn(short, 0x1, wchar_t, L'0', L'x', L'1'); \
    Fn(unsigned, 0xc, wchar_t, L'0', L'x', L'c'); \
    Fn(short, 0xa, wchar_t, L'0', L'x', L'a'); \
    Fn(unsigned, 0xC, wchar_t, L'0', L'x', L'C'); \
    Fn(short, 0xA, wchar_t, L'0', L'x', L'A'); \
    Fn(std::size_t, 0x42, wchar_t, L'0', L'x', L'4', L'2'); \
    Fn(int, -0x56, wchar_t, L'-', L'0', L'x', L'5', L'6'); \
    Fn(std::size_t, 0xE2, wchar_t, L'0', L'x', L'E', L'2'); \
    Fn(int, -0x5a, wchar_t, L'-', L'0', L'x', L'5', L'a'); \
    Fn(std::size_t, 0x42Bd, wchar_t, L'0', L'x', L'4', L'2', L'B', L'd'); \
    Fn(int, -0xaF56, wchar_t, L'-', L'0', L'x', L'a', L'F', L'5', L'6'); \
  } while (false)

FATAL_TEST(parser, literal) {
# define TEST_IMPL(T, Value, Char, ...) \
  do { \
    using expected = std::integral_constant<T, Value>; \
    \
    FATAL_EXPECT_SAME< \
      expected, \
      parser<T>::literal<Char, __VA_ARGS__> \
    >(); \
  } while (false)

  FATAL_IMPL_DECIMAL_LITERAL_PARSE_TEST_CALLS(TEST_IMPL);
  FATAL_IMPL_HEXADECIMAL_LITERAL_PARSE_TEST_CALLS(TEST_IMPL);

# undef TEST_IMPL
}

} // namespace fatal {
