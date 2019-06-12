/*
 *  Copyright (c) 2019, Marcelo Juchem.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#ifndef FATAL_INCLUDE_fatal_type_impl_traits_h
#define FATAL_INCLUDE_fatal_type_impl_traits_h

namespace fatal {
namespace impl_trt {

template <template <typename...> class, typename> struct ist;

template <template <typename...> class T, typename... Args>
struct ist<T, T<Args...>> {};


} // namespace impl_trt {
} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_type_impl_traits_h
