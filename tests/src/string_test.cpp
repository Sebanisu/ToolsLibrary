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
    {
      "remove drive letter"_test = [] {
        const auto test_drive_letter = [](std::string &&start) {
          const auto start_size = std::size(start);
          const auto end = tl::string::remove_drive_letter(std::move(start));
          const auto end_size = std::size(end);
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
          all_lowercase_letters([&test_drive_letter]<auto I>() {
            test_drive_letter(std::string{ char(I), ':', '\\' });
          });
          all_UPPERCASE_letters([&test_drive_letter]<auto I2>() {
            test_drive_letter(std::string{ char(I2), ':', '\\' });
          });
          all_lowercase_letters([&test_drive_letter]<auto I3>() {
            test_drive_letter(std::string{ char(I3), ':', '/' });
          });
          all_UPPERCASE_letters([&test_drive_letter]<auto I4>() {
            test_drive_letter(std::string{ char(I4), ':', '/' });
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
          expect(std::empty(result));
        });
      };
      "remove carriage returns from anywhere"_test = []() {
        static constexpr auto seq = tl::utility::sequence<1, 10>();
        seq([]<auto I5>() {
          const auto result =
            tl::string::remove_carriage_return([]() -> std::string {
              std::string r_string{};
              r_string.reserve(3U * I5);
              for (auto i = I5; i != 0; --i) {
                r_string.push_back(' ');
                r_string.push_back('\r');
                r_string.push_back(' ');
              }
              return r_string;
            }());
          expect(eq(std::size(result), 2U * I5));
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
    }
    "remove string from string"_test = [] {
      const auto check = [](std::string            haystack,
                            const std::string_view needle) {
        tl::string::erase_string_from_string(haystack, needle);
        const auto f = tl::string::search(haystack, needle);
        expect(!f.has_value()) << haystack;
      };
      check(R"(c:\test\test2\test3
c:\test4\test5
c:\test6\test7)"s,
            R"(c:\)"sv);
    };
    "remove all drive letters"_test = [] {
      const auto value = tl::string::remove_all_drive_letters(
        R"(a:\b:\c:\d:\e:\f:\g:\h:\i:\j:\k:\l:\m:\n:\o:\p:\q:\r:\s:\t:\u:\v:\w:\x:\y:\z:\a:/b:/c:/d:/e:/f:/g:/h:/i:/j:/k:/l:/m:/n:/o:/p:/q:/r:/s:/t:/u:/v:/w:/x:/y:/z:/)"s);
      expect(std::empty(value));
    };
  };
}
