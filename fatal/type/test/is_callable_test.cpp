/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#include <fatal/type/is_callable.h>

#include <fatal/test/driver.h>

#include <string>

namespace ftl {

struct foonctor {
  void operator ()() {}
  void operator ()(int, std::string) {}
};

using foonction = void(*)();
using foonction_is = void(*)(int, std::string);

FATAL_TEST(traits, is_callable) {
  auto const lambda = []() {};
  auto const lambda_is = [](int, std::string) {};

  FATAL_EXPECT_TRUE((is_callable_v<foonctor>));
  FATAL_EXPECT_FALSE((is_callable_v<foonctor, int>));
  FATAL_EXPECT_FALSE((is_callable_v<foonctor, int, double>));
  FATAL_EXPECT_TRUE((is_callable_v<foonctor, int, std::string>));

  FATAL_EXPECT_TRUE((is_callable_v<decltype(lambda)>));
  FATAL_EXPECT_FALSE((is_callable_v<decltype(lambda), int>));
  FATAL_EXPECT_FALSE((is_callable_v<decltype(lambda), int, double>));
  FATAL_EXPECT_FALSE((is_callable_v<decltype(lambda), int, std::string>));

  FATAL_EXPECT_FALSE((is_callable_v<decltype(lambda_is)>));
  FATAL_EXPECT_FALSE((is_callable_v<decltype(lambda_is), int>));
  FATAL_EXPECT_FALSE((is_callable_v<decltype(lambda_is), int, double>));
  FATAL_EXPECT_TRUE((
    is_callable_v<decltype(lambda_is), int, std::string>
  ));

  FATAL_EXPECT_TRUE((is_callable_v<foonction>));
  FATAL_EXPECT_FALSE((is_callable_v<foonction, int>));
  FATAL_EXPECT_FALSE((is_callable_v<foonction, int, double>));
  FATAL_EXPECT_FALSE((is_callable_v<foonction, int, std::string>));

  FATAL_EXPECT_FALSE((is_callable_v<foonction_is>));
  FATAL_EXPECT_FALSE((is_callable_v<foonction_is, int>));
  FATAL_EXPECT_FALSE((is_callable_v<foonction_is, int, double>));
  FATAL_EXPECT_TRUE((is_callable_v<foonction_is, int, std::string>));
}

} // namespace ftl {
