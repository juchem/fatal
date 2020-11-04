#include <fatal/type/foreach.h>

#include <fatal/test/driver.h>
#include <fatal/log/log.h>

#include <string>
#include <vector>

namespace ftl {

struct example_visitor {
  template <typename Type, std::size_t Index>
  void operator()(
    indexed<Type, Index>,
    const std::string suffix,
    std::vector<std::string>& out
  ) {
    out.push_back(to_string(Type::get(), "_", Index, "_", suffix));
  }
};

FATAL_TEST(foreach, empty) {
  using types = list<>;

  auto actual = std::vector<std::string>{};
  foreach<types>(example_visitor(), "s", actual);

  const auto expected = std::vector<std::string>{};
  FATAL_EXPECT_TRUE(expected == actual);
}

FATAL_TEST(foreach, short list) {
  struct foo { static std::string get() { return "foo"; } };
  struct bar { static std::string get() { return "bar"; } };
  struct baz { static std::string get() { return "baz"; } };
  using types = list<foo, bar, baz>;

  auto actual = std::vector<std::string>{};
  foreach<types>(example_visitor(), "s", actual);

  const auto expected = std::vector<std::string>{
    "foo_0_s", "bar_1_s", "baz_2_s"
  };
  FATAL_EXPECT_TRUE(expected == actual);
}

template <typename, std::size_t N, typename = make_index_sequence<N>> struct replicate;

template <typename T, std::size_t... Indexes>
struct replicate<T, sizeof...(Indexes), index_sequence<Indexes...>> {
  template <std::size_t Index> using at = T;
  using type = list<at<Indexes>...>;
};

FATAL_TEST(foreach, very long list) {
  struct foo { static std::string get() { return "foo"; } };
  constexpr auto size = 4096;
  using types = replicate<foo, size>::type;
  static_assert(size_v<types> == size, "size mismatch");

  auto actual = std::vector<std::string>{};
  foreach<types>(example_visitor(), "s", actual);

  const auto expected = [] {
    std::vector<std::string> result;
    for (std::size_t i = 0; i < size; ++i) {
      result.push_back(to_string("foo_", i, "_s"));
    };
    return result;
  }();

  FATAL_EXPECT_TRUE(expected == actual);
}

constexpr auto const sum = [](auto tag, auto value) { return value + (tag_index(tag) + 1); };
constexpr auto const product = [](auto tag, auto value) { return value * (tag_index(tag) + 1); };

FATAL_TEST(foreach_accumulate, empty) {
  using types = list<>;
  FATAL_EXPECT_EQ(0, foreach_accumulate<types>(sum, 0));
  FATAL_EXPECT_EQ(1, foreach_accumulate<types>(sum, 1));
  FATAL_EXPECT_EQ(2, foreach_accumulate<types>(sum, 2));
  FATAL_EXPECT_EQ(3, foreach_accumulate<types>(sum, 3));
  FATAL_EXPECT_EQ(4, foreach_accumulate<types>(sum, 4));
  FATAL_EXPECT_EQ(5, foreach_accumulate<types>(sum, 5));
}

FATAL_TEST(foreach_accumulate, sum) {
  using types = replicate<void, 5>::type;
  FATAL_EXPECT_EQ(15, foreach_accumulate<types>(sum, 0));
  FATAL_EXPECT_EQ(16, foreach_accumulate<types>(sum, 1));
  FATAL_EXPECT_EQ(17, foreach_accumulate<types>(sum, 2));
  FATAL_EXPECT_EQ(18, foreach_accumulate<types>(sum, 3));
  FATAL_EXPECT_EQ(19, foreach_accumulate<types>(sum, 4));
  FATAL_EXPECT_EQ(20, foreach_accumulate<types>(sum, 5));
}

FATAL_TEST(foreach_accumulate, product) {
  using types = replicate<void, 5>::type;
  FATAL_EXPECT_EQ(0, foreach_accumulate<types>(product, 0));
  FATAL_EXPECT_EQ(120, foreach_accumulate<types>(product, 1));
  FATAL_EXPECT_EQ(240, foreach_accumulate<types>(product, 2));
  FATAL_EXPECT_EQ(360, foreach_accumulate<types>(product, 3));
  FATAL_EXPECT_EQ(480, foreach_accumulate<types>(product, 4));
  FATAL_EXPECT_EQ(600, foreach_accumulate<types>(product, 5));
}

} // namespace ftl {
