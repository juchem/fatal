/*
 *  Copyright (c) 2014, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#include <fatal/container/static_array.h>

#include <fatal/type/list.h>

#include <fatal/test/driver.h>

#include <type_traits>

namespace fatal {

struct xyz { int x; int y; int z; };

template <typename T>
void check_empty() {
  using factory = typename static_array<>::type<T>;

  using empty_list_array = typename factory::template from_list<type_list<>>;
  FATAL_EXPECT_SAME<T, typename decltype(empty_list_array::get)::value_type>();
  EXPECT_EQ(0, empty_list_array::get.size());

  using empty_args_array = typename factory::template from_args<>;
  FATAL_EXPECT_SAME<T, typename decltype(empty_args_array::get)::value_type>();
  EXPECT_EQ(0, empty_args_array::get.size());
}

TEST(static_array, empty) {
  check_empty<int>();
  check_empty<xyz>();
}

struct check_array_visitor {
  template <typename TTag, typename U>
  void operator ()(TTag, U const &array) {
    ASSERT_LT(TTag::index, array.size());
    EXPECT_EQ(TTag::type::value, array[TTag::index]);
  }
};

template <typename TExpectedList, typename TArray>
void check_array() {
  EXPECT_EQ(TExpectedList::size, TArray::get.size());
  TExpectedList::foreach(check_array_visitor(), TArray::get);
}

template <int... Values>
void check_int() {
  using list = type_list<std::integral_constant<int, Values>...>;

  check_array<list, static_array<>::from_list<list>>();
  check_array<list, static_array<>::type<int>::from_list<list>>();

  check_array<
    list,
    static_array<>::from_args<std::integral_constant<int, Values>...>
  >();
  check_array<
    list,
    static_array<>::type<int>::from_args<std::integral_constant<int, Values>...>
  >();
}

TEST(static_array, int) {
  check_int<0>();
  check_int<1>();
  check_int<2>();
  check_int<100>();
  check_int<0, 1, 2>();
  check_int<1, 2, 3>();
  check_int<2, 3, 4>();
  check_int<25, 26, 27>();
  check_int<178, 849, 9, 11, 0>();
}

struct check_xyz_visitor {
  template <typename T, std::size_t Index, typename U>
  void operator ()(indexed_type_tag<T, Index>, U const &array) const {
    ASSERT_LT(Index, array.size());
    EXPECT_EQ(T::first::first::value, array[Index].x);
    EXPECT_EQ(T::first::second::value, array[Index].y);
    EXPECT_EQ(T::second::value, array[Index].z);
  }
};

template <typename TList, typename TArrayFactory>
void check_xyz_array() {
  using array = typename TArrayFactory::template from_list<TList>;
  static_assert(TList::size == array::get.size(), "size mismatch");
  TList::foreach(check_xyz_visitor(), array::get);
}

struct xyz_factory {
  template <typename T>
  static constexpr xyz create() {
    return xyz{
      T::type::first::first::value,
      T::type::first::second::value,
      T::type::second::value
    };
  }
};

template <int... Values>
void check_xyz() {
  using values = type_list<std::integral_constant<int, Values>...>;
  using x = typename values::template unzip<3, 0>;
  using y = typename values::template unzip<3, 1>;
  using z = typename values::template unzip<3, 2>;
  using list = typename x::template combine<>::template list<y>
    ::template combine<>::template list<z>;
  using factory = static_array<xyz_factory>;

  check_xyz_array<list, factory>();
  check_xyz_array<list, typename factory::template type<xyz>>();
}

TEST(static_array, struct) {
  check_xyz<0, 0, 0>();
  check_xyz<0, 1, 2>();
  check_xyz<99, 56, 43>();

  check_xyz<0, 0, 0, 0, 1, 2>();

  check_xyz<0, 0, 0, 0, 1, 2, 99, 56, 43>();
  check_xyz<0, 1, 2, 3, 4, 5, 6, 7, 8>();

  check_xyz<0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11>();
  check_xyz<99, 15, 62, 3, 8, 12, 0, 46, 85, 5, 1, 7>();
}

} // namespace fatal {
