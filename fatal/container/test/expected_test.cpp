/*
 *  Copyright (c) 2019, Marcelo Juchem
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#include <fatal/container/expected.h>

#include <fatal/test/driver.h>

#include <vector>
#include <unordered_map>

namespace fatal {

struct custom_error:
  public std::exception
{
  custom_error() = default;

  template <typename... Args>
  explicit custom_error(std::string_view message, Args &&...args):
    message_(message),
    code_((0 + ... + args))
  {}

  custom_error(custom_error const &) = default;
  custom_error(custom_error &&) = default;

  bool operator !=(custom_error const &rhs) const { return !(*this == rhs); }
  bool operator ==(custom_error const &rhs) const {
    return message_ == rhs.message_ && code_ == rhs.code_;
  }

  auto const &message() const { return message_; }

private:
  std::string_view const message_;
  int const code_ = 0;
};

FATAL_TEST(expected, success example) {
  // simulates a function that returns a success value through `expected`
  auto computation = []() -> expected<int, custom_error> {
    return 10;
  };

  FATAL_EXPECT_NO_THROW {
    auto result = computation();

    if (!result) {
      FATAL_EXPECT_UNREACHABLE();
    } else {
      FATAL_EXPECT_EQ(10, result.value());
    }

    result.try_raise();
  };
}

FATAL_TEST(expected, success with arguments example) {
  // simulates a function that returns a success value constructed with arguments through `expected`
  auto computation = []() -> expected<std::string, custom_error> {
    return {"test string"};
  };

  FATAL_EXPECT_NO_THROW {
    auto result = computation();

    if (!result) {
      FATAL_EXPECT_UNREACHABLE();
    } else {
      FATAL_EXPECT_EQ("test string", result.value());
    }

    result.try_raise();
  };
}

FATAL_TEST(expected, success with multiple arguments example) {
  // simulates a function that returns a success value constructed with multiple arguments
  // through `expected`
  auto computation = []() -> expected<std::pair<std::string, int>, custom_error> {
    return {"test string", 100};
  };

  FATAL_EXPECT_NO_THROW {
    auto result = computation();

    if (!result) {
      FATAL_EXPECT_UNREACHABLE();
    } else {
      FATAL_EXPECT_EQ("test string", result.value().first);
      FATAL_EXPECT_EQ(100, result.value().second);
    }

    result.try_raise();
  };
}

FATAL_TEST(expected, failure code example) {
  // simulates a function that returns an error code through `expected`
  auto computation = []() -> expected<int, long> {
    return {unexpected, -1};
  };

  FATAL_EXPECT_NO_THROW {
    auto result = computation();

    if (!result) {
      FATAL_EXPECT_EQ(-1, result.error());
    } else {
      FATAL_EXPECT_UNREACHABLE();
    }

    // we can't call `result.raise()` nor `result.try_raise()` because that would fail
    // to compile - this is intended because the default policy for non-exception error
    // types is to not allow throwing
  };
}

FATAL_TEST(expected, default-constructed failure example) {
  // simulates a function that returns a default constructed error through `expected`
  auto computation = []() -> expected<int, custom_error> {
    return unexpected;
  };

  FATAL_EXPECT_THROW(custom_error) {
    auto result = computation();

    if (!result) {
      FATAL_EXPECT_EQ(custom_error(), result.error());

      result.raise();
    } else {
      FATAL_EXPECT_UNREACHABLE();
    }
  };

  FATAL_EXPECT_THROW(custom_error) {
    auto result = computation();

    result.try_raise();

    FATAL_EXPECT_UNREACHABLE();
  };
}

FATAL_TEST(expected, default-constructed success example) {
  // simulates a function that returns a default constructed value through `expected`
  auto computation = []() -> expected<int, custom_error> {
    return {};
  };

  FATAL_EXPECT_NO_THROW {
    auto result = computation();

    if (!result) {
      FATAL_EXPECT_UNREACHABLE();
    } else {
      FATAL_EXPECT_EQ(int{}, result.value());
    }

    result.try_raise();
  };
}

FATAL_TEST(expected, failure with arguments example) {
  // simulates a function that returns an error constructed with arguments through `expected`
  auto computation = []() -> expected<int, custom_error> {
    return {unexpected, "error message"};
  };

  FATAL_EXPECT_THROW(custom_error) {
    auto result = computation();

    if (!result) {
      FATAL_EXPECT_EQ(custom_error("error message"), result.error());

      result.raise();
    } else {
      FATAL_EXPECT_UNREACHABLE();
    }
  };

  FATAL_EXPECT_THROW(custom_error) {
    auto result = computation();

    result.try_raise();

    FATAL_EXPECT_UNREACHABLE();
  };
}

FATAL_TEST(expected, failure with multiple arguments example) {
  // simulates a function that returns an error constructed with multiple arguments
  // through `expected`
  auto computation = []() -> expected<int, custom_error> {
    return {unexpected, "error message", 1, 2, 3};
  };

  FATAL_EXPECT_THROW(custom_error) {
    auto result = computation();

    if (!result) {
      FATAL_EXPECT_EQ(custom_error("error message", 1, 2, 3), result.error());

      result.raise();
    } else {
      FATAL_EXPECT_UNREACHABLE();
    }
  };

  FATAL_EXPECT_THROW(custom_error) {
    auto result = computation();

    result.try_raise();

    FATAL_EXPECT_UNREACHABLE();
  };
}



FATAL_TEST(expected, default_ctor int / int) {
  expected<int, int> e;

  FATAL_EXPECT_TRUE(e.has_value());
  FATAL_EXPECT_NOT_NULL(e.try_value());

  FATAL_EXPECT_NULL(e.try_error());
}

FATAL_TEST(expected, value int / int) {
  expected<int, int> e(10);

  FATAL_EXPECT_TRUE(e.has_value());

  FATAL_EXPECT_EQ(10, e.value());

  FATAL_ASSERT_NOT_NULL(e.try_value());
  FATAL_EXPECT_EQ(10, *e.try_value());

  FATAL_EXPECT_NULL(e.try_error());
}

FATAL_TEST(expected, error int / int) {
  expected<int, int> e(unexpected, 5);

  FATAL_EXPECT_FALSE(e.has_value());

  FATAL_ASSERT_NULL(e.try_value());

  FATAL_EXPECT_NOT_NULL(e.try_error());
  FATAL_EXPECT_EQ(5, *e.try_error());
}



FATAL_TEST(expected, default_ctor int / custom_error) {
  expected<int, custom_error> e;

  FATAL_EXPECT_TRUE(e.has_value());
  FATAL_EXPECT_NOT_NULL(e.try_value());

  FATAL_EXPECT_NULL(e.try_error());
}

FATAL_TEST(expected, value int / custom_error) {
  expected<int, custom_error> e(10);

  FATAL_EXPECT_TRUE(e.has_value());

  FATAL_EXPECT_EQ(10, e.value());

  FATAL_ASSERT_NOT_NULL(e.try_value());
  FATAL_EXPECT_EQ(10, *e.try_value());

  FATAL_EXPECT_NULL(e.try_error());
}

FATAL_TEST(expected, error int / custom_error) {
  expected<int, custom_error> e(unexpected, "message");

  FATAL_EXPECT_FALSE(e.has_value());

  FATAL_ASSERT_NULL(e.try_value());

  FATAL_EXPECT_NOT_NULL(e.try_error());
  FATAL_EXPECT_EQ(custom_error("message"), *e.try_error());

  FATAL_EXPECT_THROW(custom_error) { e.raise(); };
}



FATAL_TEST(expected, default_ctor custom_error / int) {
  expected<custom_error, int> e;

  FATAL_EXPECT_TRUE(e.has_value());
  FATAL_EXPECT_NOT_NULL(e.try_value());

  FATAL_EXPECT_NULL(e.try_error());
}

FATAL_TEST(expected, value custom_error / int) {
  expected<custom_error, int> e("result");

  FATAL_EXPECT_TRUE(e.has_value());

  FATAL_EXPECT_EQ(custom_error("result"), e.value());

  FATAL_ASSERT_NOT_NULL(e.try_value());
  FATAL_EXPECT_EQ(custom_error("result"), *e.try_value());

  FATAL_EXPECT_NULL(e.try_error());
}

FATAL_TEST(expected, error custom_error / int) {
  expected<custom_error, int> e(unexpected, 5);

  FATAL_EXPECT_FALSE(e.has_value());

  FATAL_ASSERT_NULL(e.try_value());

  FATAL_EXPECT_NOT_NULL(e.try_error());
  FATAL_EXPECT_EQ(5, *e.try_error());
}



FATAL_TEST(expected, default_ctor custom_error / custom_error) {
  expected<custom_error, custom_error> e;

  FATAL_EXPECT_TRUE(e.has_value());
  FATAL_EXPECT_NOT_NULL(e.try_value());

  FATAL_EXPECT_NULL(e.try_error());
}

FATAL_TEST(expected, value custom_error / custom_error) {
  expected<custom_error, custom_error> e("result");

  FATAL_EXPECT_TRUE(e.has_value());

  FATAL_EXPECT_EQ(custom_error("result"), e.value());

  FATAL_ASSERT_NOT_NULL(e.try_value());
  FATAL_EXPECT_EQ(custom_error("result"), *e.try_value());

  FATAL_EXPECT_NULL(e.try_error());
}

FATAL_TEST(expected, error custom_error / custom_error) {
  expected<custom_error, custom_error> e(unexpected, "message");

  FATAL_EXPECT_FALSE(e.has_value());

  FATAL_ASSERT_NULL(e.try_value());

  FATAL_EXPECT_NOT_NULL(e.try_error());
  FATAL_EXPECT_EQ(custom_error("message"), *e.try_error());

  FATAL_EXPECT_THROW(custom_error) { e.raise(); };
}

FATAL_TEST(expected, on_error with value) {
  auto computation = []() -> expected<int, custom_error> {
    return 10;
  };

  FATAL_EXPECT_NO_THROW {
    computation().on_error([]{
      throw custom_error();
    });
  };
}

FATAL_TEST(expected, on_error with error / no arguments) {
  auto computation = []() -> expected<int, custom_error> {
    return {unexpected, "error message"};
  };

  FATAL_EXPECT_THROW(custom_error) {
    computation().on_error([]{
      throw custom_error();
    });
  };
}

FATAL_TEST(expected, on_error with error / error argument) {
  auto computation = []() -> expected<int, custom_error> {
    return {unexpected, "error message"};
  };

  FATAL_EXPECT_THROW(custom_error) {
    computation().on_error([](custom_error const &e){
      throw e;
    });
  };
}


FATAL_TEST(expected, recover with value (const)) {
  expected<int, custom_error> const e{10};

  FATAL_EXPECT_EQ(10, e.recover([]{ return 5; }));
}

FATAL_TEST(expected, recover with value) {
  expected<int, custom_error> e{10};

  FATAL_EXPECT_EQ(10, e.recover([]{ return 5; }));
}

FATAL_TEST(expected, recover with error (const)) {
  expected<int, custom_error> const e{unexpected, "error message"};

  FATAL_EXPECT_EQ(5, e.recover([]{ return 5; }));
}

FATAL_TEST(expected, recover with error) {
  expected<int, custom_error> e{unexpected, "error message"};

  FATAL_EXPECT_EQ(5, e.recover([]{ return 5; }));
}


FATAL_TEST(expected, recover_with with value (const)) {
  expected<int, custom_error> const e{10};

  FATAL_EXPECT_EQ(10, e.recover_with(5));
}

FATAL_TEST(expected, recover_with with value) {
  expected<int, custom_error> e{10};

  FATAL_EXPECT_EQ(10, e.recover_with(5));
}

FATAL_TEST(expected, recover_with with error (const)) {
  expected<int, custom_error> const e{unexpected, "error message"};

  FATAL_EXPECT_EQ(5, e.recover_with(5));
}

FATAL_TEST(expected, recover_with with error) {
  expected<int, custom_error> e{unexpected, "error message"};

  FATAL_EXPECT_EQ(5, e.recover_with(5));
}


FATAL_TEST(expected, try_raise on success) {
  expected<int, custom_error> e{10};

  FATAL_EXPECT_EQ(10, e.try_raise().value());
}

FATAL_TEST(expected, try_raise on success) {
  expected<int, custom_error> e{unexpected, "error message"};

  FATAL_EXPECT_THROW(custom_error) {
    FATAL_EXPECT_EQ(10, e.try_raise().value());
  };
}


FATAL_TEST(expected, value checked_expected) {
  checked_expected<int, int> e;

  FATAL_EXPECT_NO_THROW { e.value(); };
  FATAL_EXPECT_THROW(std::logic_error) { e.error(); };
}

FATAL_TEST(expected, error checked_expected) {
  checked_expected<int, int> e(unexpected);

  FATAL_EXPECT_THROW(std::logic_error) { e.value(); };
  FATAL_EXPECT_NO_THROW { e.error(); };
}

} // namespace fatal {
