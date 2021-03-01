#include <boost/ut.hpp>// single header
// import boost.ut;        // single module (C++20)
#include "tl/read.hpp"
int main()
{
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;
  using namespace boost::ut;

  int umm {};
  "hello world"_test = [&umm] {
    int i = umm;
    expect(42_i == i);
  };
}