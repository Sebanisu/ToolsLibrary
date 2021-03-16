#include <boost/ut.hpp>// single header
// import boost.ut;        // single module (C++20)
#include "tl/input.hpp"
#include "tl/read.hpp"
#include <algorithm>
#include <sstream>
#include <string_view>
inline std::optional<std::filesystem::path>
  create_temp_file()
{
  using namespace std::string_view_literals;
  return tl::utility::create_temp_file("ToolsLibrary_read_test.tmp",
                                       "Hello World"sv);
}
int
  main()
{
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;
  using namespace boost::ut;
  using namespace std::string_view_literals;
  using namespace std::string_literals;
  suite read = [] {
    const auto temp_file_path = create_temp_file();
    "create temp file"_test   = [&temp_file_path] {
      expect(temp_file_path.has_value());
      expect(std::filesystem::exists(temp_file_path.value()));
    };
    "open file"_test = [&temp_file_path] {
      auto fp = tl::read::open_file(temp_file_path.value());
      expect(fp.has_value());
      expect(fp->is_open());
    };
    "get remaining"_test = [&temp_file_path] {
      auto ret = tl::read::from_file(
        [](std::istream &istream) {
          const auto bytes = tl::utility::get_remaining(istream);
          expect(bytes == 11_u);
        },
        temp_file_path.value());
      expect(ret);
    };
    "read file"_test = [&temp_file_path] {
      std::string out{};
      auto        ret = tl::read::from_file(
        [&out](std::istream &istream) {
          tl::read::input(&istream, true).output_all_remaining(out);
        },
        temp_file_path.value());
      expect(ret);
      expect(std::ranges::equal(out, "Hello World"s));
    };
    "read entire file"_test = [&temp_file_path] {
      const auto out =
        tl::read::entire_file(temp_file_path.value(), std::string());
      expect(std::ranges::equal(out, "Hello World"s));
    };
    "read value from file at offset"_test = [&temp_file_path] {
      const auto out  = tl::read::from_file<char>(3U, temp_file_path.value());
      const auto out2 = tl::read::from_file<char>(2U, temp_file_path.value());
      expect(out == 'l');
      expect(out2 == 'l');
    };
  };
}