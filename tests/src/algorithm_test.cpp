#include "boost/ut.hpp"// single header
// import boost.ut;        // single module (C++20)
#include "tl/algorithm.hpp"
#include <string_view>
int
  main()
{
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;
  using namespace boost::ut;
  using namespace std::string_view_literals;
  [[maybe_unused]] suite algorithm = [] {
    static constexpr auto test_sv = "0123456789abcdefghijklmnopqrstuvwxyz"sv;
    "for each test"_test          = [] {
      tl::algorithm::for_each(
        [](const auto &a) {
          expect(neq(a, '\0'));
        },
        test_sv);
      tl::algorithm::for_each(
        [](const auto &a, const auto &b) {
          expect(eq(a, b));
        },
        test_sv,
        test_sv);
    };
  };
}