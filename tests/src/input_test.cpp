#include "boost/ut.hpp"// single header
// import boost.ut;        // single module (C++20)
#include "tl/input.hpp"
#include <algorithm>
#include <ranges>
int
  main()
{
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;
  using namespace boost::ut;
  [[maybe_unused]] suite input = [] {
    {
      static constexpr auto buffer = std::string_view("\x01\x00\x00\x00", 4U);
      static constexpr auto longer_buffer = std::string_view(
        "\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00", 12U);
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
        auto       span = std::span(longer_buffer);
        const auto tuple =
          tl::read::input(&span)
            .template output<std::int32_t, std::int32_t, std::int32_t>();
        const int i1 = std::get<0>(tuple);
        const int i2 = std::get<1>(tuple);
        const int i3 = std::get<2>(tuple);
        expect(1_i == i1);
        expect(2_i == i2);
        expect(3_i == i3);
        expect(0_ul == std::ranges::size(span));
      };
      "read ints and return it and don't shift offset"_test = [] {
        auto       span = std::span(longer_buffer);
        const auto tuple =
          tl::read::input(span)
            .template output<std::int32_t, std::int32_t, std::int32_t>();
        const int i1 = std::get<0>(tuple);
        const int i2 = std::get<1>(tuple);
        const int i3 = std::get<2>(tuple);
        expect(1_i == i1);
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
          auto       span = std::span(longer_buffer);
          const auto tuple =
            tl::read::input(span, true)
              .output<std::int32_t, std::int32_t, std::int32_t, std::int32_t>();
          const int i1 = std::get<0>(tuple);
          const int i2 = std::get<1>(tuple);
          const int i3 = std::get<2>(tuple);
          const int i4 = std::get<3>(tuple);
          expect(1_i == i1);
          expect(2_i == i2);
          expect(3_i == i3);
          expect(0_i == i4);
          expect(12_ul == std::ranges::size(span));
        };
      "safe read ints and return it and shift offset, expect one to not be wrote to."_test =
        [] {
          auto       span = std::span(longer_buffer);
          const auto tuple =
            tl::read::input(&span, true)
              .output<std::int32_t, std::int32_t, std::int32_t, std::int32_t>();
          const int i1 = std::get<0>(tuple);
          const int i2 = std::get<1>(tuple);
          const int i3 = std::get<2>(tuple);
          const int i4 = std::get<3>(tuple);
          expect(1_i == i1);
          expect(2_i == i2);
          expect(3_i == i3);
          expect(0_i == i4);
          expect(0_ul == std::ranges::size(span));
        };
      "read ints from stream"_test = [&ss] {
        const auto tuple =
          tl::read::input(&ss)
            .output<std::int32_t, std::int32_t, std::int32_t>();
        const int i1 = std::get<0>(tuple);
        const int i2 = std::get<1>(tuple);
        const int i3 = std::get<2>(tuple);
        expect(1_i == i1);
        expect(2_i == i2);
        expect(3_i == i3);
        expect(12_l == ss.tellg());
        ss.seekg(0, std::ios::beg);
      };
      "safe read ints from stream"_test = [&ss] {
        const auto tuple =
          tl::read::input(&ss, true)
            .output<std::int32_t, std::int32_t, std::int32_t, std::int32_t>();
        const int i1 = std::get<0>(tuple);
        const int i2 = std::get<1>(tuple);
        const int i3 = std::get<2>(tuple);
        const int i4 = std::get<3>(tuple);
        expect(1_i == i1);
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
        const auto v =
          tl::read::input(buffer).output_all_remaining(std::vector<char>());
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
                         .seek(4, std::ios::cur)
                         .template output<std::vector<char>>(4);
        expect(std::ranges::equal(longer_buffer.substr(4, 4), v));
      };
      "seek from end"_test = [] {
        auto input = tl::read::input(longer_buffer);
        input.seek(-4, std::ios::end);
        const auto v      = input.template output<std::vector<char>>(4);
        const auto substr = longer_buffer.substr(8, 4);
        expect(std::ranges::equal(substr, v));
      };
      "seek from begin"_test = [] {
        auto input = tl::read::input(longer_buffer);
        input.seek(6, std::ios::beg);
        const auto v      = input.template output<std::vector<char>>(4);
        const auto substr = longer_buffer.substr(6, 4);
        expect(std::ranges::equal(substr, v));
      };
      "seek using ss"_test = [&ss] {
        auto input = tl::read::input(&ss);
        input.seek(4, std::ios::cur);
        expect(ss.tellg() == 4_l);
        const auto v = input.template output<std::vector<char>>(4);
        ss.seekg(0, std::ios::beg);
        expect(ss.tellg() == 0_l);
        expect(std::ranges::equal(longer_buffer.substr(4, 4), v));
      };
      "seek from begin using ss"_test = [&ss] {
        auto input = tl::read::input(&ss);
        expect(ss.tellg() == 0_l);
        input.seek(6, std::ios::beg);
        // ss.seekg(6U,std::ios::beg);
        expect(ss.tellg() == 6_l);
        const auto v      = input.template output<std::vector<char>>(4);
        const auto substr = longer_buffer.substr(6, 4);
        expect(std::ranges::equal(substr, v));
        ss.seekg(0, std::ios::beg);
        expect(ss.tellg() == 0_l);
      };
      "seek from end using ss"_test = [&ss] {
        auto input = tl::read::input(&ss);
        expect(ss.tellg() == 0_l);
        input.seek(-4, std::ios::end);
        // ss.seekg(4,std::ios::end);
        expect(ss.tellg() == 8_l);
        const auto v      = input.template output<std::vector<char>>(4);
        const auto substr = longer_buffer.substr(8, 4);
        expect(std::ranges::equal(substr, v));
        ss.seekg(0, std::ios::beg);
        expect(ss.tellg() == 0_l);
      };
    }
    {
      // TODO test tell
      "tell, span"_test = [] {

      };

      "tell, istream"_test = [] {

      };
      // TODO test read_line

      static constexpr std::string_view read_line_str(R"(c:\one\two
d:\three\four
e:\five\six
f:\seven\eight
g:\nine\ten)");
      std::stringstream                 ss{};
      ss << read_line_str;
      "check ss size to match read line str"_test = [&ss] {
        expect(static_cast<std::size_t>(ss.tellp()) == read_line_str.size());
      };
      "tell, read line"_test = [] {
        const tl::read::input    input(read_line_str);
        std::vector<std::string> lines{};
        while (true) {
          const std::string &line = lines.emplace_back(input.get_line());
          if (std::empty(line)) {
            lines.pop_back();
            break;
          }
        }
        expect(eq(std::ranges::size(lines), 5U))<<lines.back();
      };

      "tell, read line"_test = [] {

      };
    }
  };
}