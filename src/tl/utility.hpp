#ifndef TOOLSLIBRARY_UTILITY_HPP
#define TOOLSLIBRARY_UTILITY_HPP
#include <filesystem>
#include <fstream>
#include <istream>
#include <optional>

#include <span>
namespace tl::utility {
/**
 * Get remaining amount of bytes in a std::span.
 * @param in span
 */
[[nodiscard]] auto
  get_remaining(const std::span<const char> in)
{
  return std::size(in);
}
/**
 * Get remaining amount of bytes in a std::istream.
 * @param in stream
 */
[[nodiscard]] auto
  get_remaining(std::istream &in)
{
  const auto current = in.tellg();
  in.seekg(0, std::ios::end);
  const auto end = in.tellg();
  in.seekg(current, std::ios::beg);
  return static_cast<std::size_t>(end - current);
}
/**
 * Create a temp file filled with data you want.
 * @param file_name desired filename
 * @param data data you want wrote to the file.
 * @return optional path to file
 */
[[nodiscard]] std::optional<std::filesystem::path>
  create_temp_file(const std::filesystem::path &file_name,
                   const std::string_view &     data)
{
  using namespace std::string_view_literals;
  std::filesystem::path out_path =
    std::filesystem::temp_directory_path() / file_name;
  auto fp = std::fstream(out_path, std::ios::binary | std::ios::out);
  if (fp.is_open()) {
    const auto write = [&fp](const std::string_view &buffer) {
      fp.write(std::data(buffer), static_cast<long>(std::size(buffer)));
    };
    write(data);
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
auto
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
