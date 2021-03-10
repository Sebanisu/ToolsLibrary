#include <boost/ut.hpp>// single header
// import boost.ut;        // single module (C++20)
#include "tl/input.hpp"
#include "tl/read.hpp"
#include <algorithm>
#include <sstream>
int
  main()
{
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;
  using namespace boost::ut;
  [[maybe_unused]] suite input = [] {
    static constexpr auto buffer = std::string_view("\x01\x00\x00\x00", 4U);
    static constexpr auto longer_buffer =
      std::string_view("\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00", 12U);
    std::stringstream ss{};
    ss.write(std::ranges::data(longer_buffer),
             std::ranges::size(longer_buffer));
    "confirming I'm filling the ss correctly..."_test = [&ss] {
      expect(12_l == ss.tellp());
      expect(12_ul == std::ranges::size(longer_buffer));
    };
    "read int"_test = [] {
      std::int32_t i{};
      tl::read::input(buffer).output(i);
      expect(1_i == i);
    };
    "read int and shift offset"_test = [] {
      std::span    span = buffer;
      std::int32_t i{};
      tl::read::input(&span).output(i);
      expect(1_i == i);
      expect(std::ranges::size(span) == 0U);
    };
    "read safe int"_test = [] {
      std::int32_t i{};
      tl::read::input(std::span(buffer).subspan(0, 3), true).output(i);
      expect(0_i == i);
    };
    "read safe int and shift offset"_test = [] {
      auto         span = std::span(buffer).subspan(0, 3);
      std::int32_t i{};
      tl::read::input(&span, true).output(i);
      expect(0_i == i);
      expect(std::ranges::size(span) == 3U);
    };
    "variadic read int"_test = [] {
      std::int32_t i{};
      std::int32_t i2{};
      std::int32_t i3{};
      tl::read::input(longer_buffer).output(i, i2, i3);
      expect(1_i == i);
      expect(2_i == i2);
      expect(3_i == i3);
    };
    "variadic read int and shift offset"_test = [] {
      std::span    span(longer_buffer);
      std::int32_t i{};
      std::int32_t i2{};
      std::int32_t i3{};
      tl::read::input(&span).output(i, i2, i3);
      expect(1_i == i);
      expect(2_i == i2);
      expect(3_i == i3);
      expect(std::ranges::size(span) == 0U);
    };
    "variadic safe read int and shift offset"_test = [] {
      auto         span = std::span(longer_buffer).subspan(0, 10U);
      std::int32_t i{};
      std::int32_t i2{};
      std::int32_t i3{};
      tl::read::input(&span, true).output(i, i2, i3);
      expect(1_i == i);
      expect(2_i == i2);
      expect(0_i == i3);
      expect(std::ranges::size(span) == 2U);
    };
    "variadic safe read int"_test = [] {
      auto         span = std::span(longer_buffer).subspan(0, 10U);
      std::int32_t i{};
      std::int32_t i2{};
      std::int32_t i3{};
      tl::read::input(span, true).output(i, i2, i3);
      expect(1_i == i);
      expect(2_i == i2);
      expect(0_i == i3);
      expect(std::ranges::size(span) == 10U);
    };
    "read int from stream"_test = [&ss] {
      std::int32_t i{};
      tl::read::input(&ss).output(i);
      expect(1_i == i);
      expect(4_l == ss.tellg());
      ss.seekg(0, std::ios::beg);
    };
    "read ints from stream"_test = [&ss] {
      std::int32_t i{};
      std::int32_t i2{};
      std::int32_t i3{};
      tl::read::input(&ss).output(i, i2, i3);
      expect(1_i == i);
      expect(2_i == i2);
      expect(3_i == i3);
      expect(12_l == ss.tellg());
      ss.seekg(0, std::ios::beg);
    };
    "safe read ints from stream"_test = [&ss] {
      std::int32_t i{};
      std::int32_t i2{};
      std::int32_t i3{};
      std::int32_t i4{};
      tl::read::input(&ss, true).output(i, i2, i3, i4);
      expect(1_i == i);
      expect(2_i == i2);
      expect(3_i == i3);
      expect(0_i == i4);
      expect(12_l == ss.tellg());
      ss.seekg(0, std::ios::beg);
    };
    "read int and return it"_test = [] {
      const auto i = tl::read::input(buffer).output<std::int32_t>();
      expect(1_i == i);
    };
    "read int and return it and shift offset"_test = [] {
      auto       span = std::span(buffer);
      const auto i    = tl::read::input(&span).output<std::int32_t>();
      expect(1_i == i);
      expect(0_ul == std::ranges::size(span));
    };
    "read ints and return it and shift offset"_test = [] {
      auto span = std::span(longer_buffer);
      const auto [i, i2, i3] =
        tl::read::input(&span)
          .output<std::int32_t, std::int32_t, std::int32_t>();
      expect(1_i == i);
      expect(2_i == i2);
      expect(3_i == i3);
      expect(0_ul == std::ranges::size(span));
    };
    "read ints and return it and don't shift offset"_test = [] {
      auto span = std::span(longer_buffer);
      const auto [i, i2, i3] =
        tl::read::input(span)
          .output<std::int32_t, std::int32_t, std::int32_t>();
      expect(1_i == i);
      expect(2_i == i2);
      expect(3_i == i3);
      expect(12_ul == std::ranges::size(span));
    };
    "safe read int and return it and shift offset"_test = [] {
      auto       span = std::span(buffer);
      const auto i    = tl::read::input(&span, true).output<std::int32_t>();
      expect(1_i == i);
      expect(0_ul == std::ranges::size(span));
    };
    "safe read int and return it and don't shift offset"_test = [] {
      auto       span = std::span(buffer);
      const auto i    = tl::read::input(span, true).output<std::int32_t>();
      expect(1_i == i);
      expect(4_ul == std::ranges::size(span));
    };
    "safe read ints and return it and don't shift offset, expect one to not be wrote to."_test =
      [] {
        auto span = std::span(longer_buffer);
        const auto [i, i2, i3, i4] =
          tl::read::input(span, true)
            .output<std::int32_t, std::int32_t, std::int32_t, std::int32_t>();
        expect(1_i == i);
        expect(2_i == i2);
        expect(3_i == i3);
        expect(0_i == i4);
        expect(12_ul == std::ranges::size(span));
      };
    "safe read ints and return it and shift offset, expect one to not be wrote to."_test =
      [] {
        auto span = std::span(longer_buffer);
        const auto [i, i2, i3, i4] =
          tl::read::input(&span, true)
            .output<std::int32_t, std::int32_t, std::int32_t, std::int32_t>();
        expect(1_i == i);
        expect(2_i == i2);
        expect(3_i == i3);
        expect(0_i == i4);
        expect(0_ul == std::ranges::size(span));
      };
    "read ints from stream"_test = [&ss] {
      const auto [i, i2, i3] =
        tl::read::input(&ss).output<std::int32_t, std::int32_t, std::int32_t>();
      expect(1_i == i);
      expect(2_i == i2);
      expect(3_i == i3);
      expect(12_l == ss.tellg());
      ss.seekg(0, std::ios::beg);
    };
    "safe read ints from stream"_test = [&ss] {
      const auto [i, i2, i3, i4] =
        tl::read::input(&ss, true)
          .output<std::int32_t, std::int32_t, std::int32_t, std::int32_t>();
      expect(1_i == i);
      expect(2_i == i2);
      expect(3_i == i3);
      expect(0_i == i4);
      expect(12_l == ss.tellg());
      ss.seekg(0, std::ios::beg);
    };
    "read array of char"_test = [] {
      std::array<char, 4> i{};
      tl::read::input(buffer).output(i);
      expect(std::ranges::equal(buffer, i));
    };
    "read array of char return"_test = [] {
      auto i = tl::read::input(buffer).output<std::array<char, 4>>();
      expect(std::ranges::equal(buffer, i));
    };
    "safe read array of char"_test = [] {
      std::array<char, 4> i{};
      tl::read::input(buffer, true).output(i);
      expect(std::ranges::equal(buffer, i));
    };
    "safe read array of char return"_test = [] {
      auto i = tl::read::input(buffer, true).output<std::array<char, 4>>();
      expect(std::ranges::equal(buffer, i));
    };
    "read array of char and shift offset"_test = [] {
      auto                span = std::span(buffer);
      std::array<char, 4> i{};
      tl::read::input(&span).output(i);
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
    "read array of char return and shift offset"_test = [] {
      auto span = std::span(buffer);
      auto i    = tl::read::input(&span).output<std::array<char, 4>>();
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
    "safe read array of char and shift offset"_test = [] {
      auto                span = std::span(buffer);
      std::array<char, 4> i{};
      tl::read::input(&span, true).output(i);
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
    "safe read array of char return and shift offset"_test = [] {
      auto span = std::span(buffer);
      auto i    = tl::read::input(&span, true).output<std::array<char, 4>>();
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
    "read array of char"_test = [&ss] {
      std::array<char, 4> i{};
      tl::read::input(&ss).output(i);
      expect(std::ranges::equal(ss.str().substr(0, 4), i));
      expect(4_l == ss.tellg());
      ss.seekg(0, std::ios::beg);
    };
    "read array of char return"_test = [&ss] {
      auto i = tl::read::input(&ss).output<std::array<char, 4>>();
      expect(std::ranges::equal(ss.str().substr(0, 4), i));
      expect(4_l == ss.tellg());
      ss.seekg(0, std::ios::beg);
    };
    "safe read array of char"_test = [&ss] {
      std::array<char, 4> i{};
      tl::read::input(&ss, true).output(i);
      expect(std::ranges::equal(ss.str().substr(0, 4), i));
      expect(4_l == ss.tellg());
      ss.seekg(0, std::ios::beg);
    };
    "safe read array of char return"_test = [&ss] {
      auto i = tl::read::input(&ss, true).output<std::array<char, 4>>();
      expect(std::ranges::equal(ss.str().substr(0, 4), i));
      expect(4_l == ss.tellg());
      ss.seekg(0, std::ios::beg);
    };
    "read vector of char"_test = [] {
      std::vector<char> i(4);
      tl::read::input(buffer).output(i);
      expect(std::ranges::equal(buffer, i));
    };
    "read vector of char and shift offset"_test = [] {
      auto              span = std::span(buffer);
      std::vector<char> i(4);
      tl::read::input(&span).output(i);
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
    "safe read vector of char"_test = [] {
      std::vector<char> i(4);
      tl::read::input(buffer, true).output(i);
      expect(std::ranges::equal(buffer, i));
    };
    "safe read vector of char and shift offset"_test = [] {
      auto              span = std::span(buffer);
      std::vector<char> i(4);
      tl::read::input(&span, true).output(i);
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
    "read vector of char return"_test = [] {
      auto i = tl::read::input(buffer).output(std::vector<char>(4U));
      expect(std::ranges::equal(buffer, i));
    };
    "read vector of char return and shift offset"_test = [] {
      std::span span = buffer;
      auto      i    = tl::read::input(&span).output(std::vector<char>(4U));
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
    "read vector of char return"_test = [] {
      auto i = tl::read::input(buffer).output(std::vector<char>(4U));
      expect(std::ranges::equal(buffer, i));
    };
    "read vector of char return and shift offset"_test = [] {
      std::span span = buffer;
      auto      i    = tl::read::input(&span).output(std::vector<char>(4U));
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
    "read string of char return"_test = [] {
      auto i = tl::read::input(buffer).output(std::string(4U, '\0'));
      expect(std::ranges::equal(buffer, i));
    };
    "read string of char return and shift offset"_test = [] {
      std::span span = buffer;
      auto      i    = tl::read::input(&span).output(std::string(4U, '\0'));
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
    "read string of char return"_test = [] {
      auto i = tl::read::input(buffer).output(std::string(4U, '\0'));
      expect(std::ranges::equal(buffer, i));
    };
    "read string of char return and shift offset"_test = [] {
      std::span span = buffer;
      auto      i    = tl::read::input(&span).output(std::string(4U, '\0'));
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
    "read string of char"_test = [] {
      std::string i(4U, '\0');
      tl::read::input(buffer).output(i);
      expect(std::ranges::equal(buffer, i));
    };
    "read string of char and shift offset"_test = [] {
      auto        span = std::span(buffer);
      std::string i(4U, '0');
      tl::read::input(&span).output(i);
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
    "safe read string of char"_test = [] {
      std::string i(4U, '0');
      tl::read::input(buffer, true).output(i);
      expect(std::ranges::equal(buffer, i));
    };
    "safe read string of char and shift offset"_test = [] {
      auto        span = std::span(buffer);
      std::string i(4U, '0');
      tl::read::input(&span, true).output(i);
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
    "output all remaining via return"_test = [] {
      const auto v = tl::read::input(buffer).template output_all_remaining(
        std::vector<char>());
      expect(std::ranges::equal(buffer, v));
    };
    "output all remaining"_test = [] {
      std::vector<char> v{};
      tl::read::input(buffer).output_all_remaining(v);
      expect(std::ranges::equal(buffer, v));
    };
    "output bytes via return"_test = [] {
      const auto v =
        tl::read::input(longer_buffer).template output<std::vector<char>>(4);
      expect(std::ranges::equal(buffer.substr(0, 4), v));
    };
    "seek"_test = [] {
      const auto v = tl::read::input(longer_buffer)
                       .seek(4U, std::ios::cur)
                       .template output<std::vector<char>>(4);
      expect(std::ranges::equal(longer_buffer.substr(4, 4), v));
    };
    "seek from end"_test = [] {
      auto input = tl::read::input(longer_buffer);
      input.seek(4U, std::ios::end);
      const auto v = input.template output<std::vector<char>>(4);
      const auto substr = longer_buffer.substr(8,4);
      expect(std::ranges::equal(substr, v));
    };
    "seek from begin"_test = [] {
      auto input = tl::read::input(longer_buffer);
      input.seek(6U, std::ios::beg);
      const auto v = input.template output<std::vector<char>>(4);
      const auto substr = longer_buffer.substr(6,4);
      expect(std::ranges::equal(substr, v));
    };
  };
}