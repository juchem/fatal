/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_math_numerics_h
#define FATAL_INCLUDE_fatal_math_numerics_h

#include <fatal/type/apply.h>
#include <fatal/type/conditional.h>
#include <fatal/type/get.h>
#include <fatal/type/list.h>
#include <fatal/type/logical.h>
#include <fatal/type/scalar.h>
#include <fatal/type/sort.h>
#include <fatal/type/traits.h>
#include <fatal/type/transform.h>
#include <fatal/type/unique.h>

#include <fatal/portability.h>

#include <type_traits>
#include <limits>
#include <stdexcept>

#include <cstdint>
#include <climits>

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

#include <fatal/math/impl/numerics.h>

namespace ftl {

/**
 * Returns the amount of bits that can be represented using the
 * memory taken by the given type.
 *
 * Example:
 *
 *  // yields `1`
 *  data_bits_v<bool>
 *
 *  // yields `CHAR_BIT`
 *  data_bits_v<char>
 *
 *  // yields `32`
 *  data_bits_v<std::int32_t>
 *
 *  // yields `1`
 *  data_bits<bool>::value
 *
 *  // yields `CHAR_BIT`
 *  data_bits<char>::value
 *
 *  // yields `32`
 *  data_bits<std::int32_t>::value
 *
 *  // yields `std::integral_constant<std::size_t, 1>`
 *  get_data_bits::apply<bool>
 *
 *  // yields `std::integral_constant<std::size_t, CHAR_BIT>`
 *  get_data_bits::apply<char>
 *
 *  // yields `std::integral_constant<std::size_t, 32>`
 *  get_data_bits::apply<std::int32_t>
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <typename T>
using data_bits = std::integral_constant<std::size_t, i_nm::data_bits_v<T>>;

template <typename T>
FATAL_HIDE_SYMBOL
constexpr auto data_bits_v = i_nm::data_bits_v<T>;

struct FATAL_HIDE_SYMBOL get_data_bits {
  template <typename T>
  using apply = data_bits<T>;
};

struct FATAL_HIDE_SYMBOL data_bits_eq {
  template <typename LHS, typename RHS>
  using apply = std::bool_constant<(data_bits_v<LHS> == data_bits_v<RHS>)>;
};

struct FATAL_HIDE_SYMBOL data_bits_ne {
  template <typename LHS, typename RHS>
  using apply = std::bool_constant<(data_bits_v<LHS> != data_bits_v<RHS>)>;
};

struct FATAL_HIDE_SYMBOL data_bits_lt {
  template <typename LHS, typename RHS>
  using apply = std::bool_constant<(data_bits_v<LHS> < data_bits_v<RHS>)>;
};

struct FATAL_HIDE_SYMBOL data_bits_le {
  template <typename LHS, typename RHS>
  using apply = std::bool_constant<(data_bits_v<LHS> <= data_bits_v<RHS>)>;
};

struct FATAL_HIDE_SYMBOL data_bits_gt {
  template <typename LHS, typename RHS>
  using apply = std::bool_constant<(data_bits_v<LHS> > data_bits_v<RHS>)>;
};

struct FATAL_HIDE_SYMBOL data_bits_ge {
  template <typename LHS, typename RHS>
  using apply = std::bool_constant<(data_bits_v<LHS> >= data_bits_v<RHS>)>;
};

template <std::size_t RHS>
struct FATAL_HIDE_SYMBOL data_bits_equal_to {
  template <typename LHS>
  using apply = std::bool_constant<(data_bits_v<LHS> == RHS)>;
};

template <std::size_t RHS>
struct FATAL_HIDE_SYMBOL data_bits_not_equal_to {
  template <typename LHS>
  using apply = std::bool_constant<(data_bits_v<LHS> != RHS)>;
};

template <std::size_t RHS>
struct FATAL_HIDE_SYMBOL data_bits_less_than {
  template <typename LHS>
  using apply = std::bool_constant<(data_bits_v<LHS> < RHS)>;
};

template <std::size_t RHS>
struct FATAL_HIDE_SYMBOL data_bits_less_than_equal {
  template <typename LHS>
  using apply = std::bool_constant<(data_bits_v<LHS> <= RHS)>;
};

template <std::size_t RHS>
struct FATAL_HIDE_SYMBOL data_bits_greater_than {
  template <typename LHS>
  using apply = std::bool_constant<(data_bits_v<LHS> > RHS)>;
};

template <std::size_t RHS>
struct FATAL_HIDE_SYMBOL data_bits_greater_than_equal {
  template <typename LHS>
  using apply = std::bool_constant<(data_bits_v<LHS> >= RHS)>;
};

// TODO: DOCUMENT AND TEST
template <typename T>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
typename std::make_signed<T>::type signed_cast(T value) {
  return static_cast<typename std::make_signed<T>::type>(value);
}

// TODO: DOCUMENT AND TEST
template <typename T>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
typename std::make_unsigned<T>::type unsigned_cast(T value) {
  return static_cast<typename std::make_unsigned<T>::type>(value);
}

/**
 * Reverses the bytes of an integral in Big Theta(lg k), where k is the size of
 * the integral in bytes.
 *
 * Example:
 *
 *  // yields `1`, assuming int is 32 bit
 *  auto result1 = reverse_integral_bits(0x01000000);
 *
 *  // yields `0x0a00`
 *  auto result2 = reverse_integral_bits<std::uint16_t>(0xa);
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <typename T>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
constexpr T reverse_integral_bytes(T value) noexcept {
  return i_nm::integral_reverse(value, 4);
}

/**
 * Reverses the bits of an integral in Big Theta(lg k), where k is the size of
 * the integral in bits.
 *
 * Example:
 *
 *  // yields `1`, assuming int is 32 bit
 *  auto result1 = reverse_integral_bits(0b10000000000000000000000000000000);
 *
 *  // yields `5000`
 *  auto result2 = reverse_integral_bits<std::uint16_t>(0xa);
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <typename T>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
constexpr T reverse_integral_bits(T value) noexcept {
  return i_nm::integral_reverse(value, 0);
}

/**
 * The maximum shift count that can be applied to a non-negative value of type
 * `T` without causing overflow, given that the value already occupies `Size`
 * bits.
 *
 * Shifting the least significant bit left by this amount will result in the
 * most significant non-sign bit of an integer.
 *
 * Example:
 *
 *  // yields `6`
 *  shift_left_count_upperbound<int8_t>::value
 *
 *  // yields `7`
 *  shift_left_count_upperbound<uint8_t>::value
 *
 *  // yields `5`
 *  shift_left_count_upperbound<int8_t, 2>::value
 *
 *  // yields `4`
 *  shift_left_count_upperbound<uint8_t, 4>::value
 *
 *  // yields `14`
 *  shift_left_count_upperbound<int16_t>::value
 *
 *  // yields `15`
 *  shift_left_count_upperbound<uint16_t>::value
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <typename T, std::size_t Size = 1>
using shift_left_count_upperbound = size_constant<i_nm::slcu<T, Size>()>;

/**
 * The upper bound on values of type `T` that can be shifted left by `Shift`
 * positions without overflow.
 *
 * Example:
 *
 *  template <typename T, std::size_t Shift>
 *  T foo(T value) {
 *    if (value < shift_left_upperbound<T>::value) {
 *      std::cout << "no overflow" << std::endl;
 *    } else {
 *      std::cout << "will overflow" << std::endl;
 *    }
 *    return value << Shift;
 *  }
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <typename T, std::size_t Shift = 1>
using shift_left_upperbound = std::integral_constant<
  T, (static_cast<T>(1) << shift_left_count_upperbound<T, Shift>::value)
>;

///////////////
// pop_count //
///////////////

// TODO: DOCUMENT AND TEST
template <std::uintmax_t Value>
using pop_count = std::integral_constant<
  std::size_t,
  i_nm::pop_count_impl(Value)
>;

/**
 * lg_2 returns the floor of the base 2 logarithm of n, which is the same as 
 * the 0-based position of the most significant bit of n, or 0 for no bits set
 *
 * Usage:
 *
 * // yields `4`
 * auto msb = lg_2(10);
 *
 * // yields `std::integral_constant<std::size_t, 4>`
 * using result = std::integral_constant<std::size_t, lg_2(10)>;
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <typename T>
constexpr std::size_t lg_2(T value) noexcept {
  return i_nm::lg_2(value);
}

/**
 * most_significant_bit returns the 1-based position of the
 * most significant bit of n, or 0 for no bits set
 *
 * This is the same as log_2(n) + 1, except for n = 0
 *
 * Usage:
 *
 * auto msb = most_significant_bit(10);
 *  // msb is now 4
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <typename T>
constexpr std::size_t most_significant_bit(T value) noexcept {
  return lg_2(value) + static_cast<bool>(value);
}

template <typename T>
constexpr std::size_t most_significant_bit_mask(T value) noexcept {
  return static_cast<T>(1) << lg_2(value);
}

template <typename T>
constexpr std::size_t least_significant_bit_mask(T value) noexcept {
  // TODO: can this be optimized further?
  return (value ^ (value - 1)) & value;
}

template <typename T>
constexpr std::size_t least_significant_bit_lg_2(T value) noexcept {
  // TODO: can this be optimized further?
  return lg_2(value ^ (value - 1));
}

////////////////////
// known integers //
////////////////////

using known_signed_integers = adjacent_unique_by<
  sort_by<
    list<
      short, int, long, long long,
      std::int8_t, std::int16_t, std::int32_t, std::int64_t,
      std::intptr_t, std::intmax_t, std::ptrdiff_t
#if __SIZEOF_INT128__
      , __int128_t
#endif // __SIZEOF_INT128__
    >,
    get_data_bits
  >,
  data_bits_eq
>;

static_assert(
  logical_and_of<
    transform<known_signed_integers, applier<std::is_signed>>
  >::value,
  "invalid signed integer"
);

using known_unsigned_integers = adjacent_unique_by<
  sort_by<
    list<
      bool, unsigned short, unsigned int, unsigned long, unsigned long long,
      std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t, std::size_t,
      std::uintptr_t, std::uintmax_t
#if __SIZEOF_INT128__
      , __uint128_t
#endif // __SIZEOF_INT128__
    >,
    get_data_bits
  >,
  data_bits_eq
>;

static_assert(
  logical_and_of<
    transform<known_unsigned_integers, applier<std::is_unsigned>>
  >::value,
  "invalid unsigned integer"
);

using known_fast_signed_integers = adjacent_unique_by<
  sort_by<
    list<std::int_fast8_t, std::int_fast16_t, std::int_fast32_t, std::int_fast64_t>,
    get_data_bits
  >,
  data_bits_eq
>;

static_assert(
  logical_and_of<
    transform<known_fast_signed_integers, applier<std::is_signed>>
  >::value,
  "invalid fast signed integer"
);

using known_fast_unsigned_integers = adjacent_unique_by<
  sort_by<
    list<bool, std::uint_fast8_t, std::uint_fast16_t, std::uint_fast32_t, std::uint_fast64_t>,
    get_data_bits
  >,
  data_bits_eq
>;

static_assert(
  logical_and_of<
    transform<known_fast_unsigned_integers, applier<std::is_unsigned>>
  >::value,
  "invalid fast unsigned integer"
);

using known_least_signed_integers = adjacent_unique_by<
  sort_by<
    list<std::int_least8_t, std::int_least16_t, std::int_least32_t, std::int_least64_t>,
    get_data_bits
  >,
  data_bits_eq
>;

static_assert(
  logical_and_of<
    transform<known_least_signed_integers, applier<std::is_signed>>
  >::value,
  "invalid least signed integer"
);

using known_least_unsigned_integers = adjacent_unique_by<
  sort_by<
    list<bool, std::uint_least8_t, std::uint_least16_t, std::uint_least32_t, std::uint_least64_t>,
    get_data_bits
  >,
  data_bits_eq
>;

static_assert(
  logical_and_of<
    transform<known_least_unsigned_integers, applier<std::is_unsigned>>
  >::value,
  "invalid least unsigned integer"
);

///////////////////////////
// known_floating_points //
///////////////////////////

using known_floating_points = adjacent_unique_by<
  sort_by<
    list<float, double, long double>,
    get_data_bits
  >,
  data_bits_eq
>;

static_assert(
  logical_and_of<
    transform<known_floating_points, applier<std::is_floating_point>>
  >::value,
  "invalid floating point"
);

/////////////////////////////////
// smallest_type_for_bit_count //
/////////////////////////////////

// TODO: DOCUMENT AND TEST

template <typename List, std::size_t BitCount>
using smallest_type_for_bit_count = typename i_nm::smallest_for_impl<
  List, BitCount
>::type;

template <typename List, std::size_t BitCount>
using exact_type_for_bit_count = get<
  List,
  std::integral_constant<std::size_t, BitCount>,
  get_data_bits
>;

/**
 * smallest_signed_integral & friends allow you to get the smallest possible
 * arithmetic type that can accommodate some data with at least the given bits size
 *
 * Say, for instance, you need to store 8 bits of data.
 * smallest_signed_integral<8> will provide a member typedef named type,
 * evaluating to int8_t.
 *
 *  smallest_signed_integral: chooses among int8_t, int16_t...
 *  smallest_fast_signed_integral: chooses among int_fast8_t, int_fast16_t...
 *  smallest_least_signed_integral: chooses among int_least8_t, int_least16_t...
 *  smallest_unsigned_integral: chooses among bool, uint8_t, uint16_t...
 *  smallest_fast_unsigned_integral: chooses among bool, uint_fast8_t,
 *                                   uint_fast16_t...
 *  smallest_least_unsigned_integral: chooses among bool, uint_least8_t,
 *                                    uint_least16_t...
 *  smallest_floating_point: chooses among float, double, long double...
 *
 * Example:
 *
 *  typename smallest_unsigned_integral<8>::type data = 255;
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <std::size_t BitCount>
using smallest_signed_integral = smallest_type_for_bit_count<
  known_signed_integers, BitCount
>;

template <std::size_t BitCount>
using smallest_unsigned_integral = smallest_type_for_bit_count<
  known_unsigned_integers, BitCount
>;

template <std::size_t BitCount>
using smallest_fast_signed_integral = smallest_type_for_bit_count<
  known_fast_signed_integers, BitCount
>;

template <std::size_t BitCount>
using smallest_fast_unsigned_integral = smallest_type_for_bit_count<
  known_fast_unsigned_integers, BitCount
>;

template <std::size_t BitCount>
using smallest_least_signed_integral = smallest_type_for_bit_count<
  known_least_signed_integers, BitCount
>;

template <std::size_t BitCount>
using smallest_least_unsigned_integral = smallest_type_for_bit_count<
  known_least_unsigned_integers, BitCount
>;

template <std::size_t BitCount>
using smallest_floating_point = smallest_type_for_bit_count<
  known_floating_points, BitCount
>;

/**
 * exact_signed_integral & friends allow you to get the arithmetic type
 * with exactly the given bits size
 *
 * Say, for instance, you need to store 8 bits of data.
 * exact_signed_integral<8> will provide a member typedef named type,
 * evaluating to int8_t.
 *
 *  exact_signed_integral: chooses among int8_t, int16_t...
 *  exact_fast_signed_integral: chooses among int_fast8_t, int_fast16_t...
 *  exact_least_signed_integral: chooses among int_least8_t, int_least16_t...
 *  exact_unsigned_integral: chooses among bool, uint8_t, uint16_t...
 *  exact_fast_unsigned_integral: chooses among bool, uint_fast8_t,
 *                                uint_fast16_t...
 *  exact_least_unsigned_integral: chooses among bool, uint_least8_t,
 *                                 uint_least16_t...
 *  exact_floating_point: chooses among float, double, long double...
 *
 * Example:
 *
 *  typename exact_unsigned_integral<8>::type data = 255;
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */

template <std::size_t BitCount>
using exact_signed_integral = exact_type_for_bit_count<
  known_signed_integers, BitCount
>;

template <std::size_t BitCount>
using exact_unsigned_integral = exact_type_for_bit_count<
  known_unsigned_integers, BitCount
>;

template <std::size_t BitCount>
using exact_fast_signed_integral = exact_type_for_bit_count<
  known_fast_signed_integers, BitCount
>;

template <std::size_t BitCount>
using exact_fast_unsigned_integral = exact_type_for_bit_count<
  known_fast_unsigned_integers, BitCount
>;

template <std::size_t BitCount>
using exact_least_signed_integral = exact_type_for_bit_count<
  known_least_signed_integers, BitCount
>;

template <std::size_t BitCount>
using exact_least_unsigned_integral = exact_type_for_bit_count<
  known_least_unsigned_integers, BitCount
>;

template <std::size_t BitCount>
using exact_floating_point = exact_type_for_bit_count<
  known_floating_points, BitCount
>;

/**
 * smallest_unsigned_for_value gives you the smallest possible unsigned integer type
 * able to store the given value.
 *
 * Example:
 *
 *  smallest_unsigned_for_value<255> data = 255;
 *
 * template <typename ...Args>
 * struct Foo {
 *   using type_id = smallest_unsigned_for_value<sizeof...(Args)>;
 *   ...
 * };
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <std::uintmax_t Value>
using smallest_unsigned_for_value = smallest_unsigned_integral<
  most_significant_bit(Value)
>;

template <std::uintmax_t Value>
using smallest_fast_unsigned_for_value = smallest_fast_unsigned_integral<
  most_significant_bit(Value)
>;

template <std::uintmax_t Value>
using smallest_least_unsigned_for_value = smallest_least_unsigned_integral<
  most_significant_bit(Value)
>;

/**
 * compile-time multiplication with overflow detection
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <unsigned long long lhs, unsigned long long rhs>
struct FATAL_HIDE_SYMBOL multiply_mp:
  public std::integral_constant<unsigned long long, lhs * rhs>
{
  static_assert(
    ((std::numeric_limits<decltype(rhs)>::max() / lhs) >= rhs),
    "multiplication overflow detected"
  );
};

template <unsigned long long lhs>
struct FATAL_HIDE_SYMBOL multiply_mp<lhs, 0>:
  public std::integral_constant<unsigned long long, 0>
{};

template <unsigned long long rhs>
struct FATAL_HIDE_SYMBOL multiply_mp<0, rhs>:
  public std::integral_constant<unsigned long long, 0>
{};

template <>
struct FATAL_HIDE_SYMBOL multiply_mp<0, 0>:
  public std::integral_constant<unsigned long long, 0>
{};

template <unsigned long long n>
struct FATAL_HIDE_SYMBOL square_mp:
  public multiply_mp<n, n>
{};

/**
 * pow_mp: O(lg exponent) compile-time pow()
 *
 * Exposes a static member constant value with the result of base ^ exponent.
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <unsigned long long base, unsigned long long exponent>
struct FATAL_HIDE_SYMBOL pow_mp:
  public std::integral_constant<
    unsigned long long,
    multiply_mp<
      exponent & 1 ? base : 1,
      pow_mp<
        square_mp<base>::value,
        exponent ? (exponent >> 1) : 0
      >::value
    >::value
  >
{};

template <unsigned long long exponent>
struct FATAL_HIDE_SYMBOL pow_mp<0, exponent>:
  public std::integral_constant<unsigned long long, 0>
{};

template <unsigned long long base>
struct FATAL_HIDE_SYMBOL pow_mp<base, 0>:
  public std::integral_constant<unsigned long long, 1>
{};

// http://en.wikipedia.org/wiki/Exponentiation#Zero_to_the_power_of_zero
template <>
struct FATAL_HIDE_SYMBOL pow_mp<0, 0>:
  public std::integral_constant<unsigned long long, 1>
{};

/**
 * unchecked_pow_mp: O(lg exponent) compile-time pow()
 * that allows unsigned multiplication overflows
 *
 * Exposes a static member constant value with the result of base ^ exponent.
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <unsigned long long base, unsigned long long exponent>
struct FATAL_HIDE_SYMBOL unchecked_pow_mp:
  public std::integral_constant<
    unsigned long long,
    (exponent & 1 ? base : 1)
      * unchecked_pow_mp<base * base, exponent ? (exponent >> 1) : 0>::value
  >
{};

template <unsigned long long exponent>
struct FATAL_HIDE_SYMBOL unchecked_pow_mp<0, exponent>:
  public std::integral_constant<unsigned long long, 0>
{};

template <unsigned long long base>
struct FATAL_HIDE_SYMBOL unchecked_pow_mp<base, 0>:
  public std::integral_constant<unsigned long long, 1>
{};

template <>
struct FATAL_HIDE_SYMBOL unchecked_pow_mp<0, 0>:
  public std::integral_constant<unsigned long long, 1>
{};

/**
 * is_power_of_two: O(1) compile-time check on whether n is a power of two.
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <typename T>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
constexpr bool is_power_of_two(T n) noexcept {
  return i_nm::is_power_of_two(n);
}

/**
 * is_mersenne_number: O(1) compile-time check on whether n
 * is a mersenne number.
 *
 * http://mathworld.wolfram.com/MersenneNumber.html
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <typename T>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
constexpr bool is_mersenne_number(T n) noexcept {
  return n == std::numeric_limits<T>::max()
    ? (n & 1) && (
      n == 1 || is_mersenne_number(n >> 1)
    )
    : n != 0 && !(n & (n + 1));
}

/**
 * An integral constant representing the Mersenne Number with the given
 * `Exponent`.
 *
 * A Mersenne Number is a natural number of the form `2^p - 1`. They are
 * particularly interesting because:
 *
 * - they are closely related to powers of two
 * - the modulo operation is cheaper when the divisor is a Mersenne number as
 *   opposed to a regular natural number which is not a power of two
 * - its exponent equals the amount of bits needed to represent the number
 * - some of these numbers are primes
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <std::size_t Exponent>
using mersenne_number = std::integral_constant<
  smallest_unsigned_integral<Exponent + 1>,
  (static_cast<smallest_unsigned_integral<Exponent + 1>>(1) << Exponent) - 1
>;

/**
 * An integral constant representing the exponent `p` for the given Mersenne
 * Number expressed by `2^p - 1`.
 *
 * See also: `mersenne_number`
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <typename T>
using mersenne_number_exponent = std::integral_constant<
  smallest_unsigned_for_value<most_significant_bit(T::value)>,
  most_significant_bit(T::value)
>;

/**
 * A sorted list of all Mersenne Numbers that can be expressed with standard
 * integer types.
 *
 * http://en.wikipedia.org/wiki/Mersenne_prime
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
using mersenne_primes = ftl::list<
  mersenne_number<2>, // 3
  mersenne_number<3>, // 7
  mersenne_number<5>, // 31
  mersenne_number<7>, // 127
  mersenne_number<13>, // 8191
  mersenne_number<17>, // 131071
  mersenne_number<19>, // 524287
  mersenne_number<31>, // 2147483647
  mersenne_number<61> // 2305843009213693951
>;

template <std::size_t Nth>
using nth_mersenne_prime = at<mersenne_primes, Nth>;

/**
 * largest_mersenne_prime_under: at compile-time, gets the largets mersenne
 * prime which can be stored in bits_size bits.
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <std::size_t bits_size>
using largest_mersenne_prime_under = last<
  filter<mersenne_primes, i_nm::data_bits_filter<bits_size>>
>;

/**
 * largest_mersenne_prime_for: at compile-time, gets the largets mersenne
 * prime which can be stored in the given type (with Diff bits to spare),
 * up to the Nth mersenne prime.
 *
 * Say T has a size of 8 bits and diff is 3, then the mersenne
 * prime returned will fit in at most 8 - 3 = 5 bits
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <typename T, std::size_t diff = 0>
using largest_mersenne_prime_for_type = typename std::enable_if<
  std::is_integral<T>::value
    && (data_bits_v<T> - std::is_signed_v<T> > diff),
  largest_mersenne_prime_under<
    data_bits_v<T> - std::is_signed_v<T> - diff
  >
>::type;

/**
 * An adapter to convert a discrete range into a continuous one,
 * given upper and lower bounds.
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <typename Discrete, typename Continuous>
struct FATAL_HIDE_SYMBOL discrete_to_continuous {
  using discrete_type = Discrete;
  using continuous_type = Continuous;

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  discrete_to_continuous(
    discrete_type discrete_min, discrete_type discrete_max,
    continuous_type continuous_min, continuous_type continuous_max
  ):
    discrete_min_(discrete_min),
    continuous_min_(continuous_min),
    normalizer_(discrete_max - discrete_min_),
    adjustment_(continuous_max - continuous_min_)
  {
    if(!normalizer_) {
      throw std::invalid_argument(
        "the difference between discrete_min and discrete_max cannot be zero"
      );
    }
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  continuous_type operator ()(discrete_type discrete) const {
    return (discrete - discrete_min_) / normalizer_
      * adjustment_ + continuous_min_;
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  discrete_type discrete_min() const { return discrete_min_; }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  discrete_type discrete_max() const { return normalizer_ + discrete_min_; }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  continuous_type min() const { return continuous_min_; }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  continuous_type max() const { return adjustment_ + continuous_min_; }

private:
  FATAL_HIDE_SYMBOL
  discrete_type const discrete_min_;
  FATAL_HIDE_SYMBOL
  continuous_type const continuous_min_;
  FATAL_HIDE_SYMBOL
  continuous_type const normalizer_;
  FATAL_HIDE_SYMBOL
  continuous_type const adjustment_;
};

} // namespace ftl {

FATAL_DIAGNOSTIC_POP

#endif // FATAL_INCLUDE_fatal_math_numerics_h
