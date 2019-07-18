/*
 *  Copyright (c) 2019, Marcelo Juchem
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#include <fatal/container/bit_set.h>

#include <fatal/test/driver.h>

namespace fatal {

FATAL_TEST(bit_set, ctor 0) {
  bit_set<0> const s;
  FATAL_EXPECT_EQ(0, s.size());
}

FATAL_TEST(bit_set, flipped ctor 0) {
  bit_set<0> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(0, s.size());
}

FATAL_TEST(bit_set, ctor 1) {
  bit_set<1> const s;
  FATAL_EXPECT_EQ(1, s.size());
  FATAL_EXPECT_FALSE(s.get(0));
}

FATAL_TEST(bit_set, flipped ctor 1) {
  bit_set<1> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(1, s.size());
  FATAL_EXPECT_TRUE(s.get(0));
}

FATAL_TEST(bit_set, ctor 2) {
  bit_set<2> const s;
  FATAL_EXPECT_EQ(2, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 2) {
  bit_set<2> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(2, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, ctor 3) {
  bit_set<3> const s;
  FATAL_EXPECT_EQ(3, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 3) {
  bit_set<3> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(3, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, ctor 4) {
  bit_set<4> const s;
  FATAL_EXPECT_EQ(4, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 4) {
  bit_set<4> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(4, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, ctor 5) {
  bit_set<5> const s;
  FATAL_EXPECT_EQ(5, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 5) {
  bit_set<5> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(5, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, ctor 6) {
  bit_set<6> const s;
  FATAL_EXPECT_EQ(6, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 6) {
  bit_set<6> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(6, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, ctor 7) {
  bit_set<7> const s;
  FATAL_EXPECT_EQ(7, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 7) {
  bit_set<7> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(7, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, ctor 8) {
  bit_set<8> const s;
  FATAL_EXPECT_EQ(8, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 8) {
  bit_set<8> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(8, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, ctor 9) {
  bit_set<9> const s;
  FATAL_EXPECT_EQ(9, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 9) {
  bit_set<9> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(9, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, ctor 10) {
  bit_set<10> const s;
  FATAL_EXPECT_EQ(10, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 10) {
  bit_set<10> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(10, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, ctor 11) {
  bit_set<11> const s;
  FATAL_EXPECT_EQ(11, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 11) {
  bit_set<11> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(11, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, ctor 31) {
  bit_set<31> const s;
  FATAL_EXPECT_EQ(31, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 31) {
  bit_set<31> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(31, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, ctor 32) {
  bit_set<32> const s;
  FATAL_EXPECT_EQ(32, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 32) {
  bit_set<32> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(32, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, ctor 33) {
  bit_set<33> const s;
  FATAL_EXPECT_EQ(33, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 33) {
  bit_set<33> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(33, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, ctor 63) {
  bit_set<63> const s;
  FATAL_EXPECT_EQ(63, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 63) {
  bit_set<63> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(63, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, ctor 64) {
  bit_set<64> const s;
  FATAL_EXPECT_EQ(64, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 64) {
  bit_set<64> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(64, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, ctor 65) {
  bit_set<65> const s;
  FATAL_EXPECT_EQ(65, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 65) {
  bit_set<65> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(65, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, ctor 100) {
  bit_set<100> const s;
  FATAL_EXPECT_EQ(100, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 100) {
  bit_set<100> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(100, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, ctor 127) {
  bit_set<127> const s;
  FATAL_EXPECT_EQ(127, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 127) {
  bit_set<127> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(127, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, ctor 128) {
  bit_set<128> const s;
  FATAL_EXPECT_EQ(128, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 128) {
  bit_set<128> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(128, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, ctor 129) {
  bit_set<129> const s;
  FATAL_EXPECT_EQ(129, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 129) {
  bit_set<129> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(129, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, ctor 511) {
  bit_set<511> const s;
  FATAL_EXPECT_EQ(511, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 511) {
  bit_set<511> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(511, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, ctor 512) {
  bit_set<512> const s;
  FATAL_EXPECT_EQ(512, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 512) {
  bit_set<512> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(512, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, ctor 513) {
  bit_set<513> const s;
  FATAL_EXPECT_EQ(513, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 513) {
  bit_set<513> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(513, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, ctor 1000) {
  bit_set<1000> const s;
  FATAL_EXPECT_EQ(1000, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, flipped ctor 1000) {
  bit_set<1000> const s(flipped_bit_set{});
  FATAL_EXPECT_EQ(1000, s.size());
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}



FATAL_TEST(bit_set, flip) {
  bit_set<1000> s;

  s.flip();
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }

  s.flip();
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }

  for (auto i = s.size(); i--; ) {
    s.flip(i);
    FATAL_EXPECT_TRUE(s.get(i));
  }

  for (auto i = s.size(); i--; ) {
    s.flip(i);
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, set zeroed) {
  bit_set<1000> s;

  s.set();
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }

  s.set();
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, set flipped) {
  bit_set<1000> s(flipped_bit_set{});

  s.set();
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }

  s.set();
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, set i zeroed) {
  bit_set<1000> s;

  for (auto i = s.size(); i--; ) {
    s.set(i);
    FATAL_EXPECT_TRUE(s.get(i));
  }

  for (auto i = s.size(); i--; ) {
    s.set(i);
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, set i flipped) {
  bit_set<1000> s(flipped_bit_set{});

  for (auto i = s.size(); i--; ) {
    s.set(i);
    FATAL_EXPECT_TRUE(s.get(i));
  }

  for (auto i = s.size(); i--; ) {
    s.set(i);
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, reset zeroed) {
  bit_set<1000> s;

  s.reset();
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }

  s.reset();
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, reset flipped) {
  bit_set<1000> s(flipped_bit_set{});

  s.reset();
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }

  s.reset();
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, reset i zeroed) {
  bit_set<1000> s;

  for (auto i = s.size(); i--; ) {
    s.reset(i);
    FATAL_EXPECT_FALSE(s.get(i));
  }

  for (auto i = s.size(); i--; ) {
    s.reset(i);
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, reset i flipped) {
  bit_set<1000> s(flipped_bit_set{});

  for (auto i = s.size(); i--; ) {
    s.reset(i);
    FATAL_EXPECT_FALSE(s.get(i));
  }

  for (auto i = s.size(); i--; ) {
    s.reset(i);
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, next_set zeroed) {
  bit_set<1000> const s;

  for (std::size_t i = 0; i < s.size(); ++i) {
    FATAL_EXPECT_EQ(s.size(), s.next_set(i));
  }
}

FATAL_TEST(bit_set, next_set flipped) {
  bit_set<1000> const s(flipped_bit_set{});

  for (std::size_t i = 0; i < s.size(); ++i) {
    FATAL_EXPECT_EQ(i + 1, s.next_set(i));
  }
}

FATAL_TEST(bit_set, next_set zeroed interleaved) {
  constexpr auto size = 1000;
  for (std::size_t step = 2; step < size / 2; ++step) {
    bit_set<size> s;
    for (std::size_t i = 0; i < s.size(); i += step) {
      s.set(i);
    }

    for (std::size_t i = 0; i < s.size(); ++i) {
      FATAL_EXPECT_EQ(std::min<std::size_t>(size, i + (step - i % step)), s.next_set(i));
    }
  }
}

FATAL_TEST(bit_set, next_set flipped interleaved) {
  constexpr auto size = 1000;
  for (std::size_t step = 2; step < size / 2; ++step) {
    bit_set<size> s(flipped_bit_set{});
    for (std::size_t i = 0; i < s.size(); i += step) {
      s.reset(i);
    }

    for (std::size_t i = 0; i < s.size(); ++i) {
      FATAL_EXPECT_EQ(std::min<std::size_t>(size, i + 1 + ((i + 1) % step == 0)), s.next_set(i));
    }
  }
}

FATAL_TEST(bit_set, previous_set zeroed) {
  bit_set<1000> const s;

  for (std::size_t i = 0; i < s.size(); ++i) {
    FATAL_EXPECT_EQ(s.size(), s.previous_set(i));
  }
}

FATAL_TEST(bit_set, previous_set flipped) {
  bit_set<1000> const s(flipped_bit_set{});

  for (std::size_t i = 0; i < s.size(); ++i) {
    FATAL_EXPECT_EQ(i ? i - 1 : s.size(), s.previous_set(i));
  }
}

FATAL_TEST(bit_set, previous_set zeroed interleaved) {
  constexpr auto size = 1000;
  for (std::size_t step = 2; step < size / 2; ++step) {
    bit_set<size> s;
    for (std::size_t i = 0; i < s.size(); i += step) {
      s.set(i);
    }

    for (std::size_t i = 0; i < s.size(); ++i) {
      FATAL_EXPECT_EQ(i ? i - (i % step ? i % step : step) : size, s.previous_set(i));
    }
  }
}

FATAL_TEST(bit_set, previous_set flipped interleaved) {
  constexpr auto size = 1000;
  for (std::size_t step = 2; step < size / 2; ++step) {
    bit_set<size> s(flipped_bit_set{});
    for (std::size_t i = 0; i < s.size(); i += step) {
      s.reset(i);
    }

    for (std::size_t i = 0; i < s.size(); ++i) {
      FATAL_EXPECT_EQ(i > 1 ? i - 1 - ((i - 1) % step == 0) : size, s.previous_set(i));
    }
  }
}

FATAL_TEST(bit_set, next_reset zeroed) {
  bit_set<1000> const s;

  for (std::size_t i = 0; i < s.size(); ++i) {
    FATAL_EXPECT_EQ(i + 1, s.next_reset(i));
  }
}

FATAL_TEST(bit_set, next_reset flipped) {
  bit_set<1000> const s(flipped_bit_set{});

  for (std::size_t i = 0; i < s.size(); ++i) {
    FATAL_EXPECT_EQ(s.size(), s.next_reset(i));
  }
}

FATAL_TEST(bit_set, next_reset zeroed interleaved) {
  constexpr auto size = 1000;
  for (std::size_t step = 2; step < size / 2; ++step) {
    bit_set<size> s;
    for (std::size_t i = 0; i < s.size(); i += step) {
      s.set(i);
    }

    for (std::size_t i = 0; i < s.size(); ++i) {
      FATAL_EXPECT_EQ(std::min<std::size_t>(size, i + 1 + ((i + 1) % step == 0)), s.next_reset(i));
    }
  }
}

FATAL_TEST(bit_set, next_reset flipped interleaved) {
  constexpr auto size = 1000;
  for (std::size_t step = 2; step < size / 2; ++step) {
    bit_set<size> s(flipped_bit_set{});
    for (std::size_t i = 0; i < s.size(); i += step) {
      s.reset(i);
    }

    for (std::size_t i = 0; i < s.size(); ++i) {
      FATAL_EXPECT_EQ(std::min<std::size_t>(size, i + (step - i % step)), s.next_reset(i));
    }
  }
}

FATAL_TEST(bit_set, previous_reset zeroed) {
  bit_set<1000> const s;

  for (std::size_t i = 0; i < s.size(); ++i) {
    FATAL_EXPECT_EQ(i ? i - 1 : s.size(), s.previous_reset(i));
  }
}

FATAL_TEST(bit_set, previous_reset flipped) {
  bit_set<1000> const s(flipped_bit_set{});

  for (std::size_t i = 0; i < s.size(); ++i) {
    FATAL_EXPECT_EQ(s.size(), s.previous_reset(i));
  }
}

FATAL_TEST(bit_set, previous_reset zeroed interleaved) {
  constexpr auto size = 1000;
  for (std::size_t step = 2; step < size / 2; ++step) {
    bit_set<size> s;
    for (std::size_t i = 0; i < s.size(); i += step) {
      s.set(i);
    }

    for (std::size_t i = 0; i < s.size(); ++i) {
      FATAL_EXPECT_EQ(i > 1 ? i - 1 - ((i - 1) % step == 0) : size, s.previous_reset(i));
    }
  }
}

FATAL_TEST(bit_set, previous_reset flipped interleaved) {
  constexpr auto size = 1000;
  for (std::size_t step = 2; step < size / 2; ++step) {
    bit_set<size> s(flipped_bit_set{});
    for (std::size_t i = 0; i < s.size(); i += step) {
      s.reset(i);
    }

    for (std::size_t i = 0; i < s.size(); ++i) {
      FATAL_EXPECT_EQ(i ? i - (i % step ? i % step : step) : size, s.previous_reset(i));
    }
  }
}

FATAL_TEST(bit_set, range_on zeroed) {
  bit_set<1000> const s;

  auto const range = s.range_on();
  FATAL_EXPECT_EQ(range.begin(), range.end());
  FATAL_EXPECT_EQ(range.cbegin(), range.cend());
}

FATAL_TEST(bit_set, range_on flipped) {
  bit_set<1000> const s(flipped_bit_set{});

  auto const range = s.range_on();
  auto k = range.begin();
  for (std::size_t i = 0; i < s.size(); ++i, ++k) {
    FATAL_ASSERT_NE(k, range.end());
    FATAL_EXPECT_EQ(i, *k);
  }
  FATAL_EXPECT_EQ(k, range.end());

  for (std::size_t i = s.size(); i--; ) {
    FATAL_ASSERT_NE(k, range.begin());
    FATAL_ASSERT_NE(--k, range.end());
    FATAL_EXPECT_EQ(i, *k);
  }
  FATAL_EXPECT_EQ(k, range.begin());

  for (std::size_t i = 0; i < s.size(); ++i) {
    FATAL_ASSERT_NE(k, range.end());
    FATAL_EXPECT_EQ(i, *k++);
  }
  FATAL_EXPECT_EQ(k, range.end());

  for (std::size_t i = s.size(); i--; ) {
    FATAL_ASSERT_NE(k--, range.begin());
    FATAL_ASSERT_NE(k, range.end());
    FATAL_EXPECT_EQ(i, *k);
  }
  FATAL_EXPECT_EQ(k, range.begin());
}

FATAL_TEST(bit_set, range_on zeroed interleaved) {
  constexpr auto size = 1000;
  for (std::size_t step = 2; step < size / 2; ++step) {
    bit_set<size> s;
    for (std::size_t i = 0; i < s.size(); i += step) {
      s.reset(i);
    }

    auto const range = s.range_on();
    auto k = range.begin();
    for (std::size_t i = 1; k != range.end(); i += (step - i % step), ++k) {
      FATAL_EXPECT_EQ(i, *k);
    }
    FATAL_EXPECT_EQ(k, range.end());

    for (auto i = s.size() - 1 - ((s.size() - 1) % step ? (s.size() - 1) % step : step);
      k != range.begin();
      i -= (i % step ? i % step : step)
    ) {
      FATAL_ASSERT_NE(--k, range.end());
      FATAL_EXPECT_EQ(i, *k);
    }
    FATAL_EXPECT_EQ(k, range.begin());

    for (std::size_t i = 1; k != range.end(); i += (step - i % step)) {
      FATAL_EXPECT_EQ(i, *k++);
    }
    FATAL_EXPECT_EQ(k, range.end());

    for (auto i = s.size() - 1 - ((s.size() - 1) % step ? (s.size() - 1) % step : step);
      k != range.begin();
      i -= (i % step ? i % step : step)
    ) {
      k--;
      FATAL_ASSERT_NE(k, range.end());
      FATAL_EXPECT_EQ(i, *k);
    }
    FATAL_EXPECT_EQ(k, range.begin());
  }
}

FATAL_TEST(bit_set, range_on flipped interleaved) {
  constexpr auto size = 1000;
  for (std::size_t step = 2; step < size / 2; ++step) {
    bit_set<size> s(flipped_bit_set{});
    for (std::size_t i = 0; i < s.size(); i += step) {
      s.reset(i);
    }

    auto const range = s.range_on();
    auto k = range.begin();
    for (std::size_t i = 1; k != range.end(); i += 1 + ((i + 1) % step == 0), ++k) {
      FATAL_EXPECT_EQ(i, *k);
    }
    FATAL_EXPECT_EQ(k, range.end());

    for (auto i = s.size() - 1 - ((s.size() - 1) % step == 0);
      k != range.begin();
      i -= 1 + ((i - 1) % step == 0)
    ) {
      FATAL_ASSERT_NE(--k, range.end());
      FATAL_EXPECT_EQ(i, *k);
    }
    FATAL_EXPECT_EQ(k, range.begin());

    for (std::size_t i = 1; k != range.end(); i += 1 + ((i + 1) % step == 0)) {
      FATAL_EXPECT_EQ(i, *k++);
    }
    FATAL_EXPECT_EQ(k, range.end());

    for (auto i = s.size() - 1 - ((s.size() - 1) % step == 0);
      k != range.begin();
      i -= 1 + ((i - 1) % step == 0)
    ) {
      k--;
      FATAL_ASSERT_NE(k, range.end());
      FATAL_EXPECT_EQ(i, *k);
    }
    FATAL_EXPECT_EQ(k, range.begin());
  }
}

FATAL_TEST(bit_set, range_off zeroed) {
  bit_set<1000> const s;

  auto const range = s.range_off();
  auto k = range.begin();
  for (std::size_t i = 0; i < s.size(); ++i, ++k) {
    FATAL_ASSERT_NE(k, range.end());
    FATAL_EXPECT_EQ(i, *k);
  }
  FATAL_ASSERT_EQ(k, range.end());

  for (std::size_t i = s.size(); i--; ) {
    FATAL_ASSERT_NE(k, range.begin());
    FATAL_ASSERT_NE(--k, range.end());
    FATAL_EXPECT_EQ(i, *k);
  }
  FATAL_ASSERT_EQ(k, range.begin());

  for (std::size_t i = 0; i < s.size(); ++i) {
    FATAL_ASSERT_NE(k, range.end());
    FATAL_EXPECT_EQ(i, *k++);
  }
  FATAL_ASSERT_EQ(k, range.end());

  for (std::size_t i = s.size(); i--; ) {
    FATAL_ASSERT_NE(k--, range.begin());
    FATAL_ASSERT_NE(k, range.end());
    FATAL_EXPECT_EQ(i, *k);
  }
  FATAL_ASSERT_EQ(k, range.begin());
}

FATAL_TEST(bit_set, range_off flipped) {
  bit_set<1000> const s(flipped_bit_set{});

  auto const range = s.range_off();
  FATAL_EXPECT_EQ(range.begin(), range.end());
  FATAL_EXPECT_EQ(range.cbegin(), range.cend());
}

FATAL_TEST(bit_set, range_off zeroed interleaved) {
  constexpr auto size = 1000;
  for (std::size_t step = 2; step < size / 2; ++step) {
    bit_set<size> s;
    for (std::size_t i = 0; i < s.size(); i += step) {
      s.set(i);
    }

    auto const range = s.range_off();
    auto k = range.begin();
    for (std::size_t i = 1; k != range.end(); i += 1 + ((i + 1) % step == 0), ++k) {
      FATAL_EXPECT_EQ(i, *k);
    }
    FATAL_EXPECT_EQ(k, range.end());

    for (auto i = s.size() - 1 - ((s.size() - 1) % step == 0);
      k != range.begin();
      i -= 1 + ((i - 1) % step == 0)
    ) {
      FATAL_ASSERT_NE(--k, range.end());
      FATAL_EXPECT_EQ(i, *k);
    }
    FATAL_EXPECT_EQ(k, range.begin());

    for (std::size_t i = 1; k != range.end(); i += 1 + ((i + 1) % step == 0)) {
      FATAL_EXPECT_EQ(i, *k++);
    }
    FATAL_EXPECT_EQ(k, range.end());

    for (auto i = s.size() - 1 - ((s.size() - 1) % step == 0);
      k != range.begin();
      i -= 1 + ((i - 1) % step == 0)
    ) {
      k--;
      FATAL_ASSERT_NE(k, range.end());
      FATAL_EXPECT_EQ(i, *k);
    }
    FATAL_EXPECT_EQ(k, range.begin());
  }
}

FATAL_TEST(bit_set, range_off flipped interleaved) {
  constexpr auto size = 1000;
  for (std::size_t step = 2; step < size / 2; ++step) {
    bit_set<size> s(flipped_bit_set{});
    for (std::size_t i = 0; i < s.size(); i += step) {
      s.reset(i);
    }

    auto const range = s.range_off();
    auto k = range.begin();
    for (std::size_t i = 0; k != range.end(); i += (step - i % step), ++k) {
      FATAL_EXPECT_EQ(i, *k);
    }
    FATAL_EXPECT_EQ(k, range.end());

    for (auto i = s.size() - 1 - ((s.size() - 1) % step ? (s.size() - 1) % step : 0);
      k != range.begin();
      i -= (i % step ? i % step : step)
    ) {
      FATAL_ASSERT_NE(--k, range.end());
      FATAL_EXPECT_EQ(i, *k);
    }
    FATAL_EXPECT_EQ(k, range.begin());

    for (std::size_t i = 0; k != range.end(); i += (step - i % step)) {
      FATAL_EXPECT_EQ(i, *k++);
    }
    FATAL_EXPECT_EQ(k, range.end());

    for (auto i = s.size() - 1 - ((s.size() - 1) % step ? (s.size() - 1) % step : 0);
      k != range.begin();
      i -= (i % step ? i % step : step)
    ) {
      k--;
      FATAL_ASSERT_NE(k, range.end());
      FATAL_EXPECT_EQ(i, *k);
    }
    FATAL_EXPECT_EQ(k, range.begin());
  }
}

FATAL_TEST(bit_set, const_iterator zeroed) {
  bit_set<1000> const s;
  for (auto const i: s) {
    FATAL_EXPECT_FALSE(i);
  }
}

FATAL_TEST(bit_set, const_iterator flipped) {
  bit_set<1000> const s(flipped_bit_set{});
  for (auto const i: s) {
    FATAL_EXPECT_TRUE(i);
  }
}

FATAL_TEST(bit_set, const_iterator zeroed interleaved) {
  constexpr auto size = 1000;
  for (std::size_t step = 2; step < size / 2; ++step) {
    bit_set<size> s;
    for (std::size_t i = 0; i < s.size(); i += step) {
      s.set(i);
    }

    std::size_t i = 0;
    for (auto const bit: s) {
      FATAL_EXPECT_EQ(i++ % step == 0, bit);
    }
  }
}

FATAL_TEST(bit_set, const_iterator flipped interleaved) {
  constexpr auto size = 1000;
  for (std::size_t step = 2; step < size / 2; ++step) {
    bit_set<size> s(flipped_bit_set{});
    for (std::size_t i = 0; i < s.size(); i += step) {
      s.reset(i);
    }

    std::size_t i = 0;
    for (auto const bit: s) {
      FATAL_EXPECT_EQ(i++ % step != 0, bit);
    }
  }
}

FATAL_TEST(bit_set, operator = zeroed) {
  bit_set<1000> s;
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }

  s = bit_set<1000>(flipped_bit_set{});
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }

  s = bit_set<1000>();
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }
}

FATAL_TEST(bit_set, operator = flipped) {
  bit_set<1000> s(flipped_bit_set{});
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }

  s = bit_set<1000>();
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_FALSE(s.get(i));
  }

  s = bit_set<1000>(flipped_bit_set{});
  for (auto i = s.size(); i--; ) {
    FATAL_EXPECT_TRUE(s.get(i));
  }
}

FATAL_TEST(bit_set, operator == zeroed) {
  bit_set<1000> const s;
  FATAL_EXPECT_TRUE(s == s);
  FATAL_EXPECT_TRUE(s == bit_set<1000>());
  FATAL_EXPECT_FALSE(s == bit_set<1000>(flipped_bit_set{}));
}

FATAL_TEST(bit_set, operator == flipped) {
  bit_set<1000> const s(flipped_bit_set{});
  FATAL_EXPECT_TRUE(s == s);
  FATAL_EXPECT_FALSE(s == bit_set<1000>());
  FATAL_EXPECT_TRUE(s == bit_set<1000>(flipped_bit_set{}));
}

FATAL_TEST(bit_set, operator != zeroed) {
  bit_set<1000> const s;
  FATAL_EXPECT_FALSE(s != s);
  FATAL_EXPECT_FALSE(s != bit_set<1000>());
  FATAL_EXPECT_TRUE(s != bit_set<1000>(flipped_bit_set{}));
}

FATAL_TEST(bit_set, operator != flipped) {
  bit_set<1000> const s(flipped_bit_set{});
  FATAL_EXPECT_FALSE(s != s);
  FATAL_EXPECT_TRUE(s != bit_set<1000>());
  FATAL_EXPECT_FALSE(s != bit_set<1000>(flipped_bit_set{}));
}

} // namespace fatal {
