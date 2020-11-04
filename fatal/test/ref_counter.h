/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_test_ref_counter_h
#define FATAL_INCLUDE_fatal_test_ref_counter_h

#include <fatal/portability.h>

#include <atomic>
#include <vector>

#include <cassert>
#include <cstdint>

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

namespace ftl {
namespace detail {
namespace ref_counter_impl {

struct FATAL_HIDE_SYMBOL counters;

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static std::vector<counters *> &all_counters() {
  FATAL_HIDE_SYMBOL
  static std::vector<counters *> instance;
  return instance;
}

struct FATAL_HIDE_SYMBOL counters {
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  counters():
    created(0),
    alive(0),
    valid(0)
  {}

  FATAL_HIDE_SYMBOL
  std::atomic<std::intmax_t> created;
  FATAL_HIDE_SYMBOL
  std::atomic<std::intmax_t> alive;
  FATAL_HIDE_SYMBOL
  std::atomic<std::intmax_t> valid;

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void reset() {
    created = 0;
    alive = 0;
    valid = 0;
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void operator -=(counters const &rhs) {
    // TODO: FIX, NOT THREAD SAFE
    created -= rhs.created;
    alive -= rhs.alive;
    valid -= rhs.valid;
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  void init() {
    if (registered_.test_and_set()) {
      return;
    }

    // TODO: FIX, NOT THREAD SAFE
    all_counters().push_back(this);
  }

private:
  FATAL_HIDE_SYMBOL
  std::atomic_flag registered_ = ATOMIC_FLAG_INIT;
};

FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
static counters &global() {
  FATAL_HIDE_SYMBOL
  static counters instance;
  return instance;
}

} // namespace ref_counter_impl {
} // namespace detail {


/**
 * Tracks the number of instances created and the number of instances still
 * alive.
 *
 * Example:
 *
 *  ref_counter::reset();
 *
 *  // yields `0`
 *  ref_counter::created();
 *  // yields `0`
 *  ref_counter::alive();
 *  // yields `0`
 *  ref_counter::valid();
 *
 *  ref_counter<> g1;
 *
 *  // yields `1`
 *  ref_counter::created();
 *  // yields `1`
 *  ref_counter::alive();
 *  // yields `1`
 *  ref_counter::valid();
 *
 *  {
 *    ref_counter<> g2;
 *
 *    // yields `2`
 *    ref_counter::created();
 *    // yields `2`
 *    ref_counter::alive();
 *    // yields `2`
 *    ref_counter::valid();
 *
 *    ref_counter<> g3(std::move(g2));
 *
 *    // yields `3`
 *    ref_counter::created();
 *    // yields `3`
 *    ref_counter::alive();
 *    // yields `2`
 *    ref_counter::valid();
 *  }
 *
 *  // yields `3`
 *  ref_counter::created();
 *  // yields `1`
 *  ref_counter::alive();
 *  // yields `1`
 *  ref_counter::valid();
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
// TODO: ADD UNIT TESTS
template <typename...>
struct FATAL_HIDE_SYMBOL ref_counter {
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  ref_counter():
    valid_(true)
  {
    increase_created();
    increase_alive();
    increase_valid();
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  ref_counter(ref_counter const &rhs):
    valid_(rhs.valid_)
  {
    increase_created();
    increase_alive();

    if (valid_) {
      increase_valid();
    }
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  ref_counter(ref_counter &&rhs) noexcept:
    valid_(rhs.valid_)
  {
    increase_created();
    increase_alive();
    rhs.valid_ = false;
  }

  template <
    typename T,
    typename... Args,
    typename = typename std::enable_if<
      !std::is_same<ref_counter, typename std::decay<T>::type>::value
    >::type
  >
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  explicit ref_counter(T &&, Args &&...):
    valid_(true)
  {
    increase_created();
    increase_alive();
    increase_valid();
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  ref_counter &operator =(ref_counter const &rhs) {
    if (!valid_ && rhs.valid_) {
      increase_valid();
    }

    valid_ = rhs.valid_;

    return *this;
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  ref_counter &operator =(ref_counter &&rhs) {
    if (valid_) {
      decrease_valid();
    }

    valid_ = rhs.valid_;
    rhs.valid_ = false;

    return *this;
  }

  template <typename T>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  ref_counter &operator =(T &&) {
    if (!valid_) {
      valid_ = true;
      increase_valid();
    }

    return *this;
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  ~ref_counter() {
    decrease_alive();

    if (valid_) {
      decrease_valid();
    }
  }

  /**
   * Upon construction, calls `ref_counter_reset()`.
   *
   * Upon destruction, asserts that `alive` and `valid` counters are both 0.
   *
   * Usually constructed at the beginning of a method that uses `ref_counter`.
   *
   * @author: Marcelo Juchem <marcelo@fb.com>
   */
  struct FATAL_HIDE_SYMBOL guard {
    FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
    guard() {
      auto duplicate = singleton().test_and_set();
      assert(!duplicate);
      reset();
    }

    guard(guard const &) = delete;
    guard(guard &&) = delete;

    FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
    ~guard() {
      assert(counters_().alive == 0);
      assert(counters_().valid == 0);
      singleton().clear();
    }

  private:
    FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
    static std::atomic_flag &singleton() {
      FATAL_HIDE_SYMBOL
      static std::atomic_flag instance = ATOMIC_FLAG_INIT;
      return instance;
    }
  };

  /**
   * Resets all counters to 0.
   *
   * @author: Marcelo Juchem <marcelo@fb.com>
   */
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static void reset() {
    detail::ref_counter_impl::global() -= counters_();
    counters_().reset();
  }

  /**
   * The number of instances created.
   *
   * This number increases once a constructor is called.
   *
   * The only way to decrease this number is through `reset()`.
   *
   * @author: Marcelo Juchem <marcelo@fb.com>
   */
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static std::intmax_t created() { return counters_().created; }

  /**
   * The number of instances alive.
   *
   * This number increases once a constructor is called and decreases once the
   * destructor is called.
   *
   * @author: Marcelo Juchem <marcelo@fb.com>
   */
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static std::intmax_t alive() { return counters_().alive; }

  /**
   * The number of instances in a valid state.
   *
   * This number increases once a constructor other than the move constructor is
   * called.
   *
   * Once an instance is moved from, it enters an `invalid` state that can only
   * be restored through assignment, and its previous valid state is transfered
   * to the instance it is being moved into.
   *
   * This counter decreases once the destructor is called, as long as the
   * instance being destroyed is in a valid state.
   *
   * @author: Marcelo Juchem <marcelo@fb.com>
   */
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static std::intmax_t valid() { return counters_().valid; }

private:
  FATAL_HIDE_SYMBOL
  bool valid_;

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static detail::ref_counter_impl::counters &counters_() {
    FATAL_HIDE_SYMBOL
    static detail::ref_counter_impl::counters instance;
    instance.init();
    return instance;
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static void increase_created() {
    ++counters_().created;
    ++detail::ref_counter_impl::global().created;
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static void increase_alive() {
    ++counters_().alive;
    ++detail::ref_counter_impl::global().alive;
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static void decrease_alive() {
    --counters_().alive;
    // TODO: FIX, NOT THREAD SAFE
    assert(counters_().alive >= 0);
    --detail::ref_counter_impl::global().alive;
    // TODO: FIX, NOT THREAD SAFE
    assert(detail::ref_counter_impl::global().alive >= 0);
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static void increase_valid() {
    ++counters_().valid;
    ++detail::ref_counter_impl::global().valid;
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static void decrease_valid() {
    --counters_().valid;
    // TODO: FIX, NOT THREAD SAFE
    assert(counters_().valid >= 0);
    --detail::ref_counter_impl::global().valid;
    // TODO: FIX, NOT THREAD SAFE
    assert(detail::ref_counter_impl::global().valid >= 0);
  }
};

/**
 * Resets all global counters to 0.
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
void ref_counter_reset() {
  detail::ref_counter_impl::global().reset();

  // TODO: FIX, NOT THREAD SAFE
  for (auto &i: detail::ref_counter_impl::all_counters()) {
    i->reset();
  }
}

/**
 * The global number of instances created for all template instantiations of
 * ref_counter.
 *
 * This number increases once a constructor is called.
 *
 * The only way to decrease this number is through `reset()`.
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
std::intmax_t ref_counter_created() {
  return detail::ref_counter_impl::global().created;
}

/**
 * The number of instances alive for all template instantiations of ref_counter.
 *
 * This number increases once a constructor is called and decreases once the
 * destructor is called.
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
std::intmax_t ref_counter_alive() {
  return detail::ref_counter_impl::global().alive;
}

/**
 * The number of instances in a valid state for all template instantiations of
 * ref_counter.
 *
 * This number increases once a constructor other than the move constructor is
 * called.
 *
 * Once an instance is moved from, it enters an `invalid` state that can only
 * be restored through assignment, and its previous valid state is transfered
 * to the instance it is being moved into.
 *
 * This counter decreases once the destructor is called, as long as the
 * instance being destroyed is in a valid state.
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
std::intmax_t ref_counter_valid() {
  return detail::ref_counter_impl::global().valid;
}

/**
 * Upon construction, calls `ref_counter_reset()`.
 *
 * Upon destruction, asserts that `alive` and `valid` counters are both 0.
 *
 * Usually constructed at the beginning of a method that uses `ref_counter` and
 * checks global counters with `ref_counter_*()`.
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
struct FATAL_HIDE_SYMBOL ref_counter_guard {
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  ref_counter_guard() {
    auto duplicate = singleton().test_and_set();
    assert(!duplicate);
    ref_counter_reset();
  }

  ref_counter_guard(ref_counter_guard const &) = delete;
  ref_counter_guard(ref_counter_guard &&) = delete;

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  ~ref_counter_guard() {
    assert(detail::ref_counter_impl::global().alive == 0);
    assert(detail::ref_counter_impl::global().valid == 0);
    singleton().clear();
  }

private:
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static std::atomic_flag &singleton() {
    FATAL_HIDE_SYMBOL
    static std::atomic_flag instance = ATOMIC_FLAG_INIT;
    return instance;
  }
};

} // namespace ftl {

FATAL_DIAGNOSTIC_POP

#endif // FATAL_INCLUDE_fatal_test_ref_counter_h
