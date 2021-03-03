#include <boost/ut.hpp>// single header
// import boost.ut;        // single module (C++20)
#include "tl/read.hpp"
#include <algorithm>
#include <sstream>
int
  main()
{
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;
  using namespace boost::ut;
  [[maybe_unused]] suite read = [] {
    constexpr auto buffer = std::string_view("\x01\x00\x00\x00", 4U);
    constexpr auto longer_buffer =
      std::string_view("\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00", 12U);
    std::stringstream ss{};
    ss.write(std::ranges::data(longer_buffer),
             std::ranges::size(longer_buffer));
    "confirming I'm filling the ss correctly..."_test = [&ss, &longer_buffer] {
      expect(12_l == ss.tellp());
      expect(12_ul == std::ranges::size(longer_buffer));
    };
    "read int"_test = [&buffer] {
      std::int32_t i{};
      tl::read::value(buffer, i);
      expect(1_i == i);
    };
    "read int and shift offset"_test = [&buffer] {
      std::span    span = buffer;
      std::int32_t i{};
      tl::read::value(&span, i);
      expect(1_i == i);
      expect(std::ranges::size(span) == 0U);
    };
    "read safe int"_test = [&buffer] {
      std::int32_t i{};
      tl::read::safe::value(std::span(buffer).subspan(0, 3), i);
      expect(0_i == i);
    };
    "read safe int and shift offset"_test = [&buffer] {
      auto         span = std::span(buffer).subspan(0, 3);
      std::int32_t i{};
      tl::read::safe::value(&span, i);
      expect(0_i == i);
      expect(std::ranges::size(span) == 3U);
    };
    "variadic read int"_test = [&longer_buffer] {
      std::int32_t i{};
      std::int32_t i2{};
      std::int32_t i3{};
      tl::read::value(longer_buffer, i, i2, i3);
      expect(1_i == i);
      expect(2_i == i2);
      expect(3_i == i3);
    };
    "variadic read int and shift offset"_test = [&longer_buffer] {
      std::span    span(longer_buffer);
      std::int32_t i{};
      std::int32_t i2{};
      std::int32_t i3{};
      tl::read::value(&span, i, i2, i3);
      expect(1_i == i);
      expect(2_i == i2);
      expect(3_i == i3);
      expect(std::ranges::size(span) == 0U);
    };
    "variadic safe read int and shift offset"_test = [&longer_buffer] {
      auto         span = std::span(longer_buffer).subspan(0, 10U);
      std::int32_t i{};
      std::int32_t i2{};
      std::int32_t i3{};
      tl::read::safe::value(&span, i, i2, i3);
      expect(1_i == i);
      expect(2_i == i2);
      expect(0_i == i3);
      expect(std::ranges::size(span) == 2U);
    };
    "variadic safe read int"_test = [&longer_buffer] {
      auto         span = std::span(longer_buffer).subspan(0, 10U);
      std::int32_t i{};
      std::int32_t i2{};
      std::int32_t i3{};
      tl::read::safe::value(span, i, i2, i3);
      expect(1_i == i);
      expect(2_i == i2);
      expect(0_i == i3);
      expect(std::ranges::size(span) == 10U);
    };
    "read int from stream"_test = [&ss] {
      std::int32_t i{};
      tl::read::value(ss, i);
      expect(1_i == i);
      expect(4_l == ss.tellg());
      ss.seekg(0, std::ios::beg);
    };
    "read ints from stream"_test = [&ss] {
      std::int32_t i{};
      std::int32_t i2{};
      std::int32_t i3{};
      tl::read::value(ss, i, i2, i3);
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
      tl::read::safe::value(ss, i, i2, i3, i4);
      expect(1_i == i);
      expect(2_i == i2);
      expect(3_i == i3);
      expect(0_i == i4);
      expect(12_l == ss.tellg());
      ss.seekg(0, std::ios::beg);
    };
    "read int and return it"_test = [&buffer] {
      const auto i = tl::read::value<std::int32_t>(buffer);
      expect(1_i == i);
    };
    "read int and return it and shift offset"_test = [&buffer] {
      auto       span = std::span(buffer);
      const auto i    = tl::read::value<std::int32_t>(&span);
      expect(1_i == i);
      expect(0_ul == std::ranges::size(span));
    };
    "read ints and return it and shift offset"_test = [&longer_buffer] {
      auto span = std::span(longer_buffer);
      const auto [i, i2, i3] =
        tl::read::value<std::int32_t, std::int32_t, std::int32_t>(&span);
      expect(1_i == i);
      expect(2_i == i2);
      expect(3_i == i3);
      expect(0_ul == std::ranges::size(span));
    };
    "read ints and return it and don't shift offset"_test = [&longer_buffer] {
      auto span = std::span(longer_buffer);
      const auto [i, i2, i3] =
        tl::read::value<std::int32_t, std::int32_t, std::int32_t>(span);
      expect(1_i == i);
      expect(2_i == i2);
      expect(3_i == i3);
      expect(12_ul == std::ranges::size(span));
    };
    "safe read int and return it and shift offset"_test = [&buffer] {
      auto       span = std::span(buffer);
      const auto i    = tl::read::safe::value<std::int32_t>(&span);
      expect(1_i == i);
      expect(0_ul == std::ranges::size(span));
    };
    "safe read int and return it and don't shift offset"_test = [&buffer] {
      auto       span = std::span(buffer);
      const auto i    = tl::read::safe::value<std::int32_t>(span);
      expect(1_i == i);
      expect(4_ul == std::ranges::size(span));
    };
    "read ints and return it and don't shift offset"_test = [&longer_buffer] {
      auto span                  = std::span(longer_buffer);
      const auto [i, i2, i3, i4] = tl::read::safe::
        value<std::int32_t, std::int32_t, std::int32_t, std::int32_t>(span);
      expect(1_i == i);
      expect(2_i == i2);
      expect(3_i == i3);
      expect(0_i == i4);
      expect(12_ul == std::ranges::size(span));
    };
    "read ints and return it and don't shift offset"_test = [&longer_buffer] {
      auto span                  = std::span(longer_buffer);
      const auto [i, i2, i3, i4] = tl::read::safe::
        value<std::int32_t, std::int32_t, std::int32_t, std::int32_t>(&span);
      expect(1_i == i);
      expect(2_i == i2);
      expect(3_i == i3);
      expect(0_i == i4);
      expect(0_ul == std::ranges::size(span));
    };
    "read ints from stream"_test = [&ss] {
      const auto [i, i2, i3] =
        tl::read::value<std::int32_t, std::int32_t, std::int32_t>(ss);
      expect(1_i == i);
      expect(2_i == i2);
      expect(3_i == i3);
      expect(12_l == ss.tellg());
      ss.seekg(0, std::ios::beg);
    };
    "safe read ints from stream"_test = [&ss] {
      const auto [i, i2, i3, i4] = tl::read::safe::
        value<std::int32_t, std::int32_t, std::int32_t, std::int32_t>(ss);
      expect(1_i == i);
      expect(2_i == i2);
      expect(3_i == i3);
      expect(0_i == i4);
      expect(12_l == ss.tellg());
      ss.seekg(0, std::ios::beg);
    };
    "read array of char"_test = [&buffer] {
      std::array<char, 4> i{};
      tl::read::value(buffer, i);
      expect(std::ranges::equal(buffer, i));
    };
    "read array of char return"_test = [&buffer] {
      auto i = tl::read::value<std::array<char, 4>>(buffer);
      expect(std::ranges::equal(buffer, i));
    };
    "safe read array of char"_test = [&buffer] {
      std::array<char, 4> i{};
      tl::read::safe::value(buffer, i);
      expect(std::ranges::equal(buffer, i));
    };
    "safe read array of char return"_test = [&buffer] {
      auto i = tl::read::safe::value<std::array<char, 4>>(buffer);
      expect(std::ranges::equal(buffer, i));
    };
    "read array of char and shift offset"_test = [&buffer] {
      auto                span = std::span(buffer);
      std::array<char, 4> i{};
      tl::read::value(&span, i);
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
    "read array of char return and shift offset"_test = [&buffer] {
      auto span = std::span(buffer);
      auto i    = tl::read::value<std::array<char, 4>>(&span);
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
    "safe read array of char and shift offset"_test = [&buffer] {
      auto                span = std::span(buffer);
      std::array<char, 4> i{};
      tl::read::safe::value(&span, i);
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
    "safe read array of char return and shift offset"_test = [&buffer] {
      auto span = std::span(buffer);
      auto i    = tl::read::safe::value<std::array<char, 4>>(&span);
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
    "read array of char"_test = [&ss] {
      std::array<char, 4> i{};
      tl::read::value(ss, i);
      expect(std::ranges::equal(ss.str().substr(0, 4), i));
      expect(4_l == ss.tellg());
      ss.seekg(0, std::ios::beg);
    };
    "read array of char return"_test = [&ss] {
      auto i = tl::read::value<std::array<char, 4>>(ss);
      expect(std::ranges::equal(ss.str().substr(0, 4), i));
      expect(4_l == ss.tellg());
      ss.seekg(0, std::ios::beg);
    };
    "safe read array of char"_test = [&ss] {
      std::array<char, 4> i{};
      tl::read::safe::value(ss, i);
      expect(std::ranges::equal(ss.str().substr(0, 4), i));
      expect(4_l == ss.tellg());
      ss.seekg(0, std::ios::beg);
    };
    "safe read array of char return"_test = [&ss] {
      auto i = tl::read::safe::value<std::array<char, 4>>(ss);
      expect(std::ranges::equal(ss.str().substr(0, 4), i));
      expect(4_l == ss.tellg());
      ss.seekg(0, std::ios::beg);
    };
    "read vector of char"_test = [&buffer] {
      std::vector<char> i(4);
      tl::read::value(buffer, i);
      expect(std::ranges::equal(buffer, i));
    };
    "read vector of char and shift offset"_test = [&buffer] {
      auto              span = std::span(buffer);
      std::vector<char> i(4);
      tl::read::value(&span, i);
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
    "safe read vector of char"_test = [&buffer] {
      std::vector<char> i(4);
      tl::read::safe::value(buffer, i);
      expect(std::ranges::equal(buffer, i));
    };
    "safe read vector of char and shift offset"_test = [&buffer] {
      auto              span = std::span(buffer);
      std::vector<char> i(4);
      tl::read::safe::value(&span, i);
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
    "read vector of char return"_test = [&buffer] {
      auto i = tl::read::value<std::vector<char>>(buffer, 4U);
      expect(std::ranges::equal(buffer, i));
    };
    "read vector of char return and shift offset"_test = [&buffer] {
      std::span span = buffer;
      auto      i    = tl::read::value<std::vector<char>>(&span, 4U);
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
    "read vector of char return"_test = [&buffer] {
      auto i = tl::read::safe::value<std::vector<char>>(buffer, 4U);
      expect(std::ranges::equal(buffer, i));
    };
    "read vector of char return and shift offset"_test = [&buffer] {
      std::span span = buffer;
      auto      i    = tl::read::safe::value<std::vector<char>>(&span, 4U);
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
    "read string of char return"_test = [&buffer] {
      auto i = tl::read::value<std::string>(buffer, 4U);
      expect(std::ranges::equal(buffer, i));
    };
    "read string of char return and shift offset"_test = [&buffer] {
      std::span span = buffer;
      auto      i    = tl::read::value<std::string>(&span, 4U);
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
    "read string of char return"_test = [&buffer] {
      auto i = tl::read::safe::value<std::string>(buffer, 4U);
      expect(std::ranges::equal(buffer, i));
    };
    "read string of char return and shift offset"_test = [&buffer] {
      std::span span = buffer;
      auto      i    = tl::read::safe::value<std::string>(&span, 4U);
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
    "read string of char"_test = [&buffer] {
      std::string i{};
      i.resize(4);
      tl::read::value(buffer, i);
      expect(std::ranges::equal(buffer, i));
    };
    "read string of char and shift offset"_test = [&buffer] {
      auto        span = std::span(buffer);
      std::string i{};
      i.resize(4);
      tl::read::value(&span, i);
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
    "safe read string of char"_test = [&buffer] {
      std::string i{};
      i.resize(4);
      tl::read::safe::value(buffer, i);
      expect(std::ranges::equal(buffer, i));
    };
    "safe read string of char and shift offset"_test = [&buffer] {
      auto        span = std::span(buffer);
      std::string i{};
      i.resize(4);
      tl::read::safe::value(&span, i);
      expect(std::ranges::equal(buffer, i));
      expect(0_ul == std::ranges::size(span));
    };
  };
}