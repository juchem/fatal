/*
 *  Copyright (c) 2019, Marcelo Juchem
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#ifndef FATAL_INCLUDE_fatal_container_bit_set_h
#define FATAL_INCLUDE_fatal_container_bit_set_h

#include <fatal/math/numerics.h>
#include <fatal/type/slice.h>

#include <fatal/portability.h>

#include <array>
#include <type_traits>

#include <cassert>

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

#include <fatal/container/impl/bit_set.h>

namespace fatal {

struct flipped_bit_set {};

template <std::size_t Size, bool Compact = false>
class bit_set {
  using bucket_type = std::conditional_t<
    Compact,
    std::uint8_t,
    typename i_bs::bit_set_type_selector<Size>::type
  >;

public:
  using size_type = std::conditional_t<
    Compact,
    smallest_unsigned_for_value<Size>,
    smallest_fast_unsigned_for_value<Size>
  >;

private:
  using bucket_size_type = std::conditional_t<
    Compact,
    smallest_unsigned_for_value<data_bits_v<bucket_type>>,
    smallest_fast_unsigned_for_value<data_bits_v<bucket_type>>
  >;

  static constexpr bucket_size_type bucket_size = data_bits_v<bucket_type>;

  static constexpr bucket_size_type bucket_index_shift = lg_2(bucket_size);

  static_assert(bucket_index_shift || Size < 2);

  static constexpr bucket_type bucket_offset_mask = static_cast<size_type>(
    static_cast<size_type>(size_type(1) << bucket_index_shift) - size_type(1)
  );

  static constexpr size_type bucket_count = (Size / bucket_size)
    + static_cast<bool>(Size % bucket_size);

  static_assert(bucket_size * bucket_count >= Size);

public:
  bit_set() = default;

  bit_set(flipped_bit_set):
    bit_set(
      std::integral_constant<bucket_type, static_cast<bucket_type>(~bucket_type(0))>(),
      std::make_integer_sequence<size_type, bucket_count>()
    )
  {}

  bit_set &flip() {
    for (auto &bucket: bucket_) {
      bucket = ~bucket;
    }
    return *this;
  }

  bit_set &flip(size_type i) {
    bucket(i) ^= value_mask(i);
    return *this;
  }

  bit_set &set() {
    return *this = bit_set(flipped_bit_set{});
  }

  bit_set &set(size_type i) {
    bucket(i) |= value_mask(i);
    return *this;
  }

  bit_set &reset() {
    return *this = bit_set();
  }

  bit_set &reset(size_type i) {
    bucket(i) &= static_cast<bucket_type>(~value_mask(i));
    return *this;
  }

  // returns size if none
  size_type next_set(size_type i) const {
    auto const index = bucket_index(i);
    auto const offset = bucket_offset(i);
    auto const msbits_mask = static_cast<bucket_type>(
      offset + 1 == bucket_size
        ? 0
        : static_cast<bucket_type>(~bucket_type(0)) << (offset + 1)
    );

    if (auto const msbits = static_cast<bucket_type>(bucket_[index] & msbits_mask)) {
      // we're still in the same bucket
      return absolute_index(index, least_significant_bit_lg_2(msbits));
    }

    for (auto i = index; ++i < bucket_count; ) {
      if (auto const bucket = bucket_[i]) {
        return absolute_index(i, least_significant_bit_lg_2(bucket));
      }
    }

    return Size;
  }

  // returns size if none
  size_type previous_set(size_type i) const {
    auto const lsbits_mask = static_cast<bucket_type>(value_mask(i) - static_cast<bucket_type>(1));
    auto const index = bucket_index(i);

    if (auto const lsbits = static_cast<bucket_type>(bucket_[index] & lsbits_mask)) {
      // we're still in the same bucket
      return absolute_index(index, lg_2(lsbits));
    }

    for (auto i = index; i_bs::bit_set_decrement<size_type>::post_decrement(i); ) {
      if (auto const bucket = bucket_[i]) {
        return absolute_index(i, lg_2(bucket));
      }
    }

    return Size;
  }

  // returns size if none
  size_type next_reset(size_type i) const {
    auto const index = bucket_index(i);
    auto const offset = bucket_offset(i);
    auto const msbits_mask = static_cast<bucket_type>(
      offset + 1 == bucket_size
        ? 0
        : static_cast<bucket_type>(~bucket_type(0)) << (offset + 1)
    );

    if (auto const msbits = static_cast<bucket_type>(~bucket_[index]) & msbits_mask) {
      // we're still in the same bucket
      return absolute_index(index, least_significant_bit_lg_2(msbits));
    }

    for (auto i = index; ++i < bucket_count; ) {
      if (auto const bucket = static_cast<bucket_type>(~bucket_[i])) {
        return absolute_index(i, least_significant_bit_lg_2(bucket));
      }
    }

    return Size;
  }

  // returns size if none
  size_type previous_reset(size_type i) const {
    auto const lsbits_mask = static_cast<bucket_type>(value_mask(i) - static_cast<bucket_type>(1));
    auto const index = bucket_index(i);

    if (auto const lsbits = static_cast<bucket_type>(~bucket_[index]) & lsbits_mask) {
      // we're still in the same bucket
      return absolute_index(index, lg_2(lsbits));
    }

    for (auto i = index; i_bs::bit_set_decrement<size_type>::post_decrement(i); ) {
      if (auto const bucket = static_cast<bucket_type>(~bucket_[i])) {
        return absolute_index(i, lg_2(bucket));
      }
    }

    return Size;
  }

  constexpr bool get(size_type i) const {
    return static_cast<bool>(bucket(i) & value_mask(i));
  }

  // TODO: BETTER NAME
  struct range_on_t {
    struct const_iterator {
      explicit const_iterator(): i_(Size){}
      explicit const_iterator(bit_set const *set):
        set_(set),
        i_(Size ? (set_->get(0) ? 0 : set_->next_set(0)) : Size)
      {}

      auto index() const { return i_; }

      auto operator *() const { return i_; }

      auto &operator ++() {
        assert(i_ < Size);
        i_ = set_->next_set(i_);
        return *this;
      }

      auto operator ++(int) {
        auto copy = *this;
        ++*this;
        return copy;
      }

      auto &operator --() {
        assert(i_);
        i_ = set_->previous_set(i_);
        return *this;
      }

      auto operator --(int) {
        auto copy = *this;
        --*this;
        return copy;
      }

      bool operator ==(const_iterator const &rhs) const { return i_ == rhs.i_; }
      bool operator !=(const_iterator const &rhs) const { return !(*this == rhs); }

    private:
      bit_set const *set_ = nullptr;
      size_type i_;
    };

    explicit range_on_t(bit_set const *set): set_(set) {}

    auto cbegin() const { return const_iterator(set_); }
    auto cend() const { return const_iterator(); }

    auto begin() const { return cbegin(); }
    auto end() const { return cend(); }

  private:
    bit_set const *set_;
  };

  auto range_on() const { return range_on_t(this); }

  // TODO: BETTER NAME
  struct range_off_t {
    struct const_iterator {
      explicit const_iterator(): i_(Size){}
      explicit const_iterator(bit_set const *set):
        set_(set),
        i_(Size ? (set_->get(0) ? set_->next_reset(0) : 0) : Size)
      {}

      auto index() const { return i_; }

      auto operator *() const { return i_; }

      auto &operator ++() {
        assert(i_ < Size);
        i_ = set_->next_reset(i_);
        return *this;
      }

      auto operator ++(int) {
        auto copy = *this;
        ++*this;
        return copy;
      }

      auto &operator --() {
        assert(i_);
        i_ = set_->previous_reset(i_);
        return *this;
      }

      auto operator --(int) {
        auto copy = *this;
        --*this;
        return copy;
      }

      bool operator ==(const_iterator const &rhs) const { return i_ == rhs.i_; }
      bool operator !=(const_iterator const &rhs) const { return !(*this == rhs); }

    private:
      bit_set const *set_ = nullptr;
      size_type i_;
    };

    explicit range_off_t(bit_set const *set): set_(set) {}

    auto cbegin() const { return const_iterator(set_); }
    auto cend() const { return const_iterator(); }

    auto begin() const { return cbegin(); }
    auto end() const { return cend(); }

  private:
    bit_set const *set_;
  };

  auto range_off() const { return range_off_t(this); }

  struct const_iterator {
    explicit const_iterator(bit_set const *set): set_(set) {}
    explicit const_iterator(): i_(Size) {}

    auto index() const { return i_; }

    auto operator *() const { return set_->get(i_); }

    auto &operator ++() {
      assert(i_ < Size);
      ++i_;
      return *this;
    }

    auto operator ++(int) {
      auto copy = *this;
      ++*this;
      return copy;
    }

    auto &operator --() {
      assert(i_);
      --i_;
      return *this;
    }

    auto operator --(int) {
      auto copy = *this;
      --*this;
      return copy;
    }

    bool operator ==(const_iterator const &rhs) const { return i_ == rhs.i_; }
    bool operator !=(const_iterator const &rhs) const { return !(*this == rhs); }

  private:
    bit_set const *set_ = nullptr;
    size_type i_ = 0;
  };

  auto cbegin() const { return const_iterator(this); }
  auto cend() const { return const_iterator(); }

  auto begin() const { return cbegin(); }
  auto end() const { return cend(); }

  constexpr static size_type size() { return Size; }

  bit_set &operator =(bit_set const &rhs) {
    bucket_ = rhs.bucket_;
    return *this;
  }

  bool operator ==(bit_set const &rhs) const { return bucket_ == rhs.bucket_; }
  bool operator !=(bit_set const &rhs) const { return bucket_ != rhs.bucket_; }

private:
  constexpr size_type bucket_index(size_type i) const { return i >> bucket_index_shift; }
  constexpr bucket_size_type bucket_offset(size_type i) const { return i & bucket_offset_mask; }
  constexpr size_type absolute_index(size_type index, bucket_size_type offset) const {
    return (index << bucket_index_shift) | offset;
  }

  constexpr bucket_type value_mask(size_type i) const {
    return static_cast<bucket_type>(bucket_type(1) << bucket_offset(i));
  }

  template <bucket_type Value, size_type... Indexes>
  bit_set(std::integral_constant<bucket_type, Value>, std::integer_sequence<size_type, Indexes...>):
    bucket_{((Indexes ^ Indexes) | Value)...}
  {}

  constexpr bucket_type const &bucket(size_type i) const {
    assert(i < Size);
    return bucket_[bucket_index(i)];
  }

  bucket_type &bucket(size_type i) {
    assert(i < Size);
    return bucket_[bucket_index(i)];
  }

  std::array<bucket_type, bucket_count> bucket_ = {};
};

} // namespace fatal {

FATAL_DIAGNOSTIC_POP

#endif // FATAL_INCLUDE_fatal_container_bit_set_h
