/*
 *  Copyright (c) 2020, Marcelo Juchem
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#ifndef FATAL_INCLUDE_fatal_type_impl_interleave_h
#define FATAL_INCLUDE_fatal_type_impl_interleave_h

#include <fatal/type/slice.h>

#include <type_traits>

namespace ftl::i_i {

////////////////
// interleave //
////////////////

template <typename, typename> FATAL_HIDE_SYMBOL struct i;
template <typename...> struct FATAL_HIDE_SYMBOL I;

// empty
template <template <typename...> typename L, typename D, typename... R>
struct FATAL_HIDE_SYMBOL I<L<R...>, D> { using type = L<R...>; };

// unitary
template <template <typename...> typename L, typename D, typename... R, typename F>
struct FATAL_HIDE_SYMBOL I<L<R...>, D, F> { using type = L<R..., F>; };

// size >= 2
template <
  template <typename...> typename L, typename D, typename... R,
  typename F, typename S, typename... T
>
struct FATAL_HIDE_SYMBOL I<L<R...>, D, F, S, T...>:
  I<
    std::conditional_t<
      std::is_same_v<F, D> || std::is_same_v<S, D>,
      L<R..., F>,
      L<R..., F, D>
    >,
    D, S, T...
  >
{};

// entrypoint
template <typename D, template <typename...> typename L, typename... T>
struct FATAL_HIDE_SYMBOL i<D, L<T...>> {
  using type = typename I<L<>, D, T...>::type;
};

//////////////
// surround //
//////////////

template <typename, typename> FATAL_HIDE_SYMBOL struct s;

/*
// empty
template <template <typename...> typename L, typename D, typename... R>
struct FATAL_HIDE_SYMBOL S<L<R...>, D> { using type = L<R...>; };

// unitary
template <template <typename...> typename L, typename D, typename... R, typename F>
struct FATAL_HIDE_SYMBOL S<L<R...>, D, F> { using type = L<R..., F>; };
*/

// empty
template <typename D, template <typename...> typename L>
struct FATAL_HIDE_SYMBOL s<D, L<>> { using type = L<D>; };

/*
// unitary
template <typename D, template <typename...> typename L, typename T>
struct FATAL_HIDE_SYMBOL s<D, L<T>> {
  using type = std::conditional_t<std::is_same_v<D, T>, L<T>, L<D, T, D>>;
};
*/

template <bool, bool, bool, typename...> struct FATAL_HIDE_SYMBOL S;

template <typename D, template <typename...> typename L, typename T>
struct FATAL_HIDE_SYMBOL S<false, false, false, D, L<T>> {
  using type = L<D, T, D>;
};

template <typename D, template <typename...> typename L, typename T>
struct FATAL_HIDE_SYMBOL S<false, true, true, D, L<T>> {
  using type = L<T>;
};

template <typename D, template <typename...> typename L, typename... T>
struct FATAL_HIDE_SYMBOL S<true, false, false, D, L<T...>> {
  using type = L<D, T..., D>;
};

template <typename D, template <typename...> typename L, typename... T>
struct FATAL_HIDE_SYMBOL S<true, false, true, D, L<T...>> {
  using type = L<D, T...>;
};

template <typename D, template <typename...> typename L, typename... T>
struct FATAL_HIDE_SYMBOL S<true, true, false, D, L<T...>> {
  using type = L<T..., D>;
};

template <typename D, template <typename...> typename L, typename... T>
struct FATAL_HIDE_SYMBOL S<true, true, true, D, L<T...>> {
  using type = L<T...>;
};

// size >= 1
template <typename D, template <typename...> typename L, typename F, typename... T>
struct FATAL_HIDE_SYMBOL s<D, L<F, T...>>:
  S<
    (sizeof...(T) > 0),
    std::is_same_v<D, F>,
    std::is_same_v<D, last<L<F, T...>>>,
    D, L<F, T...>
  >
{};

} // namespace ftl::i_i {

#endif // FATAL_INCLUDE_fatal_type_impl_interleave_h
