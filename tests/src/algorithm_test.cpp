#include "boost/ut.hpp"// single header
// import boost.ut;        // single module (C++20)
#include "tl/algorithm.hpp"
#include <string_view>
//msvc says this isn't a constant eval
//template<typename... T>
//consteval void
//  for_each(const T &...ts) noexcept
//{
//  tl::algorithm::for_each(ts...);
//}
//consteval bool for_each_test()
//{
//    using namespace std::string_view_literals;
//    //these tests are just to make sure the code is executing at compile time
//    //not really expecting them to fail.
//    static constexpr auto test_sv = "0123456789abcdefghijklmnopqrstuvwxyz"sv;
//    for_each(
//        [](const auto& a) {
//        if (a == '\0') {
//            throw;
//        }
//    },
//        test_sv);
//    for_each(
//        [](const auto& a, const auto& b) {
//        if (a != b) {
//            throw;
//        }
//    },
//        test_sv,
//        test_sv);
//    // uncomment to check compile time if statement should fail to compile.
//    //      for_each(
//    //        [](const auto &a, const auto &b) {
//    //          if(a != b)
//    //          {
//    //            throw;
//    //          }
//    //        },
//    //        test_sv,
//    //        ""sv);
//    return true;
//}
//static_assert(for_each_test());
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
      // uncomment to test assert will fail at runtime for debug builds.
      //      tl::algorithm::for_each(
      //        [](const auto &a, const auto &b) {
      //          expect(eq(a, b));
      //        },
      //        test_sv,
      //        ""sv);
    };
  };
}