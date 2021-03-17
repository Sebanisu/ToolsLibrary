//
// Created by pcvii on 3/16/2021.
//

#ifndef TOOLSLIBRARY_ALGORITHM_HPP
#define TOOLSLIBRARY_ALGORITHM_HPP
#include <ranges>
namespace tl::algorithm {
template<typename B1, typename E1, typename... BS, typename OP>
void
  for_each(const OP op, B1 b, const E1 e, BS... bs)
{
  for (; b != e; ++b, (++bs, ...)) {
    op(*b, *bs...);
  }
}
template<std::ranges::forward_range R1, std::ranges::forward_range... RS, typename OP>
void
  for_each(const OP op, const R1 &r1, const RS &...rs)
{
  for_each(op,
           std::ranges::cbegin(r1),
           std::ranges::cend(r1),
           std::ranges::cbegin(rs)...);
}
}// namespace tl::algorithm
#endif// TOOLSLIBRARY_ALGORITHM_HPP
