/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_time_impl_print_h
#define FATAL_INCLUDE_fatal_time_impl_print_h

#include <fatal/time/units.h>
#include <fatal/type/array.h>
#include <fatal/type/list.h>

#include <chrono>
#include <ratio>

namespace fatal {
namespace impl_tm {

template <typename...> struct pretty;

template <typename T, typename... Args>
struct pretty<T, Args...> {
  template <typename Out, typename P, typename R>
  static void print(Out &out, std::chrono::duration<R, P> time) {
    auto const local = std::chrono::duration_cast<std::chrono::duration<R, T>>(time);

    auto const remaining = time - local;

    if (local.count()) {
      out << local.count() << z_data<time::suffix_t<T>>();

      if (sizeof...(Args) && remaining.count()) {
        out << ' ';
      }
    }

    pretty<Args...>::print(out, remaining);
  }
};

template <>
struct pretty<> {
  template <typename Out, typename D>
  static void print(Out &, D) {}
};

using pretty_print_ratios = list<
  std::ratio_multiply<std::chrono::hours::period, std::ratio<24 * 7, 1>>,
  std::ratio_multiply<std::chrono::hours::period, std::ratio<24, 1>>,
  std::chrono::hours::period,
  std::chrono::minutes::period,
  std::chrono::seconds::period,
  std::chrono::milliseconds::period,
  std::chrono::microseconds::period,
  std::chrono::nanoseconds::period
>;

} // namespace impl_tm {
} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_time_impl_print_h
