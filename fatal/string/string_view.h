/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_string_string_view_h
#define FATAL_INCLUDE_fatal_string_string_view_h

#include <fatal/math/hash.h>
#include <fatal/portability.h>
#include <fatal/type/array.h>
#include <fatal/type/call_traits.h>
#include <fatal/type/size.h>
#include <fatal/type/traits.h>

#include <algorithm>
#include <iterator>
#include <string_view>

#include <cassert>
#include <cstring>

FATAL_DIAGNOSTIC_PUSH
FATAL_DIAGNOSTIC_IGNORE_ATTRIBUTES
FATAL_DIAGNOSTIC_IGNORE_SHADOW

namespace fatal {
namespace detail {

template <typename U>
using convertible = std::integral_constant<
  bool,
  std::is_pointer<U>::value
    || std::is_array<U>::value
    || std::is_integral<typename std::decay<U>::type>::value
    || (
      call_traits::data::member_function::supports<U>::value
        && call_traits::size::member_function::supports<U>::value
    )
>;

template <typename U>
using convertible_t = typename std::enable_if<convertible<U>::value>::type;

template <typename T, typename U>
using safe_overload = typename std::enable_if<
  is_safe_overload<T, U>::value && convertible<U>::value
>::type;

} // namespace detail {

struct string_view {
  using value_type = char;
  using size_type = std::size_t;
  using const_iterator = char const *;

  constexpr string_view(): begin_(nullptr), end_(nullptr) {}

  constexpr string_view(const_iterator begin, const_iterator end):
    begin_(begin),
    end_(end)
  {
    assert(begin_ <= end_);
  }

  constexpr string_view(const_iterator s, std::size_t size):
    begin_(s),
    end_(s + size)
  {
    assert(begin_ <= end_);
  }

  constexpr string_view(std::string_view view):
    begin_(view.data()),
    end_(view.data() + view.size())
  {
    assert(begin_ <= end_);
  }

  /* implicit */ string_view(value_type *s):
    begin_(s),
    end_(std::next(s, std::strlen(s)))
  {
    assert(begin_ <= end_);
  }

  /* implicit */ string_view(value_type const *s):
    begin_(s),
    end_(std::next(s, std::strlen(s)))
  {
    assert(begin_ <= end_);
  }

  template <std::size_t N>
  constexpr string_view(value_type const (&s)[N]):
    begin_(s),
    end_(s + (N - (s[N - 1] == 0)))
  {
    assert(begin_ <= end_);
  }

  template <std::size_t N>
  constexpr string_view(value_type (&s)[N]):
    begin_(s),
    end_(s + (N - (s[N - 1] == 0)))
  {
    assert(begin_ <= end_);
  }

  constexpr explicit string_view(value_type const &c):
    begin_(&c),
    end_(&c + 1)
  {
    assert(begin_ <= end_);
  }

  template <typename U, typename = safe_overload<string_view, U>>
  constexpr explicit string_view(U &&s):
    begin_(s.data()),
    end_(s.data() + s.size())
  {
    assert(begin_ <= end_);
  }

  constexpr string_view slice(size_type offset, size_type end) const {
    assert(offset <= size());
    assert(end <= size());
    return string_view(begin_ + offset, begin_ + end);
  }

  const_iterator find(value_type needle) const {
    return find(needle, begin_);
  }

  const_iterator find(value_type needle, const_iterator offset) const {
    assert(begin_ <= offset);
    assert(offset <= end_);
    return std::find(offset, end_, needle);
  }

  template <typename CharMatcher>
  const_iterator find_first_of(CharMatcher &&matcher) const {
    return find_first_of(std::forward<CharMatcher>(matcher), begin_);
  }

  template <typename CharMatcher>
  const_iterator find_first_of(CharMatcher &&matcher, const_iterator offset) const {
    assert(begin_ <= offset);
    assert(offset <= end_);
    return std::find_if(offset, end_, matcher);
  }

  template <typename CharMatcher>
  const_iterator find_first_not_of(CharMatcher &&matcher) const {
    return find_first_not_of(std::forward<CharMatcher>(matcher), begin_);
  }

  template <typename CharMatcher>
  const_iterator find_first_not_of(CharMatcher &&matcher, const_iterator offset) const {
    assert(begin_ <= offset);
    assert(offset <= end_);
    return std::find_if_not(offset, end_, matcher);
  }

  /**
   * Removes the initial `size` characters of the view and returns a reference
   * to the view itself.
   *
   * No bounds check are performed.
   *
   * Example:
   *
   *  string_view v("hello, world");
   *  auto &c = v.skip(4);
   *
   *  assert(&c == &v);
   *
   *  // prints "o, world"
   *  std::cout << c;
   *
   *  // prints "o, world"
   *  std::cout << v;
   *
   * See also: `seek*`, `skip*`, `operator +=`, `operator +`
   *
   * @author: Marcelo Juchem <juchem at gmail dot com>
   */
  string_view &skip(size_type size) {
    return *this += size;
  }

  string_view &skip_up_to(size_type size) {
    return skip(std::min(size, this->size()));
  }

  /**
   * Removes the initial `size` characters of the view and returns the removed
   * part as a separate view.
   *
   * No bounds check are performed.
   *
   * Example:
   *
   *  string_view v("hello, world");
   *  auto c = v.seek(4);
   *
   *  // prints "hell"
   *  std::cout << c;
   *
   *  // prints "o, world"
   *  std::cout << v;
   *
   * See also: `seek*`, `skip*`, `operator +=`, `operator +`
   *
   * @author: Marcelo Juchem <juchem at gmail dot com>
   */
  string_view seek(size_type size) {
    string_view result(begin_, size);
    *this += size;
    return result;
  }

  ////////////////////////////
  // count/skip/seek to/for //
  ////////////////////////////

  /**
   * Finds the first occurence of `delimiter`, removes all characters up to (and
   * including) such occurence and returns a reference to the view itself.
   *
   * Example:
   *
   *  string_view v("hello, world");
   *  auto &c = v.skip_past_char(' ');
   *
   *  assert(&c == &v);
   *
   *  // prints "world"
   *  std::cout << c;
   *
   *  // prints "world"
   *  std::cout << v;
   *
   * See also: `seek*`, `skip*`, `operator +=`, `operator +`
   *
   * @author: Marcelo Juchem <juchem at gmail dot com>
   */
  template <typename U>
  string_view &skip_past_char(U &&delimiter) {
    begin_ = find(std::forward<U>(delimiter));
    assert(begin_ <= end_);
    if (begin_ != end_) { ++begin_; }
    return *this;
  }

  /**
   * Finds the first occurence of `delimiter`, removes all characters up to (and
   * including) such occurence and returns the removed part (not including the
   * delimiter) as a separate view.
   *
   * Example:
   *
   *  string_view v("hello, world");
   *  auto c = v.seek_past_char(' ');
   *
   *  // prints "hello,"
   *  std::cout << c;
   *
   *  // prints "world"
   *  std::cout << v;
   *
   * See also: `seek*`, `skip*`, `operator +=`, `operator +`
   *
   * @author: Marcelo Juchem <juchem at gmail dot com>
   */
  template <typename U>
  string_view seek_past_char(U &&delimiter) {
    string_view result(begin_, find(std::forward<U>(delimiter)));
    begin_ = result.end() == end_ ? end_ : std::next(result.end());
    assert(begin_ <= end_);
    return result;
  }

  /**
   * Finds the first occurence of `delimiter`, removes all characters before
   * (not including) such occurence and returns a reference to the view itself.
   *
   * Example:
   *
   *  string_view v("hello, world");
   *  auto &c = v.skip_to_char(',');
   *
   *  assert(&c == &v);
   *
   *  // prints ", world"
   *  std::cout << c;
   *
   *  // prints ", world"
   *  std::cout << v;
   *
   * See also: `seek*`, `skip*`, `operator +=`, `operator +`
   *
   * @author: Marcelo Juchem <juchem at gmail dot com>
   */
  template <typename U>
  string_view &skip_to_char(U &&delimiter) {
    begin_ = find(std::forward<U>(delimiter));
    assert(begin_ <= end_);
    return *this;
  }

  /**
   * Finds the first occurence of `delimiter`, removes all characters before
   * (not including) such occurence and returns the removed part as a separate
   * view.
   *
   * Example:
   *
   *  string_view v("hello, world");
   *  auto c = v.seek_for_char(',');
   *
   *  // prints "hello"
   *  std::cout << c;
   *
   *  // prints ", world"
   *  std::cout << v;
   *
   * See also: `seek*`, `skip*`, `operator +=`, `operator +`
   *
   * @author: Marcelo Juchem <juchem at gmail dot com>
   */
  template <typename U>
  string_view seek_for_char(U &&delimiter) {
    auto begin = begin_;
    skip_to_char(std::forward<U>(delimiter));
    return string_view(begin, begin_);
  }

  template <typename CharMatcher>
  string_view seek_for(CharMatcher &&matcher) {
    string_view result(begin_, find_first_of(std::forward<CharMatcher>(matcher)));
    begin_ = result.end();
    assert(begin_ <= end_);
    return result;
  }

  template <typename CharMatcher>
  string_view seek_past(CharMatcher &&matcher) {
    string_view result(begin_, find_first_of(std::forward<CharMatcher>(matcher)));
    begin_ = result.end() == end_ ? end_ : std::next(result.end());
    assert(begin_ <= end_);
    return result;
  }

  template <typename CharMatcher>
  string_view seek_over(CharMatcher &&matcher) {
    string_view result(begin_, find_first_not_of(std::forward<CharMatcher>(matcher)));
    begin_ = result.end();
    assert(begin_ <= end_);
    return result;
  }

  template <typename CharMatcher>
  string_view &skip_to(CharMatcher &&matcher) {
    begin_ = find_first_of(std::forward<CharMatcher>(matcher));
    assert(begin_ <= end_);
    return *this;
  }

  template <typename CharMatcher>
  string_view &skip_past(CharMatcher &&matcher) {
    begin_ = find_first_of(std::forward<CharMatcher>(matcher));
    if (begin_ != end_) { ++begin_; }
    assert(begin_ <= end_);
    return *this;
  }

  template <typename CharMatcher>
  string_view &skip_over(CharMatcher &&matcher) {
    begin_ = find_first_not_of(std::forward<CharMatcher>(matcher));
    assert(begin_ <= end_);
    return *this;
  }

  // reset //

  void reset(const_iterator begin) {
    assert(begin_ <= begin);
    begin_ = begin;
    assert(begin_ <= end_);
  }

  void reset(const_iterator begin, const_iterator end) {
    begin_ = begin;
    end_ = end;
    assert(begin_ <= end_);
  }

  void reset(const_iterator begin, size_type size) {
    begin_ = begin;
    end_ = std::next(begin, size);
    assert(begin_ <= end_);
  }

  void limit(size_type size) {
    // TODO: use `non_negative(size)` to avoid tautological comparison error
    // assert(size >= size_type(0));

    if (size < this->size()) {
      end_ = std::next(begin_, size);
    }

    assert(begin_ <= end_);
  }

  constexpr value_type front() const { return *begin_; }
  constexpr value_type back() const {
    assert(begin_ < end_);
    return *(end_ - 1);
  }

  constexpr const_iterator data() const { return begin_; }

  constexpr value_type const &operator [](size_type i) const {
    assert(begin_ <= end_);
    assert(i < static_cast<size_type>(end_ - begin_));
    return begin_[i];
  }

  void clear() { begin_ = end_; }

  constexpr size_type size() const {
    assert(begin_ <= end_);
    return end_ - begin_;
  }

  constexpr bool empty() const {
    assert(begin_ <= end_);
    return begin_ == end_;
  }

  constexpr const_iterator cbegin() const { return begin_; }
  constexpr const_iterator begin() const { return begin_; }
  constexpr const_iterator cend() const { return end_; }
  constexpr const_iterator end() const { return end_; }

  // TODO: DOCUMENT AND TEST
  template <typename T>
  constexpr T to() const {
    return T(data(), size());
  }

  string_view &operator +=(size_type i) {
    assert(i <= size());
    std::advance(begin_, i);
    assert(begin_ <= end_);
    return *this;
  }

  string_view &operator -=(size_type i) {
    begin_ -= i;
    assert(begin_ <= end_);
    return *this;
  }

  string_view operator +(size_type i) const {
    auto copy(*this);
    copy += i;
    return copy;
  }

  string_view operator -(size_type i) const {
    auto copy(*this);
    copy -= i;
    return copy;
  }

  string_view &operator ++() {
    assert(begin_ < end_);
    std::advance(begin_, 1);
    assert(begin_ <= end_);
    return *this;
  }

  string_view &operator --() {
    --begin_;
    assert(begin_ <= end_);
    return *this;
  }

  string_view operator ++(int) {
    auto copy(*this);
    ++*this;
    return copy;
  }

  string_view operator --(int) {
    auto copy(*this);
    --*this;
    return copy;
  }

  value_type operator *() const {
    assert(begin_ <= end_);
    return *begin_;
  }

  bool operator ==(string_view rhs) const {
    return size() == rhs.size() && !std::strncmp(begin_, rhs.begin_, size());
  }

  template <
    typename U,
    typename = detail::safe_overload<
      string_view,
      typename std::decay<U>::type
    >
  >
  bool operator ==(U &&rhs) const {
    return *this == string_view(std::forward<U>(rhs));
  }

  template <
    typename U,
    typename = detail::safe_overload<
      string_view,
      typename std::decay<U>::type
    >
  >
  bool operator !=(U &&rhs) const { return !(*this == std::forward<U>(rhs)); }

  bool operator <(string_view rhs) const {
    auto const result = std::strncmp(
      begin_, rhs.begin_, std::min(size(), rhs.size())
    );

    return result < 0 || (!result && size() < rhs.size());
  }

  template <
    typename U,
    typename = detail::safe_overload<
      string_view,
      typename std::decay<U>::type
    >
  >
  bool operator <(U &&rhs) const {
    return *this < string_view(std::forward<U>(rhs));
  }

  bool operator >(string_view rhs) const { return rhs < *this; }

  template <
    typename U,
    typename = detail::safe_overload<
      string_view,
      typename std::decay<U>::type
    >
  >
  bool operator >(U &&rhs) const {
    return *this > string_view(std::forward<U>(rhs));
  }

  constexpr explicit operator bool() const { return begin_ < end_; }

  constexpr bool operator !() const { return empty(); }

  struct hasher {
    using argument = string_view;
    using result_type = std::size_t;

    result_type operator ()(string_view s) const {
      return *bytes_hasher<result_type>()(s.data(), s.size());
    }
  };

private:
  const_iterator begin_;
  const_iterator end_;
};

// TODO: DOCUMENT, TEST AND BIKE-SHED
template <typename String>
struct string_view_from_type {
  string_view operator ()() const {
    return string_view(String::data(), String::size);
  }
};

template <typename String>
string_view as_string_view() noexcept {
  return string_view(z_data<String>(), size<String>::value);
}

/////////////////
// operator == //
/////////////////

template <typename T, typename = detail::safe_overload<string_view, T>>
bool operator ==(T const &lhs, string_view rhs) { return rhs == lhs; }

////////////////
// operator < //
////////////////

template <typename T, typename = detail::safe_overload<string_view, T>>
bool operator <(T const &lhs, string_view rhs) { return rhs > lhs; }

////////////////
// operator > //
////////////////

template <typename T, typename = detail::safe_overload<string_view, T>>
bool operator >(T const &lhs, string_view rhs) { return rhs < lhs; }

/////////////////
// operator != //
/////////////////

template <typename T, typename = detail::convertible_t<T>>
bool operator !=(string_view lhs, T const &rhs) { return !(lhs == rhs); }

template <typename T, typename = detail::safe_overload<string_view, T>>
bool operator !=(T const &lhs, string_view rhs) { return !(rhs == lhs); }

/////////////////
// operator <= //
/////////////////

template <typename T, typename = detail::convertible_t<T>>
bool operator <=(string_view lhs, T const &rhs) { return !(lhs > rhs); }

template <typename T, typename = detail::safe_overload<string_view, T>>
bool operator <=(T const &lhs, string_view rhs) { return !(rhs < lhs); }

/////////////////
// operator >= //
/////////////////

template <typename T, typename = detail::convertible_t<T>>
bool operator >=(string_view lhs, T const &rhs) { return !(lhs < rhs); }

template <typename T, typename = detail::safe_overload<string_view, T>>
bool operator >=(T const &lhs, string_view rhs) { return !(rhs > lhs); }

/////////////////////////////////////
// operator <<(std::basic_ostream) //
/////////////////////////////////////

template <typename C, typename T>
std::ostream &operator <<(std::basic_ostream<C, T> &out, string_view rhs) {
  return out.write(rhs.data(), rhs.size());
}

} // namespace fatal {

FATAL_DIAGNOSTIC_POP

#endif // FATAL_INCLUDE_fatal_string_string_view_h
