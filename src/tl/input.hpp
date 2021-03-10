#ifndef TOOLSLIBRARY_INPUT_HPP
#define TOOLSLIBRARY_INPUT_HPP
#include "concepts.hpp"
#include "utility.hpp"
#include <cassert>
#include <cstring>
#include <istream>
#include <span>
#include <variant>
namespace tl::read {
struct input
{
private:
  /**
   * This span is used when passed a non pointer to span. Then the temporary
   * span is stored here and is pointed to. So we can track the state. So if you
   * read multiple values from this it'll subtract bytes from the front of this
   * span. If in the future we wanted to rewind we'd need another pointer to
   * store the original beginning.
   */
  std::span<const char> m_tmp_span{};
  /**
   * Used for seeking with spans. If we are seeking from begin.
   */
  decltype(std::ranges::data(m_tmp_span)) m_tmp_span_data{};
  /**
   * Pointer to a span of char or pointer to an istream. So we can read from
   * char buffers or files. I'm using pointers here because references can't be
   * stored in a variant.
   */
  std::variant<std::span<const char> *, std::istream *> m_input{};
  /**
   * If true then we check the remaining size vs the size being read each time
   * we read.
   */
  bool m_safe{ false };
  /**
   * memcpy data from in to outvar
   * @param outvar dest
   * @param in src
   * @param bytes number of bytes
   */
  template<concepts::is_trivially_copyable outvarT>
  void
    copy(outvarT *const                 outvar,
         const std::ranges::range auto &in,
         const std::size_t              bytes = sizeof(outvarT)) const
  {
    assert(outvar != nullptr);
    assert(std::ranges::data(in) != nullptr);
    memcpy(outvar, std::ranges::data(in), bytes);
  }
  template<concepts::is_trivially_copyable outvarT>
  void
    output_span_ptr(outvarT &outvar) const
  {
    auto &in = *std::get<0>(m_input);
    copy(&outvar, in);
    in = in.subspan(sizeof(outvar));
  }
  template<concepts::is_trivially_copyable outvarT>
  void
    output_stream(outvarT &outvar) const
  {
    auto &                           in = *std::get<1>(m_input);
    std::array<char, sizeof(outvar)> tmp{};
    in.read(std::ranges::data(tmp), sizeof(outvar));
    copy(&outvar, tmp);
  }
  [[nodiscard]] std::size_t
    get_remaining()
  {
    if (m_input.index() == 0) {
      return tl::utility::get_remaining(*std::get<0>(m_input));
    } else if (m_input.index() == 1) {
      return tl::utility::get_remaining(*std::get<1>(m_input));
    }
    throw;
  }

public:
  explicit input(std::span<const char> *const in_input, bool in_safe = false)
    : m_tmp_span_data(std::ranges::data(*in_input)),
      m_input(in_input),
      m_safe(in_safe)
  {}
  explicit input(std::span<const char> in_input, bool in_safe = false)
    : m_tmp_span(in_input),
      m_tmp_span_data(std::ranges::data(m_tmp_span)),
      m_input(&m_tmp_span),
      m_safe(in_safe)
  {}
  explicit input(std::istream *in_input, bool in_safe = false)
    : m_input(in_input), m_safe(in_safe)
  {}
  template<concepts::is_trivially_copyable outvarT>
  void
    output(outvarT &outvar)
  {
    if (m_safe && sizeof(outvar) > get_remaining()) {
      return;
    }
    if (m_input.index() == 0) {
      output_span_ptr(outvar);
      return;
    } else if (m_input.index() == 1) {
      output_stream(outvar);
      return;
    }
    throw;
  }
  template<concepts::is_trivially_copyable... outvarT>
  requires(sizeof...(outvarT) > 1) void output(outvarT &...outvar)
  {
    (output(outvar), ...);
  }
  template<concepts::is_trivially_copyable_and_default_constructible outvarT>
  outvarT
    output()
  {
    outvarT outvar{};
    output(outvar);
    return outvar;
  }
  template<concepts::is_trivially_copyable_and_default_constructible... outvarT>
  requires(sizeof...(outvarT) > 1) [[nodiscard]] std::tuple<outvarT...> output()
  {
    return std::tuple<outvarT...>{ output<outvarT>()... };
  }
  template<concepts::is_contiguous_and_resizable outvarT>
  void
    output(outvarT &outvar)
  {
    using value_type = std::decay_t<typename outvarT::value_type>;
    std::size_t size = std::ranges::size(outvar) * sizeof(value_type);
    if (m_safe && size > get_remaining()) {
      return;
    }
    if (m_input.index() == 0) {
      auto &in = *std::get<0>(m_input);
      copy(std::ranges::data(outvar), in, size);
      in = in.subspan(size);
      return;
    } else if (m_input.index() == 1) {
      auto &            in = *std::get<1>(m_input);
      std::vector<char> tmp(size);
      in.read(std::ranges::data(tmp), sizeof(outvar));
      copy(std::ranges::data(outvar), tmp, size);
      return;
    }
    throw;
  }
  template<concepts::is_contiguous_and_resizable... outvarT>
  requires(sizeof...(outvarT) > 1) void output(outvarT &...outvar)
  {
    (output(outvar), ...);
  }
  template<concepts::is_contiguous_and_resizable outvarT>
  requires(!std::is_reference_v<outvarT>) [[nodiscard]] outvarT
    output(outvarT &&outvar)
  {
    output(outvar);
    return outvar;
  }
  template<concepts::is_contiguous_and_resizable... outvarT>
  requires((sizeof...(outvarT) > 1U) && (!std::is_reference_v<outvarT> && ...))
    [[nodiscard]] std::tuple<outvarT...> output(outvarT &&...outvar)
  {
    return std::tuple<outvarT...>{ output(std::forward(outvar))... };
  }
  template<concepts::is_contiguous_and_resizable outvarT>
  void
    output_all_remaining(outvarT &outvar)
  {
    using value_type = std::decay_t<typename outvarT::value_type>;
    std::size_t size = get_remaining() / sizeof(value_type);
    outvar.resize(size);
    output(outvar);
  }
  template<concepts::is_contiguous_and_resizable outvarT>
  [[nodiscard]] outvarT
    output_all_remaining(outvarT &&outvar)
  {
    output_all_remaining(outvar);
    return outvar;
  }
  template<concepts::is_contiguous_and_resizable outvarT>
  [[nodiscard]] outvarT
    output(const size_t bytes_size)
  {
    outvarT outvar{};
    using value_type = std::decay_t<typename outvarT::value_type>;
    std::size_t size = bytes_size / sizeof(value_type);
    outvar.resize(size);
    [[maybe_unused]] const auto rem = get_remaining();
    output(outvar);
    return outvar;
  }
  auto &
    seek(const size_t &bytes_size, decltype(std::ios::cur) from)
  {
    assert(m_tmp_span_data != nullptr);
    if (m_input.index() == 0) {
      auto &     in    = *std::get<0>(m_input);
      const auto reset = [&in, this]() {
        const auto offset =
          std::distance(m_tmp_span_data, std::ranges::data(in));
        if (offset > 0U) {
          const auto size = offset + std::ranges::size(in);
          in              = std::span<const char>(m_tmp_span_data, size);
        }
      };
      if (from == std::ios::beg) {
        reset();
        assert(std::ranges::size(in) >= bytes_size);
        in = in.subspan(bytes_size);
      } else if (from == std::ios::cur) {
        assert(std::ranges::size(in) >= bytes_size);
        in = in.subspan(bytes_size);
      } else if (from == std::ios::end) {
        reset();
        assert(std::ranges::size(in) >= bytes_size);
        const auto seek_v = std::ranges::size(in) - bytes_size;
        in                = in.subspan(seek_v);
      } else {
        throw;
      }
      return *this;
    } else if (m_input.index() == 1) {
      auto &in = *std::get<1>(m_input);
      in.seekg(static_cast<long>(bytes_size), from);
      return *this;
    }
    throw;
  }
};
}// namespace tl::read
#endif// TOOLSLIBRARY_INPUT_HPP
