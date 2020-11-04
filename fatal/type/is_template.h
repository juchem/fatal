/*
 *  Copyright (c) 2019, Marcelo Juchem
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#ifndef FATAL_INCLUDE_fatal_type_is_template_h
#define FATAL_INCLUDE_fatal_type_is_template_h

#include <fatal/portability.h>
#include <fatal/type/is_complete.h>

#include <type_traits>

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

#include <fatal/type/impl/is_template.h>

namespace ftl {

/**
 * Checks whether a given type is an instantiation of at least one of a list of
 * class templates.
 *
 * The template member `type` which evaluates to either `std::true_type` or
 * `std::false_type`.
 *
 * Example:
 *
 *  // yields `true` for std::string
 *  is_template<std::basic_string>::type<std::string>::value
 *
 *  // yields `true` for std::wstring
 *  is_template<std::basic_string>::type<std::wstring>::value
 *
 *  // yields `true` for std::basic_string<int>
 *  is_template<std::basic_string>::type<std::basic_string<int>>::value
 *
 *  // yields `false` for std::vector<int>
 *  is_template<std::basic_string>::type<std::vector<int>>::value
 *
 *  // yields `true` for std::vector<int>
 *  is_template<std::basic_string, std::vector>::type<std::string>::value
 *
 *  // yields `true` for std::vector<int>
 *  is_template<std::basic_string, std::vector>::type<std::vector<int>>::value
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <template <typename...> typename... Templates>
class FATAL_HIDE_SYMBOL is_template {
  template <template <typename...> typename Template, typename T>
  using impl = is_complete<i_it::ist<Template, T>>;

public:
  template <typename T>
  constexpr static bool apply_v = (false || ... || impl<Templates, T>::value);

  template <typename T>
  using apply = std::bool_constant<apply_v<T>>;

  template <typename... T>
  using any = std::bool_constant<(false || ... || apply_v<T>)>;

  template <typename... T>
  using all = std::bool_constant<(sizeof...(T) && ... && apply_v<T>)>;
};

template <template <typename...> typename Template, typename T>
FATAL_HIDE_SYMBOL
constexpr bool is_template_v = is_template<Template>::template apply<T>::value;

template <template <typename...> typename Template, typename... T>
FATAL_HIDE_SYMBOL
constexpr bool is_template_any_v = is_template<Template>::template any<T...>::value;

template <template <typename...> typename Template, typename... T>
FATAL_HIDE_SYMBOL
constexpr bool is_template_all_v = is_template<Template>::template all<T...>::value;

} // namespace ftl {

#endif // FATAL_INCLUDE_fatal_type_is_template_h
