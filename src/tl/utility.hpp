#ifndef TOOLSLIBRARY_UTILITY_HPP
#define TOOLSLIBRARY_UTILITY_HPP
#include "tl/concepts.hpp"
#include <array>
#include <cassert>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <istream>
#include <optional>
#include <span>
#include <vector>

namespace tl::utility {
/**
 * Utility functions used by the rest of the library
 */

/**
 * Convert Type to bytes array
 * @tparam T Trivially copyable value type
 * @param in Trivially copyable value
 * @return array of bytes
 */
template<concepts::is_trivially_copyable T>
[[nodiscard]] inline std::array<char, sizeof(T)>
  to_bytes(const T &in)
{
  // todo std::bit_cast will be able to make this function constexpr
  // todo this is only in gcc trunk and msvc right now.
  auto tmp = std::array<char, sizeof(T)>{};
  std::memcpy(std::data(tmp), &in, std::size(tmp));
  return tmp;
}
/**
 * Convert Type to bytes vector
 * @tparam T Trivially copyable value type
 * @param in Trivially copyable value source pointer
 * @param count number of elements from source pointer.
 * @return vector of bytes
 */
template<concepts::is_trivially_copyable T>
[[nodiscard]] inline std::vector<char>
  to_bytes(const T *const in, std::size_t count = 1)
{
  assert(count != 0);
  auto tmp = std::vector<char>(count * sizeof(T));
  std::memcpy(std::data(tmp), in, std::size(tmp));
  return tmp;
}
/**
 * Get remaining amount of bytes in a std::span.
 * @param in span
 */
[[nodiscard]] inline auto
  get_remaining(const std::span<const char> in)
{
  return std::size(in);
}
/**
 * Get remaining amount of bytes in a std::istream.
 * @param in stream
 */
[[nodiscard]] inline auto
  get_remaining(std::istream &in)
{
  const auto current = in.tellg();
  in.seekg(0, std::ios::end);
  const auto end = in.tellg();
  in.seekg(current, std::ios::beg);
  return static_cast<std::size_t>(end - current);
}
/**
 * Get amount of bytes in a std::span.
 * @param in span
 */
[[nodiscard]] inline auto
  get_position(const std::span<const char> in)
{
  return std::size(in);
}
/**
 * Get position in a std::ostream.
 * @param in stream
 */
[[nodiscard]] inline auto
  get_position(std::ostream &in)
{
  return in.tellp();
}
/**
 * Create a temp file filled with data you want.
 * @param file_name desired filename
 * @param data data you want wrote to the file.
 * @return optional path to file
 */
[[nodiscard]] inline std::optional<std::filesystem::path>
  create_temp_file(const std::filesystem::path &file_name,
                   const std::string_view      &data)
{
  using namespace std::string_view_literals;
  std::error_code       ec;
  std::filesystem::path out_path =
    std::filesystem::temp_directory_path(ec) / file_name;

  if (ec) {
    std::cerr << "Failed to get temp directory for file '" << file_name.string()
              << "': " << ec.message() << '\n';
    return std::nullopt;
  }

  auto fp = std::fstream(out_path, std::ios::binary | std::ios::out);
  if (fp.is_open()) {
    const auto write = [&fp](const std::string_view &buffer) {
      fp.write(std::data(buffer), static_cast<long>(std::size(buffer)));
    };
    write(data);
  } else {
    std::cerr << "Failed to open file '" << out_path.string()
              << "' for writing: " << std::strerror(errno) << '\n';
    return std::nullopt;
  }

  return out_path;
}

/**
 * Holds number sequence and has operator to pass a lambda that takes 1 of the
 * sequence as a template parameter.
 * @tparam number_sequence
 */
template<std::size_t... number_sequence> struct sequence_impl
{
  template<typename lambdaT>
  constexpr void
    operator()(const lambdaT &op) const
  {
    (op.template operator()<number_sequence>(), ...);
  }
};
/**
 *
 * @tparam Min min value of I.
 * @tparam I is the current number in sequence
 * @tparam Ns sequence is number of values.
 * @return
 * @see https://stackoverflow.com/a/49673314/2588183
 */
template<std::size_t Min, std::size_t I, std::size_t... Ns>
inline auto
  make_sequence_impl()
{
  static_assert(I >= Min);
  if constexpr (I == Min)
    return sequence_impl<Ns...>();
  else
    return make_sequence_impl<Min, I - 1U, I - 1U, Ns...>();
}
/**
 * Get sequence_impl from Min to Max numbers
 * @tparam Min number
 * @tparam Max number
 */
template<size_t Min, size_t Count>
using sequence = std::decay_t<decltype(make_sequence_impl<Min, Count + Min>())>;
}// namespace tl::utility
#endif// TOOLSLIBRARY_UTILITY_HPP
