/*
 *  Copyright (c) 2019, Marcelo Juchem
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#include <fatal/type/is_template.h>

#include <fatal/test/driver.h>

#include <memory>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace fatal {

template <std::size_t> struct T {};
template <std::size_t> struct S {};

template <typename T, bool Expected, template <typename...> class... TTemplates>
void check_is_template() {
  bool expected = Expected;
  using checker = is_template<TTemplates...>;
  bool actual = checker::template apply<T>::value;
  if (expected != actual) {
    FATAL_LOG(ERROR) << "checker: " << type_str<checker>();
    FATAL_LOG(ERROR) << "type: " << type_str<T>();
    FATAL_EXPECT_EQ(expected, actual);
  }
}

template <typename, typename...> struct X0 {};
template <typename, typename, typename...> struct X1 {};
template <typename, typename, typename, typename...> struct X2 {};
template <typename, typename, typename, typename, typename...> struct X3 {};

FATAL_TEST(traits, is_template) {
  check_is_template<X0<void>, true, X0>();
  check_is_template<X0<void, int>, true, X0>();
  check_is_template<X0<void, int, bool>, true, X0>();
  check_is_template<std::string, false, X0>();
  check_is_template<void, false, X0>();
  check_is_template<int, false, X0>();

  check_is_template<X1<void, double>, true, X1>();
  check_is_template<X1<void, double, int>, true, X1>();
  check_is_template<X1<void, double, int, bool>, true, X1>();
  check_is_template<std::string, false, X1>();
  check_is_template<void, false, X1>();
  check_is_template<int, false, X1>();

  check_is_template<X2<void, double, short>, true, X2>();
  check_is_template<X2<void, double, short, int>, true, X2>();
  check_is_template<X2<void, double, short, int, bool>, true, X2>();
  check_is_template<std::string, false, X2>();
  check_is_template<void, false, X2>();
  check_is_template<int, false, X2>();

  check_is_template<X3<void, double, short, float>, true, X3>();
  check_is_template<X3<void, double, short, float, int>, true, X3>();
  check_is_template<X3<void, double, short, float, int, bool>, true, X3>();
  check_is_template<std::string, false, X3>();
  check_is_template<void, false, X3>();
  check_is_template<int, false, X3>();

  check_is_template<std::string, false, std::tuple>();
  check_is_template<std::wstring, false, std::tuple>();
  check_is_template<std::basic_string<int>, false, std::tuple>();
  check_is_template<std::pair<int, double>, false, std::tuple>();
  check_is_template<std::vector<int>, false, std::tuple>();
  check_is_template<std::tuple<int>, true, std::tuple>();
  check_is_template<std::tuple<int, double>, true, std::tuple>();

  check_is_template<std::string, false, std::vector>();
  check_is_template<std::wstring, false, std::vector>();
  check_is_template<std::basic_string<int>, false, std::vector>();
  check_is_template<std::pair<int, double>, false, std::vector>();
  check_is_template<std::vector<int>, true, std::vector>();
  check_is_template<std::tuple<int>, false, std::vector>();
  check_is_template<std::tuple<int, double>, false, std::vector>();

  check_is_template<std::string, true, std::basic_string>();
  check_is_template<std::wstring, true, std::basic_string>();
  check_is_template<std::basic_string<int>, true, std::basic_string>();
  check_is_template<std::pair<int, double>, false, std::basic_string>();
  check_is_template<std::vector<int>, false, std::basic_string>();
  check_is_template<std::tuple<int>, false, std::basic_string>();
  check_is_template<std::tuple<int, double>, false, std::basic_string>();

  check_is_template<std::string, false, std::tuple, std::vector>();
  check_is_template<std::wstring, false, std::tuple, std::vector>();
  check_is_template<std::basic_string<int>, false, std::tuple, std::vector>();
  check_is_template<std::pair<int, double>, false, std::tuple, std::vector>();
  check_is_template<std::vector<int>, true, std::tuple, std::vector>();
  check_is_template<std::tuple<int>, true, std::tuple, std::vector>();
  check_is_template<std::tuple<int, double>, true, std::tuple, std::vector>();

  check_is_template<std::string, true, std::tuple, std::basic_string>();
  check_is_template<std::wstring, true, std::tuple, std::basic_string>();
  check_is_template<
    std::basic_string<int>, true,
    std::tuple, std::basic_string
  >();
  check_is_template<
    std::pair<int, double>, false,
    std::tuple, std::basic_string
  >();
  check_is_template<std::vector<int>, false, std::tuple, std::basic_string>();
  check_is_template<std::tuple<int>, true, std::tuple, std::basic_string>();
  check_is_template<
    std::tuple<int, double>, true,
    std::tuple, std::basic_string
  >();



  FATAL_EXPECT_FALSE(is_template<std::tuple>::apply<std::basic_string<char>>::value);
  FATAL_EXPECT_FALSE(is_template<std::basic_string>::apply<std::tuple<int>>::value);
  FATAL_EXPECT_TRUE(is_template<std::tuple>::apply<std::tuple<int>>::value);
  FATAL_EXPECT_TRUE(is_template<std::basic_string>::apply<std::basic_string<char>>::value);
}

FATAL_TEST(traits, is_template_any) {
  FATAL_EXPECT_FALSE(
    is_template<std::tuple>::any<
      std::basic_string<char>
    >::value
  );
  FATAL_EXPECT_FALSE(
    is_template<std::basic_string>::any<
      std::tuple<int>
    >::value
  );
  FATAL_EXPECT_TRUE(
    is_template<std::tuple>::any<
      std::tuple<int>
    >::value
  );
  FATAL_EXPECT_TRUE(
    is_template<std::basic_string>::any<
      std::basic_string<char>
    >::value
  );

  FATAL_EXPECT_TRUE(
    is_template<std::tuple>::any<
      std::tuple<int>,
      std::basic_string<char>
    >::value
  );
  FATAL_EXPECT_TRUE(
    is_template<std::tuple>::any<
      std::basic_string<char>,
      std::tuple<int>
    >::value
  );
  FATAL_EXPECT_TRUE(
    is_template<std::tuple>::any<
      std::basic_string<char>,
      std::tuple<int>,
      std::optional<int>
    >::value
  );
  FATAL_EXPECT_FALSE(
    is_template<std::tuple>::any<
      std::vector<int>
    >::value
  );
  FATAL_EXPECT_TRUE(
    is_template<std::tuple>::any<
      std::tuple<char>,
      std::tuple<int>,
      std::tuple<int, double>
    >::value
  );
  FATAL_EXPECT_TRUE(
    is_template<std::tuple>::any<
      std::tuple<char>,
      std::tuple<int>,
      std::tuple<int, double>,
      std::vector<int>
    >::value
  );

  FATAL_EXPECT_TRUE(
    is_template<
      std::tuple,
      std::basic_string,
      std::optional
    >::any<
      std::basic_string<char>
    >::value
  );
  FATAL_EXPECT_TRUE(
    is_template<
      std::tuple,
      std::basic_string,
      std::optional
    >::any<
      std::tuple<int>
    >::value
  );
  FATAL_EXPECT_TRUE(
    is_template<
      std::tuple,
      std::basic_string,
      std::optional
    >::any<
      std::optional<int>
    >::value
  );
  FATAL_EXPECT_TRUE(
    is_template<
      std::tuple,
      std::basic_string,
      std::optional
    >::any<
      std::tuple<int>,
      std::optional<int>
    >::value
  );
  FATAL_EXPECT_TRUE(
    is_template<
      std::tuple,
      std::basic_string,
      std::optional
    >::any<
      std::basic_string<char>,
      std::optional<int>
    >::value
  );
  FATAL_EXPECT_TRUE(
    is_template<
      std::tuple,
      std::basic_string,
      std::optional
    >::any<
      std::basic_string<char>,
      std::tuple<int>
    >::value
  );
  FATAL_EXPECT_TRUE(
    is_template<
      std::tuple,
      std::basic_string,
      std::optional
    >::any<
      std::basic_string<char>,
      std::tuple<int>,
      std::optional<int>
    >::value
  );
  FATAL_EXPECT_FALSE(
    is_template<
      std::tuple,
      std::basic_string,
      std::optional
    >::any<
      std::vector<int>
    >::value
  );
  FATAL_EXPECT_TRUE(
    is_template<
      std::tuple,
      std::basic_string,
      std::optional
    >::any<
      std::basic_string<char>,
      std::tuple<int>,
      std::optional<int>,
      std::vector<int>
    >::value
  );
}

FATAL_TEST(traits, is_template_all) {
  FATAL_EXPECT_FALSE(
    is_template<std::tuple>::all<
      std::basic_string<char>
    >::value
  );
  FATAL_EXPECT_FALSE(
    is_template<std::basic_string>::all<
      std::tuple<int>
    >::value
  );
  FATAL_EXPECT_TRUE(
    is_template<std::tuple>::all<
      std::tuple<int>
    >::value
  );
  FATAL_EXPECT_TRUE(
    is_template<std::basic_string>::all<
      std::basic_string<char>
    >::value
  );

  FATAL_EXPECT_FALSE(
    is_template<std::tuple>::all<
      std::tuple<int>,
      std::basic_string<char>
    >::value
  );
  FATAL_EXPECT_FALSE(
    is_template<std::tuple>::all<
      std::basic_string<char>,
      std::tuple<int>
    >::value
  );
  FATAL_EXPECT_FALSE(
    is_template<std::tuple>::all<
      std::basic_string<char>,
      std::tuple<int>,
      std::optional<int>
    >::value
  );
  FATAL_EXPECT_FALSE(
    is_template<std::tuple>::all<
      std::vector<int>
    >::value
  );
  FATAL_EXPECT_TRUE(
    is_template<std::tuple>::all<
      std::tuple<char>,
      std::tuple<int>,
      std::tuple<int, double>
    >::value
  );
  FATAL_EXPECT_FALSE(
    is_template<std::tuple>::all<
      std::tuple<char>,
      std::tuple<int>,
      std::tuple<int, double>,
      std::vector<int>
    >::value
  );

  FATAL_EXPECT_TRUE(
    is_template<
      std::tuple,
      std::basic_string,
      std::optional
    >::all<
      std::basic_string<char>
    >::value
  );
  FATAL_EXPECT_TRUE(
    is_template<
      std::tuple,
      std::basic_string,
      std::optional
    >::all<
      std::tuple<int>
    >::value
  );
  FATAL_EXPECT_TRUE(
    is_template<
      std::tuple,
      std::basic_string,
      std::optional
    >::all<
      std::optional<int>
    >::value
  );
  FATAL_EXPECT_TRUE(
    is_template<
      std::tuple,
      std::basic_string,
      std::optional
    >::all<
      std::tuple<int>,
      std::optional<int>
    >::value
  );
  FATAL_EXPECT_TRUE(
    is_template<
      std::tuple,
      std::basic_string,
      std::optional
    >::all<
      std::basic_string<char>,
      std::optional<int>
    >::value
  );
  FATAL_EXPECT_TRUE(
    is_template<
      std::tuple,
      std::basic_string,
      std::optional
    >::all<
      std::basic_string<char>,
      std::tuple<int>
    >::value
  );
  FATAL_EXPECT_TRUE(
    is_template<
      std::tuple,
      std::basic_string,
      std::optional
    >::all<
      std::basic_string<char>,
      std::tuple<int>,
      std::optional<int>
    >::value
  );
  FATAL_EXPECT_FALSE(
    is_template<
      std::tuple,
      std::basic_string,
      std::optional
    >::all<
      std::vector<int>
    >::value
  );
  FATAL_EXPECT_FALSE(
    is_template<
      std::tuple,
      std::basic_string,
      std::optional
    >::all<
      std::basic_string<char>,
      std::tuple<int>,
      std::optional<int>,
      std::vector<int>
    >::value
  );
}

FATAL_TEST(traits, is_template_v) {
  FATAL_EXPECT_FALSE(is_template_v<std::tuple, std::basic_string<char>>);
  FATAL_EXPECT_FALSE(is_template_v<std::basic_string, std::tuple<int>>);
  FATAL_EXPECT_TRUE(is_template_v<std::tuple, std::tuple<int>>);
  FATAL_EXPECT_TRUE(is_template_v<std::basic_string, std::basic_string<char>>);
}

FATAL_TEST(traits, is_template_any_v) {
  FATAL_EXPECT_FALSE(
    is_template_any_v<
      std::tuple,
      std::basic_string<char>
    >
  );
  FATAL_EXPECT_FALSE(
    is_template_any_v<
      std::basic_string,
      std::tuple<int>
    >
  );
  FATAL_EXPECT_TRUE(
    is_template_any_v<
      std::tuple,
      std::tuple<int>
    >
  );
  FATAL_EXPECT_TRUE(
    is_template_any_v<
      std::basic_string,
      std::basic_string<char>
    >
  );

  FATAL_EXPECT_TRUE(
    is_template_any_v<
      std::tuple,
      std::tuple<int>,
      std::basic_string<char>
    >
  );
  FATAL_EXPECT_TRUE(
    is_template_any_v<
      std::tuple,
      std::basic_string<char>,
      std::tuple<int>
    >
  );
  FATAL_EXPECT_TRUE(
    is_template_any_v<
      std::tuple,
      std::basic_string<char>,
      std::tuple<int>,
      std::optional<int>
    >
  );
  FATAL_EXPECT_FALSE(
    is_template_any_v<
      std::tuple,
      std::vector<int>
    >
  );
  FATAL_EXPECT_TRUE(
    is_template_any_v<
      std::tuple,
      std::tuple<char>,
      std::tuple<int>,
      std::tuple<int, double>
    >
  );
  FATAL_EXPECT_TRUE(
    is_template_any_v<
      std::tuple,
      std::tuple<char>,
      std::tuple<int>,
      std::tuple<int, double>,
      std::vector<int>
    >
  );
}

FATAL_TEST(traits, is_template_all_v) {
  FATAL_EXPECT_FALSE(
    is_template_all_v<
      std::tuple,
      std::basic_string<char>
    >
  );
  FATAL_EXPECT_FALSE(
    is_template_all_v<
      std::basic_string,
      std::tuple<int>
    >
  );
  FATAL_EXPECT_TRUE(
    is_template_all_v<
      std::tuple,
      std::tuple<int>
    >
  );
  FATAL_EXPECT_TRUE(
    is_template_all_v<
      std::basic_string,
      std::basic_string<char>
    >
  );

  FATAL_EXPECT_FALSE(
    is_template_all_v<
      std::tuple,
      std::tuple<int>,
      std::basic_string<char>
    >
  );
  FATAL_EXPECT_FALSE(
    is_template_all_v<
      std::tuple,
      std::basic_string<char>,
      std::tuple<int>
    >
  );
  FATAL_EXPECT_FALSE(
    is_template_all_v<
      std::tuple,
      std::basic_string<char>,
      std::tuple<int>,
      std::optional<int>
    >
  );
  FATAL_EXPECT_FALSE(
    is_template_all_v<
      std::tuple,
      std::vector<int>
    >
  );
  FATAL_EXPECT_TRUE(
    is_template_all_v<
      std::tuple,
      std::tuple<char>,
      std::tuple<int>,
      std::tuple<int, double>
    >
  );
  FATAL_EXPECT_FALSE(
    is_template_all_v<
      std::tuple,
      std::tuple<char>,
      std::tuple<int>,
      std::tuple<int, double>,
      std::vector<int>
    >
  );
}

} // namespace fatal {
