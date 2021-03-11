#ifndef TOOLSLIBRARY_UTILITY_HPP
#define TOOLSLIBRARY_UTILITY_HPP
#include <istream>
#include <ranges>
#include <span>
#include <filesystem>
#include <fstream>
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
static std::optional<std::filesystem::path>
create_temp_file(const std::filesystem::path & file_name, const std::string_view & data)
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
}// namespace tl::utility
#endif// TOOLSLIBRARY_UTILITY_HPP
