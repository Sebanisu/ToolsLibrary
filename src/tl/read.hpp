//
// Created by pcvii on 3/8/2021.
//
#ifndef TOOLSLIBRARY_READ_HPP
#define TOOLSLIBRARY_READ_HPP
#include "tl/input.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <thread>
namespace tl::read {
/**
 * Opens file pointer and returns an optional.
 * @param path
 * @return
 * @todo test?
 */
[[nodiscard]] std::optional<std::ifstream>
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
  for (std::size_t i = 0; i < 600U; ++i) {
    ofp->open(path, std::ios::in | std::ios::binary);
    if (ofp->is_open()) {
      break;
    }
    // TODO might need a better way to open a file than an infinite loop
    // I had issue where it'd fail to open a file sometimes but the
    // next time it'd work fine (╯°□°)╯︵ ┻━┻
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  //  std::clog << (std::string("Loading: \t\"") + path.string()
  //                + std::string("\"\n"));
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
template<tl::concepts::is_invocable<std::istream &> lambdaT>
bool
  from_file(const lambdaT &lambda, const std::filesystem::path &path)
{
  auto ofp = open_file(path);
  if (ofp.has_value() && ofp->is_open()) {// check might be redundant.
    lambda(*ofp);
    ofp->close();
    return true;
  }
  return false;
}
/**
 * Read a value out of a file at offset.
 * @tparam outputT type of value.
 * @param offset from start of file where value is.
 * @param path location on computer where the file is.
 */
template<concepts::is_trivially_copyable_and_default_constructible outputT>
[[nodiscard]] outputT
  from_file(const long &offset, const std::filesystem::path &path)
{
  outputT output{};
  tl::read::from_file(
    [&offset, &output](std::istream &istream) {
      tl::read::input(&istream).seek(offset, std::ios::beg).output(output);
    },
    path);
  return output;
}
/**
 * Read a range of data from the file at the given offset.
 * @tparam out_rangeT type of output range.
 * @param in_path location on computer where the file is.
 * @param out output range.
 */
template<concepts::is_contiguous_and_resizable out_rangeT>
[[nodiscard]] out_rangeT
  entire_file(std::filesystem::path in_path, out_rangeT &&out)
{
  from_file(
    [&out](std::istream &istream) {
      tl::read::input(&istream, true).output_all_remaining(out);
    },
    in_path);
  return std::move(out);//-Wreturn-std-move
  // clang tidy wants to not return with a move. but I got a warning from GCC
  // that this is a copy if not returned by move!
}
}// namespace tl::read
#endif// TOOLSLIBRARY_READ_HPP
