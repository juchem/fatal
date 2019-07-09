/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_log_log_h
#define FATAL_INCLUDE_fatal_log_log_h

#include <fatal/time/print.h>

#include <fatal/portability.h>
#include <fatal/preprocessor.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <ostream>
#include <type_traits>
#include <utility>

#include <cstdlib>

// for internal use only

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES

namespace fatal {
namespace log {
namespace detail {
namespace log_impl {

template <typename TOut, typename TInfo>
struct FATAL_HIDE_SYMBOL logger {
  using info = TInfo;

  struct FATAL_HIDE_SYMBOL writer {
    FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
    explicit writer(TOut *out) noexcept: out_(out) {}

    writer(writer const &) = delete;

    FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
    writer(writer &&rhs) noexcept: out_(rhs.out_) { rhs.out_ = nullptr; }

    template <typename T>
    FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
    writer &operator <<(T &&value) & {
      if (out_) {
        *out_ << std::forward<T>(value);
      }

      return *this;
    }

    template <typename T>
    FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
    writer &&operator <<(T &&value) && {
      if (out_) {
        *out_ << std::forward<T>(value);
      }

      return std::move(*this);
    }

    FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
    ~writer() {
      if (out_) {
        *out_ << '\n';
      }
    }

  private:
    FATAL_HIDE_SYMBOL
    TOut *out_;
  };

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  logger(TOut *out, source_info source) noexcept:
    writer_(out),
    source_(source)
  {}

  logger(logger const &) = delete;
  logger(logger &&rhs) = default;

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  ~logger() {
    if (info::abort::value) {
      std::abort();
    }
  }

  template <typename T>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  writer operator <<(T &&value) {
    writer_ << info::signature::value;

    if (info::show_level::value) {
      writer_ << info::value;
    }

    auto const now = std::chrono::duration_cast<std::chrono::nanoseconds>(
      std::chrono::system_clock::now().time_since_epoch()
    );

    // TODO: output date in an absolute format
    time::pretty_print(
      writer_ << " [" << source_.file() << ':' << source_.line() << "] at ",
      now
    ) << ": " << std::forward<T>(value);

    return std::move(writer_);
  }

private:
  FATAL_HIDE_SYMBOL
  writer writer_;

  FATAL_HIDE_SYMBOL
  source_info source_;
};

using level_t = unsigned;

template <typename TCategory, level_t Level>
struct FATAL_HIDE_SYMBOL log_level {
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static void set(level_t level) {
    value() = level;
  }

  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static level_t get() {
    return value();
  }

private:
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  static std::atomic<level_t> &value() {
    FATAL_HIDE_SYMBOL
    static std::atomic<level_t> instance(Level);
    return instance;
  }
};

template <
  typename TCategory,
  level_t Level,
  char Signature,
  bool ShowLevel,
  bool Abort = false
>
struct FATAL_HIDE_SYMBOL level_info:
  std::integral_constant<level_t, Level>
{
  using category = TCategory;
  using show_level = std::integral_constant<bool, ShowLevel>;
  using signature = std::integral_constant<char, Signature>;
  using abort = std::integral_constant<bool, Abort>;
};

struct FATAL_HIDE_SYMBOL log_tag {};
struct FATAL_HIDE_SYMBOL verbose_tag {};

using level_FATAL = level_info<log_tag, 0, 'F', false, true>;
using level_CRITICAL = level_info<log_tag, 1, 'C', false>;
using level_ERROR = level_info<log_tag, 2, 'E', false>;
using level_WARNING = level_info<log_tag, 3, 'W', false>;
using level_INFO = level_info<log_tag, 4, 'I', false>;

template <level_t Level>
using level_verbose = level_info<verbose_tag, Level, 'V', true>;

template <typename> struct FATAL_HIDE_SYMBOL by_category;

template <>
struct FATAL_HIDE_SYMBOL by_category<log_tag> {
  using level = detail::log_impl::log_level<
    detail::log_impl::log_tag,
    detail::log_impl::level_INFO::value
  >;
};

template <>
struct FATAL_HIDE_SYMBOL by_category<verbose_tag> {
  using level = detail::log_impl::log_level<
    detail::log_impl::verbose_tag, 0
  >;
};

} // namespace log_impl {
} // namespace detail {

struct FATAL_HIDE_SYMBOL null_logger {
  template <typename T>
  FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
  null_logger const &operator <<(T &&) const { return *this; }
};

using level = detail::log_impl::by_category<
  detail::log_impl::log_tag
>::level;

using v_level = detail::log_impl::by_category<
  detail::log_impl::verbose_tag
>::level;

// TODO: ADD THE ABILITY TO TURN VERBOSE LOGGING ON AND OFF
template <typename TInfo>
FATAL_ALWAYS_INLINE FATAL_HIDE_SYMBOL
log::detail::log_impl::logger<std::ostream, TInfo> log(source_info source) {
  return log::detail::log_impl::logger<std::ostream, TInfo>(
    TInfo::value <= detail::log_impl::by_category<
      typename TInfo::category
    >::level::get()
      ? std::addressof(std::cerr)
      : nullptr,
    source
  );
}

} // namespace log {

#define FATAL_LOG(Level) \
  ::fatal::log::log<::fatal::log::detail::log_impl::level_##Level>( \
    FATAL_SOURCE_INFO() \
  )

#define FATAL_VLOG(Level) \
  ::fatal::log::log<::fatal::log::detail::log_impl::level_verbose<Level>>( \
    FATAL_SOURCE_INFO() \
  )

#ifdef NDEBUG
# define FATAL_DLOG(Level) ::fatal::log::null_logger()
# define FATAL_DVLOG(Level) ::fatal::log::null_logger()
#else // NDEBUG
# define FATAL_DLOG(Level) FATAL_LOG(Level)
# define FATAL_DVLOG(Level) FATAL_VLOG(Level)
#endif // NDEBUG

} // namespace fatal {

FATAL_DIAGNOSTIC_POP

#endif // FATAL_INCLUDE_fatal_log_log_h
