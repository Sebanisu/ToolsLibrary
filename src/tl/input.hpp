#ifndef TOOLSLIBRARY_INPUT_HPP
#define TOOLSLIBRARY_INPUT_HPP
#include "tl/algorithm.hpp"
#include "tl/concepts.hpp"
#include "tl/utility.hpp"
#include <cassert>
#include <cstdint>
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
  mutable std::span<const char> m_tmp_span{};
  /**
   * Used for seeking with spans. If we are seeking from begin.
   */
  decltype(std::data(m_tmp_span)) m_tmp_span_data{};
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
    copy(outvarT *const    outvar,
         const char *const in,
         const std::size_t bytes = sizeof(outvarT)) const
  {
    assert(outvar != nullptr);
    assert(in != nullptr);
    memcpy(outvar, in, bytes);
  }
  template<concepts::is_trivially_copyable outvarT>
  void
    output_span_ptr(outvarT &outvar) const
  {
    auto &in = *std::get<0>(m_input);
    if (m_safe && sizeof(outvarT) > get_remaining())
      return;
    copy(&outvar, std::data(in));
    in = in.subspan(sizeof(outvar));
  }
  template<concepts::is_trivially_copyable outvarT>
  void
    output_stream(outvarT &outvar) const
  {
    auto &in = *std::get<1>(m_input);
    if (m_safe && sizeof(outvarT) > get_remaining())
      return;
    std::array<char, sizeof(outvar)> tmp{};
    in.read(std::data(tmp), sizeof(outvar));
    copy(&outvar, std::data(tmp));
  }
  [[nodiscard]] std::size_t
    get_remaining() const
  {
    if (m_input.index() == 0) {
      return tl::utility::get_remaining(*std::get<0>(m_input));
    } else if (m_input.index() == 1) {
      return tl::utility::get_remaining(*std::get<1>(m_input));
    }
    throw;
  }
  template<typename signedT>
  void
    throw_if_seek_out_of_range(const signedT &bytes_size) const
  {
    if (m_safe
        && ((bytes_size > 0)
            && (get_remaining() < static_cast<std::size_t>(bytes_size)))
        && ((bytes_size < 0)
            && (std::distance(m_tmp_span_data,
                              std::data(*std::get<0>(m_input))))
                 < [](auto i) {
                     return i < 0 ? -i : i;
                   }(bytes_size))) {
      throw;
    }
  }
  template<typename signedT>
  const input &
    seek_span(const signedT &               bytes_size,
              const std::ios_base::seekdir &from) const
  {
    assert(m_tmp_span_data != nullptr);
    auto &     in    = *std::get<0>(m_input);
    const auto reset = [&in, this]() {
      const auto offset =
        static_cast<std::size_t>(std::distance(m_tmp_span_data, std::data(in)));
      if (offset > 0U) {
        const auto size = offset + std::size(in);
        in              = std::span<const char>(m_tmp_span_data, size);
      }
    };
    if (from == std::ios::beg) {
      assert(bytes_size >= 0);
      reset();
      throw_if_seek_out_of_range(bytes_size);
      in = in.subspan(static_cast<std::size_t>(bytes_size));
    } else if (from == std::ios::cur) {
      if (bytes_size > 0) {
        throw_if_seek_out_of_range(bytes_size);
        in = in.subspan(static_cast<std::size_t>(bytes_size));
      } else if (bytes_size < 0) {
        const auto offset =
          std::distance(m_tmp_span_data, std::data(in)) + bytes_size;
        return seek(offset, std::ios::beg);
      }
    } else if (from == std::ios::end) {
      assert(bytes_size < 0);
      reset();
      throw_if_seek_out_of_range(bytes_size);
      const std::size_t seek_v =
        std::size(in) - static_cast<std::size_t>([](auto i) {
          return i < 0 ? -i : i;
        }(bytes_size));
      in = in.subspan(seek_v);
    } else {
      throw;
    }
    return *this;
  }
  template<typename signedT>
  const input &
    seek_istream(const signedT &               bytes_size,
                 const std::ios_base::seekdir &from) const
  {
    auto &in = *std::get<1>(m_input);
    assert(!((from == std::ios::end && bytes_size > 0)
             || (from == std::ios::beg && bytes_size < 0)));
    if (from == std::ios::beg) {
      in.clear();// If you read to end fail bit gets set. This will unset it
      // when you seek from begin.
    }
    in.seekg(bytes_size, from);
    return *this;
  }
  template<concepts::is_nontrivial_continuous_range outvarT>
  void
    output_span(outvarT &outvar, size_t size) const
  {
    auto &in = *std::get<0>(m_input);
    if (size == 0 || (m_safe && size > get_remaining()))
      return;
    copy(std::data(outvar), std::data(in), size);
    in = in.subspan(size);
  }
  template<concepts::is_nontrivial_continuous_range outvarT>
  void
    output_istream(outvarT &outvar, size_t size) const
  {
    auto &in = *std::get<1>(m_input);
    if (size == 0 || (m_safe && size > get_remaining()))
      return;
    std::vector<char> tmp(size);
    in.read(std::data(tmp), static_cast<long>(size));
    copy(std::data(outvar), std::data(tmp), size);
  }

  void
    get_line_span(std::string &return_value) const
  {
    const auto f             = tl::algorithm::find(*std::get<0>(m_input), '\n');
    const auto b             = std::begin(*std::get<0>(m_input));
    const auto e             = std::end(*std::get<0>(m_input));
    const auto get_line_span = [&return_value, this](const auto &first,
                                                     const auto &last,
                                                     const int   offset = 0) {
      return_value.insert(std::begin(return_value), first, last);
      seek(std::distance(first, last) + offset, std::ios::cur);
    };
    if (f != e) {
      get_line_span(b, f, 1);
    } else if (b != e) {
      get_line_span(b, e);
    }
  }

public:
  explicit input(std::span<const char> *const in_input, bool in_safe = false)
    : m_tmp_span_data(std::data(*in_input)), m_input(in_input), m_safe(in_safe)
  {}
  explicit input(std::span<const char> in_input, bool in_safe = false)
    : m_tmp_span(in_input),
      m_tmp_span_data(std::data(m_tmp_span)),
      m_input(&m_tmp_span),
      m_safe(in_safe)
  {}
  explicit input(std::istream *in_input, bool in_safe = false)
    : m_input(in_input), m_safe(in_safe)
  {}
  template<concepts::is_trivially_copyable outvarT>
  void
    output(outvarT &outvar) const
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
  template<concepts::is_trivially_copyable_and_default_constructible outvarT>
  outvarT
    output() const
  {
    outvarT outvar{};
    output(outvar);
    return outvar;
  }
  template<concepts::is_trivially_copyable_and_default_constructible... outvarT>
  requires(concepts::size_greater_than_1<outvarT...>)
    [[nodiscard]] std::tuple<outvarT...> output() const
  {
    std::tuple<outvarT...> tmp{};
    std::apply(
      [this](outvarT &...out) {
        output<outvarT...>(out...);
      },
      tmp);
    return tmp;
  }
  template<typename... outvarT>
  requires(concepts::size_greater_than_1<outvarT...>
           && (concepts::is_trivial_or_continuous_range<outvarT>, ...))
    [[nodiscard]] std::tuple<outvarT...> output(outvarT &&...constructed) const
  {
    std::tuple<outvarT...> tmp{ std::forward(constructed)... };
    std::apply(
      [this](outvarT &...out) {
        output<outvarT...>(out...);
      },
      tmp);
    return tmp;
  }
  template<concepts::is_nontrivial_continuous_range outvarT>
  void
    output(outvarT &outvar) const
  {
    using value_type = std::decay_t<typename outvarT::value_type>;
    std::size_t size = std::size(outvar) * sizeof(value_type);
    if (m_safe && size > get_remaining()) {
      return;
    }
    if (m_input.index() == 0) {
      output_span(outvar, size);
      return;
    } else if (m_input.index() == 1) {
      output_istream(outvar, size);
      return;
    }
    throw;
  }
  template<typename... outvarT>
  requires(concepts::size_greater_than_1<outvarT...> && (concepts::is_trivial_or_continuous_range<outvarT>,
               ...)) void output(outvarT &...outvar) const
  {
    (output(outvar), ...);
  }
  template<concepts::is_nontrivial_continuous_range outvarT>
  requires(!std::is_reference_v<outvarT>) [[nodiscard]] outvarT
    output(outvarT &&outvar) const
  {
    output(outvar);
    return std::move(outvar);
  }
  //  template<concepts::is_nontrivial_continuous_range... outvarT>
  //  requires(concepts::is_greater_than_1<outvarT...> && (!std::is_reference_v<outvarT> &&
  //  ...))
  //    [[nodiscard]] std::tuple<outvarT...> output(outvarT &&...outvar) const
  //  {
  //    return std::tuple<outvarT...>{ output(std::forward(outvar))... };
  //  }
  template<concepts::is_contiguous_and_resizable outvarT>
  void
    output_all_remaining(outvarT &outvar) const
  {
    using value_type = std::decay_t<typename outvarT::value_type>;
    std::size_t size = get_remaining() / sizeof(value_type);
    outvar.resize(size);
    output(outvar);
  }
  template<concepts::is_nontrivial_continuous_range outvarT>
  [[nodiscard]] outvarT
    output_all_remaining(outvarT &&outvar) const
  {
    output_all_remaining(outvar);
    return std::move(outvar);
  }
  template<concepts::is_contiguous_and_resizable outvarT>
  [[nodiscard]] outvarT
    output(const size_t bytes_size) const
  {
    outvarT outvar{};
    using value_type = std::decay_t<typename outvarT::value_type>;
    std::size_t size = bytes_size / sizeof(value_type);
    outvar.resize(size);
    //  const auto rem = get_remaining();
    output(outvar);
    return outvar;
  }
  template<tl::concepts::is_signed_integral signedT>
  const input &
    seek(const signedT &bytes_size, const std::ios_base::seekdir &from) const
  {
    if (bytes_size == 0 && from == std::ios::cur) {
      return *this;
    }
    if (m_input.index() == 0) {
      return seek_span(bytes_size, from);
    } else if (m_input.index() == 1) {
      return seek_istream(bytes_size, from);
    }
    throw;
  }
  [[nodiscard]] auto
    tell() const
  {
    if (m_input.index() == 0) {
      return static_cast<std::size_t>(
        std::distance(m_tmp_span_data, std::data(*std::get<0>(m_input))));
    } else if (m_input.index() == 1) {
      if (!std::get<1>(m_input)->good()) {
        std::get<1>(m_input)->clear();
        std::get<1>(m_input)->seekg(0, std::ios::end);
      }
      return static_cast<std::size_t>(std::get<1>(m_input)->tellg());
    }
    throw;
  };
  [[nodiscard]] std::string
    get_line() const
  {
    std::string return_value{};
    if (m_input.index() == 0) {
      get_line_span(return_value);
    } else if (m_input.index() == 1) {
      std::getline(*std::get<1>(m_input), return_value);
    }
    return return_value;
  }
};
}// namespace tl::read
#endif// TOOLSLIBRARY_INPUT_HPP