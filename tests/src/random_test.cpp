#include <boost/ut.hpp>// single header
// import boost.ut;        // single module (C++20)
#include "tl/random.hpp"
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
    const auto check_type = []<std::integral T>() {
      const auto i = tl::random::iota<char, 10U>();
      expect(std::ranges::any_of(i, [](const auto &value) -> bool {
        return value != T{};
      }));
    };
    const auto check_types = [&check_type]<std::integral... T>()
    {
      (check_type.template operator()<T>(), ...);
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