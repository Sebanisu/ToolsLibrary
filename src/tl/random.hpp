//
// Created by pcvii on 3/14/2021.
//
#ifndef TOOLSLIBRARY_RANDOM_HPP
#define TOOLSLIBRARY_RANDOM_HPP
#include "tl/algorithm.hpp"
#include "tl/concepts.hpp"
#include <random>

namespace tl::random {
/**
 * Random holder should be defined as a  value.
 * @tparam T type of value returned by operator()
 * @see if we can make this passable to a std::transform
 */
template<tl::concepts::is_integral T> struct impl
{
private:
  using unsigned_t = std::make_unsigned_t<T>;
  std::random_device                                 rd{};
  mutable std::mt19937                               gen{ rd() };
  mutable std::uniform_int_distribution<std::size_t> dis{
    std::numeric_limits<unsigned_t>::min(),
    std::numeric_limits<unsigned_t>::max()
  };

public:
  auto
    operator()([[maybe_unused]] const T &unused) const
  {
    return static_cast<T>(dis(gen));
  }
};
/**
 * Fill the range with random data.
 * @tparam T type going into range.
 * @param out range
 */
template<tl::concepts::is_range rangeT>
requires(std::is_integral_v<
         std::decay_t<typename rangeT::value_type>>) void iota(rangeT &out)
{
  using T                    = std::decay_t<typename rangeT::value_type>;
  static const auto random_T = impl<T>();
  tl::algorithm::transform(out, std::begin(out), [](const T &unused) -> T {
    return random_T(unused);
  });
}
/**
 * Get a std::array with random values.
 * @tparam T type stored in std::array
 * @tparam count is the total number of values in the array.
 * @return std::array<T,count>
 */
template<tl::concepts::is_integral T, std::size_t count>
auto
  iota()
{
  static_assert(std::is_integral_v<T>);
  std::array<T, count> out{};
  iota(out);
  return out;
}
}// namespace tl::random
#endif// TOOLSLIBRARY_RANDOM_HPP
