#include <boost/ut.hpp>// single header
// import boost.ut;        // single module (C++20)
#include "tl/read.hpp"
#include <sstream>
int main()
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
      auto span = std::span(longer_buffer);
      const auto [i, i2, i3, i4] =
      tl::read::safe::value<std::int32_t, std::int32_t, std::int32_t,std::int32_t>(span);
      expect(1_i == i);
      expect(2_i == i2);
      expect(3_i == i3);
      expect(0_i == i4);
      expect(12_ul == std::ranges::size(span));
    };
    "read ints and return it and don't shift offset"_test = [&longer_buffer] {
      auto span = std::span(longer_buffer);
      const auto [i, i2, i3, i4] =
      tl::read::safe::value<std::int32_t, std::int32_t, std::int32_t,std::int32_t>(&span);
      expect(1_i == i);
      expect(2_i == i2);
      expect(3_i == i3);
      expect(0_i == i4);
      expect(0_ul == std::ranges::size(span));
    };
    "read int from stream"_test = [&ss] {
      const auto [i,i2,i3] =
      tl::read::value<std::int32_t,std::int32_t,std::int32_t>(ss);
      expect(1_i == i);
      expect(2_i == i2);
      expect(3_i == i3);
      expect(12_l == ss.tellg());
      ss.seekg(0, std::ios::beg);
    };
    "read int from stream"_test = [&ss] {
      const auto [i,i2,i3,i4] =
      tl::read::safe::value<std::int32_t,std::int32_t,std::int32_t,std::int32_t>(ss);
      expect(1_i == i);
      expect(2_i == i2);
      expect(3_i == i3);
      expect(0_i == i4);
      expect(12_l == ss.tellg());
      ss.seekg(0, std::ios::beg);
    };
  };
}