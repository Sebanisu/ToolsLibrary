//
// Created by pcvii on 3/29/2021.
//

#ifndef TOOLSLIBRARY_WRITE_HPP
#define TOOLSLIBRARY_WRITE_HPP
#include "concepts.hpp"
#include "utility.hpp"
namespace tl::write {

template<concepts::is_contiguous_with_insert outputR,
         concepts::is_range                  inputR,
         typename outputT = std::decay_t<typename outputR::value_type>,
         typename inputT  = std::decay_t<typename inputR::value_type>>
requires(std::is_same_v<outputT, inputT>) inline void append(outputR      &out,
                                                             const inputR &in)
{
  out.insert(std::end(out), std::begin(in), std::end(in));
}

template<concepts::is_contiguous_with_insert outputR,
         concepts::is_range                  inputR,
         typename outputT = std::decay_t<typename outputR::value_type>,
         typename inputT  = std::decay_t<typename inputR::value_type>>
requires(!std::is_same_v<outputT, inputT> && std::is_same_v<outputT, char> && concepts::is_trivially_copyable<inputT>) inline void append(
  outputR &out, const inputR &in)
{
  auto tmp = utility::to_bytes<inputT>(std::data(in), std::size(in));
  append(out, tmp);
}
template<concepts::is_range inputR,
         typename inputT = std::decay_t<typename inputR::value_type>>
requires(std::is_same_v<inputT, char>) inline void append(std::ostream &out,
                                                          const inputR &in)
{
  out.write(std::data(in), static_cast<long>(std::size(in)));
}
template<concepts::is_range inputR,
         typename inputT = std::decay_t<typename inputR::value_type>>
requires(((!std::is_same_v<inputT, char>)&&(
  concepts::is_trivially_copyable<
    inputT>))) inline void append(std::ostream &out, const inputR &in)
{
  auto tmp = utility::to_bytes<inputT>(std::data(in), std::size(in));
  append(out, tmp);
}
template<concepts::is_contiguous_with_insert outputR,
         typename T = std::decay_t<typename outputR::value_type>,
         typename... Ts>
requires(std::is_same_v<std::decay_t<Ts>, T> &&...) inline void append(
  outputR &out, const Ts &...in)
{
  (((void)out.insert(std::end(out), in)), ...);
}
template<concepts::is_contiguous_with_insert outputR,
         typename T = std::decay_t<typename outputR::value_type>,
         typename... Ts>
requires(
  (!std::is_same_v<std::decay_t<Ts>, T> || ...)
  && std::is_same_v<
    T,
    char> && (concepts::is_trivially_copyable<Ts> && ...)) inline void append(outputR
                                                                                &out,
                                                                              const Ts
                                                                                &...in)
{
  ((append(out, utility::to_bytes(in))), ...);
}
template<typename... Ts>
requires((!std::is_same_v<Ts, char> || ...)
         && (concepts::is_trivially_copyable<
               Ts> && ...)) inline void append(std::ostream &out,
                                               const Ts &...in)
{
  ((append(out, utility::to_bytes(in))), ...);
}
// struct output
//{
// private:
// public:
//};
}// namespace tl::write
#endif// TOOLSLIBRARY_WRITE_HPP
