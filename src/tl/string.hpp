#ifndef TOOLSLIBRARY_STRING_HPP
#define TOOLSLIBRARY_STRING_HPP
#include <algorithm>
#include <filesystem>
#include <ranges>
namespace tl::string {

[[nodiscard]] [[maybe_unused]] auto
  search(const auto &b1, const auto &e1, const auto &b2, const auto &e2)
{
  auto f = std::search(b1, e1, b2, e2);
  if (f != e1) {
    return std::optional<decltype(f)>(f);
  }
  return std::optional<decltype(f)>();
}

[[nodiscard]] [[maybe_unused]] auto
  search(const auto              b1,
         const std::string &     haystack,
         const std::string_view &needle)
{
  const auto e1 = std::ranges::end(haystack);
  const auto b2 = std::ranges::begin(needle);
  const auto e2 = std::ranges::end(needle);
  return string::search(b1, e1, b2, e2);
}

[[nodiscard]] [[maybe_unused]] auto
  search(const std::string &haystack, const std::string_view &needle)
{

  const auto b1 = std::ranges::begin(haystack);
  return string::search(b1, haystack, needle);
}

/**
 * remove all sub string values from string.
 * @param haystack string with needles to remove
 * @param needle string_view of what needs removed.
 * @see https://www.oreilly.com/library/view/c-cookbook/0596007612/ch04s12.html
 */
template<typename... T>
requires(std::is_same_v<std::string_view, std::decay_t<T>>
           &&...) void erase_string_from_string(std::string &haystack,
                                                const T &...needle)
{
  (
    [&haystack](const std::string_view &n) {
      for (auto f = search(haystack, n); f.has_value();
           f      = search(f.value(), haystack, n)) {
        haystack.erase(f.value(),
                       f.value() + static_cast<long>(std::ranges::size(n)));
      }
    }(needle),
    ...);
}

/**
 * remove all sub string values from string.
 * @param haystack string with needles to remove
 * @param needle string_view of what needs removed.
 * @return
 * @see https://www.oreilly.com/library/view/c-cookbook/0596007612/ch04s12.html
 */
template<typename... T>
requires(std::is_same_v<std::string_view, std::decay_t<T>>)
  [[nodiscard]] [[maybe_unused]] std::string
  erase_string_from_string(std::string &&haystack, const T &...needle)
{
  erase_string_from_string(haystack, needle...);
  return std::move(haystack);
}

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
[[nodiscard]] [[maybe_unused]] static std::string
  remove_drive_letter(std::string &&input)
{
  remove_drive_letter(input);
  return std::move(input);
}

static void
  remove_all_drive_letters(std::string &heystack)
{
  const auto letters =
    std::string_view("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
  const auto slashes = std::string_view(R"(\/)");
  const auto colon   = ':';
  for (const auto &letter : letters) {
    for (const auto slash : slashes) {
      std::array<char, 3U> needle = { letter, colon, slash };
      erase_string_from_string(
        heystack,
        std::string_view(std::ranges::begin(needle), std::ranges::end(needle)));
    }
  }
}
[[nodiscard]] [[maybe_unused]] static std::string
  remove_all_drive_letters(std::string &&heystack)
{
  remove_all_drive_letters(heystack);
  return std::move(heystack);
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
[[nodiscard]] [[maybe_unused]] static std::string
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
[[nodiscard]] [[maybe_unused]] static std::string
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
