/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_impl_sort_h
#define FATAL_INCLUDE_fatal_type_impl_sort_h

#include <fatal/type/cat.h>
#include <fatal/type/conditional.h>
#include <fatal/type/tag.h>

// TODO: REMOVE THESE HEADERS WHEN A PROPER SORT FOR SEQUENCES IS IN
#include <fatal/type/convert.h>
#include <fatal/type/list.h>

#include <type_traits>

namespace fatal {
namespace i_s {

// partition recursion
template <template <typename...> typename, typename...> struct FATAL_HIDE_SYMBOL p;

template <template <typename...> typename Pair, typename L, typename R>
struct FATAL_HIDE_SYMBOL p<Pair, L, R> {
  using type = Pair<L, R>;
};

template <
  template <typename...> typename Pair, template <typename...> typename Variadic,
  typename... L, typename... R, typename T0
>
struct FATAL_HIDE_SYMBOL p<Pair, Variadic<L...>, Variadic<R...>, indexed<T0, true>> {
  using type = Pair<Variadic<L..., T0>, Variadic<R...>>;
};

template <
  template <typename...> typename Pair, template <typename...> typename Variadic,
  typename... L, typename... R, typename T0
>
struct FATAL_HIDE_SYMBOL p<Pair, Variadic<L...>, Variadic<R...>, indexed<T0, false>> {
  using type = Pair<Variadic<L...>, Variadic<R..., T0>>;
};

template <
  template <typename...> typename Pair, template <typename...> typename Variadic,
  typename... L, typename... R, typename T0, typename T1
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<L...>, Variadic<R...>,
  indexed<T0, false>, indexed<T1, false>
> {
  using type = Pair<Variadic<L...>, Variadic<R..., T0, T1>>;
};

template <
  template <typename...> typename Pair, template <typename...> typename Variadic,
  typename... L, typename... R, typename T0, typename T1
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<L...>, Variadic<R...>,
  indexed<T0, false>, indexed<T1, true>
> {
  using type = Pair<Variadic<L..., T1>, Variadic<R..., T0>>;
};

template <
  template <typename...> typename Pair, template <typename...> typename Variadic,
  typename... L, typename... R, typename T0, typename T1
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<L...>, Variadic<R...>,
  indexed<T0, true>, indexed<T1, false>
> {
  using type = Pair<Variadic<L..., T0>, Variadic<R..., T1>>;
};

template <
  template <typename...> typename Pair, template <typename...> typename Variadic,
  typename... L, typename... R, typename T0, typename T1
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<L...>, Variadic<R...>,
  indexed<T0, true>, indexed<T1, true>
> {
  using type = Pair<Variadic<L..., T0, T1>, Variadic<R...>>;
};

template <
  template <typename...> typename Pair, template <typename...> typename Variadic,
  typename... L, typename... R,
  typename T0, typename T1, typename T2, typename... Args
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<L...>, Variadic<R...>,
  indexed<T0, false>, indexed<T1, false>, indexed<T2, false>,
  Args...
>:
  p<Pair, Variadic<L...>, Variadic<R..., T0, T1, T2>, Args...>
{};

template <
  template <typename...> typename Pair, template <typename...> typename Variadic,
  typename... L, typename... R,
  typename T0, typename T1, typename T2, typename... Args
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<L...>, Variadic<R...>,
  indexed<T0, false>, indexed<T1, false>, indexed<T2, true>,
  Args...
>:
  p<Pair, Variadic<L..., T2>, Variadic<R..., T0, T1>, Args...>
{};

template <
  template <typename...> typename Pair, template <typename...> typename Variadic,
  typename... L, typename... R,
  typename T0, typename T1, typename T2, typename... Args
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<L...>, Variadic<R...>,
  indexed<T0, false>, indexed<T1, true>, indexed<T2, false>,
  Args...
>:
  p<Pair, Variadic<L..., T1>, Variadic<R..., T0, T2>, Args...>
{};

template <
  template <typename...> typename Pair, template <typename...> typename Variadic,
  typename... L, typename... R,
  typename T0, typename T1, typename T2, typename... Args
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<L...>, Variadic<R...>,
  indexed<T0, false>, indexed<T1, true>, indexed<T2, true>,
  Args...
>:
  p<Pair, Variadic<L..., T1, T2>, Variadic<R..., T0>, Args...>
{};

template <
  template <typename...> typename Pair, template <typename...> typename Variadic,
  typename... L, typename... R,
  typename T0, typename T1, typename T2, typename... Args
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<L...>, Variadic<R...>,
  indexed<T0, true>, indexed<T1, false>, indexed<T2, false>,
  Args...
>:
  p<Pair, Variadic<L..., T0>, Variadic<R..., T1, T2>, Args...>
{};

template <
  template <typename...> typename Pair, template <typename...> typename Variadic,
  typename... L, typename... R,
  typename T0, typename T1, typename T2, typename... Args
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<L...>, Variadic<R...>,
  indexed<T0, true>, indexed<T1, false>, indexed<T2, true>,
  Args...
>:
  p<Pair, Variadic<L..., T0, T2>, Variadic<R..., T1>, Args...>
{};

template <
  template <typename...> typename Pair, template <typename...> typename Variadic,
  typename... L, typename... R,
  typename T0, typename T1, typename T2, typename... Args
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<L...>, Variadic<R...>,
  indexed<T0, true>, indexed<T1, true>, indexed<T2, false>,
  Args...
>:
  p<Pair, Variadic<L..., T0, T1>, Variadic<R..., T2>, Args...>
{};

template <
  template <typename...> typename Pair, template <typename...> typename Variadic,
  typename... L, typename... R,
  typename T0, typename T1, typename T2, typename... Args
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<L...>, Variadic<R...>,
  indexed<T0, true>, indexed<T1, true>, indexed<T2, true>,
  Args...
>:
  p<Pair, Variadic<L..., T0, T1, T2>, Variadic<R...>, Args...>
{};

// placeholder for partition's filtered sequence elements
template <typename T, T, bool> struct FATAL_HIDE_SYMBOL h;

template <
  template <typename...> typename Pair,
  template <typename V, V...> typename Variadic, typename T,
  T... L, T... R, T T0
>
struct FATAL_HIDE_SYMBOL p<Pair, Variadic<T, L...>, Variadic<T, R...>, h<T, T0, true>> {
  using type = Pair<Variadic<T, L..., T0>, Variadic<T, R...>>;
};

template <
  template <typename...> typename Pair,
  template <typename V, V...> typename Variadic,
  typename T, T... L, T... R, T T0
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<T, L...>, Variadic<T, R...>, h<T, T0, false>
> {
  using type = Pair<Variadic<T, L...>, Variadic<T, R..., T0>>;
};

template <
  template <typename...> typename Pair,
  template <typename V, V...> typename Variadic,
  typename T, T... L, T... R, T T0, T T1
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<T, L...>, Variadic<T, R...>,
  h<T, T0, false>, h<T, T1, false>
> {
  using type = Pair<Variadic<T, L...>, Variadic<T, R..., T0, T1>>;
};

template <
  template <typename...> typename Pair,
  template <typename V, V...> typename Variadic,
  typename T, T... L, T... R, T T0, T T1
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<T, L...>, Variadic<T, R...>,
  h<T, T0, false>, h<T, T1, true>
> {
  using type = Pair<Variadic<T, L..., T1>, Variadic<T, R..., T0>>;
};

template <
  template <typename...> typename Pair,
  template <typename V, V...> typename Variadic,
  typename T, T... L, T... R, T T0, T T1
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<T, L...>, Variadic<T, R...>,
  h<T, T0, true>, h<T, T1, false>
> {
  using type = Pair<Variadic<T, L..., T0>, Variadic<T, R..., T1>>;
};

template <
  template <typename...> typename Pair,
  template <typename V, V...> typename Variadic,
  typename T, T... L, T... R, T T0, T T1
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<T, L...>, Variadic<T, R...>,
  h<T, T0, true>, h<T, T1, true>
> {
  using type = Pair<Variadic<T, L..., T0, T1>, Variadic<T, R...>>;
};

template <
  template <typename...> typename Pair,
  template <typename V, V...> typename Variadic,
  typename T, T... L, T... R, T T0, T T1, T T2, typename... Args
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<T, L...>, Variadic<T, R...>,
  h<T, T0, false>, h<T, T1, false>, h<T, T2, false>,
  Args...
>:
  p<Pair, Variadic<T, L...>, Variadic<T, R..., T0, T1, T2>, Args...>
{};

template <
  template <typename...> typename Pair,
  template <typename V, V...> typename Variadic,
  typename T, T... L, T... R, T T0, T T1, T T2, typename... Args
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<T, L...>, Variadic<T, R...>,
  h<T, T0, false>, h<T, T1, false>, h<T, T2, true>,
  Args...
>:
  p<Pair, Variadic<T, L..., T2>, Variadic<T, R..., T0, T1>, Args...>
{};

template <
  template <typename...> typename Pair,
  template <typename V, V...> typename Variadic,
  typename T, T... L, T... R, T T0, T T1, T T2, typename... Args
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<T, L...>, Variadic<T, R...>,
  h<T, T0, false>, h<T, T1, true>, h<T, T2, false>,
  Args...
>:
  p<Pair, Variadic<T, L..., T1>, Variadic<T, R..., T0, T2>, Args...>
{};

template <
  template <typename...> typename Pair,
  template <typename V, V...> typename Variadic,
  typename T, T... L, T... R, T T0, T T1, T T2, typename... Args
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<T, L...>, Variadic<T, R...>,
  h<T, T0, false>, h<T, T1, true>, h<T, T2, true>,
  Args...
>:
  p<Pair, Variadic<T, L..., T1, T2>, Variadic<T, R..., T0>, Args...>
{};

template <
  template <typename...> typename Pair,
  template <typename V, V...> typename Variadic,
  typename T, T... L, T... R, T T0, T T1, T T2, typename... Args
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<T, L...>, Variadic<T, R...>,
  h<T, T0, true>, h<T, T1, false>, h<T, T2, false>,
  Args...
>:
  p<Pair, Variadic<T, L..., T0>, Variadic<T, R..., T1, T2>, Args...>
{};

template <
  template <typename...> typename Pair,
  template <typename V, V...> typename Variadic,
  typename T, T... L, T... R, T T0, T T1, T T2, typename... Args
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<T, L...>, Variadic<T, R...>,
  h<T, T0, true>, h<T, T1, false>, h<T, T2, true>,
  Args...
>:
  p<Pair, Variadic<T, L..., T0, T2>, Variadic<T, R..., T1>, Args...>
{};

template <
  template <typename...> typename Pair,
  template <typename V, V...> typename Variadic,
  typename T, T... L, T... R, T T0, T T1, T T2, typename... Args
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<T, L...>, Variadic<T, R...>,
  h<T, T0, true>, h<T, T1, true>, h<T, T2, false>,
  Args...
>:
  p<Pair, Variadic<T, L..., T0, T1>, Variadic<T, R..., T2>, Args...>
{};

template <
  template <typename...> typename Pair,
  template <typename V, V...> typename Variadic,
  typename T, T... L, T... R, T T0, T T1, T T2, typename... Args
>
struct FATAL_HIDE_SYMBOL p<
  Pair, Variadic<T, L...>, Variadic<T, R...>,
  h<T, T0, true>, h<T, T1, true>, h<T, T2, true>,
  Args...
>:
  p<Pair, Variadic<T, L..., T0, T1, T2>, Variadic<T, R...>, Args...>
{};

// partition entry point
template <template <typename...> typename, typename...> struct FATAL_HIDE_SYMBOL P;

template <
  template <typename...> typename Pair,
  template <typename...> typename Variadic,
  typename... Args,
  typename Filter
>
struct FATAL_HIDE_SYMBOL P<Pair, Variadic<Args...>, Filter>:
  p<
    Pair, Variadic<>, Variadic<>,
    indexed<Args, Filter::template apply<Args>::value>...
  >
{};

template <
  template <typename...> typename Pair,
  template <typename V, V...> typename Variadic,
  typename T, T... Args,
  typename Filter
>
struct FATAL_HIDE_SYMBOL P<Pair, Variadic<T, Args...>, Filter>:
  p<
    Pair, Variadic<T>, Variadic<T>,
    h<T, Args, Filter::template apply<T, Args>::value>...
  >
{};

template <typename...> struct FATAL_HIDE_SYMBOL f;

template <
  typename Predicate,
  template <typename...> typename Variadic,
  typename... Result
>
struct FATAL_HIDE_SYMBOL f<Predicate, Variadic<Result...>> {
  using type = Variadic<Result...>;
};

template <
  typename Predicate,
  template <typename...> typename Variadic,
  typename... Result,
  typename T,
  typename... Args
>
struct FATAL_HIDE_SYMBOL f<Predicate, Variadic<Result...>, T, Args...>:
  f<
    Predicate,
    conditional<
      Predicate::template apply<T>::value,
      Variadic<Result..., T>,
      Variadic<Result...>
    >,
    Args...
  >
{};

template <typename...> struct FATAL_HIDE_SYMBOL F;

// TODO: OPTIMIZE
template <
  typename Predicate,
  template <typename...> typename Variadic,
  typename... Args
>
struct FATAL_HIDE_SYMBOL F<Predicate, Variadic<Args...>>:
  f<Predicate, Variadic<>, Args...>
{};

// merge implementation
template <typename...> struct FATAL_HIDE_SYMBOL m;

template <
  template <typename...> typename Variadic,
  typename L, typename... LHS,
  typename R, typename... RHS,
  typename... Args
>
struct FATAL_HIDE_SYMBOL m<Variadic<L, LHS...>, Variadic<R, RHS...>, Variadic<Args...>> {
  using type = conditional<
    (R::value < L::value),
    typename m<
      Variadic<L, LHS...>, Variadic<RHS...>, Variadic<Args..., R>
    >::type,
    typename m<
      Variadic<LHS...>, Variadic<R, RHS...>, Variadic<Args..., L>
    >::type
  >;
};

template <
  template <typename...> typename Variadic,
  typename... LHS, typename... Args
>
struct FATAL_HIDE_SYMBOL m<Variadic<LHS...>, Variadic<>, Variadic<Args...>> {
  using type = Variadic<Args..., LHS...>;
};

template <
  template <typename...> typename Variadic,
  typename... RHS, typename... Args
>
struct FATAL_HIDE_SYMBOL m<Variadic<>, Variadic<RHS...>, Variadic<Args...>> {
  using type = Variadic<Args..., RHS...>;
};

template <template <typename...> typename Variadic, typename... Args>
struct FATAL_HIDE_SYMBOL m<Variadic<>, Variadic<>, Variadic<Args...>> {
  using type = Variadic<Args...>;
};

template <
  template <typename V, V...> typename Variadic,
  typename T,
  T L, T... LHS,
  T R, T... RHS,
  T... Values
>
struct FATAL_HIDE_SYMBOL m<
  Variadic<T, L, LHS...>,
  Variadic<T, R, RHS...>,
  Variadic<T, Values...>
> {
  using type = conditional<
    (R < L),
    typename m<
      Variadic<T, L, LHS...>,
      Variadic<T, RHS...>,
      Variadic<T, Values..., R>
    >::type,
    typename m<
      Variadic<T, LHS...>,
      Variadic<T, R, RHS...>,
      Variadic<T, Values..., L>
    >::type
  >;
};

template <
  template <typename V, V...> typename Variadic,
  typename T,
  T... LHS,
  T... Values
>
struct FATAL_HIDE_SYMBOL m<Variadic<T, LHS...>, Variadic<T>, Variadic<T, Values...>> {
  using type = Variadic<T, Values..., LHS...>;
};

template <
  template <typename V, V...> typename Variadic,
  typename T,
  T... RHS,
  T... Values
>
struct FATAL_HIDE_SYMBOL m<Variadic<T>, Variadic<T, RHS...>, Variadic<T, Values...>> {
  using type = Variadic<T, Values..., RHS...>;
};

template <template <typename V, V...> typename Variadic, typename T, T... Values>
struct FATAL_HIDE_SYMBOL m<Variadic<T>, Variadic<T>, Variadic<T, Values...>> {
  using type = Variadic<T, Values...>;
};

// merge entry point
template <typename...> struct FATAL_HIDE_SYMBOL M;

template <
  template <typename...> typename Variadic,
  typename... LHS, typename... RHS
>
struct FATAL_HIDE_SYMBOL M<Variadic<LHS...>, Variadic<RHS...>> {
  using type = typename m<Variadic<LHS...>, Variadic<RHS...>, Variadic<>>::type;
};

template <
  template <typename V, V...> typename Variadic,
  typename T,
  T... LHS,
  T... RHS
>
struct FATAL_HIDE_SYMBOL M<Variadic<T, LHS...>, Variadic<T, RHS...>> {
  using type = typename m<
    Variadic<T, LHS...>,
    Variadic<T, RHS...>,
    Variadic<T>
  >::type;
};

// merge sort
template <typename...> struct FATAL_HIDE_SYMBOL s;

template <template <typename...> typename Variadic>
struct FATAL_HIDE_SYMBOL s<Variadic<>> {
  using type = Variadic<>;
};

template <template <typename...> typename Variadic, typename T>
struct FATAL_HIDE_SYMBOL s<Variadic<T>> {
  using type = Variadic<T>;
};

template <template <typename...> typename Variadic, typename... Args>
struct FATAL_HIDE_SYMBOL s<Variadic<Args...>> {
  using type = typename m<
    typename s<head<Variadic<Args...>, sizeof...(Args) / 2>>::type,
    typename s<tail<Variadic<Args...>, sizeof...(Args) / 2>>::type,
    Variadic<>
  >::type;
};

template <template <typename V, V...> typename Variadic, typename T>
struct FATAL_HIDE_SYMBOL s<Variadic<T>> {
  using type = Variadic<T>;
};

template <template <typename V, V...> typename Variadic, typename T, T Value>
struct FATAL_HIDE_SYMBOL s<Variadic<T, Value>> {
  using type = Variadic<T, Value>;
};

template <template <typename V, V...> typename Variadic, typename T, T... Values>
struct FATAL_HIDE_SYMBOL s<Variadic<T, Values...>> {
  using type = typename m<
    typename s<head<Variadic<T, Values...>, sizeof...(Values) / 2>>::type,
    typename s<tail<Variadic<T, Values...>, sizeof...(Values) / 2>>::type,
    Variadic<T>
  >::type;
};

// quicksort
template <typename...> struct FATAL_HIDE_SYMBOL q;

template <template <typename...> typename Variadic, typename Less>
struct FATAL_HIDE_SYMBOL q<Variadic<>, Less> {
  using type = Variadic<>;
};

template <template <typename...> typename Variadic, typename T, typename Less>
struct FATAL_HIDE_SYMBOL q<Variadic<T>, Less> {
  using type = Variadic<T>;
};

template <
  template <typename...> typename Variadic,
  typename LHS,
  typename RHS,
  typename Less
>
struct FATAL_HIDE_SYMBOL q<Variadic<LHS, RHS>, Less> {
  using type = conditional<
    Less::template apply<RHS, LHS>::value,
    Variadic<RHS, LHS>,
    Variadic<LHS, RHS>
  >;
};

// TODO: MAKE IT FOUR WAY QUICK SORT??
template <
  template <typename...> typename Variadic,
  typename Pivot, typename... Args, typename Less
>
// TODO: USE A BETTER PIVOT
struct FATAL_HIDE_SYMBOL q<Variadic<Pivot, Args...>, Less> {
  using type = lcat<
    typename q<
      second<typename P<pair, Variadic<Args...>, curry<Less, Pivot>>::type>,
      Less
    >::type,
    typename q<
      first<typename P<pair, Variadic<Args...>, curry<Less, Pivot>>::type>,
      Less
    >::type,
    Pivot
  >;
};

template <template <typename V, V...> typename Variadic, typename T, typename Less>
struct FATAL_HIDE_SYMBOL q<Variadic<T>, Less> {
  using type = Variadic<T>;
};

template <
  template <typename V, V...> typename Variadic,
  typename T, T Value,
  typename Less
>
struct FATAL_HIDE_SYMBOL q<Variadic<T, Value>, Less> {
  using type = Variadic<T, Value>;
};

template <
  template <typename V, V...> typename Variadic,
  typename T, T LHS, T RHS,
  typename Less
>
struct FATAL_HIDE_SYMBOL q<Variadic<T, LHS, RHS>, Less> {
  using type = conditional<
    Less::template vapply<T, RHS, LHS>::value,
    Variadic<T, RHS, LHS>,
    Variadic<T, LHS, RHS>
  >;
};

// curried comparer for sequences
template <typename Less, typename T, T LHS>
struct FATAL_HIDE_SYMBOL c {
  template <typename U, U RHS>
  using apply = typename Less::template vapply<T, LHS, RHS>;
};

template <
  template <typename V, V...> typename Variadic,
  typename T, T Pivot, T Arg0, T Arg1, T... Args,
  typename Less
>
// TODO: USE A BETTER PIVOT
struct FATAL_HIDE_SYMBOL q<Variadic<T, Pivot, Arg0, Arg1, Args...>, Less> {
  using type = typename vcat<
    typename q<
      second<
        typename P<
          pair,
          Variadic<T, Arg0, Arg1, Args...>,
          c<Less, T, Pivot>
        >::type
      >,
      Less
    >::type,
    typename q<
      first<
        typename P<
          pair,
          Variadic<T, Arg0, Arg1, Args...>,
          c<Less, T, Pivot>
        >::type
      >,
      Less
    >::type
  >::template apply<Pivot>;
};

// TODO: OPTIMIZE BY EXPANDING THE q LIST INTO A PAIR(FILTERED, T)??
// filtered comparer for quicksort
template <typename Less, typename Filter>
struct FATAL_HIDE_SYMBOL C {
  template <typename LHS, typename RHS>
  using apply = typename Less::template apply<
    typename Filter::template apply<LHS>,
    typename Filter::template apply<RHS>
  >;
};

// quicksort entry-point
template <typename...> struct FATAL_HIDE_SYMBOL Q;

template <typename T, typename Less>
struct FATAL_HIDE_SYMBOL Q<T, Less>: q<T, Less> {};

template <typename T, typename Less, typename By>
struct FATAL_HIDE_SYMBOL Q<T, Less, By>: q<T, C<Less, By>> {};

// reverse

template <typename> struct FATAL_HIDE_SYMBOL i;

template <template <typename, typename> typename Pair, typename LHS, typename RHS>
struct FATAL_HIDE_SYMBOL i<Pair<LHS, RHS>> {
  using type = Pair<RHS, LHS>;
};

template <
  template <typename...> typename Variadic,
  typename LHS,
  typename RHS,
  typename... Args
>
struct FATAL_HIDE_SYMBOL i<Variadic<LHS, RHS, Args...>> {
  using type = Variadic<RHS, LHS, Args...>;
};

} // namespace i_s {
} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_type_impl_sort_h
