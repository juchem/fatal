/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_test_type_h
#define FATAL_INCLUDE_fatal_test_type_h

#include <fatal/portability.h>

#include <string>
#include <type_traits>
#include <typeinfo>

#if defined __GLIBCXX__ || defined __clang__
# include <cxxabi.h>
#endif // defined __GLIBCXX__ || defined __clang__

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

namespace fatal {

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
std::string type_str(std::string &out, std::type_info const &type) {
# if defined __GLIBCXX__ || defined __clang__
  int status;
  auto name = abi::__cxa_demangle(type.name(), 0, 0, &status);

  if (name) {
    out.append(name);
    std::free(name);
    return out;
  }
# endif // defined __GLIBCXX__ || defined __clang__

  out.append(type.name());
  return out;
}

template <typename T>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
std::string &type_str(std::string &out) {
  type_str(out, typeid(T));

  using type = typename std::remove_reference<T>::type;

  if (std::is_const<type>::value) {
    out.append(" const");
  }

  if (std::is_volatile<type>::value) {
    out.append(" volatile");
  }

  if (std::is_lvalue_reference<T>::value) {
    out.append(" &");
  } else if (std::is_rvalue_reference<T>::value) {
    out.append(" &&");
  }

  return out;
}

template <typename T>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
std::string type_str() {
  std::string result;
  type_str<T>(result);
  return result;
}

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
std::string type_str(std::type_info const &type) {
  std::string result;
  type_str(result, type);
  return result;
}

template <typename T>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
std::string &type_str_for(std::string &out, T &&) {
  return type_str<T>(out);
}

template <typename T>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
std::string type_str_for(T &&) {
  return type_str<T>();
}

} // namespace fatal {

FATAL_DIAGNOSTIC_POP

#endif // FATAL_INCLUDE_fatal_test_type_h
