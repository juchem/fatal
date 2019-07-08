/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_impl_is_callable_h
#define FATAL_INCLUDE_fatal_type_impl_is_callable_h

#include <type_traits>
#include <utility>

namespace fatal {
namespace i_icl {

template <typename... Args>
class FATAL_HIDE_SYMBOL icl {
  struct FATAL_HIDE_SYMBOL impl {
    template <
      typename T,
      typename = decltype(
        std::declval<T>()(
          std::forward<Args>(std::declval<Args>())...
        )
      )
    >
    FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
    static std::true_type sfinae(T *);

    template <typename = void>
    FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
    static std::false_type sfinae(...);
  };

public:
  template <typename T>
  using type = decltype(
    impl::sfinae(
      static_cast<typename std::remove_reference<T>::type *>(nullptr)
    )
  );
};

} // namespace i_icl {
} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_type_impl_is_callable_h
