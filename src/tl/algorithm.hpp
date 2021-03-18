//
// Created by pcvii on 3/16/2021.
//

#ifndef TOOLSLIBRARY_ALGORITHM_HPP
#define TOOLSLIBRARY_ALGORITHM_HPP

#include "concepts.hpp"
#include <algorithm>
#include <cassert>
namespace tl::algorithm {
template<tl::concepts::is_iterator B1,
         tl::concepts::is_iterator E1,
         tl::concepts::is_iterator... BS,
         typename OP>
requires requires(OP op, B1 b1, E1 e1, BS... bs)
{
  op(*b1, *bs...);
  b1 != e1;
}
constexpr void
  for_each(const OP op, B1 b1, const E1 e1, BS... bs)
{
  for (; b1 != e1; ++b1, (++bs, ...)) {
    op(*b1, *bs...);
  }
}
template<tl::concepts::is_range R1, tl::concepts::is_range... RS, typename OP>
constexpr void
  for_each(const OP op, const R1 &r1, const RS &...rs)
{
  if (std::is_constant_evaluated()
      && ((std::size(r1) != std::size(rs)) || ...)) {
    throw;// causes compile error if you try to pass ranges of different size at
          // compile time.
  } else {
    assert(((std::size(r1) == std::size(rs))
            && ...));// causes runtime error if you pass ranges of different
                     // size at runtime in debug mode
  }
  for_each(op, std::cbegin(r1), std::cend(r1), std::cbegin(rs)...);
}
template<tl::concepts::is_range R,
         typename OP,
         typename T = std::decay_t<typename R::value_type>>
requires requires(OP op, T t)
{
  op(t);
}
[[nodiscard]] constexpr bool
  any_of(const R &r, const OP op)
{
  return std::any_of(std::cbegin(r), std::cend(r), op);
}
template<tl::concepts::is_range R,
         typename OI,
         typename OP,
         typename T = std::decay_t<typename R::value_type>>
requires requires(OP op, T t, OI oi)
{
  *oi = op(t);
  ++oi;
}
constexpr auto
  transform(const R &r, OI oi, const OP op)
{
  return std::transform(std::cbegin(r), std::cend(r), oi, op);
}
template<tl::concepts::is_range R,
         typename T = std::decay_t<typename R::value_type>>
constexpr void
  replace(R &r, const T &old_value, const T &new_value)
{
  std::replace(std::begin(r), std::end(r), old_value, new_value);
}
template<tl::concepts::is_range R,
         typename T = std::decay_t<typename R::value_type>>
constexpr auto
  find(const R &r, const T &value)
{
  return std::find(std::begin(r), std::end(r), value);
}
template<tl::concepts::is_range R1, tl::concepts::is_range R2>
constexpr bool
  equal(const R1 &r1, const R2 &r2)
{
  return std::equal(
    std::cbegin(r1), std::cend(r1), std::cbegin(r2), std::cend(r2));
}
}// namespace tl::algorithm
#endif// TOOLSLIBRARY_ALGORITHM_HPP
