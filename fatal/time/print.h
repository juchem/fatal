/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_time_print_h
#define FATAL_INCLUDE_fatal_time_print_h

#include <fatal/type/apply.h>
#include <fatal/type/sort.h>
#include <fatal/type/size.h>

#include <fatal/portability.h>

#include <chrono>
#include <ratio>
#include <utility>

#include <fatal/time/impl/print.h>

namespace ftl::time {

template <typename Out, typename R, typename P>
FATAL_HIDE_SYMBOL
Out &&pretty_print(Out &&out, std::chrono::duration<R, P> time) {
  if (!time.count()) {
    out << '0' << ftl::z_data<ftl::time::suffix_t<P>>();
  } else {
    using ratios = reject<
      typename impl_tm::pretty_print_ratios, curry<applier<std::ratio_greater>, P>
    >;
    static_assert(!empty<ratios>::value, "unsupported duration");

    using impl = apply_to<ratios, impl_tm::pretty>;
    impl::print(out, time);
  }

  return std::forward<Out>(out);
}

} // namespace ftl::time {

#endif // FATAL_INCLUDE_fatal_time_print_h
