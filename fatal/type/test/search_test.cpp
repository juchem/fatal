#include <fatal/type/search.h>

#include <fatal/type/list.h>
#include <fatal/type/sequence.h>

#include <fatal/test/driver.h>
#include <fatal/log/log.h>

namespace fatal {

// TODO: add tests with visitors that take args... (1, 2 and 3 args should suffice)
//  test for double moves

template <typename Expected, std::size_t ExpectedIndex>
struct search_visitor {
  template <typename Actual, std::size_t Index>
  void operator ()(indexed<Actual, Index>) const {
    FATAL_EXPECT_SAME<Expected, Actual>();
    FATAL_EXPECT_EQ(ExpectedIndex, Index);
  }

  template <typename Actual, std::size_t Index, typename Result>
  decltype(auto) operator ()(indexed<Actual, Index>, Result &&result) const {
    FATAL_EXPECT_SAME<Expected, Actual>();
    FATAL_EXPECT_EQ(ExpectedIndex, Index);
    return std::forward<Result>(result);
  }
};

template <std::size_t Value, std::size_t Index>
using value_search_visitor = search_visitor<size_constant<Value>, Index>;

FATAL_TEST(sorted_search, empty / no visitor) {
  using haystack = index_list<>;
  FATAL_EXPECT_FALSE(sorted_search<haystack>(10));
  FATAL_EXPECT_FALSE(sorted_search<haystack>(20));
  FATAL_EXPECT_FALSE(sorted_search<haystack>(30));
  FATAL_EXPECT_FALSE(sorted_search<haystack>(40));
  FATAL_EXPECT_FALSE(sorted_search<haystack>(50));
  FATAL_EXPECT_FALSE(sorted_search<haystack>(60));
}

FATAL_TEST(sorted_search, list / no visitor) {
  using haystack = index_list<10, 20, 30, 40, 50>;
  FATAL_EXPECT_TRUE(sorted_search<haystack>(10));
  FATAL_EXPECT_TRUE(sorted_search<haystack>(20));
  FATAL_EXPECT_TRUE(sorted_search<haystack>(30));
  FATAL_EXPECT_TRUE(sorted_search<haystack>(40));
  FATAL_EXPECT_TRUE(sorted_search<haystack>(50));
  FATAL_EXPECT_FALSE(sorted_search<haystack>(60));
}

FATAL_TEST(sorted_search, empty / visitor) {
  using haystack = index_list<>;
  FATAL_EXPECT_FALSE(sorted_search<haystack>(10, value_search_visitor<10, 0>()));
  FATAL_EXPECT_FALSE(sorted_search<haystack>(20, value_search_visitor<20, 1>()));
  FATAL_EXPECT_FALSE(sorted_search<haystack>(30, value_search_visitor<30, 2>()));
  FATAL_EXPECT_FALSE(sorted_search<haystack>(40, value_search_visitor<40, 3>()));
  FATAL_EXPECT_FALSE(sorted_search<haystack>(50, value_search_visitor<50, 4>()));
  FATAL_EXPECT_FALSE(sorted_search<haystack>(60, value_search_visitor<60, 5>()));
}

FATAL_TEST(sorted_search, list / visitor) {
  using haystack = index_list<10, 20, 30, 40, 50>;
  FATAL_EXPECT_TRUE(sorted_search<haystack>(10, value_search_visitor<10, 0>()));
  FATAL_EXPECT_TRUE(sorted_search<haystack>(20, value_search_visitor<20, 1>()));
  FATAL_EXPECT_TRUE(sorted_search<haystack>(30, value_search_visitor<30, 2>()));
  FATAL_EXPECT_TRUE(sorted_search<haystack>(40, value_search_visitor<40, 3>()));
  FATAL_EXPECT_TRUE(sorted_search<haystack>(50, value_search_visitor<50, 4>()));
  FATAL_EXPECT_FALSE(sorted_search<haystack>(60, value_search_visitor<60, 5>()));
}

FATAL_TEST(sorted_find, empty) {
  using haystack = index_list<>;
  FATAL_EXPECT_EQ(999, sorted_find<haystack>(10, 999, value_search_visitor<10, 0>(), 100));
  FATAL_EXPECT_EQ(999, sorted_find<haystack>(20, 999, value_search_visitor<20, 1>(), 200));
  FATAL_EXPECT_EQ(999, sorted_find<haystack>(30, 999, value_search_visitor<30, 2>(), 300));
  FATAL_EXPECT_EQ(999, sorted_find<haystack>(40, 999, value_search_visitor<40, 3>(), 400));
  FATAL_EXPECT_EQ(999, sorted_find<haystack>(50, 999, value_search_visitor<50, 4>(), 500));
  FATAL_EXPECT_EQ(999, sorted_find<haystack>(60, 999, value_search_visitor<60, 5>(), 600));
}

FATAL_TEST(sorted_find, list) {
  using haystack = index_list<10, 20, 30, 40, 50>;
  FATAL_EXPECT_EQ(100, sorted_find<haystack>(10, 999, value_search_visitor<10, 0>(), 100));
  FATAL_EXPECT_EQ(200, sorted_find<haystack>(20, 999, value_search_visitor<20, 1>(), 200));
  FATAL_EXPECT_EQ(300, sorted_find<haystack>(30, 999, value_search_visitor<30, 2>(), 300));
  FATAL_EXPECT_EQ(400, sorted_find<haystack>(40, 999, value_search_visitor<40, 3>(), 400));
  FATAL_EXPECT_EQ(500, sorted_find<haystack>(50, 999, value_search_visitor<50, 4>(), 500));
  FATAL_EXPECT_EQ(999, sorted_find<haystack>(60, 999, value_search_visitor<60, 5>(), 600));
}

FATAL_TEST(unsorted_search, empty / no visitor) {
  using haystack = index_list<>;
  FATAL_EXPECT_FALSE(unsorted_search<haystack>(10));
  FATAL_EXPECT_FALSE(unsorted_search<haystack>(20));
  FATAL_EXPECT_FALSE(unsorted_search<haystack>(30));
  FATAL_EXPECT_FALSE(unsorted_search<haystack>(40));
  FATAL_EXPECT_FALSE(unsorted_search<haystack>(50));
  FATAL_EXPECT_FALSE(unsorted_search<haystack>(60));
}

FATAL_TEST(unsorted_search, sorted list / no visitor) {
  using haystack = index_list<10, 20, 30, 40, 50>;
  FATAL_EXPECT_TRUE(unsorted_search<haystack>(10));
  FATAL_EXPECT_TRUE(unsorted_search<haystack>(20));
  FATAL_EXPECT_TRUE(unsorted_search<haystack>(30));
  FATAL_EXPECT_TRUE(unsorted_search<haystack>(40));
  FATAL_EXPECT_TRUE(unsorted_search<haystack>(50));
  FATAL_EXPECT_FALSE(unsorted_search<haystack>(60));
}

FATAL_TEST(unsorted_search, unsorted list / no visitor) {
  using haystack = index_list<50, 20, 10, 40, 30>;
  FATAL_EXPECT_TRUE(unsorted_search<haystack>(10));
  FATAL_EXPECT_TRUE(unsorted_search<haystack>(20));
  FATAL_EXPECT_TRUE(unsorted_search<haystack>(30));
  FATAL_EXPECT_TRUE(unsorted_search<haystack>(40));
  FATAL_EXPECT_TRUE(unsorted_search<haystack>(50));
  FATAL_EXPECT_FALSE(unsorted_search<haystack>(60));
}

FATAL_TEST(unsorted_search, empty / visitor) {
  using haystack = index_list<>;
  FATAL_EXPECT_FALSE(unsorted_search<haystack>(10, value_search_visitor<10, 0>()));
  FATAL_EXPECT_FALSE(unsorted_search<haystack>(20, value_search_visitor<20, 1>()));
  FATAL_EXPECT_FALSE(unsorted_search<haystack>(30, value_search_visitor<30, 2>()));
  FATAL_EXPECT_FALSE(unsorted_search<haystack>(40, value_search_visitor<40, 3>()));
  FATAL_EXPECT_FALSE(unsorted_search<haystack>(50, value_search_visitor<50, 4>()));
  FATAL_EXPECT_FALSE(unsorted_search<haystack>(60, value_search_visitor<60, 5>()));
}

FATAL_TEST(unsorted_search, sorted list / visitor) {
  using haystack = index_list<10, 20, 30, 40, 50>;
  FATAL_EXPECT_TRUE(unsorted_search<haystack>(10, value_search_visitor<10, 0>()));
  FATAL_EXPECT_TRUE(unsorted_search<haystack>(20, value_search_visitor<20, 1>()));
  FATAL_EXPECT_TRUE(unsorted_search<haystack>(30, value_search_visitor<30, 2>()));
  FATAL_EXPECT_TRUE(unsorted_search<haystack>(40, value_search_visitor<40, 3>()));
  FATAL_EXPECT_TRUE(unsorted_search<haystack>(50, value_search_visitor<50, 4>()));
  FATAL_EXPECT_FALSE(unsorted_search<haystack>(60, value_search_visitor<60, 5>()));
}

FATAL_TEST(unsorted_search, unsorted list / visitor) {
  using haystack = index_list<50, 20, 10, 40, 30>;
  FATAL_EXPECT_TRUE(unsorted_search<haystack>(10, value_search_visitor<10, 0>()));
  FATAL_EXPECT_TRUE(unsorted_search<haystack>(20, value_search_visitor<20, 1>()));
  FATAL_EXPECT_TRUE(unsorted_search<haystack>(30, value_search_visitor<30, 2>()));
  FATAL_EXPECT_TRUE(unsorted_search<haystack>(40, value_search_visitor<40, 3>()));
  FATAL_EXPECT_TRUE(unsorted_search<haystack>(50, value_search_visitor<50, 4>()));
  FATAL_EXPECT_FALSE(unsorted_search<haystack>(60, value_search_visitor<60, 5>()));
}

FATAL_TEST(unsorted_find, empty) {
  using haystack = index_list<>;
  FATAL_EXPECT_EQ(999, unsorted_find<haystack>(10, 999, value_search_visitor<10, 0>(), 100));
  FATAL_EXPECT_EQ(999, unsorted_find<haystack>(20, 999, value_search_visitor<20, 1>(), 200));
  FATAL_EXPECT_EQ(999, unsorted_find<haystack>(30, 999, value_search_visitor<30, 2>(), 300));
  FATAL_EXPECT_EQ(999, unsorted_find<haystack>(40, 999, value_search_visitor<40, 3>(), 400));
  FATAL_EXPECT_EQ(999, unsorted_find<haystack>(50, 999, value_search_visitor<50, 4>(), 500));
  FATAL_EXPECT_EQ(999, unsorted_find<haystack>(60, 999, value_search_visitor<60, 5>(), 600));
}

FATAL_TEST(unsorted_find, sorted list) {
  using haystack = index_list<50, 20, 10, 40, 30>;
  FATAL_EXPECT_EQ(100, unsorted_find<haystack>(10, 999, value_search_visitor<10, 0>(), 100));
  FATAL_EXPECT_EQ(200, unsorted_find<haystack>(20, 999, value_search_visitor<20, 1>(), 200));
  FATAL_EXPECT_EQ(300, unsorted_find<haystack>(30, 999, value_search_visitor<30, 2>(), 300));
  FATAL_EXPECT_EQ(400, unsorted_find<haystack>(40, 999, value_search_visitor<40, 3>(), 400));
  FATAL_EXPECT_EQ(500, unsorted_find<haystack>(50, 999, value_search_visitor<50, 4>(), 500));
  FATAL_EXPECT_EQ(999, unsorted_find<haystack>(60, 999, value_search_visitor<60, 5>(), 600));
}

FATAL_TEST(unsorted_find, unsorted list) {
  using haystack = index_list<10, 20, 30, 40, 50>;
  FATAL_EXPECT_EQ(100, unsorted_find<haystack>(10, 999, value_search_visitor<10, 0>(), 100));
  FATAL_EXPECT_EQ(200, unsorted_find<haystack>(20, 999, value_search_visitor<20, 1>(), 200));
  FATAL_EXPECT_EQ(300, unsorted_find<haystack>(30, 999, value_search_visitor<30, 2>(), 300));
  FATAL_EXPECT_EQ(400, unsorted_find<haystack>(40, 999, value_search_visitor<40, 3>(), 400));
  FATAL_EXPECT_EQ(500, unsorted_find<haystack>(50, 999, value_search_visitor<50, 4>(), 500));
  FATAL_EXPECT_EQ(999, unsorted_find<haystack>(60, 999, value_search_visitor<60, 5>(), 600));
}

FATAL_TEST(index_search, empty) {
  using haystack = list<>;
  FATAL_EXPECT_FALSE(index_search<haystack>(0, search_visitor<void, 0>()));
  FATAL_EXPECT_FALSE(index_search<haystack>(1, search_visitor<bool, 1>()));
  FATAL_EXPECT_FALSE(index_search<haystack>(2, search_visitor<double, 2>()));
  FATAL_EXPECT_FALSE(index_search<haystack>(3, search_visitor<int, 3>()));
  FATAL_EXPECT_FALSE(index_search<haystack>(4, search_visitor<unsigned, 4>()));
  FATAL_EXPECT_FALSE(index_search<haystack>(5, search_visitor<void *, 5>()));
}

FATAL_TEST(index_search, list) {
  using haystack = list<void, bool, double, int, unsigned>;
  FATAL_EXPECT_TRUE(index_search<haystack>(0, search_visitor<void, 0>()));
  FATAL_EXPECT_TRUE(index_search<haystack>(1, search_visitor<bool, 1>()));
  FATAL_EXPECT_TRUE(index_search<haystack>(2, search_visitor<double, 2>()));
  FATAL_EXPECT_TRUE(index_search<haystack>(3, search_visitor<int, 3>()));
  FATAL_EXPECT_TRUE(index_search<haystack>(4, search_visitor<unsigned, 4>()));
  FATAL_EXPECT_FALSE(index_search<haystack>(5, search_visitor<void *, 5>()));
}

FATAL_TEST(index_find, empty) {
  using haystack = list<>;
  FATAL_EXPECT_EQ(999, index_find<haystack>(0, 999, search_visitor<void, 0>(), 100));
  FATAL_EXPECT_EQ(999, index_find<haystack>(1, 999, search_visitor<bool, 1>(), 200));
  FATAL_EXPECT_EQ(999, index_find<haystack>(2, 999, search_visitor<double, 2>(), 300));
  FATAL_EXPECT_EQ(999, index_find<haystack>(3, 999, search_visitor<int, 3>(), 400));
  FATAL_EXPECT_EQ(999, index_find<haystack>(4, 999, search_visitor<unsigned, 4>(), 500));
  FATAL_EXPECT_EQ(999, index_find<haystack>(5, 999, search_visitor<void *, 5>(), 600));
}

FATAL_TEST(index_find, list) {
  using haystack = list<void, bool, double, int, unsigned>;
  FATAL_EXPECT_EQ(100, index_find<haystack>(0, 999, search_visitor<void, 0>(), 100));
  FATAL_EXPECT_EQ(200, index_find<haystack>(1, 999, search_visitor<bool, 1>(), 200));
  FATAL_EXPECT_EQ(300, index_find<haystack>(2, 999, search_visitor<double, 2>(), 300));
  FATAL_EXPECT_EQ(400, index_find<haystack>(3, 999, search_visitor<int, 3>(), 400));
  FATAL_EXPECT_EQ(500, index_find<haystack>(4, 999, search_visitor<unsigned, 4>(), 500));
  FATAL_EXPECT_EQ(999, index_find<haystack>(5, 999, search_visitor<void *, 5>(), 600));
}

FATAL_TEST(sorted_find, fallback/visitor type compatibility) {
  using haystack = index_list<10, 20, 30, 40, 50>;
  constexpr auto const value = "hello, world!";
  FATAL_EXPECT_EQ(value, sorted_find<haystack>(10, nullptr, [=](auto) { return value; }));
  FATAL_EXPECT_EQ(nullptr, sorted_find<haystack>(60, nullptr, [=](auto) { return value; }));
}
FATAL_TEST(unsorted_find, fallback/visitor type compatibility) {
  using haystack = index_list<10, 20, 30, 40, 50>;
  constexpr auto const value = "hello, world!";
  FATAL_EXPECT_EQ(value, unsorted_find<haystack>(10, nullptr, [=](auto) { return value; }));
  FATAL_EXPECT_EQ(nullptr, unsorted_find<haystack>(60, nullptr, [=](auto) { return value; }));
}

} // namespace fatal {
