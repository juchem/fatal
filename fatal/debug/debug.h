/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_debug_debug_h
#define FATAL_INCLUDE_fatal_debug_debug_h

// production build
#ifdef NDEBUG

namespace ftl {

using debug_mode = std::false_type;

# define FATAL_DEBUG_ONLY(...)
# define FATAL_DEBUG_STATEMENT(...) ::ftl::fn::no_op()
# define FATAL_DEBUG_BLOCK for (;false;)
# define FATAL_DEBUG_MEMBER_BLOCK for (;false;)

# define FATAL_TODO(Message) for (;false;)

} // namespace ftl {

// debug build
#else // NDEBUG

# include <iostream>

# include <cstdlib>

namespace ftl {

using debug_mode = std::true_type;

# define FATAL_DEBUG_ONLY(...) __VA_ARGS__
# define FATAL_DEBUG_STATEMENT(...) __VA_ARGS__
# define FATAL_DEBUG_BLOCK ::ftl::detail::debug_impl::debug_block() + [&]()
# define FATAL_DEBUG_MEMBER_BLOCK ::ftl::detail::debug_impl::debug_block() \
  + [this, &]()

# define FATAL_TODO(Message) \
  do { \
    ::std::cerr << "aborting, TODO: " << Message << ::std::endl; \
    ::std::abort(); \
  } while (false)

namespace detail {
namespace debug_impl {

struct debug_block {
  template <typename Block>
  debug_block operator +(Block &&block) const {
    block();
    return debug_block();
  }
};

} // namespace debug_impl {
} // namespace detail {
} // namespace ftl {

#endif // NDEBUG
#endif // FATAL_INCLUDE_fatal_debug_debug_h
