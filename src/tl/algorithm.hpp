//
// Created by pcvii on 3/16/2021.
//

#ifndef TOOLSLIBRARY_ALGORITHM_HPP
#define TOOLSLIBRARY_ALGORITHM_HPP
namespace tl::algorithm {
template<typename B1, typename E1, typename... BS, typename OP>
void
  for_each(const OP op, B1 b, const E1 e, BS... bs)
{
  for (; b != e; ++b, (++bs, ...)) {
    op(*b, *bs...);
  }
}
}// namespace tl::algorithm
#endif// TOOLSLIBRARY_ALGORITHM_HPP
