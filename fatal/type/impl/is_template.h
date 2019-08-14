/*
 *  Copyright (c) 2019, Marcelo Juchem.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#ifndef FATAL_INCLUDE_fatal_type_impl_is_template
#define FATAL_INCLUDE_fatal_type_impl_is_template

namespace fatal {
namespace i_it {

template <template <typename...> typename, typename> struct FATAL_HIDE_SYMBOL ist;

template <template <typename...> typename T, typename... Args>
struct FATAL_HIDE_SYMBOL ist<T, T<Args...>> {};


} // namespace i_it {
} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_type_impl_is_template
