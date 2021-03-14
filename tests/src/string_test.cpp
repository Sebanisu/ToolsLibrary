#include <boost/ut.hpp>// single header
// import boost.ut;        // single module (C++20)
#include "tl/string.hpp"
#include <string_view>
int
main()
{
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;
  using namespace boost::ut;
  using namespace std::string_view_literals;
  using namespace std::string_literals;
  [[maybe_unused]] suite string = [] {};
}
