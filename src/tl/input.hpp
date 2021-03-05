#ifndef TOOLSLIBRARY_INPUT_HPP
#define TOOLSLIBRARY_INPUT_HPP
#include "concepts.hpp"
#include <cstring>
#include <istream>
#include <span>
#include <variant>
namespace tl::read {
struct input
{
private:
  std::span<const char>                                 m_tmp_span{};
  std::variant<std::span<const char> *, std::istream *> m_input{};
  //  bool
  //    execute(const auto &lambda)
  //  {
  //    switch (m_input.index()) {
  //    case 0:
  //      return lambda(std::get<0>(m_input));
  //    case 1:
  //      return lambda(*std::get<1>(m_input));
  //    case 3:
  //      return lambda(*std::get<2>(m_input));
  //    }
  //  }
  template<concepts::is_trivially_copyable outputT>
  void
    copy(outputT *const                 output,
         const std::ranges::range auto &in,
         const std::size_t              bytes = sizeof(outputT)) const
  {
    memcpy(output, std::ranges::data(in), bytes);
  }
  template<concepts::is_trivially_copyable outputT>
  void
    read_span_ptr(outputT &output) const
  {
    auto &in = *std::get<0>(m_input);
    copy(&output, in);
    in = in.subspan(sizeof(output));
  }
  template<concepts::is_trivially_copyable outputT>
  void
    read_stream(outputT &output) const
  {
    auto &                           in = *std::get<1>(m_input);
    std::array<char, sizeof(output)> tmp{};
    in.read(tmp, sizeof(output));
    copy(&output, tmp);
  }

public:
  explicit input(std::span<const char> in_input)
    : m_tmp_span(in_input), m_input(&m_tmp_span)
  {}
  explicit input(std::span<const char> *const in_input) : m_input(in_input) {}
  explicit input(std::istream *in_input) : m_input(in_input) {}
  template<concepts::is_trivially_copyable outputT>
  void
    read(outputT &output)
  {
    if (m_input.index() == 1) {
      read_span_ptr(output);
    } else if (m_input.index() == 2) {
      read_stream(output);
    }
    throw;
  }
  template<concepts::is_trivially_copyable... outputT>
  requires(sizeof...(outputT) > 1) void read(outputT &...output)
  {
    (read(output), ...);
  }
  template<concepts::is_trivially_copyable_and_default_constructible outputT>
  outputT
    read()
  {
    outputT output{};
    read(output);
    return output;
  }
  template<concepts::is_trivially_copyable_and_default_constructible... outputT>
  requires(sizeof...(outputT) > 1) std::tuple<outputT...> read()
  {
    return std::tuple<outputT...>(std::in_place(read<outputT>()...));
  }
  template<concepts::is_contiguous_and_resizable outputT>
  void
    read(outputT &output)
  {
    using value_type = std::decay_t<typename outputT::value_type>;
    std::size_t size = std::ranges::size(output) * sizeof(value_type);
    if (m_input.index() == 1) {
      auto &in = *std::get<0>(m_input);
      copy(std::ranges::data(output), in, size);
      in = in.subspan(size);
    } else if (m_input.index() == 2) {
      auto &            in = *std::get<1>(m_input);
      std::vector<char> tmp(size);
      in.read(std::ranges::data(tmp), sizeof(output));
      copy(std::ranges::data(output), tmp, size);
    }
    throw;
  }
  template<concepts::is_contiguous_and_resizable... outputT>
  requires(sizeof...(outputT) > 1) void read(outputT &...output)
  {
    (read(output), ...);
  }
  //  template<concepts::is_contiguous_and_resizable outputT>
  //  outputT
  //  read()
  //  {
  //    outputT output{};
  //    read(output);
  //    return output;
  //  }
  //  template<concepts::is_contiguous_and_resizable... outputT>
  //  requires(sizeof...(outputT) > 1) std::tuple<outputT...> read()
  //  {
  //    return std::tuple<outputT...>(std::in_place(read<outputT>()...));
  //  }
};
}// namespace tl::read
#endif// TOOLSLIBRARY_INPUT_HPP
