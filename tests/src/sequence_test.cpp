#include <boost/ut.hpp>// single header
// import boost.ut;        // single module (C++20)
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
  [[maybe_unused]] suite sequence = [] {
    "create random char array"_test = [] {
      auto s = tl::utility::sequence<0, 10>();
      s([]<auto I>() {
        expect(le(I, 10U));
        expect(ge(I, 0U));
      });
    };
  };
}