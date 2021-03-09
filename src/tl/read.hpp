//
// Created by pcvii on 3/8/2021.
//
#ifndef TOOLSLIBRARY_READ_HPP
#define TOOLSLIBRARY_READ_HPP
#include <optional>
#include <filesystem>
#include <fstream>
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
[[maybe_unused]] static bool from_file(const lambdaT &lambda,
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
}
#endif// TOOLSLIBRARY_READ_HPP
