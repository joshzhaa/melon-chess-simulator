#ifndef MELON_UTIL_HPP_
#define MELON_UTIL_HPP_
// TODO: rename this file to byte.hpp if nothing else gets added here

#include <cstdint>

namespace melon {

// so much more ergonomic than std::byte
using byte = std::uint8_t;
constexpr byte True = 1;
constexpr byte False = 0;

}  // namespace melon

#endif  // MELON_UTIL_HPP_
