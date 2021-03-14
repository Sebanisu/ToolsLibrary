#include <boost/ut.hpp>// single header
// import boost.ut;        // single module (C++20)
#include "tl/string.hpp"
#include "tl/utility.hpp"
#include <string_view>
int
  main()
{
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;
  using namespace boost::ut;
  using namespace std::string_view_literals;
  using namespace std::string_literals;
  [[maybe_unused]] suite string = [] {
    "remove drive letter"_test = [] {
      const auto test_drive_letter = [](std::string &&start) {
        const auto start_size = std::ranges::size(start);
        const auto end      = tl::string::remove_drive_letter(std::move(start));
        const auto end_size = std::ranges::size(end);
        expect(gt(start_size, end_size));
        expect(eq(start_size - 3U, end_size));
      };
      const auto test_drive_letters = [&test_drive_letter](auto &&...start) {
        (test_drive_letter(std::move(start)), ...);
      };
      static constexpr auto all_lowercase_letters =
        tl::utility::sequence<'a', 'z' - 'a'>();
      static constexpr auto all_UPPERCASE_letters =
        tl::utility::sequence<'A', 'Z' - 'A'>();
      const auto test_all_drive_letters = [&test_drive_letter]() {
        all_lowercase_letters.operator()([&test_drive_letter]<auto I>() {
          test_drive_letter(std::string{ char(I), ':', '\\' });
        });
        all_UPPERCASE_letters.operator()([&test_drive_letter]<auto I>() {
          test_drive_letter(std::string{ char(I), ':', '\\' });
        });
        all_lowercase_letters.operator()([&test_drive_letter]<auto I>() {
          test_drive_letter(std::string{ char(I), ':', '/' });
        });
        all_UPPERCASE_letters.operator()([&test_drive_letter]<auto I>() {
          test_drive_letter(std::string{ char(I), ':', '/' });
        });
      };
      test_drive_letters(R"(c:\)"s,
                         R"(d:\)"s,
                         R"(e:\)"s,
                         R"(c:\test\a)"s,
                         R"(c:\test\b)"s,
                         R"(c:\test\c)"s);
      test_all_drive_letters();
    };
    "remove carriage returns from end"_test = []() {
      static constexpr auto seq = tl::utility::sequence<1, 10>();
      seq([]<auto T>() {
        const auto result =
          tl::string::remove_carriage_return_from_end(std::string(T, '\r'));
        expect(std::ranges::size(result) == 0_ull);
      });
    };
    "remove carriage returns from anywhere"_test = []() {
      static constexpr auto seq = tl::utility::sequence<1, 10>();
      seq([]<auto I>() {
        const auto result =
          tl::string::remove_carriage_return([]() -> std::string {
            std::string r_string{};
            r_string.reserve(3U * I);
            for (auto i = I; i != 0; --i) {
              r_string.push_back(' ');
              r_string.push_back('\r');
              r_string.push_back(' ');
            }
            return r_string;
          }());
        expect(eq(std::ranges::size(result), 2U * I));
      });
    };
    if constexpr (std::filesystem::path::preferred_separator == '/') {
      "replace slashes"_test = [] {
        const auto result =
          tl::string::replace_slashes(R"raw(c:\test\test2)raw"s);
        expect(eq(result, R"raw(c:/test/test2)raw"s));
      };
      "undo replace slashes"_test = [] {
        const auto result =
          tl::string::undo_replace_slashes(R"raw(c:/test/test2)raw"s);
        expect(eq(result, R"raw(c:\test\test2)raw"s));
      };
    }
  };
}
