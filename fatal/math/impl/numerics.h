/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_math_impl_numerics_h
#define FATAL_INCLUDE_fatal_math_impl_numerics_h

namespace fatal {

struct FATAL_HIDE_SYMBOL get_data_bits;
struct FATAL_HIDE_SYMBOL data_bits_eq;

namespace i_nm {

template <typename T>
FATAL_HIDE_SYMBOL
constexpr std::size_t data_bits_v = std::is_same_v<bool, std::decay_t<T>>
  ? 1
  : sizeof(std::decay_t<T>) * CHAR_BIT;

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
constexpr std::size_t pop_count_impl(std::uintmax_t Value) noexcept {
  return Value
    ? pop_count_impl(Value & (Value - 1)) + 1
    : 0;
}

template <typename T>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
constexpr bool is_power_of_two(T n) noexcept {
  return n != 0 && !(n & (n - 1));
}

template <typename T, std::size_t Stage = data_bits_v<T> / 2>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
constexpr std::size_t lg_2([[maybe_unused]] T value, std::size_t result = 0) noexcept {
  static_assert(is_power_of_two(data_bits_v<T>));

  if constexpr (!Stage) {
    return result;
  } else {
    if (value & (static_cast<T>(static_cast<T>(static_cast<T>(1) << Stage) - 1) << Stage)) {
      return lg_2<T, Stage / 2>(value >> Stage, result |= Stage);
    } else {
      return lg_2<T, Stage / 2>(value, result);
    }
  }
}

static_assert(lg_2(0) == 0);
static_assert(lg_2(1) == 0);
static_assert(lg_2(2) == 1);
static_assert(lg_2(4) == 2);
static_assert(lg_2(8) == 3);

template <typename T, std::size_t Size>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
constexpr std::size_t slcu() noexcept {
  static_assert(
    Size + std::is_signed<T>::value <= data_bits_v<T>,
    "value already uses up all the bits available"
  );

  return data_bits_v<T> - Size - std::is_signed_v<T>;
};

template <typename T>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
constexpr T integral_reverse(
  T value,
  std::size_t end_phase,
  std::size_t phase,
  T mask
) noexcept {
  return phase == end_phase
    ? value
    : integral_reverse<T>(
      ((value >> phase) & mask) | ((value << phase) & ~mask),
      end_phase,
      phase >> 1,
      mask ^ (mask << (phase >> 1))
    );
}

template <
  typename T,
  typename U = typename std::make_unsigned<T>::type,
  std::size_t Phase = ((sizeof(T) * CHAR_BIT) >> 1)
>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
constexpr T integral_reverse(T value, std::size_t end_phase) noexcept {
  static_assert(sizeof(T) == sizeof(U), "internal error");
  static_assert(!(Phase & (Phase - 1)), "phase must be a power of two");
  static_assert(std::is_integral<T>::value, "only integrals can be reversed");

  return static_cast<T>(
    integral_reverse<U>(
      value, end_phase, Phase, U(~U(0)) ^ U(U(~U(0)) << Phase)
    )
  );
}

template <std::size_t BitSizeUpperBound>
struct FATAL_HIDE_SYMBOL data_bits_filter {
  template <typename T>
  using apply = std::bool_constant<(lg_2(T::value) < BitSizeUpperBound)>;
};

template <typename List, std::size_t BitCount>
struct FATAL_HIDE_SYMBOL smallest_for_impl {
  struct FATAL_HIDE_SYMBOL p {
    template <typename T>
    using apply = std::bool_constant<
      BitCount <= data_bits_v<T>
    >;
  };

  using filtered = filter<
    adjacent_unique_by<
      sort_by<List, get_data_bits>,
      data_bits_eq
    >,
    p
  >;

  static_assert(!empty_v<filtered>, "there's no known type to hold that many bits");

  using type = first<filtered>;

  static_assert(BitCount <= data_bits_v<type>, "there's no known type to hold that many bits");
};

} // namespace i_nm {
} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_math_impl_numerics_h
