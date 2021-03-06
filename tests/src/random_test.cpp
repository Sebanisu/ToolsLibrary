#include <boost/ut.hpp>// single header
// import boost.ut;        // single module (C++20)
#include "tl/random.hpp"
#include "tl/algorithm.hpp"
#include <string_view>
int
  main()
{
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;
  using namespace boost::ut;
  using namespace std::string_view_literals;
  using namespace std::string_literals;
  [[maybe_unused]] suite random = [] {
    const auto check_type = []<tl::concepts::is_integral integralT>() {
      const auto random_values = tl::random::iota<char, 10U>();
      expect(tl::algorithm::any_of(random_values, [](const auto &value) -> bool {
        return value != integralT{};
      }));
    };
    const auto check_types = [&check_type]<tl::concepts::is_integral... integralT2>()
    {
      (check_type.template operator()<integralT2>(), ...);
    };
    "create random char array"_test = [&check_types] {
      check_types.operator()<std::int8_t,
                             std::uint8_t,
                             std::int16_t,
                             std::uint16_t,
                             std::int32_t,
                             std::uint32_t,
                             std::int64_t,
                             std::uint64_t>();
    };
  };
}