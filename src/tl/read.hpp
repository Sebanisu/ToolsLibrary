//
// Created by pcvii on 3/8/2021.
//
#ifndef TOOLSLIBRARY_READ_HPP
#define TOOLSLIBRARY_READ_HPP
#include "input.hpp"
#include <filesystem>
#include <fstream>
#include <optional>
#include <thread>
namespace tl::read {
/**
 * Opens file pointer and returns an optional.
 * @param path
 * @return
 * @todo test?
 */
static std::optional<std::ifstream>
  open_file(const std::filesystem::path &path)
{
  std::optional<std::ifstream> ofp{};
  std::error_code              ec{};
  if (!std::filesystem::exists(path, ec)) {
    return ofp;
  }
  if (ec) {
    std::cerr << ec.message() << std::endl;
    ec.clear();
    return ofp;
  }
  ofp.emplace(std::ifstream{});
  for (;;) {
    ofp->open(path, std::ios::in | std::ios::binary);
    if (ofp->is_open()) {
      break;
    }
    // TODO might need a better way to open a file than an infinite loop
    // I had issue where it'd fail to open a file sometimes but the
    // next time it'd work fine (╯°□°)╯︵ ┻━┻
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  std::cout << (std::string("Loading: \t\"") + path.string()
                + std::string("\"\n"));
  return ofp;
}
/**
 * asks for a file and then passes the istream to a lambda.
 * @tparam lambdaT
 * @param lambda
 * @param path
 * @return true if open and lambda was executed.
 * @todo test?
 */
template<typename lambdaT>
requires(std::invocable<lambdaT, std::istream &>)
  [[maybe_unused]] static bool from_file(const lambdaT &              lambda,
                                         const std::filesystem::path &path)
{
  auto ofp = open_file(path);
  if (ofp.has_value() && ofp->is_open()) {// check might be redundant.
    lambda(*ofp);
    ofp->close();
    return true;
  }
  return false;
}
template<concepts::is_trivially_copyable_and_default_constructible outputT>
  [[maybe_unused]] static outputT
  from_file(const std::size_t &offset, const std::filesystem::path &path)
{
  outputT output{};
  tl::read::from_file(
    [&offset, &output](std::istream &istream) {
      tl::read::input(&istream).seek(offset).output(output);
    },
    path);
  return output;
}
template<concepts::is_contiguous_and_resizable out_rangeT>
out_rangeT
  entire_file(std::filesystem::path in_path, out_rangeT &&out)
{
  from_file(
    [&out](std::istream &istream) {
      tl::read::input(&istream, true).output_all_remaining(out);
    },
    in_path);
  return out;
}
}// namespace tl::read
#endif// TOOLSLIBRARY_READ_HPP
