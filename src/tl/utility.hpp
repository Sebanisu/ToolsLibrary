#ifndef TOOLSLIBRARY_UTILITY_HPP
#define TOOLSLIBRARY_UTILITY_HPP
#include <istream>
#include <ranges>
#include <span>
namespace tl::utility {
static auto
  get_remaining(const std::span<const char> in)
{
  return std::ranges::size(in);
}
static auto
  get_remaining(std::istream &in)
{
  const auto current = in.tellg();
  in.seekg(0, std::ios::end);
  const auto end = in.tellg();
  in.seekg(current, std::ios::beg);
  return static_cast<std::size_t>(end - current);
}
}// namespace tl::utility
#endif// TOOLSLIBRARY_UTILITY_HPP
