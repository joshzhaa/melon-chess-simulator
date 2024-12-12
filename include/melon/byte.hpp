#ifndef MELON_BYTE_HPP
#define MELON_BYTE_HPP

#include <cstdint>

namespace melon {

// so much more ergonomic than std::byte
using byte = std::uint8_t;
constexpr byte True = 1;
constexpr byte False = 0;

}  // namespace melon

#endif  // MELON_BYTE_HPP
