#include <boost/ut.hpp>// single header
// import boost.ut;        // single module (C++20)
#include "tl/algorithm.hpp"
#include "tl/utility.hpp"
#include "tl/write.hpp"
int
  main()
{
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;
  using namespace boost::ut;
  using namespace std::string_view_literals;
  using namespace std::string_literals;
  [[maybe_unused]] suite write = [] {
    "insert_test int to vector of int"_test = [] {
      std::vector<int> vector{};
      std::vector<int> vector2{ 2, 3, 4, 5 };
      tl::write::append(vector, 1);
      expect(eq(std::size(vector), 1U));
      tl::write::append(vector, vector2);
      expect(eq(std::size(vector), 5U));
      tl::write::append(vector, 6, 7, 8, 9, 10);
      expect(eq(std::size(vector), 10U));
      expect(tl::algorithm::equal(vector,
                                  std::array{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }));
    };
    "insert_test int to vector of char"_test = [] {
      std::vector<char> vector{};
      std::vector<int>  vector2{ 2, 3, 4, 5 };
      tl::write::append(vector, 1);
      expect(eq(std::size(vector), 4U));
      tl::write::append(vector, vector2);
      expect(eq(std::size(vector), 20U));
      tl::write::append(vector, 6, 7, 8, 9, 10);
      expect(eq(std::size(vector), 40U));
    };
    "insert_test int to char stream"_test = [] {
      std::stringstream stringstream{};
      std::vector<int>  vector{ 2, 3, 4, 5 };
      tl::write::append(stringstream, 1);
      expect(eq(tl::utility::get_remaining(stringstream), 4U));
      tl::write::append(stringstream, vector);
      expect(eq(tl::utility::get_remaining(stringstream), 20U));
      tl::write::append(stringstream, 6, 7, 8, 9, 10);
      expect(eq(tl::utility::get_remaining(stringstream), 40U));
      //    std::vector<char> read
      //      {
      //
      //      };
      //    expect(tl::algorithm::equal(stringstream,
      //                                std::array{ 1, 2, 3, 4, 5, 6, 7, 8, 9,
      //                                10 }));
    };
  };
}