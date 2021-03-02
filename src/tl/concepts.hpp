#ifndef TOOLSLIBRARY_CONCEPTS_HPP
#define TOOLSLIBRARY_CONCEPTS_HPP
#include <concepts>
namespace tl::concepts {
template<typename T>
concept is_trivially_copyable = std::is_trivially_copyable_v<T>;
template<typename T>
concept is_trivially_copyable_and_default_constructible = is_trivially_copyable<T> && std::is_default_constructible_v<T>;
}
#endif// TOOLSLIBRARY_CONCEPTS_HPP
