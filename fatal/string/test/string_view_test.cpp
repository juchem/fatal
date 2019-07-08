/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#include <fatal/string/string_view.h>

#include <fatal/test/driver.h>

struct any_matcher {
  bool operator ()(char) const { return true; }
};

struct char_matcher {
  explicit char_matcher(char c): c_(c) {}

  bool operator ()(char c) const { return c == c_; }

private:
  char c_;
};

struct letter_matcher {
  bool operator ()(char c) const { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
};

namespace fatal {

#define TEST_IMPL(Haystack, Result, Operation, ...) \
  do { \
    string_view haystack(Haystack); \
    auto const result = haystack.Operation(__VA_ARGS__); \
    FATAL_EXPECT_EQ(Result, result); \
    FATAL_EXPECT_EQ(Haystack, haystack); \
  } while (false)

FATAL_TEST(string_view, slice) {
  TEST_IMPL("", "",  slice, 0, 0);
  TEST_IMPL("foobar", "", slice, 0, 0);
  TEST_IMPL("foobar", "f", slice, 0, 1);
  TEST_IMPL("foobar", "foo", slice, 0, 3);
  TEST_IMPL("foobar", "bar", slice, 3, 6);
  TEST_IMPL("foobar", "foobar", slice, 0, 6);
}

FATAL_TEST(string_view, find_first_of) {
}

FATAL_TEST(string_view, find_first_not_of) {
}

#undef TEST_IMPL

#define TEST_IMPL(Haystack, Result, Remaining, Operation, ...) \
  do { \
    string_view haystack(Haystack); \
    auto const result = haystack.Operation(__VA_ARGS__); \
    FATAL_EXPECT_EQ(Result, result); \
    FATAL_EXPECT_EQ(Remaining, haystack); \
  } while (false)

FATAL_TEST(string_view, seek_past) {
  TEST_IMPL("", "", "", seek_past, ' ');
  TEST_IMPL("", "", "", seek_past, '\0');
  TEST_IMPL("", "", "", seek_past, '\n');

  TEST_IMPL("hello, world", "hello,", "world", seek_past, ' ');
  TEST_IMPL("hello, world", "hello, world", "", seek_past, '\0');
  TEST_IMPL("hello, world", "hello, world", "", seek_past, '\n');
  TEST_IMPL("hello, world", "he", "lo, world", seek_past, 'l');
  TEST_IMPL("hello, world", "", "ello, world", seek_past, 'h');
  TEST_IMPL("hello, world", "hello, worl", "", seek_past, 'd');
}

FATAL_TEST(string_view, seek_for) {
  TEST_IMPL("", "", "", seek_for, ' ');
  TEST_IMPL("", "", "", seek_for, '\0');
  TEST_IMPL("", "", "", seek_for, '\n');

  TEST_IMPL("hello, world", "hello,", " world", seek_for, ' ');
  TEST_IMPL("hello, world", "hello, world", "", seek_for, '\0');
  TEST_IMPL("hello, world", "hello, world", "", seek_for, '\n');
  TEST_IMPL("hello, world", "he", "llo, world", seek_for, 'l');
  TEST_IMPL("hello, world", "", "hello, world", seek_for, 'h');
  TEST_IMPL("hello, world", "hello, worl", "d", seek_for, 'd');
}

FATAL_TEST(string_view, seek) {
  TEST_IMPL("", "", "", seek, 0);

  TEST_IMPL("hello, world", "", "hello, world", seek, 0);
  TEST_IMPL("hello, world", "h", "ello, world", seek, 1);
  TEST_IMPL("hello, world", "hello", ", world", seek, 5);
  TEST_IMPL("hello, world", "hello,", " world", seek, 6);
  TEST_IMPL("hello, world", "hello, ", "world", seek, 7);
  TEST_IMPL("hello, world", "hello, worl", "d", seek, 11);
  TEST_IMPL("hello, world", "hello, world", "", seek, 12);
}

FATAL_TEST(string_view, seek_over_match) {
  TEST_IMPL("", "", skip_over_match, letter_matcher());
  TEST_IMPL("hello., world", "hello., world", skip_over_match, char_matcher(' '));
  TEST_IMPL("hello., world", "., world", skip_over_match, letter_matcher());
  TEST_IMPL("hello., world", "hello., world", skip_over_match, char_matcher(' '));
  TEST_IMPL("hello., world", "hello., world", skip_over_match, char_matcher(','));
  TEST_IMPL("hello., world", "hello., world", skip_over_match, char_matcher('.'));
  TEST_IMPL("hello., world", "., world", skip_over_match, letter_matcher());
  TEST_IMPL("hello., world", "", skip_over_match, any_matcher());
}

FATAL_TEST(string_view, seek_for_match) {
  TEST_IMPL("", "", skip_to_match, char_matcher('-'));
  TEST_IMPL("hello., world", "", skip_to_match, char_matcher('-'));
  TEST_IMPL("hello., world", " world", skip_to_match, char_matcher(' '));
  TEST_IMPL("hello., world", ", world", skip_to_match, char_matcher(','));
  TEST_IMPL("hello., world", "., world", skip_to_match, char_matcher('.'));
  TEST_IMPL("hello., world", "hello., world", skip_to_match, letter_matcher());
  TEST_IMPL("hello., world", "hello., world", skip_to_match, any_matcher());
}

#undef TEST_IMPL

#define TEST_IMPL(Haystack, Remaining, Operation, ...) \
  do { \
    string_view haystack(Haystack); \
    auto const &result = haystack.Operation(__VA_ARGS__); \
    FATAL_EXPECT_EQ(std::addressof(haystack), std::addressof(result)); \
    FATAL_EXPECT_EQ(Remaining, result); \
    FATAL_EXPECT_EQ(Remaining, haystack); \
  } while (false)

FATAL_TEST(string_view, skip_past) {
  TEST_IMPL("", "", skip_past, ' ');
  TEST_IMPL("", "", skip_past, '\0');
  TEST_IMPL("", "", skip_past, '\n');

  TEST_IMPL("hello, world", "world", skip_past, ' ');
  TEST_IMPL("hello, world", "", skip_past, '\0');
  TEST_IMPL("hello, world", "", skip_past, '\n');
  TEST_IMPL("hello, world", "lo, world", skip_past, 'l');
  TEST_IMPL("hello, world", "ello, world", skip_past, 'h');
  TEST_IMPL("hello, world", "", skip_past, 'd');
}

FATAL_TEST(string_view, skip_to) {
  TEST_IMPL("", "", skip_to, ' ');
  TEST_IMPL("", "", skip_to, '\0');
  TEST_IMPL("", "", skip_to, '\n');

  TEST_IMPL("hello, world", " world", skip_to, ' ');
  TEST_IMPL("hello, world", "", skip_to, '\0');
  TEST_IMPL("hello, world", "", skip_to, '\n');
  TEST_IMPL("hello, world", "llo, world", skip_to, 'l');
  TEST_IMPL("hello, world", "hello, world", skip_to, 'h');
  TEST_IMPL("hello, world", "d", skip_to, 'd');
}

FATAL_TEST(string_view, skip) {
  TEST_IMPL("", "", skip, 0);

  TEST_IMPL("hello, world", "hello, world", skip, 0);
  TEST_IMPL("hello, world", "ello, world", skip, 1);
  TEST_IMPL("hello, world", ", world", skip, 5);
  TEST_IMPL("hello, world", " world", skip, 6);
  TEST_IMPL("hello, world", "world", skip, 7);
  TEST_IMPL("hello, world", "d", skip, 11);
  TEST_IMPL("hello, world", "", skip, 12);
}

FATAL_TEST(string_view, skip_over_match) {
  TEST_IMPL("", "", skip_over_match, letter_matcher());
  TEST_IMPL("hello., world", "hello., world", skip_over_match, char_matcher(' '));
  TEST_IMPL("hello., world", "., world", skip_over_match, letter_matcher());
  TEST_IMPL("hello., world", "hello., world", skip_over_match, char_matcher(' '));
  TEST_IMPL("hello., world", "hello., world", skip_over_match, char_matcher(','));
  TEST_IMPL("hello., world", "hello., world", skip_over_match, char_matcher('.'));
  TEST_IMPL("hello., world", "., world", skip_over_match, letter_matcher());
  TEST_IMPL("hello., world", "", skip_over_match, any_matcher());
}

FATAL_TEST(string_view, skip_to_match) {
  TEST_IMPL("", "", skip_to_match, char_matcher('-'));
  TEST_IMPL("hello., world", "", skip_to_match, char_matcher('-'));
  TEST_IMPL("hello., world", " world", skip_to_match, char_matcher(' '));
  TEST_IMPL("hello., world", ", world", skip_to_match, char_matcher(','));
  TEST_IMPL("hello., world", "., world", skip_to_match, char_matcher('.'));
  TEST_IMPL("hello., world", "hello., world", skip_to_match, letter_matcher());
  TEST_IMPL("hello., world", "hello., world", skip_to_match, any_matcher());
}

#undef TEST_IMPL

} // namespace fatal {
