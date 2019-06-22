/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_test_test_h
#define FATAL_INCLUDE_fatal_test_test_h

#include <fatal/test/string.h>
#include <fatal/test/type.h>
#include <fatal/time/print.h>
#include <fatal/type/call_traits.h>
#include <fatal/type/traits.h>

#include <fatal/portability.h>
#include <fatal/preprocessor.h>

#include <chrono>
#include <exception>
#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include <cassert>
#include <cstring>

// for internal use only

namespace fatal {
namespace test {

// TODO: organize the order of components

#define FATAL_TEST(Case, ...) \
  FATAL_IMPL_TEST_CASE( \
    FATAL_UID(fatal_test_case_impl), \
    FATAL_TO_STR(FATAL_UNPARENTHESIZE(Case)), \
    FATAL_TO_STR(FATAL_UNPARENTHESIZE(__VA_ARGS__)), \
    FATAL_UID(fatal_test_case_results_impl), \
    FATAL_UID(fatal_test_case_test_registry_placeholder_impl) \
  )

#define FATAL_IMPL_TEST_CASE(Class, Case, Name, Results, RegistryPlaceholder) \
  struct FATAL_HIDE_SYMBOL Class { \
    FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL \
    void operator ()(); \
    \
    ::fatal::test::results Results; \
  }; \
  \
  FATAL_HIDE_SYMBOL \
  static auto const RegistryPlaceholder \
    = ::fatal::test::detail::test_impl::registry::get().add<Class>(); \
  \
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL \
  std::size_t operator <<( \
    ::fatal::test::detail::test_impl::registry_index_tag, \
    Class * \
  ) { \
    return RegistryPlaceholder; \
  } \
  \
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL \
  char const *operator <<( \
    ::fatal::test::detail::test_impl::group_tag, \
    Class * \
  ) { \
    return Case; \
  } \
  \
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL \
  char const *operator <<( \
    ::fatal::test::detail::test_impl::name_tag, \
    Class * \
  ) { \
    return Name; \
  } \
  \
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL \
  ::fatal::source_info const &operator <<( \
    ::fatal::test::detail::test_impl::source_tag, \
    Class * \
  ) { \
    static auto source = FATAL_SOURCE_INFO(); \
    return source; \
  } \
  \
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL \
  ::fatal::test::results &operator <<( \
    ::fatal::test::detail::test_impl::result_tag, Class &subject \
  ) { \
    return subject.Results; \
  } \
  \
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL \
  void Class::operator ()()

#define FATAL_WARN_UNREACHABLE() \
  FATAL_IMPL_NULLARY_TEST(warning, unreachable())

#define FATAL_WARN_TRUE(...) \
  FATAL_IMPL_UNARY_TEST(warning, is_true(), __VA_ARGS__)

#define FATAL_WARN_FALSE(...) \
  FATAL_IMPL_UNARY_TEST(warning, is_false(), __VA_ARGS__)

#define FATAL_WARN_NULL(...) \
  FATAL_IMPL_UNARY_TEST(warning, is_null(), __VA_ARGS__)

#define FATAL_WARN_NOT_NULL(...) \
  FATAL_IMPL_UNARY_TEST(warning, not_null(), __VA_ARGS__)

#define FATAL_WARN_NO_THROW \
  FATAL_IMPL_BODY_TEST(warning, no_throw())

#define FATAL_WARN_THROW(Exception) \
  FATAL_IMPL_BODY_TEST(warning, exception<Exception>(FATAL_TO_STR(Exception)))

#define FATAL_WARN_SAME \
  FATAL_IMPL_TEMPLATE_TEST(warning, same_type())

#define FATAL_WARN_EQ(LHS, ...) \
  FATAL_IMPL_BINARY_TEST(warning, is_equal(), LHS, __VA_ARGS__)

#define FATAL_WARN_NE(LHS, ...) \
  FATAL_IMPL_BINARY_TEST(warning, not_equal(), LHS, __VA_ARGS__)

#define FATAL_WARN_LT(LHS, ...) \
  FATAL_IMPL_BINARY_TEST(warning, less_than(), LHS, __VA_ARGS__)

#define FATAL_WARN_LE(LHS, ...) \
  FATAL_IMPL_BINARY_TEST(warning, less_equal(), LHS, __VA_ARGS__)

#define FATAL_WARN_GT(LHS, ...) \
  FATAL_IMPL_BINARY_TEST(warning, greater_than(), LHS, __VA_ARGS__)

#define FATAL_WARN_GE(LHS, ...) \
  FATAL_IMPL_BINARY_TEST(warning, greater_equal(), LHS, __VA_ARGS__)

#define FATAL_WARN_EQ_SAME(Type, LHS, ...) \
  FATAL_WARN_SAME<Type, decltype(__VA_ARGS__)>(); \
  FATAL_WARN_EQ(LHS, __VA_ARGS__)

#define FATAL_WARN_TEMPLATE_COMPILES(...) \
  FATAL_WARN_TRUE((::fatal::check_compilability<__VA_ARGS__>::value))

#define FATAL_WARN_TEMPLATE_DOESNT_COMPILE(...) \
  FATAL_WARN_FALSE((::fatal::check_compilability<__VA_ARGS__>::value))


#define FATAL_EXPECT_UNREACHABLE() \
  FATAL_IMPL_NULLARY_TEST(expectation, unreachable())

#define FATAL_EXPECT_TRUE(...) \
  FATAL_IMPL_UNARY_TEST(expectation, is_true(), __VA_ARGS__)

#define FATAL_EXPECT_FALSE(...) \
  FATAL_IMPL_UNARY_TEST(expectation, is_false(), __VA_ARGS__)

#define FATAL_EXPECT_NULL(...) \
  FATAL_IMPL_UNARY_TEST(expectation, is_null(), __VA_ARGS__)

#define FATAL_EXPECT_NOT_NULL(...) \
  FATAL_IMPL_UNARY_TEST(expectation, not_null(), __VA_ARGS__)

#define FATAL_EXPECT_NO_THROW \
  FATAL_IMPL_BODY_TEST(expectation, no_throw())

#define FATAL_EXPECT_THROW(Exception) \
  FATAL_IMPL_BODY_TEST( \
    expectation, exception<Exception>(FATAL_TO_STR(Exception)) \
  )

#define FATAL_EXPECT_SAME \
  FATAL_IMPL_TEMPLATE_TEST(expectation, same_type())

#define FATAL_EXPECT_EQ(LHS, ...) \
  FATAL_IMPL_BINARY_TEST(expectation, is_equal(), LHS, __VA_ARGS__)

#define FATAL_EXPECT_NE(LHS, ...) \
  FATAL_IMPL_BINARY_TEST(expectation, not_equal(), LHS, __VA_ARGS__)

#define FATAL_EXPECT_LT(LHS, ...) \
  FATAL_IMPL_BINARY_TEST(expectation, less_than(), LHS, __VA_ARGS__)

#define FATAL_EXPECT_LE(LHS, ...) \
  FATAL_IMPL_BINARY_TEST(expectation, less_equal(), LHS, __VA_ARGS__)

#define FATAL_EXPECT_GT(LHS, ...) \
  FATAL_IMPL_BINARY_TEST(expectation, greater_than(), LHS, __VA_ARGS__)

#define FATAL_EXPECT_GE(LHS, ...) \
  FATAL_IMPL_BINARY_TEST(expectation, greater_equal(), LHS, __VA_ARGS__)

#define FATAL_EXPECT_EQ_SAME(Type, LHS, ...) \
  FATAL_EXPECT_SAME<Type, decltype(__VA_ARGS__)>(); \
  FATAL_EXPECT_EQ(LHS, __VA_ARGS__)

#define FATAL_EXPECT_TEMPLATE_COMPILES(...) \
  FATAL_EXPECT_TRUE((::fatal::check_compilability<__VA_ARGS__>::value))

#define FATAL_EXPECT_TEMPLATE_DOESNT_COMPILE(...) \
  FATAL_EXPECT_FALSE((::fatal::check_compilability<__VA_ARGS__>::value))


#define FATAL_ASSERT_UNREACHABLE() \
  FATAL_IMPL_NULLARY_TEST(assertion, unreachable())

#define FATAL_ASSERT_TRUE(...) \
  FATAL_IMPL_UNARY_TEST(assertion, is_true(), __VA_ARGS__)

#define FATAL_ASSERT_FALSE(...) \
  FATAL_IMPL_UNARY_TEST(assertion, is_false(), __VA_ARGS__)

#define FATAL_ASSERT_NULL(...) \
  FATAL_IMPL_UNARY_TEST(assertion, is_null(), __VA_ARGS__)

#define FATAL_ASSERT_NOT_NULL(...) \
  FATAL_IMPL_UNARY_TEST(assertion, not_null(), __VA_ARGS__)

#define FATAL_ASSERT_NO_THROW \
  FATAL_IMPL_BODY_TEST(assertion, no_throw())

#define FATAL_ASSERT_THROW(Exception) \
  FATAL_IMPL_BODY_TEST(assertion, exception<Exception>(FATAL_TO_STR(Exception)))

#define FATAL_ASSERT_SAME \
  FATAL_IMPL_TEMPLATE_TEST(assertion, same_type())

#define FATAL_ASSERT_EQ(LHS, ...) \
  FATAL_IMPL_BINARY_TEST(assertion, is_equal(), LHS, __VA_ARGS__)

#define FATAL_ASSERT_NE(LHS, ...) \
  FATAL_IMPL_BINARY_TEST(assertion, not_equal(), LHS, __VA_ARGS__)

#define FATAL_ASSERT_LT(LHS, ...) \
  FATAL_IMPL_BINARY_TEST(assertion, less_than(), LHS, __VA_ARGS__)

#define FATAL_ASSERT_LE(LHS, ...) \
  FATAL_IMPL_BINARY_TEST(assertion, less_equal(), LHS, __VA_ARGS__)

#define FATAL_ASSERT_GT(LHS, ...) \
  FATAL_IMPL_BINARY_TEST(assertion, greater_than(), LHS, __VA_ARGS__)

#define FATAL_ASSERT_GE(LHS, ...) \
  FATAL_IMPL_BINARY_TEST(assertion, greater_equal(), LHS, __VA_ARGS__)

#define FATAL_ASSERT_EQ_SAME(Type, LHS, ...) \
  FATAL_ASSERT_SAME<Type, decltype(__VA_ARGS__)>(); \
  FATAL_ASSERT_EQ(LHS, __VA_ARGS__)

// TODO: FIX IT SO IT DISPLAYS THE PROPER MESSAGE OTHER THAN (is not true)
#define FATAL_ASSERT_TEMPLATE_COMPILES(...) \
  FATAL_ASSERT_TRUE((::fatal::check_compilability<__VA_ARGS__>::value))

// TODO: FIX IT SO IT DISPLAYS THE PROPER MESSAGE OTHER THAN (is not true)
#define FATAL_ASSERT_TEMPLATE_DOESNT_COMPILE(...) \
  FATAL_ASSERT_FALSE((::fatal::check_compilability<__VA_ARGS__>::value))

using clock = std::chrono::system_clock;
using duration_t = clock::duration;
using timestamp_t = clock::time_point;

// TODO: ISSUE SHOULD NOT BUILD AN INTERNAL STRING BUT RATHER A HIERARCHICAL MAP
//       TO ALLOW PRETTY PRINTERS TO CHOOSE THE FORMATTING
struct FATAL_HIDE_SYMBOL test_issue {
  enum class FATAL_HIDE_SYMBOL severity_t { warning, error, fatal };

FATAL_DIAGNOSTIC_PUSH
FATAL_GCC_DIAGNOSTIC_IGNORED_SHADOW_IF_BROKEN

  template <typename... Args>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  test_issue(
    severity_t severity,
    timestamp_t timestamp,
    source_info source,
    Args &&...args
  ):
    severity_(severity),
    timestamp_(timestamp),
    source_(source)
  {
    append_to_string(message_, std::forward<Args>(args)...);
  }

FATAL_DIAGNOSTIC_POP

  template <typename... Args>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  std::string const &append(Args &&...args) {
    return append_to_string(message_, std::forward<Args>(args)...);
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  severity_t severity() const { return severity_; }
  
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  timestamp_t timestamp() const { return timestamp_; }
  
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  source_info const &source() const { return source_; }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  std::string const &message() const { return message_; }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  char severity_signature() const {
    switch (severity_) {
      case severity_t::warning: return 'W';
      case severity_t::error: return 'E';
      case severity_t::fatal: return 'F';
      default:
        assert(false);
        return '\0';
    }
  }

private:
  FATAL_HIDE_SYMBOL
  severity_t const severity_;

  FATAL_HIDE_SYMBOL
  timestamp_t const timestamp_;

  FATAL_HIDE_SYMBOL
  source_info const source_;

  FATAL_HIDE_SYMBOL
  std::string message_;
};

class FATAL_HIDE_SYMBOL results {
  using issue_list = std::vector<test_issue>;

public:
  using const_iterator = issue_list::const_iterator;
  using size_type = issue_list::size_type;

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  test_issue const &add(test_issue i) {
    issues_.push_back(std::move(i));
    return issues_.back();
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  const_iterator cbegin() const { return issues_.cbegin(); }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  const_iterator begin() const { return issues_.begin(); }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  const_iterator cend() const { return issues_.cend(); }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  const_iterator end() const { return issues_.end(); }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  size_type size() const { return issues_.size(); }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  bool empty() const { return issues_.empty(); }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  bool passed() const { return issues_.empty(); }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  duration_t elapsed() const { return elapsed_; }

FATAL_DIAGNOSTIC_PUSH
FATAL_GCC_DIAGNOSTIC_IGNORED_SHADOW_IF_BROKEN

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void set_elapsed(duration_t elapsed) { elapsed_ = elapsed; }

FATAL_DIAGNOSTIC_POP

private:
  FATAL_HIDE_SYMBOL
  issue_list issues_;

  FATAL_HIDE_SYMBOL
  duration_t elapsed_;

};

struct FATAL_HIDE_SYMBOL controller {
  using issue_sink = std::function<void(test_issue &&)>;

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static void add_issue(test_issue &&i) {
    auto &sink = current_issues_sink();
    assert(sink);
    sink(std::move(i));
  }

  template <typename T>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static void set_issues_sink(T &&sink) {
    current_issues_sink() = std::forward<T>(sink);
  }

private:
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static issue_sink &current_issues_sink() {
    static issue_sink sink;
    return sink;
  }
};

namespace detail {
namespace test_impl {

struct FATAL_HIDE_SYMBOL registry_index_tag {};
struct FATAL_HIDE_SYMBOL group_tag {};
struct FATAL_HIDE_SYMBOL name_tag {};
struct FATAL_HIDE_SYMBOL source_tag {};
struct FATAL_HIDE_SYMBOL result_tag {};

struct FATAL_HIDE_SYMBOL abort_test_run_exception {};

#define FATAL_IMPL_NULLARY_TEST(Category, Predicate) \
  FATAL_IMPL_TEST_WRAP_CHECK( \
    Category, Predicate, \
    ::std::tuple<>() \
  ).check()

#define FATAL_IMPL_UNARY_TEST(Category, Predicate, ...) \
  FATAL_IMPL_TEST_WRAP_CHECK( \
    Category, Predicate, \
    ::std::tuple<char const *>(FATAL_TO_STR(__VA_ARGS__)) \
  ).check(__VA_ARGS__)

#define FATAL_IMPL_BINARY_TEST(Category, Predicate, LHS, ...) \
  FATAL_IMPL_TEST_WRAP_CHECK( \
    Category, Predicate, \
    (::std::tuple<char const *, char const *>( \
      FATAL_TO_STR(LHS), \
      FATAL_TO_STR(__VA_ARGS__) \
    )) \
  ).check((LHS), (__VA_ARGS__))

#define FATAL_IMPL_TEMPLATE_TEST(Category, Predicate) \
  FATAL_IMPL_TEST_WRAP_CHECK(Category, Predicate, ::std::tuple<>()).check

#define FATAL_IMPL_BODY_TEST(Category, Predicate) \
  FATAL_IMPL_TEST_WRAP_CHECK( \
    Category, Predicate, \
    ::std::tuple<char const *>("<expression>") \
  ) << [&]()

#define FATAL_IMPL_TEST_WRAP_CHECK(Category, Predicate, ...) \
  ::fatal::test::detail::test_impl::make_check_wrapper< \
    ::fatal::test::detail::test_impl::categories::Category \
  >( \
    ::fatal::test::detail::test_impl::predicates::Predicate, \
    FATAL_SOURCE_INFO(), \
    __VA_ARGS__ \
  )

struct FATAL_HIDE_SYMBOL can_append_to_string {
  template <
    typename T,
    typename = decltype(
      append(std::declval<std::string &>(), std::declval<T>())
    )
  >
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static std::true_type sfinae(T *);

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static std::false_type sfinae(...);

  template <typename T>
  using apply = decltype(sfinae(static_cast<T *>(nullptr)));
};

template <typename T, bool = can_append_to_string::apply<T>::value>
struct FATAL_HIDE_SYMBOL any_to_string_impl {
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static void do_append(std::string &out, T const &value) {
    append(out, value);
  }
};

template <typename T>
struct FATAL_HIDE_SYMBOL any_to_string_impl<T, false> {
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static void do_append(std::string &out, T const &) {
    out.append("<instance:");
    out.append(type_str<T>());
    out.push_back('>');
  }
};

template <typename T>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
std::string &any_to_string(std::string &out, T const &value) {
  any_to_string_impl<T>::do_append(out, value);
  return out;
}

template <typename T>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
std::string &any_to_string(std::string &out, T *value) {
  if (value) {
    return any_to_string(out, *value);
  }
  out.append("<null>");
  return out;
}

template <typename T>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
std::string any_to_string(T const &value) {
  std::string result;
  any_to_string(result, value);
  return result;
}

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
void handle_exception(test_issue &issue) {
  try {
    auto e = std::current_exception();
    assert(e);
    std::rethrow_exception(e);
  } catch (std::exception const &e) {
    issue.append(
      '(', type_str(typeid(e)), "): \"",
      e.what() ? e.what() : "<no message>", '"'
    );
  } catch (std::string const &e) {
    issue.append("(std::string): \"", e, '"');
  } catch (char const *e) {
    issue.append("(char const *): \"", e, '"');
  } catch (std::int64_t e) {
    issue.append("(std::int64_t): \"", e, '"');
  } catch (std::uint64_t e) {
    issue.append("(std::uint64_t): \"", e, '"');
  } catch (long double e) {
    issue.append("(long double): \"", e, '"');
  // TODO: ADD MORE TYPES
  } catch (...) {
    issue.append("(<unknown exception>)");
  }
}

template <typename...> struct FATAL_HIDE_SYMBOL args_list {};

template <typename... Args>
struct FATAL_HIDE_SYMBOL call_predicate {
  template <typename Predicate, typename... UArgs>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static bool call(Predicate &predicate, UArgs &&...args) {
    return predicate(args_list<Args...>(), std::forward<UArgs>(args)...);
  }
};

template <>
struct FATAL_HIDE_SYMBOL call_predicate<> {
  template <typename Predicate, typename... UArgs>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static bool call(Predicate &predicate, UArgs &&...args) {
    return predicate(std::forward<UArgs>(args)...);
  }
};

template <
  typename Predicate,
  bool = call_traits::args::member_function::supports<Predicate>::value
>
struct FATAL_HIDE_SYMBOL get_args_tuple {
  template <typename Tuple>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static auto get(Tuple &&, Predicate &predicate)
    -> decltype(
      call_traits::args::member_function::call(
        std::declval<Predicate &>()
      )
    )
  { return predicate.args(); }
};

template <typename Predicate>
struct FATAL_HIDE_SYMBOL get_args_tuple<Predicate, false> {
  template <typename Tuple>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static Tuple &&get(Tuple &&tuple, Predicate &) { return tuple; }
};

template <typename Category, typename Predicate, typename ArgsTuple>
class FATAL_HIDE_SYMBOL check_wrapper {
  using predicate = Predicate;
  using args_tuple = ArgsTuple;
  using category = Category;

public:
  template <typename UPredicate, typename UArgsTuple>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  check_wrapper(
    UPredicate &&predicate,
    source_info source,
    UArgsTuple &&args
  ):
    predicate_(std::forward<UPredicate>(predicate)),
    source_(source),
    args_(std::forward<UArgsTuple>(args))
  {}

  template <typename... Args, typename... UArgs>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void check(UArgs &&...args) {
    test_issue issue(category::severity(), clock::now(), source_);

    try {
      if (call_predicate<Args...>::call(predicate_, args...)) {
        return;
      }

      issue.append(category::name(), " failed: ");

      append_args(
        issue,
        get_args_tuple<Predicate>::get(args_, predicate_),
        args...
      );
    } catch (...) {
      issue.append("unexpected exception ");
      handle_exception(issue);
    }

    controller::add_issue(std::move(issue));

    if (category::severity() == test_issue::severity_t::fatal) {
      throw abort_test_run_exception();
    }
  }

  template <typename T>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void operator <<(T &&what) {
    check(std::forward<T>(what));
  }

private:
  // TODO: CLEANUP - see TODO on unreachable::text()
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void append_args(test_issue &issue, std::tuple<> const &) {
    issue.append("\n    ", predicate_.text());
  }

  // TODO: CLEANUP - see TODO on unreachable::text()
  template <typename TValue>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void append_args(test_issue &issue, std::tuple<TValue> const &arg) {
    issue.append(
      "\n    '", std::get<0>(arg),
      "'\n      ", predicate_.text()
    );
  }

  // TODO: CLEANUP - see TODO on unreachable::text()
  template <typename TValue, typename Value>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void append_args(
    test_issue &issue,
    std::tuple<TValue> const &arg,
    Value const &value
  ) {
    issue.append(
      "\n    '", std::get<0>(arg), "' -> '", any_to_string(value),
      "'\n      ", predicate_.text()
    );
  }

  // TODO: CLEANUP - see TODO on unreachable::text()
  template <typename TLHS, typename TRHS>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void append_args(test_issue &issue, std::tuple<TLHS, TRHS> const &args) {
    issue.append(
      "\n    lhs: '", std::get<0>(args),
      "'\n      ", predicate_.text(),
      "\n    rhs: '", std::get<1>(args), '\''
    );
  }

  // TODO: CLEANUP - see TODO on unreachable::text()
  template <typename TLHS, typename TRHS, typename LHS, typename RHS>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void append_args(
    test_issue &issue,
    std::tuple<TLHS, TRHS> const &args,
    LHS const &lhs,
    RHS const &rhs
  ) {
    issue.append(
      "\n    lhs: '", std::get<0>(args), "' -> '", any_to_string(lhs),
      "'\n      ", predicate_.text(),
      "\n    rhs: '", std::get<1>(args), "' -> '", any_to_string(rhs), '\''
    );
  }

  FATAL_HIDE_SYMBOL
  predicate predicate_;

  FATAL_HIDE_SYMBOL
  source_info const source_;

  FATAL_HIDE_SYMBOL
  args_tuple const args_;
};

template <
  typename Category, typename Predicate, typename ArgsTuple = std::tuple<>
>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
check_wrapper<
  Category,
  typename std::decay<Predicate>::type,
  typename std::decay<ArgsTuple>::type
> make_check_wrapper(
  Predicate &&predicate,
  source_info source,
  ArgsTuple &&args = ArgsTuple()
) {
  return {
    std::forward<Predicate>(predicate),
    source,
    std::forward<ArgsTuple>(args)
  };
}

namespace categories {

struct FATAL_HIDE_SYMBOL warning {
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static test_issue::severity_t severity() {
    return test_issue::severity_t::warning;
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static char const *name() { return "warning"; }
};

struct FATAL_HIDE_SYMBOL expectation {
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static test_issue::severity_t severity() {
    return test_issue::severity_t::error;
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static char const *name() { return "expectation"; }
};

struct FATAL_HIDE_SYMBOL assertion {
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static test_issue::severity_t severity() {
    return test_issue::severity_t::fatal;
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static char const *name() { return "assertion"; }
};

} // namespace categories {

namespace predicates {

template <typename TLHS, typename TRHS>
using both_integral = std::integral_constant<
  bool,
  std::is_integral<typename std::decay<TLHS>::type>::value
    && std::is_integral<typename std::decay<TRHS>::type>::value
>;

template <typename T, typename TLHS, typename TRHS, bool Expected>
using if_both_integral = typename std::enable_if<
  both_integral<TLHS, TRHS>::value == Expected,
  T
>::type;

template <typename TLHS, typename TRHS>
using common = typename std::common_type<
  typename std::decay<TLHS>::type,
  typename std::decay<TRHS>::type
>::type;

struct FATAL_HIDE_SYMBOL unreachable {
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  bool operator ()() const { return false; }
  // TODO: change to `void format(test_issue &i, tuple<*> names, args...)`
  //       with good defaults for nullary/unary/binary/...

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  char const *text() const { return "unreachable"; }
};

struct FATAL_HIDE_SYMBOL is_true {
  template <typename T>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  bool operator ()(T &&what) const { return static_cast<bool>(what); }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  char const *text() const { return "is true"; }
};

struct FATAL_HIDE_SYMBOL is_false {
  template <typename T>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  bool operator ()(T &&what) const { return !what; }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  char const *text() const { return "is false"; }
};

struct FATAL_HIDE_SYMBOL is_null {
  template <typename T>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  bool operator ()(T &&what) const { return what == nullptr; }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  char const *text() const { return "is null"; }
};

struct FATAL_HIDE_SYMBOL not_null {
  template <typename T>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  bool operator ()(T &&what) const { return what != nullptr; }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  char const *text() const { return "is not null"; }
};

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
bool compare_string(char const *lhs, char const *rhs) {
  if (!lhs || !rhs) {
    return lhs == rhs;
  }

  return !std::strcmp(lhs, rhs);
}

struct FATAL_HIDE_SYMBOL is_equal {
  template <typename TLHS, typename TRHS>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  if_both_integral<bool, TLHS, TRHS, false> operator ()(
    TLHS &&lhs, TRHS &&rhs
  ) const { return lhs == rhs; }

  template <typename TLHS, typename TRHS>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  if_both_integral<bool, TLHS, TRHS, true> operator ()(
    TLHS &&lhs, TRHS &&rhs
  ) const {
    using type = common<TLHS, TRHS>;
    return static_cast<type>(lhs) == static_cast<type>(rhs);
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  bool operator()(char const *lhs, char const *rhs) const {
    return compare_string(lhs, rhs);
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  bool operator()(char const *lhs, char *rhs) const {
    return compare_string(lhs, rhs);
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  bool operator()(char *lhs, char const *rhs) const {
    return compare_string(lhs, rhs);
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  bool operator()(char *lhs, char *rhs) const {
    return compare_string(lhs, rhs);
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  char const *text() const { return "is equal to"; }
};

struct FATAL_HIDE_SYMBOL not_equal {
  template <typename TLHS, typename TRHS>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  if_both_integral<bool, TLHS, TRHS, false> operator ()(
    TLHS &&lhs, TRHS &&rhs
  ) const { return lhs != rhs; }

  template <typename TLHS, typename TRHS>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  if_both_integral<bool, TLHS, TRHS, true> operator ()(
    TLHS &&lhs, TRHS &&rhs
  ) const {
    using type = common<TLHS, TRHS>;
    return static_cast<type>(lhs) != static_cast<type>(rhs);
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  bool operator()(char const *lhs, char const *rhs) const {
    return !compare_string(lhs, rhs);
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  bool operator()(char const *lhs, char *rhs) const {
    return !compare_string(lhs, rhs);
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  bool operator()(char *lhs, char const *rhs) const {
    return !compare_string(lhs, rhs);
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  bool operator()(char *lhs, char *rhs) const {
    return !compare_string(lhs, rhs);
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  char const *text() const { return "is not equal to"; }
};

struct FATAL_HIDE_SYMBOL less_than {
  template <typename TLHS, typename TRHS>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  if_both_integral<bool, TLHS, TRHS, false> operator ()(
    TLHS &&lhs, TRHS &&rhs
  ) const { return lhs < rhs; }

  template <typename TLHS, typename TRHS>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  if_both_integral<bool, TLHS, TRHS, true> operator ()(
    TLHS &&lhs, TRHS &&rhs
  ) const {
    using type = common<TLHS, TRHS>;
    return static_cast<type>(lhs) < static_cast<type>(rhs);
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  char const *text() const { return "is less than"; }
};

struct FATAL_HIDE_SYMBOL less_equal {
  template <typename TLHS, typename TRHS>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  if_both_integral<bool, TLHS, TRHS, false> operator ()(
    TLHS &&lhs, TRHS &&rhs
  ) const { return lhs <= rhs; }

  template <typename TLHS, typename TRHS>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  if_both_integral<bool, TLHS, TRHS, true> operator ()(
    TLHS &&lhs, TRHS &&rhs
  ) const {
    using type = common<TLHS, TRHS>;
    return static_cast<type>(lhs) <= static_cast<type>(rhs);
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  char const *text() const { return "is less than or equal to"; }
};

struct FATAL_HIDE_SYMBOL greater_than {
  template <typename TLHS, typename TRHS>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  if_both_integral<bool, TLHS, TRHS, false> operator ()(
    TLHS &&lhs, TRHS &&rhs
  ) const { return lhs > rhs; }

  template <typename TLHS, typename TRHS>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  if_both_integral<bool, TLHS, TRHS, true> operator ()(
    TLHS &&lhs, TRHS &&rhs
  ) const {
    using type = common<TLHS, TRHS>;
    return static_cast<type>(lhs) > static_cast<type>(rhs);
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  char const *text() const { return "is greater than"; }
};

struct FATAL_HIDE_SYMBOL greater_equal {
  template <typename TLHS, typename TRHS>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  if_both_integral<bool, TLHS, TRHS, false> operator ()(
    TLHS &&lhs, TRHS &&rhs
  ) const { return lhs >= rhs; }

  template <typename TLHS, typename TRHS>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  if_both_integral<bool, TLHS, TRHS, true> operator ()(
    TLHS &&lhs, TRHS &&rhs
  ) const {
    using type = common<TLHS, TRHS>;
    return static_cast<type>(lhs) >= static_cast<type>(rhs);
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  char const *text() const { return "is greater than or equal to"; }
};

class FATAL_HIDE_SYMBOL same_type {
  using args_text = std::tuple<std::string, std::string>;

public:
  template <typename LHS, typename RHS>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  bool operator ()(args_list<LHS, RHS>) {
    args_ = std::make_tuple(type_str<LHS>(), type_str<RHS>());
    return std::is_same<LHS, RHS>::value;
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  args_text const &args() const { return args_; }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  char const *text() const { return "is the same type as"; }

private:
  FATAL_HIDE_SYMBOL
  args_text args_;

  FATAL_HIDE_SYMBOL
  std::string text_;
};

struct FATAL_HIDE_SYMBOL no_throw {
  template <typename T>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  bool operator ()(T &&what) const {
    try {
      what();
      return true;
    } catch (...) {
      return false;
    }
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  char const *text() const { return "does not throw"; }
};

template <typename TException>
struct FATAL_HIDE_SYMBOL exception {
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  explicit exception(char const *name):
    text_("throws ")
  {
    text_.append(name);
  }

  template <typename T>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  bool operator ()(T &&what) const {
    try {
      what();
    } catch (TException const &) {
      return true;
    } catch (...) {
    }

    return false;
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  char const *text() const { return text_.c_str(); }

private:
  FATAL_HIDE_SYMBOL
  std::string text_;
};

} // namespace predicates {

class FATAL_HIDE_SYMBOL registry {
  struct FATAL_HIDE_SYMBOL entry {
    using issue_sink = std::function<void(test_issue const &)>;

    FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
    virtual ~entry() {}

    FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
    virtual results run(issue_sink sink) const = 0;

    FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
    virtual char const *group() const = 0;

    FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
    virtual char const *name() const = 0;

    FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
    virtual source_info const &source() const = 0;
  };

  template <typename T>
  struct FATAL_HIDE_SYMBOL entry_impl:
    public entry
  {
    using type = T;

    FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
    results run(issue_sink sink) const override {
      type subject;

      auto &result = (result_tag() << subject);

      controller::set_issues_sink(
        [&](test_issue &&issue) {
          sink(result.add(std::move(issue)));
        }
      );

      auto const start = clock::now();
      try {
        subject();
      } catch (abort_test_run_exception const &) {
        // test run aborted
      } catch (...) {
        test_issue issue(test_issue::severity_t::error, clock::now(), source());
        issue.append("test case aborted by unexpected exception ");
        handle_exception(issue);
        result.add(std::move(issue));
      }

      auto const elapsed = clock::now() - start;

      result.set_elapsed(elapsed);

      return result;
    }

    FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
    char const *group() const override {
      return group_tag() << static_cast<type *>(nullptr);
    }

    FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
    char const *name() const override {
      return name_tag() << static_cast<type *>(nullptr);
    }

    FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
    source_info const &source() const override {
      return source_tag() << static_cast<type *>(nullptr);
    }
  };

public:
  using case_map = std::unordered_map<std::string, results>;
  using result_map = std::unordered_map<std::string, case_map>;
  using run_result = std::pair<result_map, bool>;
  using size_type = std::size_t;

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  registry(): size_(0) {}

  template <typename T>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  size_type add() {
    std::unique_ptr<entry> e(new entry_impl<T>());

    auto i = groups_.find(e->group());
    if (i == groups_.end()) {
      auto const index = entries_.size();
      groups_order_.push_back(e->group());
      groups_.emplace(groups_order_.back(), index);

      entries_.emplace_back();
      entries_.back().emplace_back(std::move(e));

      assert(entries_.size() == index + 1);
    } else {
      entries_[i->second].emplace_back(std::move(e));
    }

    return ++size_;
  }

  template <typename TPrinter, typename TOut>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void list(TOut &out) const {
    TPrinter printer;

    for (auto const &order: groups_order_) {
      auto g = groups_.find(order);
      assert(g != groups_.end());
      assert(g->second < entries_.size());
      auto const &group = entries_[g->second];

      printer.list_start_group(out, order);

      for (auto const &i: group) {
        printer.list_entry(out, g->first, i->name());
      }

      printer.list_end_group(out, order);
    }
  }

  template <typename TPrinter, typename TOut, typename Filter>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  run_result run(TOut &out, Filter &&filter) const {
    run_result summary;

    TPrinter printer;

    printer.start_run(out, size_, groups_.size(), clock::now());

    duration_t running_time(0);
    size_type passed = 0;
    size_type total = 0;

    for (auto const &order: groups_order_) {
      auto g = groups_.find(order);
      assert(g != groups_.end());
      assert(g->second < entries_.size());
      auto const &group = entries_[g->second];

      duration_t group_time(0);

      printer.start_group(out, g->first, group.size(), clock::now());

      for (auto const &i: group) {
        if (!filter(*i)) {
          continue;
        }

        printer.start_test(out, g->first, i->name(), i->source(), clock::now());

        std::size_t issues = 0;
        auto result = i->run(
          [&](test_issue const &issue) {
            printer.issue(out, i->name(), i->source(), issue, issues);
          }
        );

        printer.end_test(out, result, g->first, i->name(), i->source());

        group_time += result.elapsed();
        ++total;

        if (result.passed()) {
          ++passed;
        }

        summary.first[i->group()][i->name()] = std::move(result);
      }

      running_time += group_time;

      printer.end_group(out, g->first, group.size(), group_time);
    }

    printer.end_run(out, passed, total, groups_.size(), running_time);

    summary.second = passed == total;

    return summary;
  }

  template <typename TPrinter, typename TOut>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  run_result run_all(TOut &out) const {
    return run<TPrinter>(out, [](entry const &) { return true; });
  }

  // only supports exact match
  template <typename TPrinter, typename TOut>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  run_result run_one(TOut &out, std::string const &full_name) const {
    return run<TPrinter>(out, [&](entry const &e) {
      return make_full_name(e.group(), e.name()) == full_name;
    });
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  size_type size() const { return size_; }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static registry &get() {
    static registry instance;
    return instance;
  }

  // not entirely a robust way of generating full names, but it will work
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static std::string make_full_name(char const *group, char const *name) {
    return std::string() + group + " - " + name;
  }

private:
  FATAL_HIDE_SYMBOL
  std::vector<std::vector<std::unique_ptr<entry>>> entries_;

  FATAL_HIDE_SYMBOL
  std::vector<std::string> groups_order_;

  FATAL_HIDE_SYMBOL
  std::unordered_map<std::string, std::size_t> groups_;

  FATAL_HIDE_SYMBOL
  size_type size_;
};

} // namespace test_impl {
} // namespace detail {

struct FATAL_HIDE_SYMBOL default_printer {
  template <typename TOut, typename TGroup>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void list_start_group(TOut &, TGroup const &) {}

  template <typename TOut, typename TGroup, typename TName>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void list_entry(TOut &out, TGroup const &group, TName const &name) {
    out << group << " - " << name << "\n";
  }

  template <typename TOut, typename TGroup>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void list_end_group(TOut &, TGroup const &) {}

  template <typename TOut>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void start_run(
    TOut &out, std::size_t total, std::size_t groups, timestamp_t start
  ) {
    out << "running " << total << " tests from " << groups << " test cases\n";
    run_start_ = start;
  }

  template <typename TOut, typename TGroup>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void start_group(
    TOut &out, TGroup const &group, std::size_t, timestamp_t start
  ) {
    auto const time = start - run_start_;

    time::pretty_print(out << "\n== test case: '" << group << "' at [", time)
      << "] ==\n";

    group_start_ = start;
  }

  template <typename TOut, typename TGroup, typename TName>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void start_test(
    TOut &out, TGroup const &, TName const &name,
    source_info const &source, timestamp_t start
  ) {
    auto const time = start - group_start_;

    time::pretty_print(
      out << ">> test '" << name << "' (" << source.file() << ':'
        << source.line() << ") at [",
      time
    ) << "]:\n";

    test_start_ = start;
  }

  template <typename TOut, typename TName>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void issue(
    TOut &out, TName const &, source_info const &,
    test_issue const &i, std::size_t index
  ) {
    if (index) {
      out << '\n';
    }

    auto const time = i.timestamp() - test_start_;

    time::pretty_print(
      out << i.severity_signature() << " [" << i.source().file() << ':'
        << i.source().line() << "] at [",
      time
    ) << "]:\n  " << i.message() << '\n';
  }

  template <typename TOut, typename TGroup, typename TName>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void end_test(
    TOut &out, results const &result,
    TGroup const &, TName const &, source_info const &
  ) {
    time::pretty_print(
      out << "<< " << (result.passed() ? "succeeded" : "failed") << " after [",
      result.elapsed()
    ) << "]\n\n";
  }

  template <typename TOut, typename TGroup>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void end_group(TOut &, TGroup const &, std::size_t, duration_t) {}

  template <typename TOut>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void end_run(
    TOut &out, std::size_t passed, std::size_t total, std::size_t,
    duration_t time
  ) {
    time::pretty_print(
      out << '\n' << (passed == total ? "succeeded" : "FAILED") << ": passed "
        << passed << '/' << total << " after [",
      time
    ) << "]\n\n";
  }

private:
  FATAL_HIDE_SYMBOL
  timestamp_t run_start_;

  FATAL_HIDE_SYMBOL
  timestamp_t group_start_;

  FATAL_HIDE_SYMBOL
  timestamp_t test_start_;
};

// a minimal mimic of gtest test output
struct FATAL_HIDE_SYMBOL gtest_printer {
  template <typename TOut, typename TGroup>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void list_start_group(TOut &out, TGroup const &group) {
    out << group << " - \n";
  }

  template <typename TOut, typename TGroup, typename TName>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void list_entry(TOut &out, TGroup const &, TName const &name) {
    out << "  " << name << "\n";
  }

  template <typename TOut, typename TGroup>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void list_end_group(TOut &, TGroup const &) {}

  template <typename TOut>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void start_run(
    TOut &out, std::size_t total, std::size_t groups, timestamp_t
  ) {
    out << "[==========] Running " << total << " tests from " << groups
        << " test case\n";
    out << "[----------] Global test environment set-up.\n";
  }

  template <typename TOut, typename TGroup>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void start_group(
    TOut &out, TGroup const &group, std::size_t size, timestamp_t
  ) {
    out << "[----------] " << size << " tests from " << group << "\n";
  }

  template <typename TOut, typename TGroup, typename TName>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void start_test(
    TOut &out, TGroup const &group, TName const &name,
    source_info const &, timestamp_t
  ) {
    out << "[ RUN      ] " << group << " - " << name << "\n";
  }

  template <typename TOut, typename TName>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void issue(
    TOut &out, TName const &, source_info const &,
    test_issue const &i, std::size_t index
  ) {
    if (index) {
      out << '\n';
    }

    out << i.severity_signature() << " [" << i.source().file() << ':'
        << i.source().line() << "]: " << i.message() << '\n';
  }

  template <typename TOut, typename TGroup, typename TName>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void end_test(
    TOut &out, results const &result,
    TGroup const &group, TName const &name,
    source_info const &
  ) {
    auto const result_str = result.passed() ? "    OK" : "FAILED";
    auto const elapsed_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(
        result.elapsed()
      );

    out << "[   " << result_str << " ] " << group << " - " << name
        << " (" << elapsed_ms.count() << " ms)\n";
  }

  template <typename TOut, typename TGroup>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void end_group(
    TOut &out, TGroup const &group, std::size_t size, duration_t time
  ) {
    auto const elapsed_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(time);

    out << "[----------] " << size << " tests from " << group << " ("
        << elapsed_ms.count() << " ms total)\n";
    out << "\n";
  }

  template <typename TOut>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void end_run(
    TOut &out, std::size_t passed, std::size_t total,
    std::size_t groups, duration_t time
  ) {
    auto const result_str = passed == total ? "PASSED" : "FAILED";
    auto const elapsed_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(time);

    out << "[----------] Global test environment tear-down\n";
    out << "[==========] " << total << " tests from " << groups
        << " test case ran. (" << elapsed_ms.count() << " ms total)\n";
    out << "[  " << result_str << "  ] " << total << " tests.\n";
  }
};

template <typename TPrinter = default_printer, typename TOut>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
int list(TOut &out) {
  auto& registry = detail::test_impl::registry::get();
  registry.list<TPrinter>(out);

  return EXIT_SUCCESS;
}

template <typename TPrinter = default_printer, typename TOut>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
int run_all(TOut &out) {
auto& registry = detail::test_impl::registry::get();
  auto const result = registry.run_all<TPrinter>(out);

  return result.second ? EXIT_SUCCESS : EXIT_FAILURE;
}

template <typename TPrinter = default_printer, typename TOut>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
int run_one(TOut &out, std::string const &full_name) {
  auto& registry = detail::test_impl::registry::get();
  auto const result = registry.run_one<TPrinter>(out, full_name);

  return result.second ? EXIT_SUCCESS : EXIT_FAILURE;
}

} // namespace test {
} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_test_test_h
