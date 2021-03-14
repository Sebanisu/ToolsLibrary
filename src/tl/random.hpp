//
// Created by pcvii on 3/14/2021.
//
#ifndef TOOLSLIBRARY_RANDOM_HPP
#define TOOLSLIBRARY_RANDOM_HPP
#include "tl/concepts.hpp"
#include <algorithm>
#include <random>
#include <ranges>
namespace tl::random {
/**
 * Random holder should be defined as a static value.
 * @tparam T type of value returned by operator()
 * @see if we can make this passable to a std::transform
 * @todo maybe a form of random_impl should be moved to toolslibrary.
 */
template<typename T> struct impl
{
private:
  static_assert(std::is_integral_v<T>);
  std::random_device                                             rd{};
  mutable std::mt19937                                           gen{ rd() };
  mutable std::uniform_int_distribution<std::make_unsigned_t<T>> dis{
    std::numeric_limits<std::make_unsigned_t<T>>::min(),
    std::numeric_limits<std::make_unsigned_t<T>>::max()
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
template<std::ranges::contiguous_range rangeT>
requires(std::is_integral_v<
         std::decay_t<typename rangeT::value_type>>) void iota(rangeT &out)
{
  using T                    = std::decay_t<typename rangeT::value_type>;
  static const auto random_T = impl<T>();
  std::ranges::transform(
    out, std::begin(out), []([[maybe_unused]] const T &unused) -> T {
      return random_T(unused);
    });
}
/**
 * Get a std::array with random values.
 * @tparam T type stored in std::array
 * @tparam count is the total number of values in the array.
 * @return std::array<T,count>
 */
template<std::integral T, std::size_t count>
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
