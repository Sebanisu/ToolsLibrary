#ifndef TOOLSLIBRARY_CONCEPTS_HPP
#define TOOLSLIBRARY_CONCEPTS_HPP
#include <cstdint>
#include <type_traits>
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
  // std::begin(t);
  t.begin();
  // std::end(t);
  t.end();
};
template<typename T> concept is_continuous_range = is_range<T> &&requires(T t)
{
  // the free function is not global it's defined separately for each container.
  // so to use it here I need to include each of the containers.
  // std::size(t);
  t.size();
  // std::data(t);
  t.data();
};
template<typename T>
concept is_contiguous_and_resizable = is_continuous_range<T> &&is_resizable<T>;
template<typename T> concept is_iterator = requires(T t)
{
  ++t;
  *t;
};
template<typename T> concept is_integral = std::is_integral_v<T>;

template<typename T>
concept is_signed_integral = is_integral<T> &&std::is_signed_v<T>;

template<typename... T> concept is_invocable = std::is_invocable_v<T...>;

template<typename R, typename T = std::decay_t<typename R::value_type>>
concept is_contiguous_with_insert =
  concepts::is_continuous_range<R> &&requires(R r, R r2, T t)
{
  r.insert(r.begin(), t);
  r.insert(r.begin(), std::size_t(2U), t);
  r.insert(r.begin(), r2.begin(), r2.end());
};
template<typename T>
concept is_nontrivial_continuous_range =
  !is_trivially_copyable<T> && is_continuous_range<T>;
template<typename T>
concept is_trivial_or_continuous_range =
  is_nontrivial_continuous_range<
    T> || is_trivially_copyable<T>;
template<typename... outvarT>
concept size_greater_than_1                        = sizeof...(outvarT) > 1U;
template<typename... outvarT> concept is_not_empty = sizeof...(outvarT) != 0U;
template<typename... outvarT> concept is_empty     = !is_not_empty<outvarT...>;
}// namespace tl::concepts
#endif// TOOLSLIBRARY_CONCEPTS_HPP
