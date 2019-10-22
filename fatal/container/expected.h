/*
 *  Copyright (c) 2019, Marcelo Juchem
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#ifndef FATAL_INCLUDE_fatal_container_expected_h
#define FATAL_INCLUDE_fatal_container_expected_h

#include <fatal/type/safe_overload.h>
#include <fatal/type/is_callable.h>

#include <exception>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <variant>

#include <cstdlib>
#include <cassert>

namespace fatal {

////////////////////
// error policies //
////////////////////

/**
 * An error policy for `expected` for throwing unexpected errors (e.g.: exceptions).
 *
 * Author: Marcelo Juchem <juchem at gmail dot com>
 */
template <typename Error>
struct throwable_expected_error_policy {
  using error_type = Error;

  /**
   * Throws the given error.
   *
   * This function should not return.
   */
  [[ noreturn ]]
  static void raise(error_type const &error) {
    throw error;
  }

  /**
   * Tells whether this is a throwing error policy or not.
   */
  static constexpr bool throwing() { return true; }
};

/**
 * An error policy for `expected` for value-based errors (e.g.: error codes)
 * which doesn't support throwing..
 *
 * Author: Marcelo Juchem <juchem at gmail dot com>
 */
template <typename Error>
struct error_value_expected_error_policy {
  using error_type = Error;

  /**
   * Tells whether this is a throwing error policy or not.
   */
  static constexpr bool throwing() { return false; }
};

/**
 * Resolves to either `throwable_expected_error_policy` if `Error` is a subclass of `std::exception`
 * or `error_value_expected_error_policy` otherwise.
 *
 * Author: Marcelo Juchem <juchem at gmail dot com>
 */
template <typename Error>
using default_expected_error_policy = std::conditional_t<
  std::is_base_of_v<std::exception, Error>,
  throwable_expected_error_policy<Error>,
  error_value_expected_error_policy<Error>
>;

////////////////////
// check policies //
////////////////////

/**
 * A check policy for `expected` that asserts pre-conditions.
 *
 * Author: Marcelo Juchem <juchem at gmail dot com>
 */
struct assert_expected_check_policy {
  /**
   * Asserts that `expected` contains a value.
   */
  template <typename Expected>
  static void check_is_value(Expected const &expected) {
    assert(expected.has_value());
  }

  /**
   * Asserts that `expected` contains an error.
   */
  template <typename Expected>
  static void check_is_error(Expected const &expected) {
    assert(!expected.has_value());
  }
};

/**
 * A check policy for `expected` that throws on failed pre-conditions.
 *
 * Author: Marcelo Juchem <juchem at gmail dot com>
 */
struct throwing_expected_check_policy {
  template <typename Expected>
  /**
   * Checks that `expected` contains a value and throws a `std::logic_error` if not.
   */
  static void check_is_value(Expected const &expected) {
    if (!expected.has_value()) {
      throw std::logic_error("attempting to retrieve the value out of an unexpected error");
    }
  }

  /**
   * Checks that `expected` contains an error and throws a `std::logic_error` if not.
   */
  template <typename Expected>
  static void check_is_error(Expected const &expected) {
    if (expected.has_value()) {
      throw std::logic_error("attempting to retrieve the error out of an expected value");
    }
  }
};

using default_expected_check_policy = assert_expected_check_policy;

/**
 * A tag for unexcpected error conditions.
 *
 * Used in conjunction with `expected`.
 *
 * Author: Marcelo Juchem <juchem at gmail dot com>
 */
struct unexpected_t {};

constexpr unexpected_t unexpected = {};

/**
 * A wrapper of return values for return-based error handling.
 *
 * Allows the representation of both expected return values and unexpected error conditions.
 *
 * Author: Marcelo Juchem <juchem at gmail dot com>
 */
template <
  typename T,
  typename Error,
  typename CheckPolicy = default_expected_check_policy,
  template <typename> typename ErrorPolicy = default_expected_error_policy
>
struct expected {
  using value_type = T;
  using error_type = Error;
  using error_policy = ErrorPolicy<Error>;
  using check_policy = CheckPolicy;

private:
  struct error_t {
    template <typename... UArgs, typename = fatal::safe_overload<error_t, UArgs...>>
    explicit error_t(UArgs &&...args):
      error_(std::forward<UArgs>(args)...)
    {}

    error_t(error_t const &) = default;
    error_t(error_t &&) = default;

    /**
     * Getter for the wrapped error.
     */
    auto const &get() const { return error_; }

    /**
     * Getter for the wrapped error.
     */
    auto &get() { return error_; }

  private:
    error_type error_;
  };

public:

  /**
   * Constructs an `expected` with inplace construction of value from the given arguments.
   */
  /* implicit */
  template <
    typename... UArgs,
    typename = fatal::safe_overload<expected, UArgs...>,
    typename = std::enable_if_t<
      sizeof...(UArgs) != 1
        || !(std::is_same_v<unexpected_t, UArgs> && ... && true)
    >
  >
  expected(UArgs &&...args):
    data_(std::in_place_type<value_type>, std::forward<UArgs>(args)...)
  {}

  /**
   * Constructs an `expected` with an unexpected error.
   */
  template <typename... UArgs>
  expected(unexpected_t, UArgs &&...args):
    data_(std::in_place_type<error_t>, std::forward<UArgs>(args)...)
  {}

  expected(expected const &) = default;
  expected(expected &&) = default;

  /**
   * Getter for the wrapped value.
   *
   * Calls the check policy to ensure this object contains a value.
   */
  value_type const &value() const {
    check_policy::check_is_value(*this);
    return std::get<value_type>(data_);
  }

  /**
   * Getter for the wrapped value.
   *
   * Calls the check policy to ensure this object contains a value.
   */
  value_type &value() {
    check_policy::check_is_value(*this);
    return std::get<value_type>(data_);
  }

  /**
   * Conditional getter for the wrapped value.
   *
   * Returns a pointer to the value if present, or `nullptr` otherwise.
   */
  value_type const *try_value() const {
    if (!has_value()) { return nullptr; }
    return std::addressof(value());
  }

  /**
   * Conditional getter for the wrapped value.
   *
   * Returns a pointer to the value if present, or `nullptr` otherwise.
   */
  value_type *try_value() {
    if (!has_value()) { return nullptr; }
    return std::addressof(value());
  }

  /**
   * Getter for the wrapped error.
   *
   * Calls the check policy to ensure this object contains an error.
   */
  error_type const &error() const {
    check_policy::check_is_error(*this);
    return std::get<error_t>(data_).get();
  }

  /**
   * Getter for the wrapped error object.
   *
   * Calls the check policy to ensure this object contains an error.
   */
  error_type &error() {
    check_policy::check_is_error(*this);
    return std::get<error_t>(data_).get();
  }

  /**
   * Conditional getter for the wrapped error.
   *
   * Returns a pointer to the error if present, or `nullptr` otherwise.
   */
  error_type const *try_error() const {
    if (has_value()) { return nullptr; }
    return std::addressof(error());
  }

  /**
   * Conditional getter for the wrapped error.
   *
   * Returns a pointer to the error if present, or `nullptr` otherwise.
   */
  error_type *try_error() {
    if (has_value()) { return nullptr; }
    return std::addressof(error());
  }

  /**
   * Returns `true` if a value is present, otherwise returns `false` as an error is.
   */
  bool has_value() const { return !data_.index(); }

  /**
   * Calls the given function `fn` if a value is present, otherwise doesn't do anything.
   *
   * `fn` will be given the value as its sole argument if such overload exists, otherwise
   * it will be called with no arguments.
   */
  template <typename Fn>
  expected const &on_value(Fn &&fn) const {
    if (has_value()) {
      if constexpr (is_callable_v<Fn &&, value_type const &>) {
        std::forward<Fn>(fn)(value());
      } else {
        std::forward<Fn>(fn)();
      }
    }

    return *this;
  }

  /**
   * Calls the given function `fn` if a value is present, otherwise doesn't do anything.
   *
   * `fn` will be given the value as its sole argument if such overload exists, otherwise
   * it will be called with no arguments.
   */
  template <typename Fn>
  expected &on_value(Fn &&fn) {
    if (has_value()) {
      if constexpr (is_callable_v<Fn &&, value_type &>) {
        std::forward<Fn>(fn)(value());
      } else {
        std::forward<Fn>(fn)();
      }
    }

    return *this;
  }

  /**
   * Calls the given function `fn` if an error is present, otherwise doesn't do anything.
   *
   * `fn` will be given the error as its sole argument if such overload exists, otherwise
   * it will be called with no arguments.
   */
  template <typename Fn>
  expected const &on_error(Fn &&fn) const {
    if (!has_value()) {
      if constexpr (is_callable_v<Fn &&, error_type const &>) {
        std::forward<Fn>(fn)(error());
      } else {
        std::forward<Fn>(fn)();
      }
    }

    return *this;
  }

  /**
   * Calls the given function `fn` if an error is present, otherwise doesn't do anything.
   *
   * `fn` will be given the error as its sole argument if such overload exists, otherwise
   * it will be called with no arguments.
   */
  template <typename Fn>
  expected &on_error(Fn &&fn) {
    if (!has_value()) {
      if constexpr (is_callable_v<Fn &&, error_type &>) {
        std::forward<Fn>(fn)(error());
      } else {
        std::forward<Fn>(fn)();
      }
    }

    return *this;
  }

  /**
   * Returns the value if present, or the result of calling the given function `fn` if an
   * error is present.
   *
   * `fn` will be given the error as its sole argument if such overload exists, otherwise
   * it will be called with no arguments.
   */
  template <typename Fn>
  value_type recover(Fn &&fn) const {
    if (has_value()) {
      return value();
    }

    if constexpr (is_callable_v<Fn &&, error_type const &>) {
      return std::forward<Fn>(fn)(error());
    } else {
      return std::forward<Fn>(fn)();
    }
  }

  /**
   * Returns the value if present, or the result of calling the given function `fn` if an
   * error is present.
   *
   * `fn` will be given the error as its sole argument if such overload exists, otherwise
   * it will be called with no arguments.
   */
  template <typename Fn>
  value_type recover(Fn &&fn) {
    if (has_value()) {
      return value();
    }

    if constexpr (is_callable_v<Fn &&, error_type &>) {
      return std::forward<Fn>(fn)(error());
    } else {
      return std::forward<Fn>(fn)();
    }
  }

  /**
   * Returns the value if present, or the `with` if an error is present.
   */
  template <typename Value>
  value_type recover_with(Value &&with) const {
    if (has_value()) {
      return value();
    }

    return std::forward<Value>(with);
  }

  /**
   * Returns the value if present, or the `with` if an error is present.
   */
  template <typename Value>
  value_type recover_with(Value &&with) {
    if (has_value()) {
      return value();
    }

    return std::forward<Value>(with);
  }

  /**
   * This method can't be called on non-throwable error policies.
   */
  [[ noreturn ]]
  void raise() const {
    check_policy::check_is_error(*this);
    if constexpr (throwing()) {
      error_policy::raise(error());
    } else {
      static_assert(throwing(), "can't raise on a non-throwing error policy");
    }

    // ensure we won't return in case of a misbehaving error policy
    std::abort();
  }

  expected const &try_raise() const {
    if (has_value()) { return *this; }
    raise();
  }

  expected &try_raise() {
    if (has_value()) { return *this; }
    raise();
  }

  value_type const &operator *() const { return value(); }
  value_type &operator *() { return value(); }

  value_type const *operator ->() const { return try_value(); }
  value_type *operator ->() { return try_value(); }

  explicit operator bool() const { return has_value(); }
  bool operator !() const { return !has_value(); }

  expected &operator =(expected const &) = default;
  expected &operator =(expected &&) = default;

  constexpr static bool throwing() { return error_policy::throwing(); }

private:
  std::variant<value_type, error_t> data_;
};

/**
 * Convenient alias for an `expected` that uses `throwing_expected_check_policy`.
 *
 * Author: Marcelo Juchem <juchem at gmail dot com>
 */
template <
  typename T,
  typename Error,
  template <typename> typename ErrorPolicy = default_expected_error_policy
>
using checked_expected = expected<T, Error, throwing_expected_check_policy, ErrorPolicy>;

} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_container_expected_h
