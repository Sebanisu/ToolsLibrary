#ifndef TOOLSLIBRARY_READ_HPP
#define TOOLSLIBRARY_READ_HPP
#include "concepts.hpp"
#include <cstdint>
#include <cstring>
#include <istream>
#include <ranges>
#include <span>
/**
 * @todo if memcpy is replaced with std::bitcast these can be constexpr. But we
 * lack compiler support right now.
 */
namespace tl::read {
/**
 * Read Value
 * @tparam outputT trivially_copyable, need to take sizeof(outputT)
 * @param input source of data.
 * @param output where the input is wrote to.
 */
template<concepts::is_trivially_copyable outputT>
static void
  value(std::span<const char> input, outputT &output)
{
  std::memcpy(&output, std::ranges::data(input), sizeof(outputT));
}
/**
 * Read Value
 * @tparam outputT trivially_copyable, need to take sizeof(outputT)
 * @param input source of data.
 * @param output where the input is wrote to.
 * @note modifies input to offset of bytes read.
 */
template<concepts::is_trivially_copyable outputT>
static void
  value(std::span<const char> *const input, outputT &output)
{
  value(*input, output);
  *input = input->subspan(sizeof(outputT));
} /**
   * Read Value
   * @tparam outputT trivially_copyable, need to take sizeof(outputT)
   * @param input source of data.
   * @param output where the input is wrote to.
   * @note modifies input to offset of bytes read.
   */
template<concepts::is_trivially_copyable... outputT>
requires(sizeof...(outputT)
         > 1U) static void value(std::span<const char> *const input,
                                 outputT &...output)
{
  (value(input, output), ...);
}
/**
 * Read Value
 * @tparam outputT trivially_copyable, need to take sizeof(outputT)
 * @param input source of data.
 * @param output where the input is wrote to.
 */
template<concepts::is_trivially_copyable... outputT>
requires(sizeof...(outputT) > 1U) static void value(std::span<const char> input,
                                                    outputT &...output)
{
  value(&input, output...);
}
/**
 *
 * @tparam outputT trivially_copyable, need to take sizeof(outputT)
 * @param input stream of data.
 * @param output where the input is wrote to.
 */
template<concepts::is_trivially_copyable... outputT>
static void
  value(std::istream &input, outputT &...output)
{
  constexpr auto         size = (sizeof(outputT) + ...);
  std::array<char, size> tmp{};
  input.read(std::ranges::data(tmp), size);
  value(tmp, output...);
}
/**
 * Read one value
 * @tparam outputT trivially_copyable and default constructable, need to take
 * sizeof(outputT)
 * @param input input source of data.
 * @return value
 */
template<concepts::is_trivially_copyable_and_default_constructible outputT>
static outputT
  value(std::span<const char> input)
{
  auto output = outputT{};
  value(input, output);
  return output;
}
/**
 * Read one value
 * @tparam outputT trivially_copyable and default constructable, need to take
 * sizeof(outputT)
 * @param input input source of data.
 * @return value
 * @note modifies input to offset of bytes read.
 */
template<concepts::is_trivially_copyable_and_default_constructible outputT>
static outputT
  value(std::span<const char> *const input)
{
  auto output = value<outputT>(*input);
  *input      = input->subspan(sizeof(outputT));
  return output;
}
/**
 * Read all values
 * @tparam outputT trivially_copyable and default constructable, need to take
 * sizeof(outputT)
 * @param input source of data.
 * @return tuple of values.
 * @note modifies input to offset of bytes read.
 */
template<concepts::is_trivially_copyable_and_default_constructible... outputT>
requires(sizeof...(outputT)
         > 1) static auto value(std::span<const char> *const input)
{
  return std::tuple<outputT...>{ value<outputT>(input)... };
}
/**
 * Read all values
 * @tparam outputT trivially_copyable and default constructable, need to take
 * sizeof(outputT)
 * @param input source of data.
 * @return tuple of values.
 */
template<concepts::is_trivially_copyable_and_default_constructible... outputT>
requires(sizeof...(outputT) > 1) static auto value(std::span<const char> input)
{
  return value<outputT...>(&input);
}
/**
 * Read one value
 * @tparam outputT trivially_copyable and default constructable, need to take
 * sizeof(outputT)
 * @param input stream of data.
 * @return value
 */
template<concepts::is_trivially_copyable_and_default_constructible outputT>
static outputT
  value(std::istream &input)
{
  auto output = outputT{};
  value(input, output);
  return output;
}
/**
 * Read all values
 * @tparam outputT trivially_copyable and default constructable, need to take
 * sizeof(outputT)
 * @param input stream of data.
 * @return tuple of values.
 */
template<concepts::is_trivially_copyable_and_default_constructible... outputT>
requires(sizeof...(outputT) > 1) static auto value(std::istream &input)
{
  return std::tuple<outputT...>{ value<outputT>(input)... };
}
namespace safe {
  /**
   * Safe Read Value it checks the size of the span before hand.
   * @tparam outputT trivially_copyable, need to take sizeof(outputT)
   * @tparam inputT type of input, using requires below to test if valid to pass
   * to read::value
   * @param input source of data.
   * @param output where the input is wrote to.
   * @note doesn't throw any errors just doesn't do anything if lacking space
   */
  template<concepts::is_trivially_copyable outputT, typename inputT>
  requires requires(inputT i, outputT o)
  {
    read::value(i, o);
  }
  static void
    value(inputT input, outputT &output)
  {
    std::size_t size = [&input, &output] {
      if constexpr (std::is_pointer_v<inputT>) {
        return std::ranges::size(*input);
      } else {
        return std::ranges::size(input);
      }
    }();
    if (sizeof(outputT) <= size) {
      read::value(input, output);
    }
  }
  /**
   * Safe Read Value it checks the size of the span before hand.
   * @tparam outputT trivially_copyable, need to take sizeof(outputT)
   * @tparam inputT
   * @param input source of data.
   * @param output where the input is wrote to.
   * @note reads values till it runs out of input
   */
  template<concepts::is_trivially_copyable... outputT, typename inputT>
  requires(sizeof...(outputT) > 1U && requires(inputT i, outputT... o) {
    (value(i, o), ...);
  }) static void value(inputT input, outputT &...output)
  {
    if constexpr (std::is_pointer_v<inputT>) {
      (value(input, output), ...);
    } else {
      (value(&input, output), ...);
    }
  }
  /**
   * get size of remaining istream, from current point.
   * @param input stream of data.
   */
  static auto
    sizeof_stream(std::istream &input)
  {
    const auto current = input.tellg();
    input.seekg(0, std::ios::end);
    const auto end = input.tellg();
    input.seekg(current, std::ios::beg);
    return static_cast<std::size_t>(end - current);
  }
  /**
   * Read all these values from stream
   * @tparam outputT trivially_copyable, need to take sizeof(outputT)
   * @param input stream of data.
   * @param output where the input is wrote to.
   */
  template<concepts::is_trivially_copyable... outputT>
  static void
    value(std::istream &input, outputT &...output)
  {
    auto       remaining = sizeof_stream(input);
    const auto lambda    = [&input, &remaining](auto &one_item) {
      if (remaining >= sizeof(one_item)) {
        read::value(input, one_item);
        remaining -= sizeof(one_item);
      }
    };
    (lambda(output), ...);
  }
  /**
   * Read all these values from stream
   * @tparam outputT trivially_copyable and default constructable, need to take
   * sizeof(outputT)
   * @param input source of data.
   * @return value if only 1 or a tuple of values if more.
   * @note modifies input to offset of bytes read.
   */
  template<concepts::is_trivially_copyable_and_default_constructible... outputT>
  static auto
    value(std::span<const char> *const input)
  {
    auto       remaining = std::ranges::size(*input);
    const auto lambda    = [&input, &remaining]<typename oneT>() {
      if (remaining >= sizeof(oneT)) {
        remaining -= sizeof(oneT);
        return read::value<oneT>(input);
      }
      return oneT{};
    };
    if constexpr (sizeof...(outputT) > 1) {
      return std::tuple<outputT...>{ lambda.template operator()<outputT>()... };
    } else if constexpr (sizeof...(outputT) == 1) {
      return lambda.template operator()<outputT...>();
    }
  }
  /**
   * Read all these values from stream
   * @tparam outputT trivially_copyable and default constructable, need to take
   * sizeof(outputT)
   * @param input source of data.
   * @return value if only 1 or a tuple of values if more.
   */
  template<concepts::is_trivially_copyable_and_default_constructible... outputT>
  static auto
    value(std::span<const char> input)
  {
    return value<outputT...>(&input);
  }
  /**
   * Read all these values from stream
   * @tparam outputT trivially_copyable and default constructable, need to take
   * sizeof(outputT)
   * @param input source of data.
   * @return value if only 1 or a tuple of values if more.
   * @note modifies input to offset of bytes read.
   */
  template<concepts::is_trivially_copyable_and_default_constructible... outputT>
  static auto
    value(std::istream &input)
  {
    auto       remaining = sizeof_stream(input);
    const auto lambda    = [&input, &remaining]<typename oneT>() {
      if (remaining >= sizeof(oneT)) {
        remaining -= sizeof(oneT);
        return read::value<oneT>(input);
      }
      return oneT{};
    };
    if constexpr (sizeof...(outputT) > 1) {
      return std::tuple<outputT...>{ lambda.template operator()<outputT>()... };
    } else if constexpr (sizeof...(outputT) == 1) {
      return lambda.template operator()<outputT...>();
    }
  }
}// namespace safe
}// namespace tl::read
#endif// TOOLSLIBRARY_READ_HPP
