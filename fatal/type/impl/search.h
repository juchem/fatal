/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_impl_search_h
#define FATAL_INCLUDE_fatal_type_impl_search_h

#include <fatal/type/size.h>
#include <fatal/type/tag.h>

namespace fatal {
namespace i_S {

// TODO: OPTIMIZE COMPILE TIMES
// TODO: HIGHER LOG BASE OPTIMIZATION (3 or 4 should be enough)
// TODO: SWITCH CASE OPTIMIZATION

template <typename T, std::size_t Offset = 0, std::size_t Size = size<T>::value>
struct FATAL_HIDE_SYMBOL s {
  template <
    typename Comparer,
    typename Filter,
    typename Needle,
    typename Visitor,
    typename... Args
  >
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static constexpr bool S(Needle &&needle, Visitor &&visitor, Args &&...args) {
    if constexpr (Size == 0) {
      return false;
    } else if constexpr (Size == 1) {
      static_assert(Offset < size<T>::value,  "");
      using filtered = typename Filter::template apply<at<T, Offset>>;
      if (Comparer::template equal<filtered>(needle)) {
        visitor(
          indexed<at<T, Offset>, Offset>(),
          std::forward<Args>(args)...
        );
        return true;
      } else {
        return false;
      }
    } else if constexpr (Size == 2) {
      static_assert(Offset + 1 < size<T>::value,  "");
      using filtered = typename Filter::template apply<at<T, Offset>>;
      using filtered_next = typename Filter::template apply<at<T, Offset + 1>>;
      if (Comparer::template equal<filtered>(needle)) {
        visitor(
          indexed<at<T, Offset>, Offset>(),
          std::forward<Args>(args)...
        );
        return true;
      } else if (Comparer::template equal<filtered_next>(needle)) {
        visitor(
          indexed<at<T, Offset + 1>, Offset + 1>(),
          std::forward<Args>(args)...
        );
        return true;
      } else {
        return false;
      }
    } else {
      static_assert(Offset + (Size / 2) < size<T>::value, "");
      using pivot = at<T, Offset + (Size / 2)>;
      using filtered = typename Filter::template apply<pivot>;

      if (Comparer::template greater<filtered>(needle)) {
        return s<T, Offset, Size / 2>::template S<Comparer, Filter>(
          std::forward<Needle>(needle),
          std::forward<Visitor>(visitor),
          std::forward<Args>(args)...
        );
      } else if (Comparer::template less<filtered>(needle)) {
        return s<T, (Offset + Size / 2) + 1, Size / 2 - !(Size & 1)>::template S<Comparer, Filter>(
          std::forward<Needle>(needle),
          std::forward<Visitor>(visitor),
          std::forward<Args>(args)...
        );
      } else {
        visitor(
          indexed<pivot, Offset + (Size / 2)>(),
          std::forward<Args>(args)...
        );
        return true;
      }
    }
  }
};

template <typename T, std::size_t Offset = 0, std::size_t Size = size<T>::value>
struct FATAL_HIDE_SYMBOL f {
  template <
    typename Comparer,
    typename Filter,
    typename Needle,
    typename Fallback,
    typename Visitor,
    typename... Args
  >
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static constexpr decltype(auto) S(
    Needle &&needle,
    Fallback &&fallback,
    Visitor &&visitor,
    Args &&...args
  ) {
    if constexpr (Size == 0) {
      return std::forward<Fallback>(fallback);
    } else if constexpr (Size == 1) {
      static_assert(Offset < size<T>::value,  "");
      using filtered = typename Filter::template apply<at<T, Offset>>;
      if (Comparer::template equal<filtered>(needle)) {
        return visitor(
          indexed<at<T, Offset>, Offset>(),
          std::forward<Args>(args)...
        );
      } else {
        return std::forward<Fallback>(fallback);
      }
    } else if constexpr (Size == 2) {
      static_assert(Offset + 1 < size<T>::value,  "");
      using filtered = typename Filter::template apply<at<T, Offset>>;
      using filtered_next = typename Filter::template apply<at<T, Offset + 1>>;
      if (Comparer::template equal<filtered>(needle)) {
        return visitor(
          indexed<at<T, Offset>, Offset>(),
          std::forward<Args>(args)...
        );
      } else if (Comparer::template equal<filtered_next>(needle)) {
        return visitor(
          indexed<at<T, Offset + 1>, Offset + 1>(),
          std::forward<Args>(args)...
        );
      } else {
        return std::forward<Fallback>(fallback);
      }
    } else {
      static_assert(Offset + (Size / 2) < size<T>::value, "");
      using pivot = at<T, Offset + (Size / 2)>;
      using filtered = typename Filter::template apply<pivot>;
      if (Comparer::template greater<filtered>(needle)) {
        return f<T, Offset, Size / 2>::template S<Comparer, Filter>(
          std::forward<Needle>(needle),
          std::forward<Fallback>(fallback),
          std::forward<Visitor>(visitor),
          std::forward<Args>(args)...
        );
      } else if (Comparer::template less<filtered>(needle)) {
        return f<T, (Offset + Size / 2) + 1, Size / 2 - !(Size & 1)>::template S<Comparer, Filter>(
          std::forward<Needle>(needle),
          std::forward<Fallback>(fallback),
          std::forward<Visitor>(visitor),
          std::forward<Args>(args)...
        );
      } else {
        return visitor(
          indexed<pivot, Offset + (Size / 2)>(),
          std::forward<Args>(args)...
        );
      }
    }
  }
};

} // namespace i_S {
} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_type_impl_search_h
