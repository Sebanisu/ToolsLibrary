#ifndef TOOLSLIBRARY_CONCEPTS_HPP
#define TOOLSLIBRARY_CONCEPTS_HPP

namespace tl::concepts {
template<typename T>
concept is_trivially_copyable = std::is_trivially_copyable_v<T>;
template<typename T>
concept is_trivially_copyable_and_default_constructible =
  is_trivially_copyable<T> &&std::is_default_constructible_v<T>;
template<typename T> concept is_resizable = requires(T t)
{
  t.resize(1U);
};

template<typename T> concept is_range = requires(T t)
{
  std::begin(t);
  std::end(t);
};
template<typename T> concept is_continuous_range = is_range<T> && requires(T t)
{
  std::size(t);
  std::data(t);
};
template<typename T>
concept is_contiguous_and_resizable = is_continuous_range<T> &&is_resizable<T>;
template<typename T> concept is_iterator = requires(T t)
{
  ++t;
  *t;
};
template<typename T>
concept is_integral = std::is_integral_v<T>;

template<typename T>
concept is_signed_integral = is_integral<T> && std::is_signed_v<T>;
}// namespace tl::concepts
#endif// TOOLSLIBRARY_CONCEPTS_HPP
