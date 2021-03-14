#ifndef TOOLSLIBRARY_STRING_HPP
#define TOOLSLIBRARY_STRING_HPP
#include <algorithm>
#include <filesystem>
#include <ranges>
namespace tl::string {

/**
 * Remove c:\ drive letter from start of file.
 * @param input path string
 */
static void
  remove_drive_letter(std::string &input)
{
  constexpr static auto letters =
    std::string_view("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
  if (std::ranges::size(input) >= 3 && input[1] == ':'
      && (input[2] == '\\' || input[2] == '/')
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
[[maybe_unused]] static std::string
  remove_drive_letter(std::string &&input)
{
  remove_drive_letter(input);
  return std::move(input);
}

/**
 * Remove the \r from the end of the string
 * @param input
 */
static void
  remove_carriage_return_from_end(std::string &input)
{
  while (input.back() == '\r') {
    input.pop_back();
  }
}

/**
 * Remove the \r from the end of the string
 * @param input
 */
[[maybe_unused]] static std::string
  remove_carriage_return_from_end(std::string &&input)
{
  remove_carriage_return_from_end(input);
  return std::move(input);
}
/**
 * Remove the \r from the any part of the string
 * @param input
 */
static void
  remove_carriage_return(std::string &input)
{
  std::erase(input, '\r');
}
/**
 * Remove the \r from the any part of the string
 * @param input
 */
[[maybe_unused]] static std::string
  remove_carriage_return(std::string &&input)
{
  remove_carriage_return(input);
  return std::move(input);
}

/**
 * replace all slashes with the os's slashes.
 * @param haystack string with slashes
 */
[[maybe_unused]] static void
  replace_slashes(std::string &haystack)
{
  if constexpr (std::filesystem::path::preferred_separator == '/') {
    std::ranges::replace(
      haystack, '\\', std::filesystem::path::preferred_separator);
  }
}
/**
 * replace all slashes with the os's slashes.
 * @param haystack string with slashes
 */
[[maybe_unused]] static std::string
  replace_slashes(std::string &&haystack)
{
  replace_slashes(haystack);
  return std::move(haystack);
}
/**
 * replace all slashes with the \ slashes.
 * @param haystack string with slashes
 */
[[maybe_unused]] static void
  undo_replace_slashes(std::string &haystack)
{
  if constexpr (std::filesystem::path::preferred_separator == '/') {
    std::ranges::replace(
      haystack, std::filesystem::path::preferred_separator, '\\');
  }
}
/**
 * replace all slashes with the \ slashes.
 * @param haystack string with slashes
 */
[[maybe_unused]] static std::string
  undo_replace_slashes(std::string &&haystack)
{
  undo_replace_slashes(haystack);
  return std::move(haystack);
}
}// namespace tl::string
#endif// TOOLSLIBRARY_STRING_HPP
