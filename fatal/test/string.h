/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_test_string_h
#define FATAL_INCLUDE_fatal_test_string_h

#include <fatal/string/string_view.h>
#include <fatal/test/type.h>
#include <fatal/time/units.h>
#include <fatal/type/tag.h>

#include <fatal/portability.h>

#include <chrono>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#include <cstring>

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

namespace fatal {

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
void append(std::string &out, bool from) {
  out.append(from ? "true" : "false");
}

template <
  typename T,
  typename = decltype(std::to_string(std::declval<T>())),
  typename = std::enable_if_t<!std::is_enum_v<T>>
>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
void append(std::string &out, T from) {
  out.append(std::to_string(from));
}

template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
void append(std::string &out, T from) {
  type_str<T>(out);
  out.push_back('(');
  out.append(std::to_string(static_cast<std::underlying_type_t<T>>(from)));
  out.push_back(')');
}

template <typename R, typename P>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
void append(std::string &out, std::chrono::duration<R, P> from) {
  append(out, from.count());
  out.append(time::suffix_of(from));
}

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
void append(std::string &out, std::string const &from) {
  out.append(from);
}

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
void append(std::string &out, string_view from) {
  out.append(from.data(), from.size());
}

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
void append(std::string &out, char from) {
  out.push_back(from);
}

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
void append(std::string &out, char const *from) {
  out.append(from);
}

namespace i_str {

template <typename... Args, std::size_t... Index>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
void append_tuple(
  std::string &out,
  std::tuple<Args...> const &from,
  std::index_sequence<Index...>
) {
  out.append("tuple { ");
  bool _[sizeof...(Args)] = {
    (append(out.append(Index ? ", " : ""), std::get<Index>(from)), false)...
  };
  (void)_;
  out.append(" }");
}

} // namespace i_str {

template <typename... Args>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
void append(std::string &out, std::tuple<Args...> const &from) {
  i_str::append_tuple(out, from, std::make_index_sequence<sizeof...(Args)>());
}

namespace detail {
namespace string_impl {

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
bool parse(tag<bool>, std::string const &from) {
  // TODO: use a compile-time trie??
  if (from == "true") {
    return true;
  }

  if (from == "false") {
    return false;
  }

  throw std::invalid_argument("unrecognized boolean");
}

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
short parse(tag<short>, std::string const &from) {
  return static_cast<short>(std::stoi(from));
}

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
int parse(tag<int>, std::string const &from) {
  return std::stoi(from);
}

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
long parse(tag<long>, std::string const &from) {
  return std::stol(from);
}

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
long long parse(tag<long long>, std::string const &from) {
  return std::stoll(from);
}

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
unsigned long parse(tag<unsigned long>, std::string const &from) {
  return std::stoul(from);
}

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
unsigned long long parse(
  tag<unsigned long long>, std::string const &from
) {
  return std::stoull(from);
}

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
float parse(tag<float>, std::string const &from) {
  return std::stof(from);
}

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
double parse(tag<double>, std::string const &from) {
  return std::stod(from);
}

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
long double parse(tag<long double>, std::string const &from) {
  return std::stold(from);
}

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
std::string parse(tag<std::string>, std::string const &from) {
  return from;
}

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
void to_string_impl(std::string &) {}

template <typename T, typename... Args>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
void to_string_impl(std::string &out, T &&value, Args &&...args) {
  append(out, std::forward<T>(value));

  to_string_impl(out, std::forward<Args>(args)...);
}

} // namespace string_impl {
} // namespace detail {

// for internal tests only - no guaranteed efficiency
// TODO: TEST
template <typename To>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
To parse(std::string const &from) {
  return detail::string_impl::parse(tag<To>(), from);
}

// for internal tests only - no guaranteed efficiency
// TODO: TEST
template <typename... Args>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
std::string &append_to_string(std::string &out, Args &&...args) {
  detail::string_impl::to_string_impl(out, std::forward<Args>(args)...);

  return out;
}

// TODO: TEST
template <typename... Args>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
std::string to_string(Args &&...args) {
  std::string out;

  append_to_string(out, std::forward<Args>(args)...);

  return out;
}

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
std::string const &to_string(std::string const &s) { return s; }

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
std::string &to_string(std::string &s) { return s; }

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
std::string &&to_string(std::string &&s) { return std::move(s); }

} // namespace fatal {

FATAL_DIAGNOSTIC_POP

#endif // FATAL_INCLUDE_fatal_test_string_h
