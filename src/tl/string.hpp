#ifndef TOOLSLIBRARY_STRING_HPP
#define TOOLSLIBRARY_STRING_HPP
#include <ranges>
#include <algorithm>
namespace tl::string
{

/**
 * Remove c:\ drive letter from start of file.
 * @param input path string
 */
static void
remove_drive_letter(std::string &input)
{
  constexpr static auto letters =
    std::string_view("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
  if (std::ranges::size(input) >=3 && input[1] == ':' && (input[2] == '\\' || input[2] == '/')
      && std::ranges::any_of(letters, [&input](const char &c) {
    return input[0] == c;
  })) {
    input.erase(0, 3);// remove c:\ from the start of the strings.
  }
}
/**
 * Remove c:\ drive letter from start of file.
 * @param input path string
 * @return modified string
 */
static std::string
remove_drive_letter(std::string &&input)
{
  remove_drive_letter(input);
  return std::move(input);
}
}
#endif// TOOLSLIBRARY_STRING_HPP
