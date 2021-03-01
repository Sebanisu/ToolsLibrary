#ifndef TOOLSLIBRARY_CONCEPTS_HPP
#define TOOLSLIBRARY_CONCEPTS_HPP
#include <concepts>
namespace tl::concepts {
template<typename T>
concept is_trivially_copyable = std::is_trivially_copyable_v<T>;
}
#endif// TOOLSLIBRARY_CONCEPTS_HPP
